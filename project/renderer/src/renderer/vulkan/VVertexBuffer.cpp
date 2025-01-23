#include "renderer/vulkan/VVertexBuffer.hpp"
#include "renderer/vulkan/VCommandPool.hpp"
#include "renderer/vulkan/VCommandBuffer.hpp"
#include "renderer/vulkan/VQueue.hpp"

namespace elcad::renderer
{
	VVertexBuffer::VVertexBuffer(SPtr<const VLogicalDevice> logicalDevice, SPtr<VAllocationCallbacks> vkAllocator)
		: m_logicalDevice{ logicalDevice }
		, m_vkAllocator{ vkAllocator } 
	{

	}

	auto VVertexBuffer::destroy() -> void
	{
		if (m_vertexBuffer)
		{
			m_vertexBuffer->destroy();
		}

		if (m_indexBuffer)
		{
			m_indexBuffer->destroy();
		}
	}

	auto VVertexBuffer::create(SPtr<const VCommandPool> commandPool, Span<const model::Vertex> vertices, Span<const u32> indices) -> void
	{
		const u32 vertexBufferSize = static_cast<u32>(vertices.size()) * sizeof(model::Vertex);

		const u32 indexBufferSize = static_cast<u32>(indices.size()) * sizeof(u32);

		auto stagingBuffer = VBuffer
		{
			m_logicalDevice, m_vkAllocator
		};

		stagingBuffer.create
		(
			vertexBufferSize + indexBufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
		);

		stagingBuffer.bind(0);

		auto mappedSection = stagingBuffer.map
		(
			stagingBuffer.getSize()
		);
		
		//copy vertices
		std::memcpy(mappedSection, vertices.data(), vertexBufferSize);

		//copy indices
		std::memcpy((u8*)mappedSection + vertexBufferSize, indices.data(), indexBufferSize);

		m_vertexBuffer = makeUnique<VBuffer>
		(
			m_logicalDevice, m_vkAllocator
		);

		m_vertexBuffer->create
		(
			vertexBufferSize, vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst, vk::MemoryPropertyFlagBits::eDeviceLocal
		);

		m_vertexBuffer->bind(0);

		m_indexBuffer = makeUnique<VBuffer>
		(
			m_logicalDevice, m_vkAllocator
		);

		m_indexBuffer->create
		(
			indexBufferSize, vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst, vk::MemoryPropertyFlagBits::eDeviceLocal
		);

		m_indexBuffer->bind(0);

		auto vertexBufferRegion = vk::BufferCopy
		{
			.size = vertexBufferSize
		};

		auto commandBuffer = makeUnique<VCommandBuffer>
		(
			m_logicalDevice, commandPool
		);

		commandBuffer->allocateAndBeginSingleUse();

		commandBuffer->getVkHandler().copyBuffer
		(
			stagingBuffer.getVkHandler(), m_vertexBuffer->getVkHandler(), { vertexBufferRegion }
		);

		auto indexBufferRegion = vk::BufferCopy
		{
			.srcOffset = vertexBufferSize,
			.size = indexBufferSize
		};

		commandBuffer->getVkHandler().copyBuffer
		(
			stagingBuffer.getVkHandler(), m_indexBuffer->getVkHandler(), { indexBufferRegion }
		);

		auto submitInfo = vk::SubmitInfo
		{
			.commandBufferCount = 1,
			.pCommandBuffers = { &commandBuffer->getVkHandler() }
		};

		m_logicalDevice->getGraphicsQueue()->getVkHandler().submit
		(
			{ submitInfo }
		);

		m_logicalDevice->getGraphicsQueue()->getVkHandler().waitIdle();

		commandBuffer->freeAndEndSingleUse
		(
			m_logicalDevice->getGraphicsQueue()->getVkHandler()
		);

		stagingBuffer.destroy();
	}
}