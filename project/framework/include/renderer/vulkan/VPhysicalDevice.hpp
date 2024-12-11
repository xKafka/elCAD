#pragma once

#include "VTypes.hpp"
#include "VAllocationCallbacks.hpp"

namespace elcad::renderer
{
    class VPhysicalDevice
    {
    public:
        struct QueueFamily
        {
            Opt<u32> graphics{};
            Opt<u32> present{};
            Opt<u32> transfer{};
            Opt<u32> compute{};
        };

    private:
        auto create() -> void;

    public:
        VPhysicalDevice(const vk::PhysicalDevice& vkPhysicalDevice, SPtr<VAllocationCallbacks> vkAllocator);

        auto getBestQueueFamilies(const vk::SurfaceKHR& surface) const -> QueueFamily;

        auto getSwapchainCapabilities(const vk::SurfaceKHR& surface) const -> vk::SurfaceCapabilitiesKHR;

        auto getSwapchainFormats(const vk::SurfaceKHR& surface) const -> Vec<vk::SurfaceFormatKHR>;

        auto getSwapchainPresentModes(const vk::SurfaceKHR& surface) const -> Vec<vk::PresentModeKHR>;

        auto isDiscrete() const -> bool;

        auto isExtensionSupported(StringView extension) const -> bool;

        auto isSamplerAnisotropySupported() const -> bool;

        auto detectDepthFormat() -> Opt<vk::Format>;

        auto getMemoryIndex(u32 memoryTypeBits, vk::MemoryPropertyFlags memFlags) const -> Opt<u32>;

        [[nodiscard]] auto getProperties() const -> const vk::PhysicalDeviceProperties&;

        [[nodiscard]] auto getFeatures() const -> const vk::PhysicalDeviceFeatures&;

        [[nodiscard]] auto getVkHandler() const -> const vk::PhysicalDevice&;

    private:
        SPtr<VAllocationCallbacks>          m_vkAllocator{};

        vk::PhysicalDevice                  m_vkHandler{};

        vk::PhysicalDeviceFeatures          m_features{};

        vk::PhysicalDeviceProperties        m_properties{};

        vk::PhysicalDeviceMemoryProperties  m_memory{};
    };
}