#include "Item.h"
#include "Game.h"
#include "GameUtils.h"
#include "Level.h"
#include "Utils.h"

using Utils::str2int;

void Item::update(Game& game, const Level& level)
{
	auto rect = sprite.getGlobalBounds();
	if (rect.contains(level.MousePosition()))
	{
		if (game.getMouseButton() == sf::Mouse::Left)
		{
			if (game.wasMouseClicked() == true)
			{
				game.clearMouseClicked();
				if (clickAction != nullptr)
				{
					game.Events().addBack(clickAction);
				}
			}
		}
	}
}

Variable Item::getProperty(const std::string& prop) const
{
	if (prop.size() > 1)
	{
		switch (str2int(prop.c_str()))
		{
		case str2int("name"):
			return Variable(name);
		case str2int("description1"):
			return Variable(description1);
		case str2int("description2"):
			return Variable(description2);
		case str2int("description3"):
			return Variable(description3);
		default:
			break;
		}
	}
	return Variable();
}
