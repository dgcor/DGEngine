#include "Scrollable.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils/Utils.h"

void Scrollable::reset()
{
	const auto& vSize = view.getVisibleSize();
	view.reset(sf::FloatRect(0.f, -(vSize.y + offset), vSize.x, vSize.y));
	height = vSize.y;
}

std::shared_ptr<Action> Scrollable::getAction(uint16_t nameHash16) const noexcept
{
	switch (nameHash16)
	{
	case str2int16("complete"):
		return completeAction;
	default:
		return nullptr;
	}
}

bool Scrollable::setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept
{
	switch (nameHash16)
	{
	case str2int16("complete"):
		completeAction = action;
		break;
	default:
		return false;
	}
	return true;
}

void Scrollable::draw(const Game& game, sf::RenderTarget& target) const
{
	if (visible == true)
	{
		if (auto obj = uiObj.lock())
		{
			auto origView = target.getView();
			target.setView(view.getView());
			obj->draw(game, target);
			target.setView(origView);
		}
	}
}

void Scrollable::updateSize(const Game& game)
{
	view.updateSize(game);
}

void Scrollable::update(Game& game)
{
	if (pause == true || visible == false)
	{
		return;
	}

	elapsedTime.update(game.getElapsedTime(), [&]()
	{
		if (auto obj = uiObj.lock())
		{
			auto rect = view.getCenter();
			auto sizeY = obj->Size().y;
			if (rect.y - (height / 2.f) < sizeY)
			{
				rect.y++;
			}
			else
			{
				rect.y -= sizeY + height;

				if (loop == false)
				{
					pause = true;
				}

				game.Events().addBack(completeAction);
			}
			view.setCenter(rect);
		}
	});
}

bool Scrollable::getProperty(const std::string_view prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	if (getUIObjProp(str2int16(props.first), props.second, var) == true)
	{
		return true;
	}
	if (auto obj = uiObj.lock())
	{
		return obj->getProperty(prop, var);
	}
	return false;
}
