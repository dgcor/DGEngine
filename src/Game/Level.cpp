#include "Level.h"
#include "Game.h"
#include "Game/LevelHelper.h"
#include "GameUtils.h"
#include <iostream>
#include "Utils.h"

using Utils::str2int;

void Level::Init(const LevelMap& map_, Min& min_, CelFrameCache& cel_)
{
	map = map_;
	currentMapPosition = MapCoord(map.Width() / 2, map.Height() / 2);
	tiles = LevelHelper::loadTilesetSprite(cel_, min_, false);
	tiles2 = LevelHelper::loadTilesetSprite(cel_, min_, true);
}

void Level::updateViewPort(const Game& game)
{
	view.setViewport(game);
}

void Level::executeHoverEnterAction(Game& game)
{
	if (hoverEnterAction != nullptr)
	{
		game.Events().addBack(hoverEnterAction);
	}
}

void Level::executeHoverLeaveAction(Game& game)
{
	if (hoverLeaveAction != nullptr)
	{
		game.Events().addFront(hoverLeaveAction);
	}
}

void Level::deleteLevelObject(const LevelObject* obj)
{
	auto it = std::find_if(levelObjects.begin(),
		levelObjects.end(),
		[&](std::shared_ptr<LevelObject> const& p)
	{
		return p.get() == obj;
	});
	if (it != levelObjects.end())
	{
		levelObjects.erase(it);
		if (clickedObject == obj)
		{
			clickedObject = nullptr;
		}
		if (hoverObject == obj)
		{
			hoverObject = nullptr;
		}
	}
}

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (visible == false)
	{
		return;
	}

	auto origView = target.getView();
	target.setView(view);

	sf::Sprite sprite;

	auto viewCenter = target.getView().getCenter();
	auto viewSize = target.getView().getSize();

	sf::FloatRect drawRect(viewCenter.x - (viewSize.x / 2) - 64,
		viewCenter.y - (viewSize.y / 2) - 256,
		viewSize.x + 64, viewSize.y + 256);

	for (size_t x = 0; x < map.Width(); x++)
	{
		for (size_t y = 0; y < map.Height(); y++)
		{
			size_t index = map[x][y].minIndex;
			auto coords = map.getCoords(MapCoord(x, y));

			if (drawRect.contains(coords) == true &&
				index < tiles.size())
			{
				sprite.setTexture(*tiles[index], true);
				sprite.setPosition(coords);
				target.draw(sprite, states);
			}
		}
	}

	for (size_t x = 0; x < map.Width(); x++)
	{
		for (size_t y = 0; y < map.Height(); y++)
		{
			size_t index = map[x][y].minIndex;
			auto coords = map.getCoords(MapCoord(x, y));

			if (drawRect.contains(coords) == true)
			{
				auto drawObj = map[x][y].object.get();
				if (drawObj != nullptr)
				{
					target.draw(*drawObj, states);
				}
				if (index < tiles2.size())
				{
					sprite.setTexture(*tiles2[index], true);
					sprite.setPosition(coords);
					target.draw(sprite, states);
				}
			}
		}
	}

	target.setView(origView);
}

void Level::updateSize(const Game& game)
{
	auto size = view.getSize();
	if (game.StretchToFit() == true)
	{
		view.setViewport(game);
	}
	else
	{
		auto vPos = view.getPosition();
		GameUtils::setAnchorPosSize(anchor, vPos, size, game.OldWindowSize(), game.WindowSize());
		view.setPosition(vPos);
		view.setSize(size);
		view.setViewport(game);
	}
}

void Level::updateMouse(Game& game)
{
	mousePositionf = view.getCenter() - (view.getSize() / 2.f) + game.MousePositionf();
	mousePositionf.x = std::round(mousePositionf.x);
	mousePositionf.y = std::round(mousePositionf.y);

	mapCoordOverMouse = map.getTile(mousePositionf);
}

void Level::update(Game& game)
{
	if (pause == true || visible == false)
	{
		return;
	}

	updateMouse(game);

	sf::FloatRect rect(view.getPosition(), view.getSize());
	if (rect.contains(game.MousePositionf()) == true)
	{
		hasMouseInside = true;
		if (game.wasMouseClicked() == true &&
			game.getMouseButton() == sf::Mouse::Left)
		{
			clickedMapPosition = getMapCoordOverMouse();
			clickedObject = nullptr;
			if (leftAction != nullptr)
			{
				game.Events().addBack(leftAction);
			}
		}
		else if (game.wasMouseReleased() == true &&
			game.getMouseButton() == sf::Mouse::Right &&
			rightAction != nullptr)
		{
			game.Events().addBack(rightAction);
		}
	}
	else
	{
		hasMouseInside = false;
	}

	for (auto& obj : levelObjects)
	{
		obj->update(game, *this);
	}

	for (auto& player : players)
	{
		player->update(game, *this);
	}

	if (followCurrentPlayer == true && currentPlayer != nullptr)
	{
		currentMapPosition = currentPlayer->MapPosition();
		view.setCenter(currentPlayer->getBasePosition());
	}
}

bool Level::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int(props.first.c_str());
	switch (propHash)
	{
	case str2int("clickedObject"):
	{
		if (clickedObject != nullptr)
		{
			return clickedObject->getProperty(props.second, var);
		}
	}
	case str2int("currentPlayer"):
	{
		if (currentPlayer != nullptr)
		{
			return currentPlayer->getProperty(props.second, var);
		}
	}
	break;
	case str2int("hasSelectedItem"):
	{
		var = Variable(selectedItem != nullptr);
		return true;
	}
	break;
	case str2int("hoverObject"):
	{
		if (hoverObject != nullptr)
		{
			return hoverObject->getProperty(props.second, var);
		}
	}
	break;
	case str2int("name"):
		var = Variable(name);
		return true;
		break;
	case str2int("player"):
	{
		auto props2 = Utils::splitStringIn2(props.second, '.');
		for (const auto& player : players)
		{
			if (player->Id() == props2.first)
			{
				return player->getProperty(props2.second, var);
			}
		}
	}
	break;
	case str2int("quest"):
	{
		auto props2 = Utils::splitStringIn2(props.second, '.');
		for (const auto& quest : quests)
		{
			if (quest.Id() == props2.first)
			{
				return quest.getProperty(props2.second, var);
			}
		}
	}
	break;
	case str2int("selectedItem"):
	{
		if (selectedItem != nullptr)
		{
			return selectedItem->getProperty(props.second, var);
		}
	}
	break;
	default:
		return GameUtils::getUIObjProp(*this, propHash, props.second, var);
	}
	return false;
}

Player* Level::getPlayer(const std::string id)
{
	for (auto& player : players)
	{
		if (player->Id() == id)
		{
			return player.get();
		}
	}
	return nullptr;
}

Player* Level::getPlayerOrCurrent(const std::string id)
{
	if (id.empty() == true)
	{
		return currentPlayer;
	}
	return getPlayer(id);
}

void Level::clearPlayerClasses(size_t clearIdx)
{
	if (clearIdx < playerClasses.size())
	{
		playerClasses.erase(playerClasses.begin() + clearIdx, playerClasses.end());
	}
}

void Level::clearPlayers(size_t clearIdx)
{
	if (clearIdx < players.size())
	{
		players.erase(players.begin() + clearIdx, players.end());
	}
	for (const auto& player : players)
	{
		if (player.get() == currentPlayer)
		{
			return;
		}
	}
	currentPlayer = nullptr;
}

void Level::addQuest(const Quest& quest_)
{
	for (const auto& quest : quests)
	{
		if (quest.Id() == quest_.Id())
		{
			return;
		}
	}
	quests.push_back(quest_);
}

void Level::deleteQuest(const std::string& questId)
{
	for (auto it = quests.begin(); it != quests.end(); ++it)
	{
		if (it->Id() == questId)
		{
			quests.erase(it);
			return;
		}
	}
}

void Level::setQuestState(const std::string& questId, int state)
{
	for (auto& quest : quests)
	{
		if (quest.Id() == questId)
		{
			quest.State(state);
			return;
		}
	}
}

void Level::updateLevelObjectPositions()
{
	for (auto& obj : levelObjects)
	{
		const auto& mapPosition = obj->MapPosition();
		map[mapPosition.x][mapPosition.y].object = obj;
	}
	for (auto& obj : players)
	{
		const auto& mapPosition = obj->MapPosition();
		map[mapPosition.x][mapPosition.y].object = obj;
	}
}
