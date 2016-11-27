#pragma once

#include "Actions/Action.h"
#include "CelCache.h"
#include "ItemClass.h"
#include "LevelMap.h"
#include <memory>
#include "Min.h"
#include "Palette.h"
#include "Player.h"
#include "PlayerClass.h"
#include "Quest.h"
#include "Sol.h"
#include <string>
#include "TileSet.h"
#include "UIObject.h"
#include <unordered_map>
#include <utility>
#include "View2.h"

class Level : public UIObject
{
private:
	LevelMap map;

	sf::Vector2f mousePositionf;
	bool hasMouseInside{ false };
	View2 view;
	MapCoord currentMapPosition;

	Anchor anchor{ Anchor::Top | Anchor::Left };

	std::string name;

	std::vector<std::shared_ptr<sf::Texture>> tiles;
	std::vector<std::shared_ptr<sf::Texture>> tiles2;

	std::shared_ptr<Action> leftAction;
	std::shared_ptr<Action> rightAction;
	std::shared_ptr<Action> hoverEnterAction;
	std::shared_ptr<Action> hoverLeaveAction;

	LevelObject* hoverObject{ nullptr };

	std::vector<std::shared_ptr<LevelObject>> levelObjects;

	std::unordered_map<std::string, std::shared_ptr<ItemClass>> itemClasses;
	std::vector<std::pair<std::string, std::shared_ptr<PlayerClass>>> playerClasses;
	std::vector<std::shared_ptr<Player>> players;
	Player* currentPlayer{ nullptr };
	bool followCurrentPlayer{ true };

	std::shared_ptr<Item> selectedItem;

	bool pause{ false };
	bool visible{ true };

	std::vector<Quest> quests;

	static const LevelCell& get(size_t x, size_t y, const Level& level)
	{
		return level.map[x][y];
	}

	void updateMouse(Game& game);

public:
	void Init(const LevelMap& map, Min& min, CelFrameCache& cel);

	Misc::Helper2D<const Level, const LevelCell&> operator[] (size_t x) const
	{
		return Misc::Helper2D<const Level, const LevelCell&>(*this, x, get);
	}

	const sf::Vector2f& MousePositionf() const { return mousePositionf; }
	bool HasMouseInside() const { return hasMouseInside; }

	LevelMap& Map() { return map; }
	const LevelMap& Map() const { return map; }

	size_t Width() const { return map.Width(); }
	size_t Height() const { return map.Height(); }

	void Name(const std::string& name_) { name = name_; }

	void clearLevelObjects() { levelObjects.clear(); }

	void addItemClass(const std::string& key, const std::shared_ptr<ItemClass>& obj)
	{
		itemClasses.insert(std::make_pair(key, obj));
	}

	std::shared_ptr<ItemClass> getItemClass(const std::string& key) const
	{
		for (const auto& item : itemClasses)
		{
			if (item.first == key)
			{
				return item.second;
			}
		}
		return nullptr;
	}

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
	Player* getPlayerOrCurrent(const std::string id);

	void clearPlayerClasses(size_t clearIdx);
	void clearPlayers(size_t clearIdx);

	void resetView()
	{
		const auto& vSize = view.getSize();
		view.reset(sf::FloatRect(0, 0, vSize.x, vSize.y));
	}

	void updateViewPort(const Game& game);

	void executeHoverEnterAction(Game& game);
	void executeHoverLeaveAction(Game& game);

	void setLeftAction(const std::shared_ptr<Action>& action) { leftAction = action; }
	void setRightAction(const std::shared_ptr<Action>& action) { rightAction = action; }
	void setHoverEnterAction(const std::shared_ptr<Action>& action) { hoverEnterAction = action; }
	void setHoverLeaveAction(const std::shared_ptr<Action>& action) { hoverLeaveAction = action; }

	LevelObject* getHoverObject() const { return hoverObject; }
	void setHoverObject(LevelObject* object) { hoverObject = object; }

	virtual void setAnchor(const Anchor anchor_) { anchor = anchor_; }
	virtual void updateSize(const Game& game);

	virtual const sf::Vector2f& DrawPosition() const { return view.getPosition(); }
	virtual const sf::Vector2f& Position() const { return view.getPosition(); }
	virtual void Position(const sf::Vector2f& position) { view.setPosition(position); }
	virtual sf::Vector2f Size() const { return view.getSize(); }
	virtual void Size(const sf::Vector2f& size) { view.setSize(size); }

	void addLevelObject(const std::shared_ptr<LevelObject>& obj) { levelObjects.push_back(obj); }

	void deleteLevelObject(const LevelObject* obj)
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
		}
	}

	MapCoord getMapCoordOverMouse() const
	{
		return map.getTile(mousePositionf);
	}

	void move(const MapCoord& mapPos)
	{
		currentMapPosition = mapPos;
		auto center = map.getCoords(mapPos);
		view.setCenter(center.x, center.y);
	}

	void move(Game& game)
	{
		view.setCenter(mousePositionf);
	}

	void addPlayer(const std::shared_ptr<Player>& player_) { players.push_back(player_); }
	void setCurrentPlayer(Player* player_)
	{
		currentPlayer = player_;
		if (currentPlayer != nullptr)
		{
			currentMapPosition = currentPlayer->MapPosition();
		}
	}

	void FollowCurrentPlayer(bool follow) { followCurrentPlayer = follow; }

	void updateLevelObjectPositions();

	virtual bool Pause() const { return pause; }
	virtual void Pause(bool pause_) { pause = pause_; }

	virtual bool Visible() const { return visible; }
	virtual void Visible(bool visible_) { visible = visible_; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void update(Game& game);
	virtual bool getProperty(const std::string& prop, Variable& var) const;

	const std::shared_ptr<Item>& SelectedItem() const { return selectedItem; }
	void SelectedItem(const std::shared_ptr<Item>& item) { selectedItem = item; }

	const std::vector<Quest>& Quests() const { return quests; };
	void addQuest(const Quest& quest_);
	void deleteQuest(const std::string& questId);
	void setQuestState(const std::string& questId, int state);
};
