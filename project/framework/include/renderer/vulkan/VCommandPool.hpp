#pragma once

#include <functional>

#include "VTypes.hpp"

namespace elcad::renderer
{
	class VAllocationCallbacks;
	class VLogicalDevice;
	class VQueue;
}

namespace elcad::renderer
{
	class VCommandPool
	{
		auto create(SPtr<const VQueue> queue) -> void;

	public:
		VCommandPool() = default;

		VCommandPool(SPtr<const VQueue> queue, SPtr<const VLogicalDevice> logicalDevice, SPtr<VAllocationCallbacks> vkAllocator);

		auto destroy() -> void;

		[[nodiscard]] auto getVkHandler() const -> const vk::CommandPool&;

	private:
		SPtr<VAllocationCallbacks>	m_vkAllocator{};

		SPtr<const VLogicalDevice>	m_logicalDevice{};

		vk::CommandPool				m_vkHandler{};
	};
}
