#pragma once

#include "BitmapFont.h"
#include "FreeTypeFont.h"
#include <variant>

typedef std::variant<
	std::nullptr_t,
	std::shared_ptr<BitmapFont>,
	std::shared_ptr<FreeTypeFont>>
	Font;

#define holdsNullFont std::holds_alternative<std::nullptr_t>
#define holdsBitmapFont std::holds_alternative<std::shared_ptr<BitmapFont>>
#define holdsFreeTypeFont std::holds_alternative<std::shared_ptr<FreeTypeFont>>
