#pragma once

#include <util/Types.hpp>

namespace elcad::model
{
namespace path
{
	auto getSystemRootPath() -> StringView;

	auto getResourcePath(StringView path) -> String;
}
}