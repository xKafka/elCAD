#include "model/importers/Importers.hpp"
#include "model/assets/Geometry.hpp"

#include <util/Filesystem.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace elcad::model::importer
{
	class AssimpReader
	{
		using Vertices = Vec<Vertex>;

		using Faces = Vec<Face>;

		//mesh processing
		auto getVertices(const aiMesh* assimpMesh) -> Vertices;

		auto getFaces(const aiMesh* assimpMesh) -> Faces;

		auto getMesh(const aiMesh* aMesh, const aiMaterial* const* aMats, const aiNode* aNode) -> SPtr<Mesh>;

		//material processing
		auto loadImageFromFile(StringView name, StringView path, i32 stbiChannels) -> SPtr<TextureMap>;

		auto getTextureMapSet(const aiMaterial* aMat) -> SPtr<TextureMapSet>;

		auto getColorImagePath(const aiMaterial* aMat) -> String;

		auto readColorFromPath(StringView path) -> SPtr<TextureMap>;

		auto readColorFromZip(StringView path) -> SPtr<TextureMap>;

		auto getSpecularImagePath(const aiMaterial* aMat) -> String;

		auto readSpecularFromPath(StringView path) -> SPtr<TextureMap>;

		auto readSpecularFromZip(StringView path) -> SPtr<TextureMap>;

		auto getChildNode(const aiNode* parent, StringView name) -> const aiNode*;

		//create geometry
		auto modelOk() -> bool;

		auto createGeometryFromScene() -> void;

		void processMeshes();

		//initializers
		auto sceneOk() -> bool;

		auto loadScene() -> void;

		auto initScene() -> void;

	public:
		AssimpReader(StringView path);

		auto run() -> SPtr<Geometry>;

	private:
		SPtr<Geometry>		m_geometry{};

		Assimp::Importer	m_assimpImporter{ };

		String				m_geometryPath{};

		String				m_rootPath{};
	};

	inline auto getRootPath(StringView path) -> String
	{
		return fs::path{ path }.parent_path().string();
	}

	inline auto is3mf(StringView path) -> bool
	{
		const auto extension = fs::path{ path }.extension().string();

		return extension == ".3mf";
	}

	AssimpReader::AssimpReader(StringView path)
		: m_geometryPath{ path }
		, m_rootPath{ getRootPath(path) }
		, m_assimpImporter{}
	{
		if (m_geometryPath.empty())
		{
			throw std::runtime_error{ "Required path in AssimpReader::Constructor empty" };
		}
	}

	auto AssimpReader::run() -> SPtr<Geometry>
	{
		initScene();

		createGeometryFromScene();

		return m_geometry;
	}

	auto AssimpReader::initScene() -> void
	{
		loadScene();

		if (!sceneOk())
		{
			throw std::runtime_error{ "Failed to load geometry" };
		}
	}

	auto AssimpReader::loadScene() -> void
	{
		const aiScene* scene = m_assimpImporter.ReadFile
		(
			m_geometryPath.data(), 0 |
			aiProcess_CalcTangentSpace |
			aiProcess_GenSmoothNormals |
			aiProcess_Debone |
			aiProcess_Triangulate |
			aiProcess_GenUVCoords |
			aiProcess_FindDegenerates |
			aiProcess_FindInvalidData |
			aiProcess_FindInstances |
			aiProcess_ValidateDataStructure
		);

		if (!scene)
		{
			throw std::runtime_error{ cfm("Failed to load model at [{}]", m_geometryPath) };
		}
	}

	auto AssimpReader::sceneOk() -> bool
	{
		const u32 meshCount = m_assimpImporter.GetScene()->mNumMeshes;

		return meshCount;
	}

	auto AssimpReader::modelOk() -> bool
	{
		return m_geometry->getMeshes().size() > 0;
	}

	auto AssimpReader::createGeometryFromScene() -> void
	{
		const auto name = fs::path{ m_geometryPath }.filename().string();

		m_geometry = makeShared<Geometry>(name);

		processMeshes();

		if (!modelOk())
		{
			throw std::runtime_error{ "Geometry not processed properly" };
		}
	}

	auto AssimpReader::getChildNode(const aiNode* parent, StringView name) -> const aiNode*
	{
		if (auto node = parent->FindNode(name.data()))
		{
			return node;
		}

		return nullptr;
	}

	auto AssimpReader::processMeshes() -> void
	{
		const auto scene = m_assimpImporter.GetScene();

		const u32 meshCount = scene->mNumMeshes;

		if (meshCount == 0)
		{
			throw std::runtime_error{ "Meshes in model not found" };
		}

		auto rootNode = scene->mRootNode;

		for (u32 i{ 0 }; i < meshCount; ++i)
		{
			auto aMesh = scene->mMeshes[i];

			auto aMaterials = scene->mMaterials;

			auto aNode = getChildNode(rootNode, aMesh->mName.C_Str());

			m_geometry->addMesh
			(
				getMesh(aMesh, aMaterials, aNode)
			);
		}
	}

	auto AssimpReader::loadImageFromFile(StringView name, StringView path, i32 stbiChannels) -> SPtr<TextureMap>
	{
		stbi_set_flip_vertically_on_load_thread(true);

		i32 channel{ 0 };

		i32 width{}, height{};

		auto pixels = stbi_load(path.data(), &width, &height, &channel, stbiChannels);

		if (!pixels)
		{
			return {};
		}

		const u32 size = width * height * stbiChannels;

		auto out = makeShared<TextureMap>(name);

		out->setData
		(
			Vec<u8>(pixels, pixels + size),

			channel
		);

		out->setExtent
		(
			static_cast<u32>(width),
			static_cast<u32>(height)
		);

		stbi_image_free(pixels);

		return out;
	}

	auto AssimpReader::readColorFromPath(StringView path) -> SPtr<TextureMap>
	{
		const auto name = fs::path{ path }.stem().generic_string();

		return loadImageFromFile(name, path, TextureMap::RGBA8);
	}

	auto AssimpReader::readSpecularFromPath(StringView path) -> SPtr<TextureMap>
	{
		const auto name = fs::path{ path }.stem().generic_string();

		return loadImageFromFile(name, path, TextureMap::R8);
	}

	//material processing functions start
	auto AssimpReader::getColorImagePath(const aiMaterial* aMat) -> String
	{
		const u32 textureCount = aMat->GetTextureCount(aiTextureType_DIFFUSE);

		if (textureCount == 0)
		{
			return {};
		}

		auto path = aiString{};

		aMat->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL);

		return path.C_Str();
	}

	auto AssimpReader::getSpecularImagePath(const aiMaterial* aMat) -> String
	{
		const u32 textureCount = aMat->GetTextureCount(aiTextureType_SPECULAR);

		if (textureCount == 0)
		{
			return {};
		}

		auto path = aiString{};

		aMat->GetTexture(aiTextureType_SPECULAR, 0, &path, NULL, NULL, NULL, NULL, NULL);

		return path.C_Str();
	}

	auto AssimpReader::getTextureMapSet(const aiMaterial* aMat) -> SPtr<TextureMapSet>
	{
		auto out = makeShared<TextureMapSet>(aMat->GetName().C_Str());

		auto colorPath = getColorImagePath(aMat);

		if (!colorPath.empty())
		{
			out->setColorMap
			(
				readColorFromPath(colorPath)
			);
		}

		auto specularPath = getSpecularImagePath(aMat);

		if (!specularPath.empty())
		{
			out->setSpecularMap
			(
				 readSpecularFromPath(specularPath)
			);
		}

		return out;
	}

	inline auto assimpTransformToMath(const aiMatrix4x4& t) -> math::mat4
	{
		return math::mat4
		{
			t.a1, t.b1, t.c1, t.d1,
			t.a2, t.b2, t.c2, t.d2,
			t.a3, t.b3, t.c3, t.d3,
			t.a4, t.b4, t.c4, t.d4
		};
	}

	inline auto getAssimpNodeTotalTransform(const aiNode* aNode) -> math::mat4
	{
		auto t = assimpTransformToMath(aNode->mTransformation);

		for (auto next{ aNode->mParent }; next; next = next->mParent)
		{
			t *= assimpTransformToMath(next->mTransformation);
		}

		return t;
	}

	auto AssimpReader::getMesh(const aiMesh* aMesh, const aiMaterial* const* aMats, const aiNode* aNode) -> SPtr<Mesh>
	{
		auto out = makeShared<Mesh>(aMesh->mName.C_Str());

		out->setVertices(getVertices(aMesh));

		out->setFaces(getFaces(aMesh));

		if (aNode)
		{
			out->setTransform
			(
				getAssimpNodeTotalTransform(aNode)
			);
		}

		if (const auto aMat = aMats[aMesh->mMaterialIndex])
		{
			out->setTextureMapSet
			(
				getTextureMapSet(aMat)
			);
		}

		return out;
	}

	inline auto getVertexPos(const aiVector3D& vertex) -> math::vec3
	{
		return math::vec3{ vertex[0], vertex[1], vertex[2] };
	}

	inline auto getVertexColor(const aiColor4D& color) -> math::vec4
	{
		return math::vec4{ color[0] , color[1], color[2], color[3] };
	}

	inline auto getVertexNormal(const aiVector3D& normal) -> math::vec3
	{
		return math::vec3{ normal[0], normal[1], normal[2] };
	}

	inline auto getVertexTextureCoords(const aiVector3D& textureCoords) -> math::vec2
	{
		return math::vec2{ textureCoords[0], textureCoords[1] };
	}

	auto AssimpReader::getVertices(const aiMesh* assimpMesh) -> Vertices
	{
		const auto vertexCount = assimpMesh->mNumVertices;

		if (vertexCount == 0)
		{
			throw std::runtime_error{ "Could not process mesh no vertices found" };
		}

		auto out = Vertices(vertexCount);

		for (u32 i{ 0 }; i < vertexCount; ++i)
		{
			auto& vertex = out.at(i);

			if (assimpMesh->mVertices)
			{
				vertex.position = getVertexPos
				(
					assimpMesh->mVertices[i]
				);
			}

			if (assimpMesh->mColors[0])
			{
				vertex.color = getVertexColor
				(
					assimpMesh->mColors[0][i]
				);
			}

			if (assimpMesh->mNormals)
			{
				vertex.normal = getVertexNormal
				(
					assimpMesh->mNormals[i]
				);
			}

			if (assimpMesh->mTextureCoords[0])
			{
				vertex.texCoord0 = getVertexTextureCoords
				(
					assimpMesh->mTextureCoords[0][i]
				);
			}
		}

		return out;
	}

	auto AssimpReader::getFaces(const aiMesh* assimpMesh) -> Faces
	{
		const u32 facesCount = assimpMesh->mNumFaces;

		if (facesCount == 0)
		{
			throw std::runtime_error{ "Could not process mesh no faces found" };
		}

		auto out = Faces(facesCount);

		for (u32 i{ 0 }; i < facesCount; ++i)
		{
			const auto& assimpFace = assimpMesh->mFaces[i];

			if (assimpFace.mNumIndices != 3)
			{
				throw std::runtime_error{ cfm("Face with unexpected number of indices (expected 3, got {})", assimpFace.mNumIndices) };
			}

			out[i].indices = 
			{ 
				assimpFace.mIndices[0], assimpFace.mIndices[1], assimpFace.mIndices[2] 
			};
		}

		return out;
	}

	template<>
	auto loadFromFile<Geometry>(StringView path) -> SPtr<Geometry>
	{
		auto reader = AssimpReader{ path };

		return reader.run();
	}
}