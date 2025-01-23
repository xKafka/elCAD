#pragma once

#include "VTypes.hpp"

namespace elcad::renderer
{
	class VFence;
}

namespace elcad::renderer
{
	class VQueue
	{
		auto create(const vk::Device vkLogicalDevice, u32 familyIndex, u32 index) -> void;

	public:		
		VQueue() = default;

		VQueue(const vk::Device vkLogicalDevice, u32 familyIndex, u32 index);
		
		auto submit(const Vec<vk::SubmitInfo> infos, SPtr<VFence> fence) -> void;

		auto present(const vk::PresentInfoKHR& info) const -> vk::Result;

		auto destroy() -> void;

		[[nodiscard]] auto getVkHandler() const -> const vk::Queue&;

		[[nodiscard]] auto getFamilyIndex() const->u32;

		[[nodiscard]] auto getIndex() const->u32;

	private:
		vk::Queue		m_vkHandler{};

		u32				m_familyIndex{};

		u32				m_index{};
	};
}