#pragma once

#include <functional>

#include "VLogicalDevice.hpp"

namespace elcad::renderer
{
	class VRenderPass;
}

namespace elcad::renderer
{
	class VFrameBuffer
	{
		auto create(SPtr<const VRenderPass> renderPass, u32 width, u32 height, Span<const vk::ImageView> attachments) -> void;

	public:
		VFrameBuffer(SPtr<const VLogicalDevice> logicalDevice, SPtr<const VRenderPass> renderPass, u32 width, u32 height, Span<const vk::ImageView> attachments, SPtr<VAllocationCallbacks> vkAllocator);

		auto destroy() -> void;

		[[nodiscard]] auto getVkHandler() const -> const vk::Framebuffer&;

	private:
		SPtr<const VLogicalDevice>	m_logicalDevice{};

		SPtr<VAllocationCallbacks>	m_vkAllocator{};

		vk::Framebuffer				m_vkHandler{};
	};
}
