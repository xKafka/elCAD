#include "renderer/vulkan/VPipeline.hpp"
#include "renderer/vulkan/VLogicalDevice.hpp"
#include "renderer/vulkan/VShader.hpp"
#include "renderer/vulkan/VRenderPass.hpp"
#include "renderer/vulkan/VPipelineCache.hpp"
#include "renderer/vulkan/VPipelineLayout.hpp"

#include <util/Filesystem.hpp>
#include <fstream>

namespace elcad::renderer
{
	VPipeline::VPipeline(SPtr<const VLogicalDevice> logicalDevice, SPtr<VAllocationCallbacks> vkAllocator)
		: m_logicalDevice{ logicalDevice }
		, m_vkAllocator{ vkAllocator }
	{

	}

	auto VPipeline::destroy() -> void
	{
        if (m_vkHandler)
        {
            m_logicalDevice->getVkHandler().destroyPipeline
            (
                m_vkHandler, *m_vkAllocator
            );

            m_vkHandler = nullptr;
        }
	}

    auto VPipeline::create(SPtr<const VRenderPass> renderPass, SPtr<const VPipelineCache> cache, SPtr<const VPipelineLayout> layout, SPtr<VShader> vertexShader, SPtr<VShader> fragmentShader) -> void
    {
        static constexpr auto VertexInputInfo = vk::PipelineVertexInputStateCreateInfo
        {
            .vertexBindingDescriptionCount = 0,
            .vertexAttributeDescriptionCount = 0
        };

        static constexpr auto InputAssembly = vk::PipelineInputAssemblyStateCreateInfo
        {
            .topology = vk::PrimitiveTopology::eTriangleList,
            .primitiveRestartEnable = false
        };

        static constexpr auto ViewportState = vk::PipelineViewportStateCreateInfo
        {
            .viewportCount = 1,
            .scissorCount = 1
        };

        static constexpr auto Rasterizer = vk::PipelineRasterizationStateCreateInfo
        {
            .depthClampEnable = false,
            .rasterizerDiscardEnable = false,
            .polygonMode = vk::PolygonMode::eFill,
            .cullMode = vk::CullModeFlagBits::eBack,
            .frontFace = vk::FrontFace::eClockwise,
            .depthBiasEnable = false,
            .lineWidth = 1.0f
        };

        static constexpr auto Multisampling = vk::PipelineMultisampleStateCreateInfo
        {
            .rasterizationSamples = vk::SampleCountFlagBits::e1,
            .sampleShadingEnable = false
        };

        static constexpr auto ColorBlendAttachment = vk::PipelineColorBlendAttachmentState
        {
            .blendEnable = false,
            .colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
        };

        static constexpr auto ColorBlending = vk::PipelineColorBlendStateCreateInfo
        {
            .logicOpEnable = false,
            .logicOp = vk::LogicOp::eCopy,
            .attachmentCount = 1,
            .pAttachments = &ColorBlendAttachment,
            .blendConstants = std::array{ 0.0f, 0.0f, 0.0f, 0.0f }
        };

        static constexpr std::array DynamicStates
        {
            vk::DynamicState::eViewport,
            vk::DynamicState::eScissor
        };

        const auto dynamicState = vk::PipelineDynamicStateCreateInfo
        {
            .dynamicStateCount = static_cast<u32>(DynamicStates.size()),
            .pDynamicStates = DynamicStates.data()
        };

        const auto shaderStages = std::array
        {
            //vertex shader stage
            vk::PipelineShaderStageCreateInfo
            {
                .stage = vk::ShaderStageFlagBits::eVertex,
                .module = vertexShader->getVkHandler(),
                .pName = "main"
            },

            //fragment shader stage
            vk::PipelineShaderStageCreateInfo
            {
                .stage = vk::ShaderStageFlagBits::eFragment,
                .module = fragmentShader->getVkHandler(),
                .pName = "main"
            }
        };

        const auto pipelineCreateInfo = vk::GraphicsPipelineCreateInfo
        {
            .stageCount = static_cast<u32>(shaderStages.size()),
            .pStages = shaderStages.data(),
            .pVertexInputState = &VertexInputInfo,
            .pInputAssemblyState = &InputAssembly,
            .pViewportState = &ViewportState,
            .pRasterizationState = &Rasterizer,
            .pMultisampleState = &Multisampling,
            .pColorBlendState = &ColorBlending,
            .pDynamicState = &dynamicState,
            .layout = layout->getVkHandler(),
            .renderPass = renderPass->getVkHandler(),
            .subpass = 0,
            .basePipelineHandle = nullptr
        };

        auto [result, pipeline] = m_logicalDevice->getVkHandler().createGraphicsPipeline
        (
            {}, pipelineCreateInfo, *m_vkAllocator
        );

        if (result != vk::Result::eSuccess)
        {
            throw std::runtime_error{ "failed to create graphics pipeline!" };
        }

        m_vkHandler = pipeline;
    }

    auto VPipeline::getVkHandler() const -> const vk::Pipeline&
    {
        return m_vkHandler;
    }
}