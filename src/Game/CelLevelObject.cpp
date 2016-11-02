#include "CelLevelObject.h"
#include "Game.h"
#include "GameUtils.h"
#include "Game/Level.h"
#include "Utils.h"

using Utils::str2int;

void CelLevelObject::executeAction(Game& game) const
{
	if (action != nullptr)
	{
		game.Events().addBack(action);
	}
}

void CelLevelObject::update(Game& game, Level& level)
{
	auto rect = sprite.getGlobalBounds();
	if (level.HasMouseInside() == true &&
		rect.contains(level.MousePositionf()) == true)
	{
		if (hovered == false)
		{
			hovered = true;
			level.setHoverObject(this);
			level.executeHoverEnterAction(game);
		}
	}
	else
	{
		if (hovered == true)
		{
			hovered = false;
			level.setHoverObject(nullptr);
			level.executeHoverLeaveAction(game);
		}
	}

	if (celTexture == nullptr
		|| frameRange.first > frameRange.second)
	{
		return;
	}

	// add delta time
	m_currentTime += game.getElapsedTime();

	// if current time is bigger then the frame time advance one frame
	if (m_currentTime >= m_frameTime)
	{
		// reset time, but keep the remainder
		m_currentTime = sf::microseconds(m_currentTime.asMicroseconds() % m_frameTime.asMicroseconds());

		currentFrame++;
		if (currentFrame < frameRange.first || currentFrame >= frameRange.second)
		{
			currentFrame = frameRange.first;
		}

		if (currentFrame < celTexture->size(celIdx))
		{
			sprite.setTexture(celTexture->get(celIdx, currentFrame), true);
		}
	}
}

bool CelLevelObject::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.empty() == true)
	{
		return false;
	}
	switch (str2int(prop.c_str()))
	{
	case str2int("type"):
		var = Variable("levelObject");
		break;
	case str2int("id"):
		var = Variable(id);
		break;
	case str2int("name"):
		var = Variable(name);
		break;
	default:
		return false;
	}
	return true;
}
