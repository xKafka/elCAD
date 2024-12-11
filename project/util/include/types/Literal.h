#pragma once

#ifndef FRAMEWORK_UTIL_TYPES_LITERAL_H
#define FRAMEWORK_UTIL_TYPES_LITERAL_H

#include <expected>
#include <memory>
#include <optional>
#include <type_traits>
#include <string_view>
#include <vector>
#include <array>
#include <span>
#include <string>
#include <cstddef>
#include <stdint.h>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using f32 = float;
using f64 = double;

using USize = std::size_t;

using StringView = std::string_view;

template<typename T> using Vec = std::vector<T>;

template<typename T, USize Size> using Array = std::array<T, Size>;

template<typename T, USize Extent = std::dynamic_extent> using Span = std::span<T, Extent>;

template<typename T> using SPtr = std::shared_ptr<T>;

template<typename T> using UPtr = std::unique_ptr<T>;

template<typename T> using WPtr = std::weak_ptr<T>;

template<typename T> using Opt = std::optional<T>;

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

static constexpr auto None{ std::nullopt };

#endif //LITERAL_H