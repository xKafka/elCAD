#pragma once

#include <functional>

#include "VContext.hpp"
#include "VImage.hpp"
#include "VLogicalDevice.hpp"
#include "VPhysicalDevice.hpp"
#include "VSemaphore.hpp"
#include "VFence.hpp"

namespace elcad::renderer
{
	class VSwapChain
	{
		auto getSupportedFormat() -> vk::SurfaceFormatKHR;

		auto getPresentMode() -> vk::PresentModeKHR;

		auto getClampedExtent(u32 width, u32 height) -> vk::Extent2D;

		auto getMinimalImageCount() -> u32;

		auto getSurfaceTransform() -> vk::SurfaceTransformFlagBitsKHR;

		auto setImageSharingModeExclusive(vk::SwapchainCreateInfoKHR* createInfo) -> void;

		auto setImageSharingModeConcurrent(vk::SwapchainCreateInfoKHR* createInfo) -> void;

		auto setImageSharingMode(vk::SwapchainCreateInfoKHR* createInfo) -> void;

		auto loadSurfaceFormat() -> void;

		auto loadExtent() -> void;

		auto initCreateSwapChainInfo() -> vk::SwapchainCreateInfoKHR;

		auto createSwapChain(Opt<vk::SwapchainKHR> oldSwapChain) -> void;

		auto createImages() -> void;

		auto create(Opt<vk::SwapchainKHR> oldSwapChain) -> void;

	public:
		VSwapChain(SPtr<const win::Window> window, vk::SurfaceKHR surface, SPtr<const VLogicalDevice> logicalDevice, SPtr<VAllocationCallbacks>	vkAllocator);

		~VSwapChain();

		auto recreate() -> void;

		auto destroy() -> void;

		auto acquireNextImageIndex(const vk::Semaphore& imageAvailableSemaphore, const vk::Fence& fence, u64 timeoutNs) -> Opt<u32>;

		auto present(SPtr<const VQueue> graphics, SPtr<const VQueue> present, const vk::Semaphore& renderCompleteSemaphore, u32 imageIndex) -> void;

		[[nodiscard]] auto getImageColorSpace() const -> vk::ColorSpaceKHR;

		[[nodiscard]] auto getImageFormat() const -> vk::Format;

		[[nodiscard]] auto getImageExtent() const -> vk::Extent2D;

		[[nodiscard]] auto getImageViews() const -> Span<const vk::ImageView>;

		[[nodiscard]] auto getImageCount() const -> u32;

		[[nodiscard]] auto getMaxFramesInFlight() const -> u32;
		
		[[nodiscard]] auto isRecreating() const -> bool;

	private:
		SPtr<VAllocationCallbacks>	m_vkAllocator{};

		SPtr<const win::Window>		m_window{};
		
		SPtr<const VLogicalDevice>	m_logicalDevice{};

		vk::SurfaceKHR				m_surface{};

		vk::SwapchainKHR			m_vkHandler{};

		//image format and images aquired by a swapchain
		vk::SurfaceFormatKHR		m_vkFormat{};

		vk::Extent2D				m_extent{};

		Vec<vk::Image>				m_swapChainImages{};

		Vec<vk::ImageView>			m_swapChainImageViews{};

		bool						m_recreating{};
	};
}