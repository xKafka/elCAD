#include "renderer/vulkan/VRenderPass.hpp"
#include "renderer/vulkan/VFrameBuffer.hpp"

namespace elcad::renderer
{
	VRenderPass::VRenderPass(SPtr<const VLogicalDevice> logicalDevice, SPtr<const VSwapChain> swapChain, SPtr<VAllocationCallbacks> vkAllocator)
		: m_logicalDevice{ logicalDevice }
		, m_swapChain{ swapChain }
		, m_vkAllocator{ vkAllocator }
		, m_state{ State::NotAllocated }
	{
		create();
	}

	auto VRenderPass::create() -> void
	{
		auto attachmentDescriptions = std::array
		{
			vk::AttachmentDescription
			{
				.format = m_swapChain->getImageFormat(),
				.samples = vk::SampleCountFlagBits::e1,
				.loadOp = vk::AttachmentLoadOp::eClear,
				.storeOp = vk::AttachmentStoreOp::eStore,
				.stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
				.stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
				.initialLayout = vk::ImageLayout::eUndefined,
				.finalLayout = vk::ImageLayout::ePresentSrcKHR
			}
		};

		auto colorAttReference = vk::AttachmentReference
		{
			.attachment = 0,
			.layout = vk::ImageLayout::eColorAttachmentOptimal
		};

		auto subPass = vk::SubpassDescription
		{
			.pipelineBindPoint = vk::PipelineBindPoint::eGraphics,
			.colorAttachmentCount = 1,
			.pColorAttachments = &colorAttReference,
		};

		//auto dependency = vk::SubpassDependency
		//{
		//	.srcSubpass = VK_SUBPASS_EXTERNAL,
		//	.dstSubpass = 0,
		//	.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput,
		//	.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput,
		//	.srcAccessMask = {},
		//	.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite
		//};

		auto createInfo = vk::RenderPassCreateInfo
		{
			.attachmentCount = static_cast<u32>(attachmentDescriptions.size()),
			.pAttachments = attachmentDescriptions.data(),
			.subpassCount = 1,
			.pSubpasses = &subPass
		};

		m_vkHandler = m_logicalDevice->getVkHandler().createRenderPass
		(
			createInfo, *m_vkAllocator
		);
	}

	auto VRenderPass::destroy() -> void
	{
		if (m_vkHandler)
		{
			m_logicalDevice->getVkHandler().destroyRenderPass
			(
				m_vkHandler, *m_vkAllocator
			);

			m_vkHandler = nullptr;
		}
	}

	auto VRenderPass::begin(SPtr<const VCommandBuffer> commandBuffer, SPtr<const VFrameBuffer> fbo, const glm::ivec2 offset, const glm::uvec2 extent, const glm::vec4 color, f32 depth, u8 stencil) -> void
	{
		auto renderArea = vk::Rect2D
		{
			.offset = vk::Offset2D
			{
				.x = offset.x,
				.y = offset.y
			},

			.extent = vk::Extent2D
			{
				.width = extent.x,
				.height = extent.y
			}
		};

		auto clearColor = vk::ClearValue
		{
			.color = vk::ClearColorValue
			{
				std::array{ color.r, color.g, color.b, color.a }
			}
		};

		auto clearDepthStencil = vk::ClearValue
		{
			.depthStencil = vk::ClearDepthStencilValue
			{
				.depth = depth,
				.stencil = stencil
			}
		};

		auto clearValues = std::array
		{
			clearColor
			//		m_clearDepthStencil 
		};

		auto beginInfo = vk::RenderPassBeginInfo
		{
			.renderPass = m_vkHandler,
			.framebuffer = fbo->getVkHandler(),
			.renderArea = renderArea,
			.clearValueCount = static_cast<u32>(clearValues.size()),
			.pClearValues = clearValues.data()
		};

		commandBuffer->getVkHandler().beginRenderPass
		(
			beginInfo, vk::SubpassContents::eInline
		);

		m_state = State::InRenderPass;
	}

	auto VRenderPass::end(SPtr<const VCommandBuffer> commandBuffer) -> void
	{
		commandBuffer->getVkHandler().endRenderPass();

		m_state = State::Recording;
	}

	auto VRenderPass::getVkHandler() const -> const vk::RenderPass&
	{
		return m_vkHandler;
	}
}