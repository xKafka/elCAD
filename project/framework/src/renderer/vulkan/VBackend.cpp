#include "renderer/vulkan/VBackend.hpp"

#include <window/Window.hpp>

#include <spdlog/spdlog.h>

namespace elcad::renderer
{
	VBackend::VBackend()
	{
	}

	auto VBackend::init(StringView appName, u32 version, SPtr<const win::Window> window) -> void
	{
		setMainWindow(window);

		initContext(appName, version);

		initSwapChain();

		initRenderPass();

		initFrameBuffers();

		initCommandBuffers();

		initSyncObjects();
	}

	auto VBackend::setMainWindow(SPtr<const win::Window> window) -> void
	{
		if (!window)
		{
			throw std::runtime_error{ "window can't be empty" };
		}

		m_mainWindow = window;
	}

	auto VBackend::initContext(StringView appName, u32 version) -> void
	{
		spdlog::info("Initializing Vulkan Context...");

		m_context = makeShared<VContext>
		(
			appName, version
		);

		m_mainWindowSurface = m_context->createVulkanSurface(m_mainWindow);

		m_context->initWithWindow(m_mainWindowSurface);

		spdlog::info("Vulkan Context initialized.");
	}

	auto VBackend::initSwapChain() -> void
	{
		spdlog::info("Initializing SwapChain...");

		m_mainWindowSwapChain = makeShared<VSwapChain>
		(
			m_mainWindow, m_mainWindowSurface, m_context->getLogicalDevice(), m_context->getAllocator()
		);

		spdlog::info("SwapChain initialized.");
	}

	auto VBackend::initRenderPass() -> void
	{
		spdlog::info("Initializing Render Pass...");

		m_mainRenderPass = makeShared<VRenderPass>
		(
			m_context->getLogicalDevice(), m_mainWindowSwapChain, m_context->getAllocator()
		);

		spdlog::info("Render Pass initialized.");
	}

	auto VBackend::initFrameBuffers() -> void
	{
		spdlog::info("Initializing Frame Buffers...");

		auto imageViews = m_mainWindowSwapChain->getImageViews();

		m_swapChainFrameBuffers.reserve
		(
			imageViews.size()
		);

		for (auto& view : imageViews)
		{
			auto fbo = makeShared<VFrameBuffer>
			(
				m_context->getLogicalDevice(), m_mainRenderPass, m_mainWindow->getWidth(), m_mainWindow->getHeight(), Vec<vk::ImageView>{ view }, m_context->getAllocator()
			);

			m_swapChainFrameBuffers.emplace_back(fbo);
		}

		spdlog::info("Frame Buffers initialized.");
	}

	auto VBackend::initCommandBuffers() -> void
	{
		spdlog::info("Creating Command Buffers...");

		m_graphicsCommandBuffers.resize
		(
			m_mainWindowSwapChain->getImageCount()
		);

		for (auto& buffer : m_graphicsCommandBuffers)
		{
			buffer = makeShared<VCommandBuffer>
			(
				m_context->getLogicalDevice(), m_context->getGraphicsPool()
			);

			buffer->allocate(true);
		}

		spdlog::info("Command Buffers created for SwapChain images.");
	}

	auto VBackend::initSemaphores() -> void
	{
		spdlog::info("Creating Semaphores...");

		const u32 imageCount = m_mainWindowSwapChain->getImageCount();

		m_imageAvailableSemaphores.resize(imageCount);

		m_renderFinishedSemaphores.resize(imageCount);

		for (u32 i{ 0 }; i < imageCount; ++i)
		{
			m_imageAvailableSemaphores[i] = makeShared<VSemaphore>
			(
				m_context->getLogicalDevice(), m_context->getAllocator()
			);

			m_renderFinishedSemaphores[i] = makeShared<VSemaphore>
			(
				m_context->getLogicalDevice(), m_context->getAllocator()
			);
		}

		spdlog::info("Semaphores created.");
	}

	auto VBackend::initFences() -> void
	{
		spdlog::info("Creating Fences...");

		m_inFlightFences.resize
		(
			m_mainWindowSwapChain->getImageCount()
		);

		for (auto& fence : m_inFlightFences)
		{
			fence = makeShared<VFence>
			(
				true, m_context->getLogicalDevice(), m_context->getAllocator()
			);
		}

		spdlog::info("Fences created.");
	}

	auto VBackend::initSyncObjects() -> void
	{
		spdlog::info("Creating Sync objects...");

		initSemaphores();

		initFences();

		spdlog::info("Sync objects created.");
	}

	auto VBackend::destroyFrameBuffers() -> void
	{
		spdlog::info("Destroying Framebuffers...");

		for (auto& fbo : m_swapChainFrameBuffers)
		{
			fbo->destroy();
		}

		m_swapChainFrameBuffers.clear();

		spdlog::info("Framebuffers destroyed...");
	}

	auto VBackend::regenerateFramebuffers() -> void
	{
		spdlog::info("Regenerating Framebuffers...");

		destroyFrameBuffers();

		initFrameBuffers();

		spdlog::info("Framebuffers regenerated.");
	}

	auto VBackend::recreateSwapChain() -> void
	{
		if (m_mainWindowSwapChain->isRecreating())
		{
			return;
		}

		waitIdle();

		m_mainWindowSwapChain->recreate();

		regenerateFramebuffers();

		resetCommandBuffers();
	}

	auto VBackend::destroyCommandBuffers() -> void
	{
		spdlog::info("Destroying Command Buffers...");

		for (auto& buffer : m_graphicsCommandBuffers)
		{
			if (buffer)
			{
				buffer->free();
			}
		}

		m_graphicsCommandBuffers.clear();

		spdlog::info("Command Buffers destroyed...");
	}

	auto VBackend::destroySemaphores() -> void
	{
		spdlog::info("Destroying Semaphores...");

		for (auto& semaphore : m_imageAvailableSemaphores)
		{
			semaphore->destroy();
		}

		m_imageAvailableSemaphores.clear();

		for (auto& semaphore : m_renderFinishedSemaphores)
		{
			semaphore->destroy();
		}

		m_renderFinishedSemaphores.clear();

		spdlog::info("Semaphores destroyed");
	}

	auto VBackend::destroyFences() -> void
	{
		spdlog::info("Destroying Fences...");

		for (auto& fence : m_inFlightFences)
		{
			fence->destroy();
		}

		m_inFlightFences.clear();

		spdlog::info("Fences destroyed");
	}

	auto VBackend::destroyRenderPass() -> void
	{
		spdlog::info("Destroying RenderPass...");

		m_mainRenderPass->destroy();

		spdlog::info("RenderPass destroyed");
	}

	auto VBackend::destroySwapChain() -> void
	{
		spdlog::info("Destroying SwapChain...");

		m_mainWindowSwapChain->destroy();

		spdlog::info("SwapChain destroyed");
	}

	auto VBackend::destroyContext() -> void
	{
		spdlog::info("Destroying Context...");

		m_context->destroy();

		spdlog::info("Context destroyed");
	}

	auto VBackend::resetCommandBuffers() -> void
	{
		spdlog::info("Resetting Command Buffers...");

		destroyCommandBuffers();

		initCommandBuffers();

		spdlog::info("Command Buffers reset and reallocated.");
	}

	auto VBackend::waitIdle() -> void
	{
		auto logicalDevice = m_context->getLogicalDevice();

		logicalDevice->getVkHandler().waitIdle();
	}

	auto VBackend::createWindowViewPort() -> vk::Viewport
	{
		auto width = static_cast<f32>(m_mainWindow->getWidth());

		auto height = static_cast<f32>(m_mainWindow->getHeight());

		return vk::Viewport
		{
			.x = 0.0f,
			.y = height,
			.width = width,
			.height = -1.0f * height,
			.minDepth = 0.0f,
			.maxDepth = 1.0f
		};
	}

	auto VBackend::createWindowScissors() -> vk::Rect2D
	{
		auto width = m_mainWindow->getWidth();

		auto height = m_mainWindow->getHeight();

		return vk::Rect2D
		{
			.offset = { 0, 0 },
			.extent = { width, height }
		};
	}

	auto VBackend::shutdown() -> void
	{
		waitIdle();

		destroySemaphores();

		destroyFences();

		destroyFrameBuffers();

		destroyCommandBuffers();

		destroyRenderPass();

		destroySwapChain();

		destroyContext();
	}

	auto VBackend::resize(u32 width, u32 height) -> void
	{
		spdlog::info("Vulkan renderer backend->resized: w/h", width, height);

		if (width > 0 && height > 0)
		{
			recreateSwapChain();
		}
	}

	auto VBackend::beginFrame(f32 delta) -> bool
	{
		if (m_mainWindowSwapChain->isRecreating())
		{
			waitIdle();
		}

		const auto fenceOk = m_inFlightFences[m_currentFrame]->wait
		(
			std::numeric_limits<u64>::max()
		);

		if (!fenceOk)
		{
			spdlog::error("Failed to wait for fences!");

			return false;
		}

		m_inFlightFences[m_currentFrame]->reset();

		auto nextImage = m_mainWindowSwapChain->acquireNextImageIndex
		(
			m_imageAvailableSemaphores[m_currentFrame]->getVkHandler(),
			{},
			0
		);

		if (!nextImage)
		{
			spdlog::error("Failed to acquire swap chain image!");

			return false;
		}

		m_currentImageIndex = nextImage.value();

		auto commandBuffer = m_graphicsCommandBuffers[m_currentImageIndex];

		commandBuffer->reset();

		commandBuffer->begin(false, false, false);

		commandBuffer->getVkHandler().setViewport
		(
			0, createWindowViewPort()
		);

		commandBuffer->getVkHandler().setScissor
		(
			0, createWindowScissors()
		);

		const auto offset = glm::ivec2{ 0, 0 };

		const auto extent = glm::uvec2{ m_mainWindow->getWidth(), m_mainWindow->getHeight() };

		const auto clearColor = glm::vec4{ 0.0f, 0.4f, 0.2f, 1.0f };

		m_mainRenderPass->begin
		(
			commandBuffer, m_swapChainFrameBuffers[m_currentImageIndex], offset, extent, clearColor, 1.0f, 0
		);

		return true;
	}

	auto VBackend::endFrame(f32 delta) -> bool
	{
		auto commandBuffer = m_graphicsCommandBuffers[m_currentImageIndex];

		m_mainRenderPass->end
		(
			commandBuffer
		);

		commandBuffer->end();

		auto flags = std::array<vk::PipelineStageFlags, 1>{ vk::PipelineStageFlagBits::eColorAttachmentOutput };

		auto commandBuffers = Vec<vk::CommandBuffer>{ commandBuffer->getVkHandler() };

		auto waitSemaphores = Vec<vk::Semaphore>{ m_imageAvailableSemaphores[m_currentFrame]->getVkHandler() };

		auto signalSemaphores = Vec<vk::Semaphore>{ m_renderFinishedSemaphores[m_currentFrame]->getVkHandler() };

		auto submitInfo = vk::SubmitInfo
		{
			.waitSemaphoreCount = static_cast<u32>(waitSemaphores.size()),
			.pWaitSemaphores = waitSemaphores.data(),
			.pWaitDstStageMask = flags.data(),
			.commandBufferCount = static_cast<u32>(commandBuffers.size()),
			.pCommandBuffers = commandBuffers.data(),
			.signalSemaphoreCount = static_cast<u32>(signalSemaphores.size()),
			.pSignalSemaphores = signalSemaphores.data(),
		};

		auto logicalDevice = m_context->getLogicalDevice();

		logicalDevice->getGraphicsQueue()->getVkHandler().submit
		(
			{ submitInfo }, m_inFlightFences[m_currentFrame]->getVkHandler()
		);

		commandBuffer->updateSubmitted();

		m_mainWindowSwapChain->present
		(
			logicalDevice->getGraphicsQueue(), logicalDevice->getPresentQueue(), m_renderFinishedSemaphores[m_currentFrame]->getVkHandler(), m_currentImageIndex
		);

		m_currentFrame = (m_currentFrame + 1) % m_mainWindowSwapChain->getMaxFramesInFlight();

		return true;
	}
}