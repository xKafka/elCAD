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
	class VPipelineLayout
	{
	public:
		VPipelineLayout(SPtr<const VLogicalDevice> logicalDevice, SPtr<VAllocationCallbacks> vkAllocator);

		auto destroy() -> void;

		auto create() -> void;

		[[nodiscard]] auto getVkHandler() const -> const vk::PipelineLayout&;

	private:
		SPtr<const VLogicalDevice>		m_logicalDevice{};

		SPtr<VAllocationCallbacks>		m_vkAllocator{};

		vk::PipelineLayout				m_vkHandler{};
	};
}