#pragma once

#include "BitmapFont.h"
#include "FreeTypeFont.h"
#include <variant>

typedef std::variant<
	nullptr_t,
	std::shared_ptr<BitmapFont>,
	std::shared_ptr<FreeTypeFont>>
	Font;

#define hasNullFont std::holds_alternative<nullptr_t>
#define hasBitmapFont std::holds_alternative<std::shared_ptr<BitmapFont>>
#define hasFreeTypeFont std::holds_alternative<std::shared_ptr<FreeTypeFont>>
