#pragma once

#include "Template.hpp"

template<typename T, typename...Args>
inline auto makeShared(Args &&...args) -> SPtr<T>
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename...Args>
inline auto makeUnique(Args &&...args) -> UPtr<T>
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
inline auto makeWeak(SPtr<T> ptr) -> WPtr<T>
{
	return std::weak_ptr<T>(ptr);
}

template<typename T, typename...Args>
inline auto makeOpt(Args &&...args) -> Opt<T>
{
	return std::make_optional<T>(std::forward<Args>(args)...);
}

