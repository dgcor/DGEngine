#pragma once

#include "Actions/Action.h"
#include "CelCache.h"
#include "Player.h"
#include "LevelMap.h"
#include <memory>
#include "Min.h"
#include "Palette.h"
#include "Quest.h"
#include "Sol.h"
#include "TileSet.h"
#include "UIObject.h"
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
	int32_t levelX_{ 0 }, levelY_{ 0 };

	std::vector<std::shared_ptr<sf::Texture>> tiles;
	std::vector<std::shared_ptr<sf::Texture>> tiles2;

	std::shared_ptr<Action> leftAction;
	std::shared_ptr<Action> rightAction;

	std::shared_ptr<Player> mainPlayer;
	std::vector<std::shared_ptr<Player>> players;
	std::shared_ptr<Player> currentPlayer;

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
	int32_t getLevelX_() const { return levelX_; }
	int32_t getLevelY_() const { return levelY_; }

	size_t Width() const { return map.Width(); }
	size_t Height() const { return map.Height(); }

	std::shared_ptr<Player>& MainPlayer() { return mainPlayer; }
	const std::shared_ptr<Player>& MainPlayer() const { return mainPlayer; }

	std::vector<std::shared_ptr<Player>>& Players() { return players; }
	const std::vector<std::shared_ptr<Player>>& Players() const { return players; }

	Player* getPlayer(const std::string id);

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
	virtual void Size(const sf::Vector2f& size)
	{
		view.setSize(size);
		levelX_ = 0;
	}

	void move(const sf::Vector2i& pos_)
	{
		pos = pos_;
	}

	void move(Game& game);


	void setMainPlayer(const std::shared_ptr<Player>& player_) { mainPlayer = player_; }
	void addPlayer(const std::shared_ptr<Player>& player_) { players.push_back(player_); }
	void setCurrentPlayer(const std::shared_ptr<Player>& player_)
	{
		currentPlayer = player_;
		if (currentPlayer != nullptr)
		{
			pos = currentPlayer->MapPosition();
		}
	}

	virtual bool Visible() const { return visible; }
	virtual void Visible(bool visible_) { visible = visible_; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void update(Game& game);
	virtual Variable getProperty(const std::string& prop) const;

	const std::vector<Quest>& Quests() const { return quests; };
	void addQuest(const Quest& quest_);
	void deleteQuest(const std::string& questId);
	void setQuestState(const std::string& questId, int state);
};
