#pragma once

#include "renderer/IBackend.hpp"

#include "VContext.hpp"
#include "VCommandBuffer.hpp"
#include "VSwapchain.hpp"
#include "VRenderPass.hpp"
#include "VFrameBuffer.hpp"
#include "VSemaphore.hpp"
#include "VFence.hpp"

namespace elcad::renderer
{
	class VBackend final : public IBackend
	{
		auto initContext(StringView appName, u32 version) -> void;

		auto initRenderPass() -> void;

		auto initSwapChain() -> void;

		auto initFrameBuffers() -> void;

		auto initCommandBuffers() -> void;

		auto initSemaphores() -> void;
		
		auto initFences() -> void;

		auto initSyncObjects() -> void;

		auto recreateSwapChain() -> void;

		auto regenerateFramebuffers() -> void;

		auto resetCommandBuffers() -> void;

		auto destroyFrameBuffers() -> void;

		auto destroyCommandBuffers() -> void;

		auto destroySemaphores() -> void;

		auto destroyFences() -> void;

		auto destroyRenderPass() -> void;

		auto destroySwapChain() -> void;

		auto destroyContext() -> void;

		auto waitIdle() -> void;

		auto createWindowViewPort() -> vk::Viewport;

		auto createWindowScissors() -> vk::Rect2D;

		auto setMainWindow(SPtr<const win::Window> window) -> void;

	public:
		VBackend();

		~VBackend() = default;

		auto init(StringView appName, u32 version, SPtr<const win::Window> window) -> void;

		auto shutdown() -> void;

		auto resize(u32 width, u32 height) -> void;

		auto beginFrame(f32 delta) -> bool;

		auto endFrame(f32 delta) -> bool;

	private:
		SPtr<const win::Window>		m_mainWindow{};

		SPtr<VContext>				m_context{};

		vk::SurfaceKHR				m_mainWindowSurface{};

		SPtr<VSwapChain>			m_mainWindowSwapChain{};

		SPtr<VRenderPass>			m_mainRenderPass{};

		Vec<SPtr<VCommandBuffer>>	m_graphicsCommandBuffers{};

		Vec<SPtr<VFrameBuffer>>		m_swapChainFrameBuffers{};

		Vec<SPtr<VSemaphore>>		m_imageAvailableSemaphores{};

		Vec<SPtr<VSemaphore>>		m_renderFinishedSemaphores{};

		Vec<SPtr<VFence>>			m_inFlightFences{};

		u32							m_currentFrame{};

		u32							m_currentImageIndex{};
	};
}