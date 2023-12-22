#include "MakeTexturePack.h"
#include "Game/Game.h"
#include "MakeTexture.h"
#include <memory>
#include "Resources/TexturePacks/BitmapFontTexturePack.h"

std::shared_ptr<TexturePack> makeFontTexturePack(Game& game,
	const std::string_view idTexturePack, const std::string_view idTexture)
{
	auto tex = makeTexture({64, 64});
	game.Resources().addTexture(idTexture, tex);
	return makeFontTexturePack(game, idTexturePack, tex);
}

std::shared_ptr<TexturePack> makeFontTexturePack(Game& game,
	const std::string_view idTexturePack, const std::shared_ptr<sf::Texture>& tex)
{
	auto texturePack = std::make_shared<BitmapFontTexturePack>(tex, nullptr, 16, 16, 4, 4, 8, false);
	game.Resources().addTexturePack(idTexturePack, texturePack);
	return texturePack;
}
