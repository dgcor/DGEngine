#pragma once

#include <memory>
#include "Predicates/Predicate.h"
#include <variant>

typedef std::variant<Variable, std::shared_ptr<Predicate>> VarOrPredicate;

#define holdsVariable std::holds_alternative<Variable>
#define holdsPredicate std::holds_alternative<std::shared_ptr<Predicate>>
