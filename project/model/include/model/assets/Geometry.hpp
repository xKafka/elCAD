#pragma once

#include <util/Types.hpp>
#include <util/Math.hpp>

#include "model/types/Vertex.hpp"

#include "TextureMapSet.hpp"

namespace elcad::model
{
	struct Face
	{
		static constexpr u32 IndicesPerFace{ 3 };

		std::array<u32, IndicesPerFace> indices{};
	};

	class Geometry;

	class Mesh
	{
		friend class Geometry;

	public:
		Mesh(StringView name);

		//set
		auto setParent(Geometry* parent) -> void;

		auto setVertices(const Vec<Vertex>& vertices) -> void;

		auto setFaces(const Vec<Face>& faces) -> void;

		auto setTextureMapSet(SPtr<TextureMapSet> set) -> void;

		auto setTransform(const math::mat4& t) -> void;

		//get
		[[nodiscard]] auto getName() const -> StringView;

		[[nodiscard]] auto getVertices() const -> Span<const Vertex>;

		[[nodiscard]] auto getVertexCount() const -> u32;

		[[nodiscard]] auto getFaces() const -> Span<const Face>;

		[[nodiscard]] auto getFaceCount() const -> u32;

		[[nodiscard]] auto getTextureMapSet() const -> SPtr<const TextureMapSet>;

		[[nodiscard]] auto getTransform() const -> math::mat4;

		[[nodiscard]] auto getParent() const -> const Geometry*;

	private:
		Geometry*			m_parent{};

		String				m_name{};

		Vec<Vertex>			m_vertices{};

		Vec<Face>			m_faces{};

		SPtr<TextureMapSet>	m_mapSet{};

		math::mat4			m_t{};
	};

	class Geometry
	{
		using Meshes = std::unordered_map<String, SPtr<Mesh>>;

	public:
		Geometry(StringView name);

		//set
		auto addMesh(SPtr<Mesh> mesh) -> void;

		auto allowVertexColoring(bool allow) -> void;

		auto forceVertexColoring() -> void;

		auto setTransform(const math::mat4& t) -> void;

		auto setOrigin(const math::vec3& pos) -> void;

		[[nodiscard]] auto getName() const -> StringView;

		[[nodiscard]] auto getMeshes() const -> const Meshes&;

		[[nodiscard]] auto getTransform() const -> const math::mat4&;

	private:
		String		m_name{};

		Meshes		m_meshes{};

		math::mat4	m_t{};
	};
}