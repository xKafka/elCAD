#include "renderer/vulkan/VBuffer.hpp"
#include "renderer/vulkan/VLogicalDevice.hpp"
#include "renderer/vulkan/VPhysicalDevice.hpp"

namespace elcad::renderer
{
	VBuffer::VBuffer(SPtr<const VLogicalDevice> logicalDevice, SPtr<VAllocationCallbacks> vkAllocator)
		: m_logicalDevice{ logicalDevice }
		, m_vkAllocator{ vkAllocator }
	{
	}

	auto VBuffer::destroy() -> void
	{
		if (m_buffer)
		{
			m_logicalDevice->getVkHandler().destroyBuffer
			(
				m_buffer, *m_vkAllocator
			);
		}

		if (m_memory)
		{
			m_logicalDevice->getVkHandler().freeMemory
			(
				m_memory, *m_vkAllocator
			);
		}
	}

	auto VBuffer::create(const vk::DeviceSize& size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags memoryProperty) -> void
	{
		auto createInfo = vk::BufferCreateInfo
		{
			.size = size,
			.usage = usage
		};

		m_buffer = m_logicalDevice->getVkHandler().createBuffer
		(
			createInfo, *m_vkAllocator
		);

		auto requirements = m_logicalDevice->getVkHandler().getBufferMemoryRequirements
		(
			m_buffer
		);

		auto memoryIndex = m_logicalDevice->getPhysicalDevice()->getMemoryIndex
		(
			requirements.memoryTypeBits, memoryProperty
		);

		if (!memoryIndex)
		{
			throw std::runtime_error{ "index not obtained" };
		}

		auto allocateInfo = vk::MemoryAllocateInfo
		{
			.allocationSize = requirements.size,
			.memoryTypeIndex = memoryIndex.value()
		};

		m_memory = m_logicalDevice->getVkHandler().allocateMemory
		(
			allocateInfo, *m_vkAllocator
		);

		m_size = requirements.size;
	}

	auto VBuffer::getSize() const -> u32
	{
		return m_size;
	}

	auto VBuffer::getVkHandler() const -> const vk::Buffer&
	{
		return m_buffer;
	}

	auto VBuffer::map(const vk::DeviceSize& size, const vk::DeviceSize& offset) -> void*
	{
		return m_logicalDevice->getVkHandler().mapMemory
		(
			m_memory, offset, size, {}
		);
	}

	auto VBuffer::unmap(void* mappedRegion) -> void
	{
		if (mappedRegion)
		{
			m_logicalDevice->getVkHandler().unmapMemory
			(
				m_memory
			);

			mappedRegion = nullptr;
		}
	}

	auto VBuffer::bind(const vk::DeviceSize& offset) -> void
	{
		m_logicalDevice->getVkHandler().bindBufferMemory
		(
			m_buffer, m_memory, offset
		);
	}
}