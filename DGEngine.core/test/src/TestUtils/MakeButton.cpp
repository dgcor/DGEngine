#include "MakeButton.h"
#include "Game/Game.h"

std::shared_ptr<BitmapButton> makeBitmapButton(Game& game,
	const std::string_view idButton, const std::string_view idTexture)
{
	auto tex = game.Resources().getTexture(idTexture);
	return makeBitmapButton(game, idButton, tex);
}

std::shared_ptr<BitmapButton> makeBitmapButton(Game& game,
	const std::string_view idButton, const std::shared_ptr<sf::Texture>& tex)
{
	auto button = std::make_shared<BitmapButton>(*tex);
	game.Resources().addDrawable(idButton, button, true);
	return button;
}

std::shared_ptr<StringButton> makeBitmapFontStringButton(Game& game,
	const std::string_view idButton, const std::string_view idFont)
{
	auto font = std::get<std::shared_ptr<BitmapFont>>(game.Resources().getFont(idFont));
	return makeBitmapFontStringButton(game, idButton, font);
}

std::shared_ptr<StringButton> makeBitmapFontStringButton(Game& game,
	const std::string_view idButton, const std::shared_ptr<BitmapFont>& font)
{
	auto button = std::make_shared<StringButton>(font);
	game.Resources().addDrawable(idButton, button, true);
	return button;
}
