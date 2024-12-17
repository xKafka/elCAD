#include "renderer/vulkan/VVertexBuffer.hpp"
#include "renderer/vulkan/VCommandBuffer.hpp"

namespace elcad::renderer
{
	VVertexBuffer::VVertexBuffer(SPtr<const VLogicalDevice> logicalDevice, SPtr<VAllocationCallbacks> vkAllocator)
	{

	}

	auto VVertexBuffer::create(SPtr<const VCommandBuffer> commandBuffer, Span<const model::Vertex> vertices, Span<const u32> indices) -> void
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

		auto vertexBuffer = VBuffer
		{
			m_logicalDevice, m_vkAllocator
		};

		vertexBuffer.create
		(
			vertexBufferSize, vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst, vk::MemoryPropertyFlagBits::eDeviceLocal
		);

		vertexBuffer.bind(0);

		auto indexBuffer = VBuffer
		{
			m_logicalDevice, m_vkAllocator
		};

		indexBuffer.create
		(
			indexBufferSize, vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst, vk::MemoryPropertyFlagBits::eDeviceLocal
		);

		indexBuffer.bind(0);

		auto vertexBufferRegion = vk::BufferCopy
		{
			.size = vertexBufferSize
		};

		commandBuffer->getVkHandler().copyBuffer
		(
			stagingBuffer.getVkHandler(), vertexBuffer.getVkHandler(), { vertexBufferRegion }
		);

		auto indexBufferRegion = vk::BufferCopy
		{
			.srcOffset = vertexBufferSize,
			.size = indexBufferSize
		};

		commandBuffer->getVkHandler().copyBuffer
		(
			stagingBuffer.getVkHandler(), indexBuffer.getVkHandler(), { indexBufferRegion }
		);
	}
}

//// Buffer copies have to be submitted to a queue, so we need a command buffer for them
//VkCommandBuffer copyCmd;
//
//VkCommandBufferBeginInfo cmdBufInfo = vks::initializers::commandBufferBeginInfo();
//VK_CHECK_RESULT(vkBeginCommandBuffer(copyCmd, &cmdBufInfo));
//// Copy vertex and index buffers to the device
//VkBufferCopy copyRegion{};
//copyRegion.size = vertexBufferSize;
//vkCmdCopyBuffer(copyCmd, stagingBuffer.handle, vertexBuffer.handle, 1, &copyRegion);
//copyRegion.size = indexBufferSize;
//// Indices are stored after the vertices in the source buffer, so we need to add an offset
//copyRegion.srcOffset = vertexBufferSize;
//vkCmdCopyBuffer(copyCmd, stagingBuffer.handle, indexBuffer.handle, 1, &copyRegion);
//VK_CHECK_RESULT(vkEndCommandBuffer(copyCmd));
//
//// Submit the command buffer to the queue to finish the copy
//VkSubmitInfo submitInfo{ VK_STRUCTURE_TYPE_SUBMIT_INFO };
//submitInfo.commandBufferCount = 1;
//submitInfo.pCommandBuffers = &copyCmd;
//
//// Create fence to ensure that the command buffer has finished executing
//VkFenceCreateInfo fenceCI{ VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
//VkFence fence;
//VK_CHECK_RESULT(vkCreateFence(device, &fenceCI, nullptr, &fence));
//// Submit copies to the queue
//VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, fence));
//// Wait for the fence to signal that command buffer has finished executing
//VK_CHECK_RESULT(vkWaitForFences(device, 1, &fence, VK_TRUE, DEFAULT_FENCE_TIMEOUT));
//vkDestroyFence(device, fence, nullptr);
//vkFreeCommandBuffers(device, commandPool, 1, &copyCmd);
//
//// The fence made sure copies are finished, so we can safely delete the staging buffer
//vkDestroyBuffer(device, stagingBuffer.handle, nullptr);
//vkFreeMemory(device, stagingBuffer.memory, nullptr);