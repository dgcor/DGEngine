#pragma once

#include "Resources/Font.h"
#include "Resources/TexturePack.h"

class Game;

std::shared_ptr<BitmapFont> makeBitmapFont(Game& game,
	const std::string_view idFont, const std::string_view idTexturePack);

std::shared_ptr<BitmapFont> makeBitmapFont(Game& game,
	const std::string_view idFont, const std::shared_ptr<TexturePack>& texturePack);
