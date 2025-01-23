#include "renderer/vulkan/VShader.hpp"
#include "renderer/vulkan/VLogicalDevice.hpp"

#include <util/Filesystem.hpp>
#include <fstream>

namespace elcad::renderer
{
	VShader::VShader(SPtr<const VLogicalDevice> logicalDevice, SPtr<VAllocationCallbacks> vkAllocator)
		: m_logicalDevice{ logicalDevice }
		, m_vkAllocator{ vkAllocator }
	{

	}

	auto VShader::destroy() -> void
	{
		if (m_vkHandler)
		{
			m_logicalDevice->getVkHandler().destroyShaderModule
			(
				m_vkHandler, *m_vkAllocator
			);

			m_vkHandler = nullptr;
		}
	}

	auto VShader::createFromFile(StringView path) -> void
	{
        const auto code = loadFile(path);

        auto createInfo = vk::ShaderModuleCreateInfo
        {
            .codeSize = static_cast<u32>(code.size()),
            .pCode = reinterpret_cast<const u32*>(code.data())
        };

        m_vkHandler = m_logicalDevice->getVkHandler().createShaderModule
        (
            createInfo, *m_vkAllocator
        );

        if (!m_vkHandler)
        {
            throw std::runtime_error{ "failed to create shader module!" };
        }
    }

	auto VShader::loadFile(StringView path) -> Vec<char>
	{
		auto f = std::ifstream{ path.data(), std::ios::ate | std::ios::binary };

		if (!f.is_open())
		{
			throw std::runtime_error{ "failed to open file" };
		}

		const auto fileSize = fs::file_size
		(
			fs::path{ path }
		);

		auto out = Vec<char>(fileSize);

		f.seekg(0);

		f.read(out.data(), fileSize);

		f.close();

		return out;
	}

	auto VShader::getVkHandler() const -> const vk::ShaderModule&
	{
		return m_vkHandler;
	}
}