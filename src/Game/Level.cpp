#include "Level.h"
#include "Game.h"
#include "GameUtils.h"
#include <iostream>
#include "LevelHelper.h"
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

void Level::move(Game& game)
{
	auto mousePos = game.MousePosition() - view.getPosition();
	const auto& size = view.getSize();
	mousePos.y = (mousePos.y * (float)game.WindowTexSize().y) / size.y;

	mousePos.y -= 48.f;

	auto posNew = Render::getClickedTile(size.x, size.y, *this, mousePos.x, mousePos.y, pos.x, pos.y, pos.x, pos.y, 0);
	pos = sf::Vector2i(posNew.x, posNew.y);
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
			Render::drawLevelHelper(*this, tiles, x, y, levelX_, levelY_, target, states);
		}
	}

	for (size_t x = 0; x < map.Width(); x++)
	{
		for (size_t y = 0; y < map.Height(); y++)
		{
			Render::drawLevelHelper2(*this, tiles2, x, y, levelX_, levelY_, target, states);
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
	levelX_ = levelX;
	levelY_ = levelY;
}

void Level::update(Game& game)
{
	if (visible == false)
	{
		return;
	}

	mousePos = game.MousePosition() - view.getPosition();
	if (mainPlayer != nullptr)
	{
		mainPlayer->update(game, *this);
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

	if (levelX_ == 0)
	{
		levelX_ = levelX;
		levelY_ = levelY;
	}

	if (levelX > levelX_) {
		levelX_++;
	}
	else if (levelX < levelX_) {
		levelX_--;
	}

	if (levelY > levelY_) {
		levelY_++;
	}
	else if (levelY < levelY_) {
		levelY_--;
	}

	if (currentPlayer != nullptr)
	{
		auto oldPos = currentPlayer->MapPosition();
		map[oldPos.x][oldPos.y].drawable = nullptr;
		currentPlayer->MapPosition(pos);
		map[pos.x][pos.y].drawable = currentPlayer.get();
	}
}

Variable Level::getProperty(const std::string& prop) const
{
	if (prop.size() > 1)
	{
		auto props = Utils::splitString(prop, '.');
		if (props.size() > 0)
		{
			auto propHash = str2int(props[0].c_str());
			switch (propHash)
			{
			case str2int("player"):
			{
				if (props.size() > 2)
				{
					if (mainPlayer != nullptr && mainPlayer->Id() == props[1])
					{
						return mainPlayer->getProperty(props[2]);
					}
					for (const auto& player : players)
					{
						if (player->Id() == props[1])
						{
							return player->getProperty(props[2]);
						}
					}
				}
			}
			break;
			case str2int("currentPlayer"):
			{
				if (props.size() > 1 && currentPlayer != nullptr)
				{
					return currentPlayer->getProperty(props[1]);
				}
			}
			break;
			default:
				return GameUtils::getProperty(*this, propHash, props);
			}
		}
	}
	return Variable();
}

Player* Level::getPlayer(const std::string id)
{
	if (mainPlayer != nullptr)
	{
		if (mainPlayer->Id() == id)
		{
			return mainPlayer.get();
		}
	}
	for (auto& player : players)
	{
		if (player->Id() == id)
		{
			return player.get();
		}
	}
	return nullptr;
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
