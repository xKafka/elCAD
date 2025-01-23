#include "renderer/vulkan/VSwapchain.hpp"
#include "renderer/vulkan/VQueue.hpp"

#include "window/Window.hpp"

#include <spdlog/spdlog.h>
#include <glm/glm.hpp>

namespace elcad::renderer
{
	static constexpr u32 MaxFramesInFlight{ 2 }; //triple buffering, 2 frames are being rendered to 

	VSwapChain::VSwapChain(SPtr<const win::Window> window, vk::SurfaceKHR surface, SPtr<const VLogicalDevice> logicalDevice, SPtr<VAllocationCallbacks>	vkAllocator)
		: m_window{ window }
		, m_surface{ surface }
		, m_logicalDevice{ logicalDevice }
		, m_vkAllocator{ vkAllocator }
	{
		create(std::nullopt /*no old swapchain*/);
	}
	
	VSwapChain::~VSwapChain()
	{
	}

	auto VSwapChain::recreate() -> void
	{
		auto oldSwapChain = std::make_optional<vk::SwapchainKHR>
		(
			m_vkHandler
		);

		m_recreating = true;

		destroy();

		create(oldSwapChain);

		m_recreating = false;
	}

	auto VSwapChain::acquireNextImageIndex(const vk::Semaphore& imageAvailableSemaphore, const vk::Fence& fence, u64 timeoutNs) -> Opt<u32>
	{
		auto [result, imageIndex] = m_logicalDevice->getVkHandler().acquireNextImageKHR
		(
			m_vkHandler, timeoutNs, imageAvailableSemaphore, fence
		);

		if (result == vk::Result::eErrorOutOfDateKHR)
		{
			recreate();

			return std::nullopt;
		}

		if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
		{
			spdlog::critical("Failed to acquire SwapChain image");

			return std::nullopt;
		}

		return imageIndex;
	}

	auto VSwapChain::present(SPtr<const VQueue> graphics, SPtr<const VQueue> present, const vk::Semaphore& renderCompleteSemaphore, u32 imageIndex) -> void
	{
		const auto presentInfo = vk::PresentInfoKHR
		{
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = &renderCompleteSemaphore,
			.swapchainCount = 1,
			.pSwapchains = &m_vkHandler,
			.pImageIndices = &imageIndex
		};

		const auto result = present->present
		(
			presentInfo
		);

		if (result != vk::Result::eSuccess)
		{
			throw std::runtime_error{ "Failed to present swap chain image" };
		}

		if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR)
		{
			recreate();
		}
	}

	auto VSwapChain::getSupportedFormat() -> vk::SurfaceFormatKHR
	{
		auto formats = m_logicalDevice->getPhysicalDevice()->getSwapchainFormats
		(
			m_surface
		);

		if (formats.empty())
		{
			throw std::runtime_error("No surface formats available.");
		}

		for (const auto& format : formats)
		{
			if (format.format == vk::Format::eB8G8R8A8Unorm && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
			{
				return format;
			}
		}

		return formats.front();
	}

	auto VSwapChain::getPresentMode() -> vk::PresentModeKHR
	{
		auto presentModes = m_logicalDevice->getPhysicalDevice()->getSwapchainPresentModes
		(
			m_surface
		);

		for (const auto& presentMode : presentModes)
		{
			if (presentMode == vk::PresentModeKHR::eMailbox)
			{
				return presentMode;
			}
		}

		return vk::PresentModeKHR::eFifo; //default
	}

	auto VSwapChain::getClampedExtent(u32 width, u32 height) -> vk::Extent2D
	{
		auto capabilities = m_logicalDevice->getPhysicalDevice()->getSwapchainCapabilities
		(
			m_surface
		);

		if (capabilities.currentExtent.width != std::numeric_limits<u32>::max())
		{
			return capabilities.currentExtent;
		}

		return vk::Extent2D
		{
			.width = std::clamp(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
			.height = std::clamp(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
		};
	}

	auto VSwapChain::getMinimalImageCount() -> u32
	{
		auto capabilities = m_logicalDevice->getPhysicalDevice()->getSwapchainCapabilities
		(
			m_surface
		);

		const u32 imageCount = capabilities.minImageCount + 1;

		if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
		{
			return capabilities.maxImageCount;
		}

		return imageCount;
	}

	auto VSwapChain::getSurfaceTransform() -> vk::SurfaceTransformFlagBitsKHR
	{
		auto capabilities = m_logicalDevice->getPhysicalDevice()->getSwapchainCapabilities
		(
			m_surface
		);

		return capabilities.currentTransform;
	}

	auto VSwapChain::setImageSharingModeExclusive(vk::SwapchainCreateInfoKHR* createInfo) -> void
	{
		createInfo->setImageSharingMode(vk::SharingMode::eExclusive);

		createInfo->setQueueFamilyIndexCount(0);

		createInfo->setPQueueFamilyIndices(0);
	}

	auto VSwapChain::setImageSharingModeConcurrent(vk::SwapchainCreateInfoKHR* createInfo) -> void
	{
		const auto graphicsQueue = m_logicalDevice->getGraphicsQueue();

		const auto presentQueue = m_logicalDevice->getPresentQueue();

		auto familyIndices = std::array
		{
			graphicsQueue->getFamilyIndex(), presentQueue->getFamilyIndex()
		};

		createInfo->setImageSharingMode(vk::SharingMode::eConcurrent);

		createInfo->setQueueFamilyIndexCount(familyIndices.size());

		createInfo->setPQueueFamilyIndices(familyIndices.data());
	}

	auto VSwapChain::createSwapChain(Opt<vk::SwapchainKHR> oldSwapChain) -> void
	{
		auto createInfo = vk::SwapchainCreateInfoKHR
		{
			.surface = m_surface,
			.imageFormat = m_vkFormat.format,
			.imageColorSpace = m_vkFormat.colorSpace,
			.imageExtent = m_extent,
			.imageArrayLayers = 1,
			.imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
			.imageSharingMode = vk::SharingMode::eExclusive,
			.clipped = true,
			.oldSwapchain = oldSwapChain ? oldSwapChain.value() : vk::SwapchainKHR{}
		};

		createInfo.setPreTransform
		(
			getSurfaceTransform()
		);

		createInfo.setPresentMode
		(
			getPresentMode()
		);

		createInfo.setMinImageCount
		(
			getMinimalImageCount()
		);

		const auto gQueue = m_logicalDevice->getGraphicsQueue();

		const auto pQueue = m_logicalDevice->getPresentQueue();

		if (gQueue->getVkHandler() != pQueue->getVkHandler())
		{
			setImageSharingModeConcurrent(&createInfo);
		}
		else
		{
			setImageSharingModeExclusive(&createInfo);
		}

		m_vkHandler = m_logicalDevice->getVkHandler().createSwapchainKHR
		(
			createInfo, *m_vkAllocator
		);

		if (oldSwapChain)
		{
			m_logicalDevice->getVkHandler().destroySwapchainKHR
			(
				oldSwapChain.value(), *m_vkAllocator
			);
		}
	}

	auto VSwapChain::createImages() -> void
	{
		m_swapChainImages = m_logicalDevice->getVkHandler().getSwapchainImagesKHR
		(
			m_vkHandler
		);

		m_swapChainImageViews.resize
		(
			m_swapChainImages.size()
		);

		for (u32 i{ 0 }; i < static_cast<u32>(m_swapChainImages.size()); ++i)
		{
			auto subResourceRange = vk::ImageSubresourceRange
			{
				.aspectMask = vk::ImageAspectFlagBits::eColor,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			};

			auto createInfo = vk::ImageViewCreateInfo
			{
				.image = m_swapChainImages[i],
				.viewType = vk::ImageViewType::e2D,
				.format = m_vkFormat.format,
				.subresourceRange = subResourceRange
			};

			m_swapChainImageViews[i] = m_logicalDevice->getVkHandler().createImageView
			(
				createInfo, *m_vkAllocator
			);
		}
	}

	auto VSwapChain::loadSurfaceFormat() -> void
	{
		m_vkFormat = getSupportedFormat();
	}

	auto VSwapChain::loadExtent() -> void
	{
		m_extent = getClampedExtent
		(
			m_window->getWidth(), m_window->getHeight()
		);
	}

	auto VSwapChain::create(Opt<vk::SwapchainKHR> oldSwapChain) -> void
	{
		loadExtent();

		loadSurfaceFormat();

		createSwapChain(oldSwapChain);

		createImages();

		spdlog::info("Swapchain created successfully");
	}

	auto VSwapChain::destroy() -> void
	{
		auto vkLogical = m_logicalDevice->getVkHandler();

		vkLogical.waitIdle();

		for (auto& view : m_swapChainImageViews)
		{
			vkLogical.destroyImageView
			(
				view, *m_vkAllocator
			);

			view = nullptr;
		}

		if (m_vkHandler)
		{
			vkLogical.destroySwapchainKHR
			(
				m_vkHandler, *m_vkAllocator
			);

			m_vkHandler = nullptr;
		}
	}

	auto VSwapChain::getImageColorSpace() const -> vk::ColorSpaceKHR
	{
		return m_vkFormat.colorSpace;
	}

	auto VSwapChain::getImageFormat() const -> vk::Format
	{
		return m_vkFormat.format;
	}

	auto VSwapChain::getImageExtent() const -> vk::Extent2D
	{
		return m_extent;
	}

	auto VSwapChain::getImageViews() const -> Span<const vk::ImageView>
	{
		return m_swapChainImageViews;
	}

	auto VSwapChain::getImageCount() const -> u32
	{
		return static_cast<u32>(m_swapChainImages.size());
	}

	auto VSwapChain::getMaxFramesInFlight() const -> u32
	{
		return MaxFramesInFlight;
	}

	auto VSwapChain::isRecreating() const -> bool
	{
		return m_recreating;
	}
}