#include "SimpleLevelObject.h"
#include "Game.h"
#include "Game/Level.h"

void SimpleLevelObject::executeAction(Game& game) const
{
	if (action != nullptr)
	{
		game.Events().addBack(action);
	}
}

void SimpleLevelObject::update(Game& game, Level& level)
{
	base.updateHover(game, level, this);

	const auto& rect = base.sprite.getTextureRect();
	if (rect.width > 0 && rect.height > 0)
	{
		base.updateDrawPosition(level);
	}
	if (base.hasValidState() == true)
	{
		base.animation.update(game.getElapsedTime());
	}
}

bool SimpleLevelObject::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.empty() == true)
	{
		return false;
	}
	switch (str2int16(prop.c_str()))
	{
	case str2int16("type"):
		var = Variable(std::string("levelObject"));
		break;
	case str2int16("id"):
		var = Variable(id);
		break;
	case str2int16("name"):
		var = Variable(name);
		break;
	default:
		return false;
	}
	return true;
}
