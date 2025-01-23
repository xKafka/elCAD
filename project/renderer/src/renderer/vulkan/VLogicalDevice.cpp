#include "renderer/vulkan/VLogicalDevice.hpp"
#include "renderer/vulkan/VPhysicalDevice.hpp"
#include "renderer/vulkan/VQueue.hpp"
#include "renderer/vulkan/VCommandPool.hpp"

#include <spdlog/spdlog.h>

#include <set>

namespace elcad::renderer
{
    VLogicalDevice::VLogicalDevice(const vk::SurfaceKHR& surface, SPtr<const VPhysicalDevice> physicalDevice, SPtr<VAllocationCallbacks> vkAllocator)
        : m_physicalDevice{ physicalDevice }
        , m_vkAllocator{ vkAllocator }
    {
        create(surface);
    }

    auto VLogicalDevice::destroy() -> void
    {
        if (m_vkHandler)
        {
            spdlog::info("Releasing logical device...");

            m_vkHandler.destroy();

            m_vkHandler = nullptr;
        }
    }

    auto VLogicalDevice::create(const vk::SurfaceKHR& surface) -> void
    {
        auto queueFamilies = m_physicalDevice->getBestQueueFamilies(surface);

        const auto uniqueFamilies = std::set<u32>
        {
            queueFamilies.graphics.value(),
            queueFamilies.present.value(),
            queueFamilies.transfer.value()
        };

        auto queuesCreateInfo = Vec<vk::DeviceQueueCreateInfo>{};

        queuesCreateInfo.reserve(uniqueFamilies.size());

        static constexpr f32 QueuePriority{ 1.0f };

        for (const u32 queueFamily : uniqueFamilies)
        {
            auto createInfo = vk::DeviceQueueCreateInfo
            {
                .queueFamilyIndex = queueFamily,
                .queueCount = 1,
                .pQueuePriorities = &QueuePriority
            };

            queuesCreateInfo.emplace_back(createInfo);
        }

        const auto deviceFeatures = vk::PhysicalDeviceFeatures
        {
            .samplerAnisotropy = true
        };

        static constexpr auto DeviceExtensions = std::array
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        const auto deviceCreateInfo = vk::DeviceCreateInfo
        {
            .queueCreateInfoCount = static_cast<u32>(queuesCreateInfo.size()),
            .pQueueCreateInfos = queuesCreateInfo.data(),
            .enabledExtensionCount = static_cast<u32>(DeviceExtensions.size()),
            .ppEnabledExtensionNames = DeviceExtensions.data(),
            .pEnabledFeatures = &deviceFeatures
        };
        
        m_vkHandler = m_physicalDevice->getVkHandler().createDevice
        (
            deviceCreateInfo, *m_vkAllocator
        );

        m_graphicsQueue = makeShared<VQueue>
        (
            m_vkHandler, queueFamilies.graphics.value(), 0 
        );

        m_presentQueue = makeShared<VQueue>
        (
            m_vkHandler, queueFamilies.present.value(), 0 
        );

        m_transferQueue = makeShared<VQueue>
        (
            m_vkHandler, queueFamilies.transfer.value(), 0 
        );
    }

    auto VLogicalDevice::getVkHandler() const -> const vk::Device&
    {
        return m_vkHandler;
    }

    auto VLogicalDevice::getGraphicsQueue() const -> SPtr<const VQueue>
    {
        return m_graphicsQueue;
    }

    auto VLogicalDevice::getPresentQueue() const -> SPtr<const VQueue>
    {
        return m_presentQueue;
    }

    auto VLogicalDevice::getTransferQueue() const -> SPtr<const VQueue>
    {
        return m_transferQueue;
    }

    auto VLogicalDevice::getPhysicalDevice() const -> SPtr<const VPhysicalDevice>
    {
        return m_physicalDevice;
    }
}