#include "Item.h"
#include "Game.h"
#include "GameUtils.h"
#include "Game/Level.h"
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

bool Item::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.empty() == true)
	{
		return false;
	}
	switch (str2int(prop.c_str()))
	{
	case str2int("name"):
		var = Variable(name);
		break;
	case str2int("description1"):
		var = Variable(description1);
		break;
	case str2int("description2"):
		var = Variable(description2);
		break;
	case str2int("description3"):
		var = Variable(description3);
		break;
	default:
		return false;
	}
	return true;
}
