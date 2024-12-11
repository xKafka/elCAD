#include "renderer/vulkan/VContext.hpp"
#include "window/Window.hpp"

#include <spdlog/spdlog.h>
#include <array>

namespace elcad::renderer
{
	VContext::VContext(StringView name, u32 version)
		: m_vkAllocator{ nullptr }
	{
		createInstance(name, version);

		loadPhysicalDevices();
	}

	auto VContext::initWithWindow(const vk::SurfaceKHR& surface) -> void
	{
		chooseBestPhysicalDevice(surface);

		createLogicalDevice(surface);

		createGraphicsCommandPool();
	}

	auto VContext::destroy() -> void
	{
		m_graphicsPool->destroy();

		m_logicalDevice->destroy();

		m_instance->destroy();
	}

	auto VContext::createInstance(StringView name, u32 version) -> void
	{
		m_instance = makeShared<VInstance>
		(
			name, version, m_vkAllocator
		);
	}

	auto VContext::loadPhysicalDevices() -> void
	{
		m_availablePhysicalDevices = m_instance->loadPhysicalDevices();
	}

	auto VContext::createVulkanSurface(SPtr<const win::Window> window) -> vk::SurfaceKHR
	{
		return m_instance->loadSurface(window);
	}

	auto VContext::isPhysicalDeviceGood(SPtr<const VPhysicalDevice> physicalDevice, const vk::SurfaceKHR& surface) const -> bool
	{
		if (!physicalDevice->isDiscrete())
		{
			spdlog::info("Device is not a discrete GPU, and one is required");

			return false;
		}

		auto queueFamilies = physicalDevice->getBestQueueFamilies(surface);

		if (!queueFamilies.graphics.has_value())
		{
			spdlog::info("Required graphics index family not present");

			return false;
		}

		if (!queueFamilies.present.has_value())
		{
			spdlog::info("Required present index family not present");

			return false;
		}

		if (!queueFamilies.compute.has_value())
		{
			spdlog::info("Required compute index family not present");

			return false;
		}

		if (!queueFamilies.transfer.has_value())
		{
			spdlog::info("Required transfer index family not present");

			return false;
		}

		static constexpr auto RequiredExtensions = std::array
		{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		for (auto& required : RequiredExtensions)
		{
			if (!physicalDevice->isExtensionSupported({ required }))
			{
				spdlog::info("Required extension {} not found", required);

				return false;
			}
		}

		if (!physicalDevice->isSamplerAnisotropySupported())
		{
			spdlog::info("Device does not support samplerAnisotropy");

			return false;
		}

		const auto formats = physicalDevice->getSwapchainFormats(surface);

		const auto presentModes = physicalDevice->getSwapchainPresentModes(surface);

		if (formats.empty() || presentModes.empty())
		{
			spdlog::info("Required swapchain support not present");

			return false;
		}

		return true;
	}

	auto VContext::chooseBestPhysicalDevice(const vk::SurfaceKHR& surface) -> void
	{
		for (const auto& physicalDevice : m_availablePhysicalDevices)
		{
			if (isPhysicalDeviceGood(physicalDevice, surface))
			{
				m_physicalDevice = physicalDevice;

				break;
			}
			else
			{
				spdlog::info("skipping device");
			}
		}
	}

	auto VContext::createLogicalDevice(const vk::SurfaceKHR& surface) -> void
	{
		m_logicalDevice = makeShared<VLogicalDevice>
		(
			surface, m_physicalDevice, m_vkAllocator
		);
	}

	auto VContext::createGraphicsCommandPool() -> void
	{
		const auto graphicsQueue = m_logicalDevice->getGraphicsQueue();

		m_graphicsPool = makeShared<VCommandPool>
		(
			graphicsQueue, m_logicalDevice, m_vkAllocator
		);
	}

	auto VContext::getInstance() const -> SPtr<const VInstance>
	{
		return m_instance;
	}

	auto VContext::getPhysicalDevice() const -> SPtr<const VPhysicalDevice>
	{
		return m_physicalDevice;
	}

	auto VContext::getLogicalDevice() const -> SPtr<const VLogicalDevice>
	{
		return m_logicalDevice;
	}

	auto VContext::getGraphicsPool() const -> SPtr<const VCommandPool>
	{
		return m_graphicsPool;
	}

	auto VContext::getAllocator() -> SPtr<VAllocationCallbacks>
	{
		return m_vkAllocator;
	}
}