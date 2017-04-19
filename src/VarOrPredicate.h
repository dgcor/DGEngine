#pragma once

#include <memory>
#include "Predicates/Predicate.h"
#include "Variable.h"

typedef mapbox::util::variant<Variable, std::shared_ptr<Predicate>> VarOrPredicate;
