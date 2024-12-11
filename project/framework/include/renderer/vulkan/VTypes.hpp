#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

#include <Types.h>

namespace elcad::renderer
{
	auto vkCheck(vk::Result result) -> void;
}