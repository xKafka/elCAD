#pragma once

#include <functional>

#include "VContext.hpp"

#include <glm/glm.hpp>

namespace elcad::renderer
{
	class VLogicalDevice;
	class VCommandPool;
}

namespace elcad::renderer
{
	class VCommandBuffer
	{
		auto destroy() -> void;

	public:
		enum State : u32
		{
			NotAllocated,
			Ready = 0,
			Recording,
			InRenderPass,
			RecordingEnded,
			Submitted
		};

		VCommandBuffer(SPtr<const VLogicalDevice> logicalDevice, SPtr<const VCommandPool> commandPool);

		~VCommandBuffer();

		auto allocate(bool primary) -> void;

		auto free() -> void;

		auto begin(bool singleUse, bool renderpassContinue, bool simultaneous) -> void;

		auto end() -> void;

		auto updateSubmitted() -> void;

		auto reset() -> void;

		auto allocateAndBeginSingleUse() -> void;

		auto allocateAndEndSingleUse(const vk::Queue& queue) -> void;

		[[nodiscard]] auto getVkHandler() const -> const vk::CommandBuffer&;

	private:
		SPtr<const VLogicalDevice>	m_logicalDevice{};
			
		SPtr<const VCommandPool>	m_commandPool{};

		vk::CommandBuffer			m_vkHandler{};

		State						m_state{};
	};
}
