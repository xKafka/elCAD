#include "renderer/vulkan/VSemaphore.hpp"
#include "renderer/vulkan/VLogicalDevice.hpp"

#include <spdlog/spdlog.h>

namespace elcad::renderer
{
	VSemaphore::VSemaphore(SPtr<const VLogicalDevice> logicalDevice, SPtr<VAllocationCallbacks>	vkAllocator)
		: m_logicalDevice{ logicalDevice }
		, m_vkAllocator{ vkAllocator }
	{
		create();
	}

	auto VSemaphore::create() -> void
	{
		static constexpr auto CreateInfo = vk::SemaphoreCreateInfo
		{
		};

		m_vkHandler = m_logicalDevice->getVkHandler().createSemaphore
		(
			CreateInfo, *m_vkAllocator
		);
	}

	auto VSemaphore::destroy() -> void
	{
		if (m_vkHandler)
		{
			m_logicalDevice->getVkHandler().destroySemaphore
			(
				m_vkHandler, *m_vkAllocator
			);

			m_vkHandler = nullptr;
		}
	}

	auto VSemaphore::getVkHandler() const -> const vk::Semaphore&
	{
		return m_vkHandler;
	}
}