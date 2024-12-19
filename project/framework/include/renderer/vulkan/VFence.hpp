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
	class VFence
	{
		auto create(bool signaled) -> void;

	public:
		VFence(bool signaled, SPtr<const VLogicalDevice> logicalDevice, SPtr<VAllocationCallbacks> vkAllocator);

		auto destroy() -> void;

		auto wait(u64 timeout) -> bool;

		auto reset() -> void;

		[[nodiscard]] auto getVkHandler() const -> const vk::Fence&;

	private:
		SPtr<const VLogicalDevice>		m_logicalDevice{};

		SPtr<VAllocationCallbacks>		m_vkAllocator{};

		vk::Fence						m_vkHandler{};

		bool							m_signaled{};
	};
}