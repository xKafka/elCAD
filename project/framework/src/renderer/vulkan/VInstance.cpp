#include "renderer/vulkan/VInstance.hpp"
#include "renderer/vulkan/VPhysicalDevice.hpp"

#include "window/Window.hpp"

#include <spdlog/spdlog.h>
#include <span>

namespace elcad::renderer
{
    // Debug callback for Vulkan validation layers
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback
    (
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData
    )
    {
        auto message = StringView{ pCallbackData->pMessage };

        if (!message.empty())
        {
            reinterpret_cast<DebugCallback*>(pUserData)->call(message);
        }

        return VK_FALSE;
    }

    DebugCallback::DebugCallback(std::function<void(StringView)> f)
        : m_f{ std::move(f) }
    {
    }

    auto DebugCallback::call(StringView msg) -> void
    {
        if (m_f) 
        {
            m_f(msg);
        }
    }

    VInstance::VInstance(StringView name, u32 version, SPtr<VAllocationCallbacks> vkAllocator)
        : m_vkAllocator{ vkAllocator }
    {
        create(name, version);
    }

    auto VInstance::create(StringView name, u32 version) -> void
    {
        const auto appInfo = createAppInfo(name, version);

        auto layers = gatherRequiredLayers();

        for (const auto& layer : layers)
        {
            spdlog::debug("Using layer: {}", layer);
        }

        auto extensions = gatherRequiredExtensions();

        for (const auto& extension : extensions)
        {
            spdlog::debug("Using extension: {}", extension);
        }

        createVulkanInstance(appInfo, layers, extensions);

#if defined(_DEBUG)
        setupDebugMessenger();
#endif

        spdlog::info("Vulkan renderer initialized successfully");
    }

    auto VInstance::destroy() -> void
    {
#if defined(_DEBUG)
        if (m_debugMessenger)
        {
            spdlog::info("Destroying debug messenger...");

            m_vkHandler.destroyDebugUtilsMessengerEXT(m_debugMessenger, *m_vkAllocator, m_dispatch);

            m_debugMessenger = nullptr;
        }
#endif

        if (m_vkHandler)
        {
            spdlog::info("Destroying Vulkan instance...");

            m_vkHandler.destroy(*m_vkAllocator);

            m_vkHandler = nullptr;
        }
    }

    auto VInstance::createAppInfo(StringView name, u32 version) -> vk::ApplicationInfo
    {
        static constexpr auto EngineName{ "ElCad Engine" };

        static constexpr auto VulkanEngineVersion{ VK_MAKE_VERSION(1, 0, 0) };

        static constexpr auto VulkanApiVersion{ VK_API_VERSION_1_3 };

        return vk::ApplicationInfo
        {
            .pApplicationName = name.data(),
            .applicationVersion = version,
            .pEngineName = EngineName,
            .engineVersion = VulkanEngineVersion,
            .apiVersion = VulkanApiVersion
        };
    }

    auto VInstance::gatherRequiredExtensions() -> Vec<const char*>
    {
        auto requiredExtensions = win::Window::requiredVulkanInstanceExtensions();

#if defined(_DEBUG)
        requiredExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

        if (!areExtensionsSupported(requiredExtensions))
        {
            throw std::runtime_error{ "Required extensions not found; Vulkan instance creation failed." };
        }

        spdlog::info("All required extensions are present");
        return requiredExtensions;
    }

    auto VInstance::gatherRequiredLayers() -> Vec<const char*>
    {
        auto requiredLayers = Vec<const char*>{};

#if defined(_DEBUG)
        requiredLayers.emplace_back("VK_LAYER_KHRONOS_validation");
#endif

        if (!areLayersSupported(requiredLayers))
        {
            throw std::runtime_error{ "Required layers not found; Vulkan instance creation failed." };
        }

        spdlog::info("All required layers are present");
        return requiredLayers;
    }

    auto VInstance::createVulkanInstance(const vk::ApplicationInfo& appInfo, Span<const char*> layers, Span<const char*> extensions) -> void
    {
        auto createInfo = vk::InstanceCreateInfo
        {
            .pApplicationInfo = &appInfo,
            .enabledLayerCount = static_cast<u32>(layers.size()),
            .ppEnabledLayerNames = layers.data(),
            .enabledExtensionCount = static_cast<u32>(extensions.size()),
            .ppEnabledExtensionNames = extensions.data()
        };

        m_vkHandler = vk::createInstance(createInfo, *m_vkAllocator);

        m_dispatch = vk::DispatchLoaderDynamic{ m_vkHandler, vkGetInstanceProcAddr };

        spdlog::debug("Vulkan instance created");
    }

#if defined(_DEBUG)
    auto VInstance::setupDebugMessenger() -> void
    {
        using MessageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT;

        using MessageType = vk::DebugUtilsMessageTypeFlagBitsEXT;

        static constexpr auto Severity = MessageSeverity::eVerbose | MessageSeverity::eWarning | MessageSeverity::eError;

        static constexpr auto Type = MessageType::eGeneral | MessageType::eValidation;

        m_dbgCallback = makeUnique<DebugCallback>();

        auto debugCreateInfo = vk::DebugUtilsMessengerCreateInfoEXT
        {
            .messageSeverity = Severity,
            .messageType = Type,
            .pfnUserCallback = debugCallback,
            .pUserData = m_dbgCallback.get()
        };

        m_debugMessenger = m_vkHandler.createDebugUtilsMessengerEXT(debugCreateInfo, *m_vkAllocator, m_dispatch);

        spdlog::debug("Vulkan debugger created");
    }

    auto VInstance::setupDebugCallback(std::function<void(StringView)> f) -> void
    {
        m_dbgCallback = makeUnique<DebugCallback>(std::move(f));
    }
#endif


    auto VInstance::areExtensionsSupported(Span<const char*> requiredExtensions) -> bool
    {
        auto supported = vk::enumerateInstanceExtensionProperties();

        for (const auto& requiredExtension : requiredExtensions)
        {
            auto it = std::find_if
            (
                supported.begin(), supported.end(),

                [&](const vk::ExtensionProperties& extension) 
                {
                    return StringView{ requiredExtension } == extension.extensionName;
                }
            );

            if (it == supported.end())
            {
                return false;
            }
        }

        return true;
    }

    auto VInstance::areLayersSupported(Span<const char*> layers) -> bool
    {
        auto available = vk::enumerateInstanceLayerProperties();

        for (const char* layerName : layers)
        {
            auto it = std::find_if
            (
                available.begin(), available.end(),

                [&](const vk::LayerProperties& layer) 
                {
                    return StringView{ layerName } == layer.layerName;
                }
            );

            if (it == available.end())
            {
                return false;
            }
        }

        return true;
    }

    auto VInstance::loadPhysicalDevices() const -> Vec<SPtr<VPhysicalDevice>>
    {
        const auto handlers = m_vkHandler.enumeratePhysicalDevices();

        if (handlers.empty())
        {
            throw std::runtime_error{ "No devices which support Vulkan were found." };
        }

        auto out = Vec<SPtr<VPhysicalDevice>>{};

        out.reserve(handlers.size());

        for (auto& handler : handlers)
        {
            out.emplace_back
            (
                makeShared<VPhysicalDevice>(handler, m_vkAllocator)
            );
        }

        return out;
    }

    auto VInstance::loadSurface(SPtr<const win::Window> window) const -> vk::SurfaceKHR
    {
        return window->createVulkanSurface(m_vkHandler);
    }

    auto VInstance::destroySurface(const vk::SurfaceKHR& surface) const -> void
    {
        m_vkHandler.destroySurfaceKHR
        (
            surface, *m_vkAllocator
        );
    }

    auto VInstance::getVkHandler() const -> const vk::Instance&
    {
        return m_vkHandler;
    }
}