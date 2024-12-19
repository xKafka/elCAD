#pragma once

#include <util/Types.hpp>

#include "VTypes.hpp"
#include "VAllocationCallbacks.hpp"

namespace elcad::renderer
{
	class VLogicalDevice;
}

namespace elcad::renderer
{
	class VShader
	{
		auto create() -> void;

		auto loadFile(StringView path) -> Vec<char>;

	public:
		VShader(SPtr<const VLogicalDevice> logicalDevice, SPtr<VAllocationCallbacks> vkAllocator);

		auto createFromFile(StringView path) -> void;

		auto destroy() -> void;

		[[nodiscard]] auto getVkHandler() const -> const vk::ShaderModule&;

	private:
		SPtr<const VLogicalDevice>		m_logicalDevice{};

		SPtr<VAllocationCallbacks>		m_vkAllocator{};

		vk::ShaderModule				m_vkHandler{};
	};
}