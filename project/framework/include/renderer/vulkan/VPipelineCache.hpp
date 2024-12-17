#pragma once

#include <Types.h>

#include "VTypes.hpp"
#include "VAllocationCallbacks.hpp"

namespace elcad::renderer
{
	class VLogicalDevice;
}

namespace elcad::renderer
{
	class VPipelineCache
	{
		auto create() -> void;

	public:
		VPipelineCache(SPtr<const VLogicalDevice> logicalDevice, SPtr<VAllocationCallbacks> vkAllocator);

		auto destroy() -> void;

		auto wait(u64 timeout) -> bool;

		auto reset() -> void;

		[[nodiscard]] auto getVkHandler() const -> const vk::Fence&;

	private:
		SPtr<const VLogicalDevice>		m_logicalDevice{};

		SPtr<VAllocationCallbacks>		m_vkAllocator{};

		vk::PipelineCache				m_vkHandler{};
	};
}