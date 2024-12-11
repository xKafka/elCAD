#include "renderer/vulkan/VPhysicalDevice.hpp"

#include <spdlog/spdlog.h>

namespace elcad::renderer
{
    VPhysicalDevice::VPhysicalDevice(const vk::PhysicalDevice& vkPhysicalDevice, SPtr<VAllocationCallbacks> vkAllocator)
        : m_vkAllocator{ vkAllocator }
        , m_vkHandler{ vkPhysicalDevice }
    {
        create();
    }

    auto VPhysicalDevice::create() -> void
    {
        m_features = m_vkHandler.getFeatures();

        m_properties = m_vkHandler.getProperties();

        m_memory = m_vkHandler.getMemoryProperties();
    }

    auto VPhysicalDevice::isExtensionSupported(StringView extension) const -> bool
    {
        auto supportedExtensions = m_vkHandler.enumerateDeviceExtensionProperties();

        if (supportedExtensions.empty())
        {
            return false;
        }

        for (const auto& supported : supportedExtensions)
        {
            if (extension == supported.extensionName)
            {
                return true;
            }
        }

        return false;
    }

    auto VPhysicalDevice::getBestQueueFamilies(const vk::SurfaceKHR& surface) const -> QueueFamily
    {
        auto out = QueueFamily{};

        auto queueFamilies = m_vkHandler.getQueueFamilyProperties();

        if (queueFamilies.empty())
        {
            throw std::runtime_error{ "No queue families found for this physical device." };
        }

        auto minTransferScore = u8{ 255u }; // Start with the highest possible score for transfer

        for (u32 i{ 0 }; i < queueFamilies.size(); ++i)
        {
            const auto& family = queueFamilies[i];

            auto currentTransferScore = u8{ 0 };

            // Check for graphics support
            if (family.queueFlags & vk::QueueFlagBits::eGraphics)
            {
                if (!out.graphics) 
                {
                    out.graphics = i;
                }

                ++currentTransferScore;

                // Check if the queue family supports surface presentation
                if (m_vkHandler.getSurfaceSupportKHR(i, surface))
                {
                    if (!out.present) 
                    {
                        out.present = i;
                    }

                    ++currentTransferScore;
                }
            }

            // Check for compute support
            if (family.queueFlags & vk::QueueFlagBits::eCompute)
            {
                if (!out.compute)
                {
                    out.compute = i;
                }

                ++currentTransferScore;
            }

            // Check for transfer support and prioritize lower scores
            if (family.queueFlags & vk::QueueFlagBits::eTransfer)
            {
                if (currentTransferScore <= minTransferScore)
                {
                    minTransferScore = currentTransferScore;

                    out.transfer = i;
                }
            }
        }

        if (!out.graphics || !out.present || !out.compute || !out.transfer)
        {
            throw std::runtime_error{ "Failed to find suitable queue families." };
        }

        return out;
    }

    auto VPhysicalDevice::getProperties() const -> const vk::PhysicalDeviceProperties&
    {
        return m_properties;
    }

    auto VPhysicalDevice::getFeatures() const -> const vk::PhysicalDeviceFeatures&
    {
        return m_features;
    }

    auto VPhysicalDevice::getVkHandler() const -> const vk::PhysicalDevice&
    {
        return m_vkHandler;
    }

    auto VPhysicalDevice::getSwapchainCapabilities(const vk::SurfaceKHR& surface) const -> vk::SurfaceCapabilitiesKHR
    {
        return m_vkHandler.getSurfaceCapabilitiesKHR(surface);
    }

    auto VPhysicalDevice::getSwapchainFormats(const vk::SurfaceKHR& surface) const -> Vec<vk::SurfaceFormatKHR>
    {
        return m_vkHandler.getSurfaceFormatsKHR(surface);
    }

    auto VPhysicalDevice::getSwapchainPresentModes(const vk::SurfaceKHR& surface) const -> Vec<vk::PresentModeKHR>
    {
        return m_vkHandler.getSurfacePresentModesKHR(surface);
    }

    auto VPhysicalDevice::isDiscrete() const -> bool
    {
        return m_properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu;
    }

    auto VPhysicalDevice::isSamplerAnisotropySupported() const -> bool
    {
        return static_cast<bool>(m_features.samplerAnisotropy);
    }

    auto VPhysicalDevice::getMemoryIndex(u32 memoryTypeBits, vk::MemoryPropertyFlags memFlags) const -> Opt<u32>
    {
        for (u32 i{ 0 }; i < m_memory.memoryTypeCount; ++i)
        {
            if (memoryTypeBits & (1 << i) && (m_memory.memoryTypes[i].propertyFlags & memFlags) == memFlags)
            {
                return i;
            }
        }

        return std::nullopt;
    }

    auto VPhysicalDevice::detectDepthFormat() -> Opt<vk::Format>
    {
        static constexpr auto candidates = std::array
        {
            vk::Format::eD32Sfloat,
            vk::Format::eD32SfloatS8Uint,
            vk::Format::eD24UnormS8Uint
        };

        auto flags = vk::FormatFeatureFlagBits::eDepthStencilAttachment;

        for (auto& format : candidates)
        {
            const auto formatProperties = m_vkHandler.getFormatProperties(format);

            if ((formatProperties.linearTilingFeatures & flags) == flags)
            {
                return format;
            }
            else if ((formatProperties.optimalTilingFeatures & flags) == flags)
            {
                return format;
            }
        }

        return std::nullopt;
    }
}