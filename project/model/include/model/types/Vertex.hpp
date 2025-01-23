#pragma once

#include <util/Math.hpp>

namespace elcad::model
{
	struct Vertex
	{
		math::vec3		position{};

		math::u8vec4	color{};

		math::vec3		normal{};

		math::vec2		texCoord0{};
	};
}