#pragma once

#include <Types.h>

#include "VTypes.hpp"
#include "VAllocationCallbacks.hpp"
#include "VPhysicalDevice.hpp"

namespace elcad::win
{
    class Window;
}

namespace elcad::renderer
{
    class DebugCallback
    {
    public:
        DebugCallback() = default;

        DebugCallback(std::function<void(StringView)> f);

        auto call(StringView message) -> void;

    private:
        std::function<void(StringView)> m_f{};
    };

    class VInstance
    {
        auto createAppInfo(StringView name, u32 version) -> vk::ApplicationInfo;

        auto gatherRequiredExtensions() -> Vec<const char*>;

        auto gatherRequiredLayers() -> Vec<const char*>;

        auto createVulkanInstance(const vk::ApplicationInfo& appInfo, Span<const char*> extensions, Span<const char*> layers) -> void;

        auto createInstance(StringView name, u32 version) -> void;

        auto areLayersSupported(Span<const char*> layers) -> bool;

        auto areExtensionsSupported(Span<const char*> requiredExtensions) -> bool;

        auto create(StringView name, u32 version) -> void;

#if defined(_DEBUG)
        auto setupDebugMessenger() -> void;
#endif

    public:
        VInstance(StringView name, u32 version, SPtr<VAllocationCallbacks> vkAllocator);

        auto setupDebugCallback(std::function<void(StringView)> f) -> void;

        auto destroy() -> void;

        [[nodiscard]] auto getVkHandler() const -> const vk::Instance&;

        [[nodiscard]] auto loadSurface(SPtr<const win::Window> window) const -> vk::SurfaceKHR;

        [[nodiscard]] auto loadPhysicalDevices() const -> Vec<SPtr<VPhysicalDevice>>;

    private:
        SPtr<VAllocationCallbacks>      m_vkAllocator{};

        vk::Instance                    m_vkHandler{};

        vk::DispatchLoaderDynamic       m_dispatch{};

#if defined(_DEBUG)
        vk::DebugUtilsMessengerEXT      m_debugMessenger{};
#endif

        UPtr<DebugCallback>             m_dbgCallback{};
    };
}