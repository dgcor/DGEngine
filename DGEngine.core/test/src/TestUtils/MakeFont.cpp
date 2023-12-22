#include "MakeFont.h"
#include "Game/Game.h"
#include "MakeTexturePack.h"

std::shared_ptr<BitmapFont> makeBitmapFont(Game& game,
	const std::string_view idFont, const std::string_view idTexturePack)
{
	auto texturePack = makeFontTexturePack(game, idTexturePack, idTexturePack);
	game.Resources().addTexturePack(idTexturePack, texturePack);
	return makeBitmapFont(game, idFont, texturePack);
}

std::shared_ptr<BitmapFont> makeBitmapFont(Game& game,
	const std::string_view idFont, const std::shared_ptr<TexturePack>& texturePack)
{
	auto font = std::make_shared<BitmapFont>(texturePack, 4, 4, 8);
	game.Resources().addFont(idFont, font);
	return font;
}
