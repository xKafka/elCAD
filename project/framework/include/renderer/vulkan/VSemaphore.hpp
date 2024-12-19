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
	class VSemaphore
	{
		auto create() -> void;

	public:
		VSemaphore(SPtr<const VLogicalDevice> logicalDevice, SPtr<VAllocationCallbacks>	vkAllocator);

		auto destroy() -> void;

		[[nodiscard]] auto getVkHandler() const -> const vk::Semaphore&;

	private:
		SPtr<const VLogicalDevice>	m_logicalDevice{};

		SPtr<VAllocationCallbacks>	m_vkAllocator{};

		vk::Semaphore				m_vkHandler{};
	};
}