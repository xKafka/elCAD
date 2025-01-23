#include "renderer/vulkan/VPipelineCache.hpp"
#include "renderer/vulkan/VLogicalDevice.hpp"

namespace elcad::renderer
{
	VPipelineCache::VPipelineCache(SPtr<const VLogicalDevice> logicalDevice, SPtr<VAllocationCallbacks> vkAllocator)
		: m_logicalDevice{ logicalDevice }
		, m_vkAllocator{ vkAllocator }
	{
		create();
	}

	auto VPipelineCache::destroy() -> void
	{
		if (m_vkHandler)
		{
			m_logicalDevice->getVkHandler().destroyPipelineCache
			(
				m_vkHandler, *m_vkAllocator
			);

			m_vkHandler = nullptr;
		}
	}

	auto VPipelineCache::create() -> void
	{
		auto createInfo = vk::PipelineCacheCreateInfo
		{

		};

		m_vkHandler = m_logicalDevice->getVkHandler().createPipelineCache
		(
			createInfo, *m_vkAllocator
		);
	}

	auto VPipelineCache::getVkHandler() const -> const vk::PipelineCache&
	{
		return m_vkHandler;
	}
}
