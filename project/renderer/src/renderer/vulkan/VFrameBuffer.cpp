#include "renderer/vulkan/VFrameBuffer.hpp"
#include "renderer/vulkan/VRenderPass.hpp"

namespace elcad::renderer
{
	VFrameBuffer::VFrameBuffer(SPtr<const VLogicalDevice> logicalDevice, SPtr<const VRenderPass> renderPass, u32 width, u32 height, Span<const vk::ImageView> attachments, SPtr<VAllocationCallbacks> vkAllocator)
		: m_logicalDevice{ logicalDevice }
		, m_vkAllocator{ vkAllocator }
	{
		create(renderPass, width, height, attachments);
	}

	auto VFrameBuffer::create(SPtr<const VRenderPass> renderPass, u32 width, u32 height, Span<const vk::ImageView> attachments) -> void
	{
		auto createInfo = vk::FramebufferCreateInfo
		{
			.renderPass = renderPass->getVkHandler(),
			.attachmentCount = static_cast<u32>(attachments.size()),
			.pAttachments = attachments.data(),
			.width = width,
			.height = height,
			.layers = 1
		};

		m_vkHandler = m_logicalDevice->getVkHandler().createFramebuffer
		(
			createInfo, *m_vkAllocator
		);
	}

	auto VFrameBuffer::destroy() -> void
	{
		if (m_vkHandler)
		{
			m_logicalDevice->getVkHandler().destroyFramebuffer
			(
				m_vkHandler, *m_vkAllocator
			);

			m_vkHandler = nullptr;
		}
	}

	auto VFrameBuffer::getVkHandler() const -> const vk::Framebuffer&
	{
		return m_vkHandler;
	}
}