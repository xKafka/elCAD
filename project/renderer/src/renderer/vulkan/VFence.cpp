#include "renderer/vulkan/VFence.hpp"
#include "renderer/vulkan/VLogicalDevice.hpp"

#include <spdlog/spdlog.h>

namespace elcad::renderer
{
	VFence::VFence(bool signaled, SPtr<const VLogicalDevice> logicalDevice, SPtr<VAllocationCallbacks> vkAllocator)
		: m_logicalDevice{ logicalDevice }
		, m_vkAllocator{ vkAllocator }
	{
		create(signaled);
	}

	auto VFence::create(bool signaled) -> void
	{
		m_signaled = signaled;

		auto flags = vk::FenceCreateFlagBits{};

		if (signaled)
		{
			flags = vk::FenceCreateFlagBits::eSignaled;
		}

		auto createInfo = vk::FenceCreateInfo
		{
			.flags = flags
		};

		m_vkHandler = m_logicalDevice->getVkHandler().createFence
		(
			createInfo, *m_vkAllocator
		);
	}

	auto VFence::destroy() -> void
	{
		if (m_vkHandler)
		{
			m_logicalDevice->getVkHandler().destroyFence
			(
				m_vkHandler, *m_vkAllocator
			);

			m_vkHandler = nullptr;
		}

		m_signaled = false;
	}

	auto VFence::wait(u64 timeout) -> bool
	{
		//if already signaled fo not wait
		if (m_signaled)
		{
			return true;
		}

		auto result = m_logicalDevice->getVkHandler().waitForFences
		(
			{ m_vkHandler }, true, timeout
		);

		switch (result)
		{
		case vk::Result::eSuccess:
		{
			m_signaled = true;

			return true;
		}
		case vk::Result::eTimeout:
		{
			spdlog::warn("VFence wait - Timed out");

			return false;
		}
		case vk::Result::eErrorDeviceLost:
		{
			spdlog::critical("VkFence wait - error device lost");

			return false;
		}
		case vk::Result::eErrorOutOfHostMemory:
		{
			spdlog::critical("VkFence wait - out of host memory");

			return false;
		}
		case vk::Result::eErrorOutOfDeviceMemory:
		{
			spdlog::critical("VkFence wait - out of device memory");

			return false;
		}
		default:
			spdlog::critical("VkFence wait - an uknown error has occurred");
		}

		return false;
	}

	auto VFence::reset() -> void
	{
		if (m_signaled)
		{
			m_logicalDevice->getVkHandler().resetFences
			(
				{ m_vkHandler }
			);

			m_signaled = false;
		}
	}

	auto VFence::getVkHandler() const -> const vk::Fence&
	{
		return m_vkHandler;
	}
}