#include "renderer/vulkan/VQueue.hpp"
#include "renderer/vulkan/VLogicalDevice.hpp"
#include "renderer/vulkan/VFence.hpp"

namespace elcad::renderer
{
	VQueue::VQueue(const vk::Device vkLogicalDevice, u32 familyIndex, u32 index)
		: m_familyIndex{ familyIndex }
		, m_index{ index }
	{
		create(vkLogicalDevice, familyIndex, index);
	}

	auto VQueue::create(const vk::Device vkLogicalDevice, u32 familyIndex, u32 index) -> void
	{
		m_vkHandler = vkLogicalDevice.getQueue
		(
			familyIndex, index
		);
	}

	auto VQueue::destroy() -> void
	{
		if (m_vkHandler)
		{
			m_vkHandler = nullptr;
		}
	}

	auto VQueue::submit(const Vec<vk::SubmitInfo> infos, SPtr<VFence> fence) -> void
	{
		m_vkHandler.submit
		(
			infos, fence->getVkHandler()
		);
	}

	auto VQueue::present(const vk::PresentInfoKHR& info) const -> vk::Result
	{
		return m_vkHandler.presentKHR(info);
	}

	auto VQueue::getVkHandler() const -> const vk::Queue&
	{
		return m_vkHandler;
	}

	auto VQueue::getFamilyIndex() const -> u32
	{
		return m_familyIndex;
	}
	 
	auto VQueue::getIndex() const -> u32
	{
		return m_index;
	}
}