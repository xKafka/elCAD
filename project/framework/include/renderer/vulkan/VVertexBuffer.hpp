#pragma once

#include <Types.h>

#include "VBuffer.hpp"

#include <model/Vertex.hpp>

namespace elcad::renderer
{
	class VCommandBuffer;
}

namespace elcad::renderer
{
	class VVertexBuffer
	{
		auto create() -> void;

	public:
		VVertexBuffer(SPtr<const VLogicalDevice> logicalDevice, SPtr<VAllocationCallbacks> vkAllocator);

		auto create(SPtr<const VCommandBuffer> commandBuffer, Span<const model::Vertex> vertices, Span<const u32> indices) -> void;


	private:
		SPtr<const VLogicalDevice>		m_logicalDevice{};

		SPtr<VAllocationCallbacks>		m_vkAllocator{};

		vk::DeviceMemory				m_memory{};

		vk::Buffer						m_buffer{};

		void* m_mappedBlock{};
	};
}