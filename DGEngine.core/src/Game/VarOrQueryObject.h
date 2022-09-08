#pragma once

#include "QueryObject.h"
#include "Variable.h"

using VarOrQueryObject = std::variant<Variable, QueryObject>;

template <class... T>
constexpr bool holdsQueryObject(const std::variant<T...>& v) noexcept { return std::holds_alternative<QueryObject>(v); }
