#pragma once

#include "Actions/Action.h"
#include "CelCache.h"
#include "Player.h"
#include "LevelMap.h"
#include <memory>
#include "Min.h"
#include "Palette.h"
#include "PlayerClass.h"
#include "Quest.h"
#include "Sol.h"
#include "TileSet.h"
#include "UIObject.h"
#include <unordered_map>
#include <utility>
#include "View2.h"

class Level : public UIObject
{
private:
	LevelMap map;

	sf::Vector2f mousePos;
	View2 view;
	sf::Vector2i pos;

	Anchor anchor{ Anchor::Top | Anchor::Left };

	int32_t levelX, levelY;

	std::vector<std::shared_ptr<sf::Texture>> tiles;
	std::vector<std::shared_ptr<sf::Texture>> tiles2;

	std::shared_ptr<Action> leftAction;
	std::shared_ptr<Action> rightAction;

	std::vector<std::pair<std::string, std::shared_ptr<PlayerClass>>> playerClasses;
	std::vector<std::shared_ptr<Player>> players;
	Player* currentPlayer;
	bool followCurrentPlayer{ true };

	size_t playerClassClearIdx{ 0 };
	size_t playerClearIdx{ 0 };

	bool visible{ true };

	std::vector<Quest> quests;

	static const LevelCell get(size_t x, size_t y, const Level& level)
	{
		return level.map[x][y];
	}

public:
	void Init(const LevelMap& map, Min& min, CelFrameCache& cel);

	Misc::Helper2D<const Level, const LevelCell> operator[] (size_t x) const
	{
		return Misc::Helper2D<const Level, const LevelCell>(*this, x, get);
	}

	const sf::Vector2f& MousePosition() const { return mousePos; }

	LevelMap& Map() { return map; }
	const LevelMap& Map() const { return map; }

	int32_t getLevelX() const { return levelX; }
	int32_t getLevelY() const { return levelY; }

	size_t Width() const { return map.Width(); }
	size_t Height() const { return map.Height(); }

	void addPlayerClass(const std::string& key, const std::shared_ptr<PlayerClass>& obj)
	{
		playerClasses.push_back(std::make_pair(key, obj));
	}

	std::shared_ptr<PlayerClass> getPlayerClass(const std::string& key) const
	{
		for (const auto& player : playerClasses)
		{
			if (player.first == key)
			{
				return player.second;
			}
		}
		return nullptr;
	}

	std::vector<std::shared_ptr<Player>>& Players() { return players; }
	const std::vector<std::shared_ptr<Player>>& Players() const { return players; }

	Player* getPlayer(const std::string id);

	void setPlayerClassClearIdx(size_t idx) { playerClassClearIdx = idx; }
	void setPlayerClearIdx(size_t idx) { playerClearIdx = idx; }

	void clearPlayers();

	void resetView()
	{
		const auto& vSize = view.getSize();
		view.reset(sf::FloatRect(0, 0, vSize.x, vSize.y));
	}

	void updateViewPort(const Game& game);

	void setLeftAction(const std::shared_ptr<Action>& action) { leftAction = action; }
	void setRightAction(const std::shared_ptr<Action>& action) { rightAction = action; }

	virtual void setAnchor(const Anchor anchor_) { anchor = anchor_; }
	virtual void updateSize(const Game& game);

	virtual const sf::Vector2f& DrawPosition() const { return view.getPosition(); }
	virtual const sf::Vector2f& Position() const { return view.getPosition(); }
	virtual void Position(const sf::Vector2f& position) { view.setPosition(position); }
	virtual sf::Vector2f Size() const { return view.getSize(); }
	virtual void Size(const sf::Vector2f& size) { view.setSize(size); }

	sf::Vector2f getDrawPosition(const sf::Vector2i& pos_) const;

	sf::Vector2i getMapClickPosition(Game& game);

	void move(const sf::Vector2i& pos_) { pos = pos_; }

	void move(Game& game) { pos = getMapClickPosition(game); }

	void addPlayer(const std::shared_ptr<Player>& player_) { players.push_back(player_); }
	void setCurrentPlayer(Player* player_)
	{
		currentPlayer = player_;
		if (currentPlayer != nullptr)
		{
			pos = currentPlayer->MapPosition();
		}
	}

	void FollowCurrentPlayer(bool follow) { followCurrentPlayer = follow; }

	virtual bool Visible() const { return visible; }
	virtual void Visible(bool visible_) { visible = visible_; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void update(Game& game);
	virtual bool getProperty(const std::string& prop, Variable& var) const;

	const std::vector<Quest>& Quests() const { return quests; };
	void addQuest(const Quest& quest_);
	void deleteQuest(const std::string& questId);
	void setQuestState(const std::string& questId, int state);
};
