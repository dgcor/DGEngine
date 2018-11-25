#pragma once

#include "Action.h"
#include "Font.h"
#include "Game.h"

class ActFontSetPalette : public Action
{
private:
	std::string id;
	std::string idPalette;
	sf::Color color;

public:
	ActFontSetPalette(const std::string& id_,
		const std::string& idPalette_, const sf::Color& color_)
		: id(id_), idPalette(idPalette_), color(color_) {}

	virtual bool execute(Game& game)
	{
		auto font = game.Resources().getFont(id);
		if (holdsBitmapFont(font) == true)
		{
			auto bitmapFont = std::get<std::shared_ptr<BitmapFont>>(font);
			if (bitmapFont->hasPalette() == true)
			{
				auto palette = game.Resources().getPalette(idPalette);
				if (palette != nullptr)
				{
					bitmapFont->setPalette(palette);
				}
			}
			else
			{
				bitmapFont->setColor(color);
			}
		}
		return true;
	}
};
