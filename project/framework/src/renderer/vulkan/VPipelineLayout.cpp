#include "renderer/vulkan/VPipelineLayout.hpp"
#include "renderer/vulkan/VLogicalDevice.hpp"

#include <util/Filesystem.hpp>
#include <fstream>

namespace elcad::renderer
{
	VPipelineLayout::VPipelineLayout(SPtr<const VLogicalDevice> logicalDevice, SPtr<VAllocationCallbacks> vkAllocator)
		: m_logicalDevice{ logicalDevice }
		, m_vkAllocator{ vkAllocator }
	{

	}

	auto VPipelineLayout::destroy() -> void
	{
		if (m_vkHandler)
		{
			m_logicalDevice->getVkHandler().destroyPipelineLayout
			(
				m_vkHandler, *m_vkAllocator
			);

			m_vkHandler = nullptr;
		}
	}

	auto VPipelineLayout::create() -> void
	{
		auto pipelineLayoutInfo = vk::PipelineLayoutCreateInfo
		{
			.setLayoutCount = 0,
			.pushConstantRangeCount = 0
		};

		m_vkHandler = m_logicalDevice->getVkHandler().createPipelineLayout
		(
			pipelineLayoutInfo, *m_vkAllocator
		);
	}

	auto VPipelineLayout::getVkHandler() const -> const vk::PipelineLayout&
	{
		return m_vkHandler;
	}
}