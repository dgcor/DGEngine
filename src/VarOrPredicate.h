#pragma once

#include <memory>
#include "Predicates/Predicate.h"
#include <variant>

typedef std::variant<Variable, std::shared_ptr<Predicate>> VarOrPredicate;
