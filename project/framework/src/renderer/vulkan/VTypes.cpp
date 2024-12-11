#include "renderer/vulkan/VTypes.hpp"

#include <stdexcept>
#include <fmt/format.h>

namespace elcad::renderer
{
	auto vkCheck(vk::Result result) -> void
	{
		if (result != vk::Result::eSuccess)
		{
			throw std::runtime_error{ fmt::format("VK error : [{}]", std::to_underlying(result)) };
		}
	}
}