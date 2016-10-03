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
	pos = sf::Vector2i((int)map.Width() / 2, (int)map.Height() / 2);
	tiles = Render::loadTilesetSprite(cel_, min_, false);
	tiles2 = Render::loadTilesetSprite(cel_, min_, true);
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

sf::Vector2f Level::getDrawPosition(const sf::Vector2i& pos_) const
{
	int32_t drawX, drawY;
	const auto& size = view.getSize();
	Render::getMapScreenCoords(size.x, size.y, *this,
		pos_.x, pos_.y, pos_.x, pos_.y, 0, drawX, drawY);

	return sf::Vector2f((float)drawX, (float)drawY);
}

sf::Vector2i Level::getMapClickPosition(Game& game)
{
	auto mousePos = game.MousePosition() - view.getPosition();
	const auto& size = view.getSize();
	mousePos.y = (mousePos.y * (float)game.WindowTexSize().y) / size.y;

	mousePos.y -= 48.f;

	return Render::getClickedTile(size.x, size.y, *this, mousePos.x, mousePos.y, pos.x, pos.y, pos.x, pos.y, 0);
}

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (visible == false)
	{
		return;
	}

	auto origView = target.getView();
	target.setView(view);

	for (size_t x = 0; x < map.Width(); x++)
	{
		for (size_t y = 0; y < map.Height(); y++)
		{
			Render::drawLevelHelper(*this, tiles, x, y, levelX, levelY, target, states);
		}
	}

	for (size_t x = 0; x < map.Width(); x++)
	{
		for (size_t y = 0; y < map.Height(); y++)
		{
			Render::drawLevelHelper2(*this, tiles2, x, y, levelX, levelY, target, states);
		}
	}

	target.setView(origView);
}

void Level::updateSize(const Game& game)
{
	auto size = view.getSize();
	if (game.StretchToFit() == true)
	{
		view.setCenter(size.x / 2, size.y / 2);
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
	view.setCenter(size.x / 2, size.y / 2);

	Render::getMapScreenCoords(size.x, size.y, *this, pos.x, pos.y, pos.x, pos.y, 0, levelX, levelY);
}

void Level::update(Game& game)
{
	if (visible == false)
	{
		return;
	}

	mousePos = game.MousePosition() - view.getPosition();

	for (auto& obj : levelObjects)
	{
		obj->update(game, *this);
	}

	for (auto& player : players)
	{
		player->update(game, *this);
	}

	sf::FloatRect rect(view.getPosition(), view.getSize());
	if (rect.contains(game.MousePosition()))
	{
		if (game.wasMouseClicked() == true &&
			game.getMouseButton() == sf::Mouse::Left &&
			leftAction != nullptr)
		{
			game.Events().addBack(leftAction);
		}
		else if (game.wasMouseReleased() == true &&
			game.getMouseButton() == sf::Mouse::Right &&
			rightAction != nullptr)
		{
			game.Events().addBack(rightAction);
		}
	}

	const auto& size = view.getSize();
	Render::getMapScreenCoords(size.x, size.y, *this, pos.x, pos.y, pos.x, pos.y, 0, levelX, levelY);

	if (followCurrentPlayer == true && currentPlayer != nullptr)
	{
		pos = currentPlayer->MapPosition();
	}
}

bool Level::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitString(prop, '.');
	if (props.empty() == true)
	{
		return false;
	}
	auto propHash = str2int(props[0].c_str());
	switch (propHash)
	{
	case str2int("currentPlayer"):
	{
		if (props.size() > 1 && currentPlayer != nullptr)
		{
			return currentPlayer->getProperty(props[1], var);
		}
	}
	break;
	case str2int("hoverObject"):
	{
		if (props.size() > 1 && hoverObject != nullptr)
		{
			return hoverObject->getProperty(props[1], var);
		}
	}
	break;
	case str2int("player"):
	{
		if (props.size() > 2)
		{
			for (const auto& player : players)
			{
				if (player->Id() == props[1])
				{
					return player->getProperty(props[2], var);
				}
			}
		}
	}
	break;
	case str2int("quest"):
	{
		if (props.size() > 2)
		{
			for (const auto& quest : quests)
			{
				if (quest.Id() == props[1])
				{
					return quest.getProperty(props[2], var);
				}
			}
		}
	}
	break;
	default:
		return GameUtils::getUIObjProp(*this, propHash, props, var);
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
