#pragma once

#include <format>

template<typename...Args>
constexpr inline auto cfm(std::format_string<Args...> fmt, Args&&... args)
{
	return std::format
	(
		fmt, std::forward<Args>(args)...
	);
}