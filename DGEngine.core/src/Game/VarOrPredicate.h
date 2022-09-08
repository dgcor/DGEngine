#pragma once

#include <memory>
#include "Predicate.h"

using VarOrPredicate = std::variant<Variable, std::shared_ptr<Predicate>>;

template <class... T>
constexpr bool holdsPredicate(const std::variant<T...>& v) noexcept { return std::holds_alternative<std::shared_ptr<Predicate>>(v); }
