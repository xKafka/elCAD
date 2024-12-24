#include "renderer/vulkan/VBackend.hpp"
#include "renderer/vulkan/VShader.hpp"

#include <window/Window.hpp>
#include <model/SystemPaths.hpp>

#include <spdlog/spdlog.h>

#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>

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

		initPipelineCache();

		initPipelineLayout();

		initPipeline();

		initFrameBuffers();

		initCommandBuffers();

		initSyncObjects();
		
		initImgui(window);
	}

	auto VBackend::renderTest() -> void
	{
		auto vertices = Vec<model::Vertex>
		{
			model::Vertex
			{
				.position = { 1.0f,  1.0f, 0.0f }
			},
			model::Vertex
			{
				.position = { -1.0f,  1.0f, 0.0f }
			},
			model::Vertex
			{
				.position = { 0.0f, -1.0f, 0.0f }
			}
		};

		auto indices = Vec<u32>
		{
			0, 1, 2
		};

		auto buffer = makeUnique<VVertexBuffer>
		(
			m_context->getLogicalDevice(), m_context->getAllocator()
		);

		buffer->create
		(
			m_context->getGraphicsPool(), vertices, indices
		);

		m_vertexBuffer.emplace_back(std::move(buffer));
	}

	static vk::DescriptorPool         m_descriptorPool = VK_NULL_HANDLE;


	auto VBackend::initImgui(SPtr<const win::Window> window) -> void
	{
		auto poolSizes = std::array
		{
			vk::DescriptorPoolSize
			{
				vk::DescriptorType::eCombinedImageSampler, 1
			}
		};

		vk::DescriptorPoolCreateInfo poolInfo
		{
			.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
			.maxSets = 1,
			.poolSizeCount = static_cast<u32>(poolSizes.size()),
			.pPoolSizes = poolSizes.data()
		};

		m_descriptorPool = m_context->getLogicalDevice()->getVkHandler().createDescriptorPool
		(
			poolInfo, *m_context->getAllocator()
		);

		// Initialize ImGui context
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;

		ImGui::StyleColorsDark();

		// Initialize ImGui for GLFW and Vulkan
		ImGui_ImplGlfw_InitForVulkan(const_cast<GLFWwindow*>(window->getHandler()), true);

		// Vulkan initialization info
		auto initInfo = ImGui_ImplVulkan_InitInfo{};
		initInfo.Instance = m_context->getInstance()->getVkHandler();
		initInfo.PhysicalDevice = m_context->getPhysicalDevice()->getVkHandler();
		initInfo.Device = m_context->getLogicalDevice()->getVkHandler();
		initInfo.QueueFamily = m_context->getLogicalDevice()->getGraphicsQueue()->getFamilyIndex();
		initInfo.Queue = m_context->getLogicalDevice()->getGraphicsQueue()->getVkHandler();
		initInfo.DescriptorPool = m_descriptorPool;
		initInfo.Subpass = 0; // Assuming subpass 0 is used for rendering ImGui
		initInfo.MinImageCount = m_mainWindowSwapChain->getMaxFramesInFlight();
		initInfo.ImageCount = m_mainWindowSwapChain->getImageCount();
		initInfo.RenderPass = m_mainRenderPass->getVkHandler();
		initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT; // Match your render pass MSAA settings
		initInfo.CheckVkResultFn = [](VkResult result) {
			if (result != VK_SUCCESS) {
				throw std::runtime_error("Vulkan error occurred in ImGui initialization!");
			}
			};

		ImGui_ImplVulkan_Init(&initInfo);
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

	auto VBackend::initPipelineCache() -> void
	{
		spdlog::info("Creating Pipeline cache...");

		m_pipelineCache = makeShared<VPipelineCache>
		(
			m_context->getLogicalDevice(), m_context->getAllocator()
		);

		spdlog::info("Pipeline cache created.");
	}

	auto VBackend::initPipelineLayout() -> void
	{
		spdlog::info("Creating Pipeline layout...");

		m_pipelineLayout = makeShared<VPipelineLayout>
		(
			m_context->getLogicalDevice(), m_context->getAllocator()
		);

		m_pipelineLayout->create();

		spdlog::info("Pipeline layout created.");
	}

	auto VBackend::initPipeline() -> void
	{
		spdlog::info("Creating Pipeline...");

		auto vertexShader = makeShared<VShader>
		(
			m_context->getLogicalDevice(), m_context->getAllocator()
		);

		vertexShader->createFromFile
		(
			model::path::getResourcePath("/shaders/simple_vertex.spv")
		);

		auto fragmentShader = makeShared<VShader>
		(
			m_context->getLogicalDevice(), m_context->getAllocator()
		);

		fragmentShader->createFromFile
		(
			model::path::getResourcePath("/shaders/simple_frag.spv")
		);

		m_pipeline = makeShared<VPipeline>
		(
			m_context->getLogicalDevice(), m_context->getAllocator()
		);

		m_pipeline->create
		(
			m_mainRenderPass, m_pipelineCache, m_pipelineLayout, vertexShader, fragmentShader
		);

		vertexShader->destroy();

		fragmentShader->destroy();

		spdlog::info("Pipeline created.");
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

	auto VBackend::destroyPipelineCache() -> void
	{
		spdlog::info("Destroying Pipeline cache...");

		m_pipelineCache->destroy();

		spdlog::info("Pipeline cache destroyed");
	}

	auto VBackend::destroyPipelineLayout() -> void
	{
		spdlog::info("Destroying Pipeline layout...");

		m_pipelineLayout->destroy();

		spdlog::info("Pipeline layout destroyed");
	}

	auto VBackend::destroyPipeline() -> void
	{
		spdlog::info("Destroying Pipeline...");

		m_pipeline->destroy();

		spdlog::info("Pipeline destroyed");
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

		m_context->getInstance()->destroySurface(m_mainWindowSurface);

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
			.y = 0.0f,
			.width = width,
			.height = height,
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

	auto VBackend::renderWindowUi(SPtr<const win::Window> window, SPtr<const VCommandBuffer> commandBuffer) -> void
	{
		ImGui_ImplGlfw_NewFrame();

		ImGui_ImplVulkan_NewFrame();

		ImGui::NewFrame();

		ImGui::Begin("main window");

		window->render();

		ImGui::End();

		ImGui::Render();

		ImGui_ImplVulkan_RenderDrawData
		(
			ImGui::GetDrawData(), commandBuffer->getVkHandler()
		);
	}

	auto VBackend::shutdown() -> void
	{
		waitIdle();

		destroySemaphores();

		destroyFences();

		destroyFrameBuffers();

		destroyCommandBuffers();

		destroyPipelineCache();

		destroyPipelineLayout();

		destroyPipeline();

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

	auto VBackend::renderCameraNode(const CameraNode& cameraNode) -> void
	{

	}

	auto VBackend::renderRenderNode(const RenderNode& renderNode) -> void
	{

	}

	auto VBackend::renderScene(SPtr<Scene> scene) -> void
	{
		for (auto& node : scene->getNodes())
		{
			std::visit
			(
				[self = this](auto&& visitedNode)
				{
					using NodeType = std::decay_t<decltype(visitedNode)>;

					if constexpr (std::is_same_v<NodeType, CameraNode>)
					{
						self->renderCameraNode(visitedNode);
					}
					else if constexpr (std::is_same_v<NodeType, RenderNode>)
					{
						self->renderRenderNode(visitedNode);
					}
					else
					{
						static_assert(always_false<NodeType>::value, "Unhandled node type");
					}
				},

				node
			);
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

		const auto offset = glm::ivec2
		{
			0, 0 
		};

		const auto extent = glm::uvec2
		{ 
			m_mainWindow->getWidth(), m_mainWindow->getHeight() 
		};

		const auto clearColor = glm::vec4
		{
			0.0f, 0.4f, 0.2f, 1.0f 
		};

		m_mainRenderPass->begin
		(
			commandBuffer, m_swapChainFrameBuffers[m_currentImageIndex], offset, extent, clearColor, 1.0f, 0
		);

		commandBuffer->getVkHandler().bindPipeline
		(
			vk::PipelineBindPoint::eGraphics, m_pipeline->getVkHandler()
		);

		commandBuffer->getVkHandler().setViewport
		(
			0, createWindowViewPort()
		);

		commandBuffer->getVkHandler().setScissor
		(
			0, createWindowScissors()
		);

		commandBuffer->getVkHandler().draw
		(
			3, 1, 0, 0
		);

		renderWindowUi(m_mainWindow, commandBuffer);

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