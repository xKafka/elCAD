#pragma once

#include <util/Types.hpp>

#include "VBuffer.hpp"

#include <model/Vertex.hpp>

namespace elcad::renderer
{
	class VCommandPool;
	class VQueue;
}

namespace elcad::renderer
{
	class VVertexBuffer
	{
	public:
		VVertexBuffer(SPtr<const VLogicalDevice> logicalDevice, SPtr<VAllocationCallbacks> vkAllocator);

		auto create(SPtr<const VCommandPool> commandPool, Span<const model::Vertex> vertices, Span<const u32> indices) -> void;

		auto destroy() -> void;

	private:
		SPtr<const VLogicalDevice>	m_logicalDevice{};

		SPtr<VAllocationCallbacks>	m_vkAllocator{};

		UPtr<VBuffer>				m_vertexBuffer{};

		UPtr<VBuffer>				m_indexBuffer{};
	};
}