#pragma once

#include "VAllocationCallbacks.hpp"
#include "VInstance.hpp"
#include "VPhysicalDevice.hpp"
#include "VLogicalDevice.hpp"
#include "VCommandPool.hpp"

namespace elcad::win
{
	class Window;
}

namespace elcad::renderer
{
	class VContext
	{
		auto createInstance(StringView name, u32 version) -> void;

		auto loadPhysicalDevices() -> void;

		auto isPhysicalDeviceGood(SPtr<const VPhysicalDevice> physicalDevice, const vk::SurfaceKHR& surface) const -> bool;

		auto chooseBestPhysicalDevice(const vk::SurfaceKHR& surface) -> void;

		auto createLogicalDevice(const vk::SurfaceKHR& surface) -> void;

		auto createGraphicsCommandPool() -> void;

	public:
		VContext(StringView name, u32 version);

		auto initWithWindow(const vk::SurfaceKHR& surface) -> void;

		auto destroy() -> void;

		auto createVulkanSurface(SPtr<const win::Window> window) -> vk::SurfaceKHR;

		[[nodiscard]] auto getInstance() const -> SPtr<const VInstance>;

		[[nodiscard]] auto getPhysicalDevice() const -> SPtr<const VPhysicalDevice>;

		[[nodiscard]] auto getLogicalDevice() const -> SPtr<const VLogicalDevice>;
		
		[[nodiscard]] auto getGraphicsPool() const -> SPtr<const VCommandPool>;

		[[nodiscard]] auto getAllocator() -> SPtr<VAllocationCallbacks>;

	private:
		SPtr<VAllocationCallbacks>		m_vkAllocator{};

		SPtr<const win::Window>			m_window{};

		SPtr<VInstance>					m_instance{};

		Vec<SPtr<VPhysicalDevice>>		m_availablePhysicalDevices{};

		SPtr<VPhysicalDevice>			m_physicalDevice{};

		SPtr<VLogicalDevice>			m_logicalDevice{};

		SPtr<VCommandPool>				m_graphicsPool{};
	};

}
