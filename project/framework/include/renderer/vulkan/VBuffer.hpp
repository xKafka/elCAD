#pragma once

#include <util/Types.hpp>

#include "VTypes.hpp"
#include "VAllocationCallbacks.hpp"

namespace elcad::renderer
{
	class VLogicalDevice;
}

namespace elcad::renderer
{
	class VBuffer
	{
		auto create() -> void;

	public:
		VBuffer(SPtr<const VLogicalDevice> logicalDevice, SPtr<VAllocationCallbacks> vkAllocator);

		auto destroy() -> void;

		auto create(const vk::DeviceSize& size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags memoryProperty) -> void;

		auto map(const vk::DeviceSize& size, const vk::DeviceSize& offset = 0) -> void*;

		auto unmap(void* mappedRegion) -> void;

		auto bind(const vk::DeviceSize& offset) -> void;

		auto getSize() const -> u32;

		auto getVkHandler() const -> const vk::Buffer&;

	private:
		SPtr<const VLogicalDevice>		m_logicalDevice{};

		SPtr<VAllocationCallbacks>		m_vkAllocator{};

		vk::DeviceMemory				m_memory{};

		vk::Buffer						m_buffer{};

		u32								m_size{};
	};
}