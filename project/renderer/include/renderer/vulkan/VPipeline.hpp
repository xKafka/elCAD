#pragma once

#include <util/Types.hpp>

#include "VTypes.hpp"
#include "VAllocationCallbacks.hpp"

namespace elcad::renderer
{
	class VLogicalDevice;
	class VShader;
	class VRenderPass;
	class VPipelineCache;
	class VPipelineLayout;
}

namespace elcad::renderer
{
	class VPipeline
	{
		auto create() -> void;

	public:
		VPipeline(SPtr<const VLogicalDevice> logicalDevice, SPtr<VAllocationCallbacks> vkAllocator);

		auto destroy() -> void;

		auto create(SPtr<const VRenderPass> renderPass, SPtr<const VPipelineCache> cache, SPtr<const VPipelineLayout> layout, SPtr<VShader> vertexShader, SPtr<VShader> fragmentShader) -> void;

		[[nodiscard]] auto getVkHandler() const -> const vk::Pipeline&;

	private:
		SPtr<const VLogicalDevice>		m_logicalDevice{};

		SPtr<VAllocationCallbacks>		m_vkAllocator{};

		vk::Pipeline					m_vkHandler{};
	};
}