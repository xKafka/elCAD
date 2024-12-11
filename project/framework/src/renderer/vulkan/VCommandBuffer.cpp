#include "renderer/vulkan/VCommandBuffer.hpp"
#include "renderer/vulkan/VLogicalDevice.hpp"
#include "renderer/vulkan/VCommandPool.hpp"

namespace elcad::renderer
{
	VCommandBuffer::VCommandBuffer(SPtr<const VLogicalDevice> logicalDevice, SPtr<const VCommandPool> commandPool)
		: m_logicalDevice{ logicalDevice }
		, m_commandPool{ commandPool }
		, m_state{ State::NotAllocated }
	{}

	VCommandBuffer::~VCommandBuffer()
	{
	}

	auto VCommandBuffer::destroy() -> void
	{
		if (m_state != State::NotAllocated)
		{
			free();
		}
	}

	auto VCommandBuffer::allocate(bool primary) -> void
	{
		const auto level = primary ? vk::CommandBufferLevel::ePrimary : vk::CommandBufferLevel::eSecondary;

		auto allocateInfo = vk::CommandBufferAllocateInfo
		{
			.commandPool = m_commandPool->getVkHandler(),
			.level = level,
			.commandBufferCount = 1
		};

		auto buffers = m_logicalDevice->getVkHandler().allocateCommandBuffers
		(
			allocateInfo
		);

		if (buffers.empty())
		{
			throw std::runtime_error{ "Command buffers could not be allocated" };
		}

		m_vkHandler = buffers.front();

		m_state = State::Ready;
	}

	auto VCommandBuffer::free() -> void
	{
		m_logicalDevice->getVkHandler().freeCommandBuffers
		(
			m_commandPool->getVkHandler(), { m_vkHandler }
		);

		m_vkHandler = nullptr;

		m_state = State::NotAllocated;
	}

	auto VCommandBuffer::begin(bool singleUse, bool renderpassContinue, bool simultaneous) -> void
	{
		auto flags = vk::CommandBufferUsageFlags{};

		if (singleUse)
		{
			flags |= vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
		}

		if (renderpassContinue)
		{
			flags |= vk::CommandBufferUsageFlagBits::eRenderPassContinue;
		}

		if (simultaneous)
		{
			flags |= vk::CommandBufferUsageFlagBits::eSimultaneousUse;
		}

		auto beginInfo = vk::CommandBufferBeginInfo
		{
			.flags = flags
		};

		m_vkHandler.begin(beginInfo);

		m_state = State::Recording;
	}

	auto VCommandBuffer::end() -> void
	{
		m_vkHandler.end();

		m_state = State::RecordingEnded;
	}

	auto VCommandBuffer::updateSubmitted() -> void
	{
		m_state = State::Submitted;
	}

	auto VCommandBuffer::reset() -> void
	{
		m_state = State::Ready;
	}

	auto VCommandBuffer::allocateAndBeginSingleUse() -> void
	{
		allocate(true);

		begin(true, false, false);
	}

	auto VCommandBuffer::allocateAndEndSingleUse(const vk::Queue& queue) -> void
	{
		//end the command buffer
		end();

		//submit the queue
		auto submitInfo = vk::SubmitInfo
		{
			.commandBufferCount = 1,
			.pCommandBuffers = &m_vkHandler
		};

		queue.submit(submitInfo);

		//wait for this to be finished
		queue.waitIdle();

		//free the command buffer
		free();
	}

	auto VCommandBuffer::getVkHandler() const -> const vk::CommandBuffer&
	{
		return m_vkHandler;
	}
}