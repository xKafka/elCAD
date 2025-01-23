#pragma once

#include <util/Types.hpp>

namespace elcad::win
{
	class IRenderable
	{
	public:
		virtual auto render() -> void = 0;
	};
}