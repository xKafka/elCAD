#pragma once

#include <util/Types.hpp>

namespace elcad::model::importer
{
	template<typename T>
	extern auto loadFromFile(StringView path) -> SPtr<T>;
}