#pragma once

#include "BitmapFont.h"
#include "FreeTypeFont.h"
#include <variant>

using Font = std::variant<
	std::nullptr_t,
	std::shared_ptr<BitmapFont>,
	std::shared_ptr<FreeTypeFont>>;

template <class... T>
constexpr bool holdsNullFont(const std::variant<T...>& v) noexcept { return std::holds_alternative<std::nullptr_t>(v); }

template <class... T>
constexpr bool holdsBitmapFont(const std::variant<T...>& v) noexcept { return std::holds_alternative<std::shared_ptr<BitmapFont>>(v); }

template <class... T>
constexpr bool holdsFreeTypeFont(const std::variant<T...>& v) noexcept { return std::holds_alternative<std::shared_ptr<FreeTypeFont>>(v); }
