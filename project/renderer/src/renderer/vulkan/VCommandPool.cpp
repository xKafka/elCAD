#include "renderer/vulkan/VCommandPool.hpp"
#include "renderer/vulkan/VLogicalDevice.hpp"
#include "renderer/vulkan/VQueue.hpp"

#include <spdlog/spdlog.h>

namespace elcad::renderer
{
	VCommandPool::VCommandPool(SPtr<const VQueue> queue, SPtr<const VLogicalDevice> logicalDevice, SPtr<VAllocationCallbacks> vkAllocator)
		: m_vkAllocator{ vkAllocator }
		, m_logicalDevice{ logicalDevice }
	{
		create(queue);
	}

	auto VCommandPool::create(SPtr<const VQueue> queue) -> void
	{
		auto createInfo = vk::CommandPoolCreateInfo
		{
			.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
			.queueFamilyIndex = queue->getFamilyIndex()
		};

		m_vkHandler = m_logicalDevice->getVkHandler().createCommandPool
		(
			createInfo, *m_vkAllocator
		);

		spdlog::info("Graphics command pool created");
	}

	auto VCommandPool::destroy() -> void
	{
		if (m_vkHandler)
		{
			spdlog::info("Releasing graphics command pool...");

			m_logicalDevice->getVkHandler().destroyCommandPool
			(
				m_vkHandler, *m_vkAllocator
			);

			m_vkHandler = nullptr;
		}
	}

	auto VCommandPool::getVkHandler() const -> const vk::CommandPool&
	{
		return m_vkHandler;
	}
}