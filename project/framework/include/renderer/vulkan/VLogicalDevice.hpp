#pragma once

#include <Types.h>

#include "VTypes.hpp"
#include "VAllocationCallbacks.hpp"
#include "VQueue.hpp"

namespace elcad::win
{
    class Window;
}

namespace elcad::renderer
{
    class VPhysicalDevice;
}

namespace elcad::renderer
{
    class VLogicalDevice 
    {
        auto createLogicalDevice(SPtr<const win::Window> window) -> void;

        auto create(const vk::SurfaceKHR& surface) -> void;

    public:
        VLogicalDevice(const vk::SurfaceKHR& surface, SPtr<const VPhysicalDevice> physicalDevice, SPtr<VAllocationCallbacks> vkAllocator);

        auto destroy() -> void;

        [[nodiscard]] auto getVkHandler() const -> const vk::Device&;

        [[nodiscard]] auto getGraphicsQueue() const -> SPtr<const VQueue>;

        [[nodiscard]] auto getPresentQueue() const -> SPtr<const VQueue>;

        [[nodiscard]] auto getTransferQueue() const -> SPtr<const VQueue>;
        
        [[nodiscard]] auto getPhysicalDevice() const -> SPtr<const VPhysicalDevice>;

    private:
        SPtr<const VPhysicalDevice>     m_physicalDevice{};

        SPtr<vk::AllocationCallbacks>   m_vkAllocator{};

        vk::Device                      m_vkHandler{};

        SPtr<VQueue>                    m_graphicsQueue{};

        SPtr<VQueue>                    m_presentQueue{};

        SPtr<VQueue>                    m_transferQueue{};
    };
}