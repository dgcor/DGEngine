#pragma once

#include <memory>
#include "Resources/TexturePack.h"
#include <string_view>

class Game;

std::shared_ptr<TexturePack> makeFontTexturePack(Game& game,
	const std::string_view idTexturePack, const std::string_view idTexture);

std::shared_ptr<TexturePack> makeFontTexturePack(Game& game,
	const std::string_view idTexturePack, const std::shared_ptr<sf::Texture>& tex);
