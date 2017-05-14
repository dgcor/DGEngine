#pragma once

#include "Actions/Action.h"
#include "CelCache.h"
#include "ItemClass.h"
#include "ItemLocation.h"
#include "LevelMap.h"
#include <memory>
#include "Min.h"
#include "Namer.h"
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
	View2 view;
	float currentZoomFactor{ 1.f };
	float startZoomFactor{ 1.f };
	float stopZoomFactor{ 1.f };
	float diffZoomFactor{ 0.f };
	bool hasNewZoom{ false };
	bool smoothZoom{ false };
	float zoomStepStart{ 0.f };		// in seconds
	float zoomStepStop{ 1.f };		// in seconds

	LevelMap map;

	sf::Vector2f mousePositionf;
	bool hasMouseInside{ false };

	MapCoord mapCoordOverMouse;
	MapCoord currentMapPosition;

	std::string name;

	std::vector<std::shared_ptr<sf::Texture>> tiles;
	std::vector<std::shared_ptr<sf::Texture>> tiles2;

	std::shared_ptr<Action> leftAction;
	std::shared_ptr<Action> rightAction;
	std::shared_ptr<Action> hoverEnterAction;
	std::shared_ptr<Action> hoverLeaveAction;
	std::shared_ptr<Action> scrollDownAction;
	std::shared_ptr<Action> scrollUpAction;

	MapCoord clickedMapPosition;

	LevelObject* clickedObject{ nullptr };
	LevelObject* hoverObject{ nullptr };

	std::vector<std::shared_ptr<LevelObject>> levelObjects;

	std::unordered_map<std::string, std::shared_ptr<Namer>> namers;
	std::unordered_map<std::string, std::shared_ptr<ItemClass>> itemClasses;
	std::vector<std::pair<std::string, std::shared_ptr<PlayerClass>>> playerClasses;
	std::vector<std::shared_ptr<Player>> players;
	Player* currentPlayer{ nullptr };
	bool followCurrentPlayer{ true };

	bool pause{ false };
	bool visible{ true };

	std::vector<Quest> quests;

	static const LevelCell& get(Coord x, Coord y, const Level& level)
	{
		return level.map[x][y];
	}

	void updateZoom(const Game& game);

	void updateMouse(const Game& game);

public:
	void Init(const LevelMap& map, Min& min, CelFrameCache& cel);

	Misc::Helper2D<const Level, const LevelCell&, Coord> operator[] (Coord x) const
	{
		return Misc::Helper2D<const Level, const LevelCell&, Coord>(*this, x, get);
	}

	const sf::Vector2f& MousePositionf() const { return mousePositionf; }
	bool HasMouseInside() const { return hasMouseInside; }

	LevelMap& Map() { return map; }
	const LevelMap& Map() const { return map; }

	Coord Width() const { return map.Width(); }
	Coord Height() const { return map.Height(); }

	void Name(const std::string& name_) { name = name_; }

	void clearLevelObjects() { levelObjects.clear(); }

	void addNamer(const std::string& key, const std::shared_ptr<Namer>& obj)
	{
		namers.insert(std::make_pair(key, obj));
	}

	std::shared_ptr<Namer> getNamer(const std::string& key) const
	{
		auto it = namers.find(key);
		if (it != namers.end())
		{
			return it->second;
		}
		return nullptr;
	}

	bool hasItemClass(const std::string& key) const
	{
		return itemClasses.find(key) != itemClasses.end();
	}

	void addItemClass(const std::string& key, const std::shared_ptr<ItemClass>& obj)
	{
		itemClasses.insert(std::make_pair(key, obj));
	}

	ItemClass* getItemClass(const std::string& key) const
	{
		auto it = itemClasses.find(key);
		if (it != itemClasses.end())
		{
			return it->second.get();
		}
		return nullptr;
	}

	void addPlayerClass(const std::string& key, const std::shared_ptr<PlayerClass>& obj)
	{
		playerClasses.push_back(std::make_pair(key, obj));
	}

	PlayerClass* getPlayerClass(const std::string& key) const
	{
		for (const auto& player : playerClasses)
		{
			if (player.first == key)
			{
				return player.second.get();
			}
		}
		return nullptr;
	}

	std::vector<std::shared_ptr<Player>>& Players() { return players; }
	const std::vector<std::shared_ptr<Player>>& Players() const { return players; }

	Player* getPlayer(const std::string& id) const;
	Player* getPlayerOrCurrent(const std::string& id) const;

	// doesn't clear currently used player classes
	void clearPlayerClasses(size_t clearIdx);
	void clearPlayers(size_t clearIdx);

	void resetView() { view.reset(); }

	void executeHoverEnterAction(Game& game);
	void executeHoverLeaveAction(Game& game);

	const MapCoord& getClickedMapPosition() const { return clickedMapPosition; }

	LevelObject* getClickedObject() const { return clickedObject; }
	void setClickedObject(LevelObject* object) { clickedObject = object; }

	LevelObject* getHoverObject() const { return hoverObject; }
	void setHoverObject(LevelObject* object) { hoverObject = object; }

	virtual void setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action);

	virtual void setAnchor(const Anchor anchor) { view.setAnchor(anchor); }
	virtual void updateSize(const Game& game);

	virtual const sf::Vector2f& DrawPosition() const { return view.getPosition(); }
	virtual const sf::Vector2f& Position() const { return view.getPosition(); }
	virtual void Position(const sf::Vector2f& position) { view.setPosition(position); }
	virtual sf::Vector2f Size() const { return view.getSize(); }
	virtual void Size(const sf::Vector2f& size) { view.setSize(size); }

	float Zoom() const { return stopZoomFactor; }
	void Zoom(float factor, bool smooth = false);

	void updateViewport(const Game& game) { view.updateViewport(game); }

	void addLevelObject(const std::shared_ptr<LevelObject>& obj) { levelObjects.push_back(obj); }

	void deleteLevelObject(const LevelObject* obj);

	MapCoord getMapCoordOverMouse() const { return mapCoordOverMouse; }

	void move(const MapCoord& mapPos)
	{
		currentMapPosition = mapPos;
		auto center = map.getCoord(mapPos);
		view.setCenter(center.x, center.y);
	}

	void move(Game& game)
	{
		view.setCenter(mousePositionf);
	}

	void addPlayer(const std::shared_ptr<Player>& player_) { players.push_back(player_); }
	Player* getCurrentPlayer() const { return currentPlayer; }
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
	virtual const Queryable* getQueryable(const std::string& prop) const;

	std::shared_ptr<Item> getItem(const MapCoord& mapCoord) const;
	std::shared_ptr<Item> getItem(const ItemCoordInventory& itemCoord) const;
	std::shared_ptr<Item> getItem(const ItemLocation& location) const;
	bool setItem(const MapCoord& mapCoord, const std::shared_ptr<Item>& item);
	bool setItem(const ItemCoordInventory& itemCoord, const std::shared_ptr<Item>& item);
	bool setItem(const ItemLocation& location, const std::shared_ptr<Item>& item);

	const std::vector<Quest>& Quests() const { return quests; };
	void addQuest(const Quest& quest_);
	void deleteQuest(const std::string& questId);
	void setQuestState(const std::string& questId, int state);
};
