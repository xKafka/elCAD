#pragma once

#include <expected>
#include <memory>
#include <optional>
#include <string_view>
#include <vector>
#include <array>
#include <span>
#include <string>

#include "Basic.hpp"

using String = std::string;

using StringView = std::string_view;

template<typename T> using Vec = std::vector<T>;

template<typename T> using SPtr = std::shared_ptr<T>;

template<typename T> using UPtr = std::unique_ptr<T>;

template<typename T> using WPtr = std::weak_ptr<T>;

template<typename T> using Opt = std::optional<T>;

template<typename T, std::size_t Size> using Array = std::array<T, Size>;

template<typename T, std::size_t Extent = std::dynamic_extent> using Span = std::span<T, Extent>;

template<typename T, typename E> using Result = std::expected<T, E>;

template<typename E> class NoResult : std::unexpected<E>{};