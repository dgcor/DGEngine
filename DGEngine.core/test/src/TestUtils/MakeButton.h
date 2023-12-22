#pragma once

#include "Game/Drawables/BitmapButton.h"
#include "Game/Drawables/StringButton.h"

std::shared_ptr<BitmapButton> makeBitmapButton(Game& game,
	const std::string_view idButton, const std::string_view idTexture);

std::shared_ptr<BitmapButton> makeBitmapButton(Game& game,
	const std::string_view idButton, const std::shared_ptr<sf::Texture>& tex);

std::shared_ptr<StringButton> makeBitmapFontStringButton(Game& game,
	const std::string_view idButton, const std::string_view idFont);

std::shared_ptr<StringButton> makeBitmapFontStringButton(Game& game,
	const std::string_view idButton, const std::shared_ptr<BitmapFont>& font);
