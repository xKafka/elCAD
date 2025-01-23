#pragma once

#include <functional>

#include "VContext.hpp"
#include "VCommandBuffer.hpp"
#include "VSwapChain.hpp"

#include <glm/glm.hpp>

namespace elcad::renderer
{
	class VFrameBuffer;
}

namespace elcad::renderer
{
	class VRenderPass
	{
		auto create() -> void;

		auto setClearColor(const glm::vec4& clearColor) -> void;

		auto setDepthStencilClear(f32 depth, u32 stencil) -> void;

		auto setRenderOffset(const glm::vec2& renderOffset) -> void;

		auto setRenderExtent(const glm::vec2& renderExtent) -> void;

	public:
		enum State : u32
		{
			NotAllocated,
			Ready = 0,
			Recording,
			InRenderPass,
			RecordingEnded,
			Submitted
		};

		VRenderPass(SPtr<const VLogicalDevice> logicalDevice, SPtr<const VSwapChain> swapChain, SPtr<VAllocationCallbacks> vkAllocator);

		auto destroy() -> void;

		auto begin(SPtr<const VCommandBuffer> commandBuffer, SPtr<const VFrameBuffer> fbo, const glm::ivec2 offset, const glm::uvec2 extent, const glm::vec4 color, f32 depth, u8 stencil) -> void;

		auto end(SPtr<const VCommandBuffer> commandBuffer) -> void;

		[[nodiscard]] auto getVkHandler() const -> const vk::RenderPass&;

	private:
		SPtr<const VLogicalDevice>	m_logicalDevice{};

		SPtr<const VSwapChain>		m_swapChain{};

		SPtr<VAllocationCallbacks>	m_vkAllocator{};

		vk::RenderPass				m_vkHandler{};

		State						m_state{};

	};
}
