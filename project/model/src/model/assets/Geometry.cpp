#include "model/assets/Geometry.hpp"

namespace elcad::model
{
	Mesh::Mesh(StringView name)
		: m_name{ name }
		, m_t{ math::mat4{ 1.0f } }
	{

	}

	auto Mesh::setParent(Geometry* parent) -> void
	{
		m_parent = parent;
	}

	auto Mesh::setVertices(const Vec<Vertex>& vertices) -> void
	{
		m_vertices = vertices;
	}

	auto Mesh::setFaces(const Vec<Face>& faces) -> void
	{
		m_faces = faces;
	}

	auto Mesh::setTextureMapSet(SPtr<TextureMapSet> set) -> void
	{
		m_mapSet = set;
	}

	auto Mesh::setTransform(const math::mat4& t) -> void
	{
		m_t = t;
	}

	auto Mesh::getName() const -> StringView
	{
		return m_name;
	}

	auto Mesh::getVertices() const -> Span<const Vertex>
	{
		return m_vertices;
	}

	auto Mesh::getVertexCount() const -> u32
	{
		return static_cast<u32>(m_vertices.size());
	}

	auto Mesh::getFaces() const -> Span<const Face>
	{
		return m_faces;
	}

	auto Mesh::getFaceCount() const -> u32
	{
		return static_cast<u32>(m_faces.size());
	}

	auto Mesh::getTextureMapSet() const -> SPtr<const TextureMapSet>
	{
		return m_mapSet;
	}

	auto Mesh::getTransform() const -> math::mat4
	{
		return m_parent ? m_t * m_parent->getTransform() : m_t;
	}

	Geometry::Geometry(StringView name)
		: m_name{ name }
		, m_t{ math::mat4{ 1.0f } }
	{

	}

	auto Geometry::addMesh(SPtr<Mesh> mesh) -> void
	{
		mesh->setParent(this);

		m_meshes[mesh->getName().data()] = mesh;
	}

	auto Geometry::getName() const -> StringView
	{
		return m_name;
	}

	auto Geometry::getMeshes() const -> const Meshes&
	{
		return m_meshes;
	}

	auto Geometry::getTransform() const -> const math::mat4&
	{
		return m_t;
	}
}