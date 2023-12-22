#include "Scrollable.h"
#include "Game/Game.h"
#include "Game/Utils/GameUtils.h"
#include "Utils/StringHash.h"
#include "Utils/Utils.h"

void Scrollable::reset()
{
	sf::Vector2f uiObjSize;

	if (auto obj = uiObj.lock())
	{
		uiObjSize = obj->Size();
	}

	const auto& vSize = view.getVisibleSize();

	switch (direction)
	{
	case Direction::Up:
	{
		view.reset(sf::FloatRect(0.f, uiObjSize.y + offset, vSize.x, vSize.y));
		scrollSize = vSize.y;
		break;
	}
	case Direction::Down:
	{
		view.reset(sf::FloatRect(0.f, -(vSize.y + offset), vSize.x, vSize.y));
		scrollSize = vSize.y;
		break;
	}
	case Direction::Left:
	{
		view.reset(sf::FloatRect(uiObjSize.x + offset, 0.f, vSize.x, vSize.y));
		scrollSize = vSize.x;
		break;
	}
	case Direction::Right:
	{
		view.reset(sf::FloatRect(-(vSize.x + offset), 0.f, vSize.x, vSize.y));
		scrollSize = vSize.x;
		break;
	}
	default:
		break;
	}
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

	if (auto obj = uiObj.lock())
	{
		obj->update(game);

		elapsedTime.update(game.getElapsedTime(), [&]()
		{
			auto rect = view.getCenter();
			auto finished = false;

			switch (direction)
			{
			case Direction::Up:
			{
				auto sizeY = obj->Size().y;
				if (rect.y + (scrollSize / 2.f) > 0)
				{
					rect.y--;
				}
				else
				{
					if (loop == true)
					{
						rect.y += sizeY + scrollSize;
					}
					finished = true;
				}
				break;
			}
			case Direction::Down:
			{
				auto sizeY = obj->Size().y;
				if (rect.y - (scrollSize / 2.f) < sizeY)
				{
					rect.y++;
				}
				else
				{
					if (loop == true)
					{
						rect.y -= sizeY + scrollSize;
					}
					finished = true;
				}
				break;
			}
			case Direction::Left:
			{
				auto sizeX = obj->Size().x;
				if (rect.x + (scrollSize / 2.f) > 0)
				{
					rect.x--;
				}
				else
				{
					if (loop == true)
					{
						rect.x += sizeX + scrollSize;
					}
					finished = true;
				}
				break;
			}
			case Direction::Right:
			{
				auto sizeX = obj->Size().x;
				if (rect.x - (scrollSize / 2.f) < sizeX)
				{
					rect.x++;
				}
				else
				{
					if (loop == true)
					{
						rect.x -= sizeX + scrollSize;
					}
					finished = true;
				}
				break;
			}
			default:
				break;
			}

			if (finished == true)
			{
				if (loop == false)
				{
					pause = true;
				}

				game.Events().tryAddBack(completeAction);
			}

			view.setCenter(rect);
		});
	}
}

bool Scrollable::getProperty(const std::string_view prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	switch (propHash)
	{
	case str2int16("loop"):
		var = Variable(loop);
		break;
	case str2int16("paused"):
		var = Variable(pause);
		break;
	case str2int16("scrollPosition"):
	{
		auto rect = view.getCenter();
		switch (direction)
		{
		case Direction::Up:
		case Direction::Down:
			var = Variable((int64_t)rect.y);
			break;
		case Direction::Left:
		case Direction::Right:
			var = Variable((int64_t)rect.x);
			break;
		default:
			break;
		}
		break;
	}
	default:
	{
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
	}
	return true;
}
