#pragma once

#include "Actions/Action.h"
#include "ItemClass.h"
#include "ItemLocation.h"
#include "LevelMap.h"
#include <memory>
#include "Namer.h"
#include "Palette.h"
#include "Player.h"
#include "PlayerClass.h"
#include "Quest.h"
#include "Save/SaveLevel.h"
#include "Sol.h"
#include <string>
#include "TexturePacks/TexturePack.h"
#include "UIObject.h"
#include <unordered_map>
#include <utility>
#include "View2.h"

class Level : public UIObject
{
private:
	View2 view;
	sf::FloatRect visibleRect;
	MapCoord visibleStart;
	MapCoord visibleEnd;
	bool updateViewPort{ false };
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
	sf::Vector2f currentMapViewCenter;

	std::string id;
	std::string name;
	std::string path;

	std::shared_ptr<TexturePack> tilesBottom;
	std::shared_ptr<TexturePack> tilesTop;

	int tileWidth{ 0 };
	int tileHeight{ 0 };

	std::shared_ptr<Action> leftAction;
	std::shared_ptr<Action> rightAction;
	std::shared_ptr<Action> hoverEnterAction;
	std::shared_ptr<Action> hoverLeaveAction;
	std::shared_ptr<Action> scrollDownAction;
	std::shared_ptr<Action> scrollUpAction;

	MapCoord clickedMapPosition;

	std::vector<std::unique_ptr<LevelObject>> levelObjects;
	std::vector<std::unique_ptr<Player>> players;

	LevelObject* clickedObject{ nullptr };
	LevelObject* hoverObject{ nullptr };
	Player* currentPlayer{ nullptr };

	std::unordered_map<std::string, std::unique_ptr<Namer>> namers;
	std::unordered_map<std::string, std::unique_ptr<ItemClass>> itemClasses;
	std::vector<std::pair<std::string, std::unique_ptr<PlayerClass>>> playerClasses;

	bool followCurrentPlayer{ true };

	bool pause{ false };
	bool visible{ true };
	bool captureInputEvents{ true };

	std::vector<Quest> quests;
	std::vector<uint32_t> experiencePoints;
	std::unordered_map<uint16_t, std::string> propertyNames;

	static const LevelCell& get(Coord x, Coord y, const Level& level) noexcept
	{
		return level.map[x][y];
	}

	void setCurrentMapPosition(const MapCoord& mapPos)
	{
		currentMapPosition = mapPos;
		currentMapViewCenter = map.getCoord(currentMapPosition);
	}

	void addLevelObject(std::unique_ptr<LevelObject> obj, const MapCoord& mapCoord, bool addToFront);
	void addLevelObject(std::unique_ptr<LevelObject> obj, LevelCell& mapCell, bool addToFront);
	void addPlayer(std::unique_ptr<Player> player, const MapCoord& mapCoord);

	void updateLevelObjectPositions();
	void updateMouse(const Game& game);
	void updateVisibleArea();
	void updateZoom(const Game& game);

	void onMouseButtonPressed(Game& game);
	void onMouseScrolled(Game& game);
	void onTouchBegan(Game& game);

	friend void Save::save(const std::string& filePath, const Level& level,
		bool skipDefaults, bool skipCurrentPlayer);
	friend void Save::serialize(void* serializeObj, const Level& level,
		bool skipDefaults, bool skipCurrentPlayer);

public:
	void Init(LevelMap map_,
		std::shared_ptr<TexturePack> tilesBottom_,
		std::shared_ptr<TexturePack> tilesTop_,
		int tileWidth_, int tileHeight_);

	Misc::Helper2D<const Level, const LevelCell&, Coord> operator[] (Coord x) const noexcept
	{
		return Misc::Helper2D<const Level, const LevelCell&, Coord>(*this, x, get);
	}

	const sf::Vector2f& MousePositionf() const noexcept { return mousePositionf; }
	bool HasMouseInside() const noexcept { return hasMouseInside; }

	LevelMap& Map() noexcept { return map; }
	const LevelMap& Map() const noexcept { return map; }

	Coord Width() const noexcept { return map.Width(); }
	Coord Height() const noexcept { return map.Height(); }

	void Id(const std::string& id_) { id = id_; }
	void Name(const std::string& name_) { name = name_; }
	void Path(const std::string& path_) { path = path_; }

	void clearLevelObjects();

	void addNamer(const std::string key, std::unique_ptr<Namer> obj)
	{
		namers.insert(std::make_pair(key, std::move(obj)));
	}

	Namer* getNamer(const std::string& key) const
	{
		auto it = namers.find(key);
		if (it != namers.end())
		{
			return it->second.get();
		}
		return nullptr;
	}

	bool hasItemClass(const std::string& key) const
	{
		return itemClasses.find(key) != itemClasses.end();
	}

	void addItemClass(const std::string key, std::unique_ptr<ItemClass> obj)
	{
		itemClasses.insert(std::make_pair(key, std::move(obj)));
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

	void addPlayerClass(const std::string key, std::unique_ptr<PlayerClass> obj)
	{
		playerClasses.push_back(std::make_pair(key, std::move(obj)));
	}

	PlayerClass* getPlayerClass(const std::string& key) const noexcept
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

	std::vector<std::unique_ptr<Player>>& Players() noexcept { return players; }
	const std::vector<std::unique_ptr<Player>>& Players() const noexcept { return players; }

	Player* getPlayer(const std::string& id) const noexcept;
	Player* getPlayerOrCurrent(const std::string& id) const noexcept;

	// doesn't clear currently used player classes
	void clearPlayerClasses(size_t clearIdx);
	// doesn't clear currently used player classes
	void clearPlayerTextures() noexcept;
	void clearPlayers(size_t clearIdx);

	void resetView() { view.reset(); }

	void executeHoverEnterAction(Game& game);
	void executeHoverLeaveAction(Game& game);

	const MapCoord& getClickedMapPosition() const noexcept { return clickedMapPosition; }

	LevelObject* getClickedObject() const noexcept { return clickedObject; }
	void setClickedObject(LevelObject* object) noexcept { clickedObject = object; }

	LevelObject* getHoverObject() const noexcept { return hoverObject; }
	void setHoverObject(LevelObject* object) noexcept { hoverObject = object; }

	virtual std::shared_ptr<Action> getAction(uint16_t nameHash16) const noexcept;
	virtual bool setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept;

	virtual void setAnchor(const Anchor anchor) noexcept { view.setAnchor(anchor); }
	virtual void updateSize(const Game& game);

	virtual const sf::Vector2f& DrawPosition() const noexcept { return view.getPosition(); }
	virtual const sf::Vector2f& Position() const noexcept { return view.getPosition(); }
	virtual void Position(const sf::Vector2f& position) noexcept
	{
		view.setPosition(position);
		updateViewPort = true;
	}
	virtual sf::Vector2f Size() const noexcept { return view.getSize(); }
	virtual void Size(const sf::Vector2f& size)
	{
		view.setSize(size);
		updateViewPort = true;
	}

	float Zoom() const noexcept { return stopZoomFactor; }
	void Zoom(float factor, bool smooth = false) noexcept;

	void updateViewport(const Game& game) { view.updateViewport(game); }

	void addLevelObject(std::unique_ptr<LevelObject> obj, bool addToFront);

	template <class T>
	std::unique_ptr<T> deleteLevelObject(const LevelObject* obj)
	{
		std::unique_ptr<T> oldObj;
		for (auto it = levelObjects.begin(); it != levelObjects.end(); ++it)
		{
			if (it->get() == obj)
			{
				auto oldObjPtr = dynamic_cast<T*>(it->get());
				it->release();
				oldObj.reset(oldObjPtr);
				levelObjects.erase(it);
				break;
			}
		}
		if (clickedObject == obj)
		{
			clickedObject = nullptr;
		}
		if (hoverObject == obj)
		{
			hoverObject = nullptr;
		}
		return oldObj;
	}

	MapCoord getMapCoordOverMouse() const noexcept { return mapCoordOverMouse; }

	void move(const MapCoord& mapPos)
	{
		setCurrentMapPosition(mapPos);
	}
	void move() noexcept
	{
		currentMapViewCenter = mousePositionf;
	}

	void addPlayer(std::unique_ptr<Player> player);

	Player* getCurrentPlayer() const noexcept { return currentPlayer; }
	void setCurrentPlayer(Player* player_) noexcept
	{
		currentPlayer = player_;
		if (currentPlayer != nullptr)
		{
			setCurrentMapPosition(currentPlayer->MapPosition());
		}
	}

	void FollowCurrentPlayer(bool follow) noexcept { followCurrentPlayer = follow; }

	int TileWidth() const noexcept { return tileWidth; }
	int TileHeight() const noexcept { return tileHeight; }

	virtual bool Pause() const noexcept { return pause; }
	virtual void Pause(bool pause_) noexcept { pause = pause_; }

	virtual bool Visible() const noexcept { return visible; }
	virtual void Visible(bool visible_) noexcept { visible = visible_; }

	bool getCaptureInputEvents() const noexcept { return captureInputEvents; }
	void setCaptureInputEvents(bool captureEvents) noexcept { captureInputEvents = captureEvents; }

	void save(const std::string& filePath, bool skipDefaults, bool skipCurrentPlayer) const
	{
		Save::save(filePath, *this, skipDefaults, skipCurrentPlayer);
	}
	void save(const std::string& filePath, const LevelObject& obj, bool skipDefaults) const
	{
		Save::save(filePath, *this, obj, skipDefaults);
	}
	virtual void serialize(void* serializeObj, const Level& level, bool skipDefaults)
	{
		Save::serialize(serializeObj, *this, skipDefaults, false);
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void update(Game& game);
	virtual bool getProperty(const std::string& prop, Variable& var) const;
	virtual const Queryable* getQueryable(const std::string& prop) const;

	Item* getItem(const MapCoord& mapCoord) const noexcept;
	Item* getItem(const ItemCoordInventory& itemCoord) const;
	Item* getItem(const ItemCoordInventory& itemCoord, Player*& player) const;
	Item* getItem(const ItemLocation& location) const;
	Item* getItem(const ItemLocation& location, Player*& player) const;

	std::unique_ptr<Item> removeItem(const MapCoord& mapCoord);
	std::unique_ptr<Item> removeItem(const ItemCoordInventory& itemCoord);
	std::unique_ptr<Item> removeItem(const ItemCoordInventory& itemCoord, Player*& player);
	std::unique_ptr<Item> removeItem(const ItemLocation& location);
	std::unique_ptr<Item> removeItem(const ItemLocation& location, Player*& player);

	bool setItem(const MapCoord& mapCoord, std::unique_ptr<Item>& item);
	bool setItem(const ItemCoordInventory& itemCoord, std::unique_ptr<Item>& item);
	bool setItem(const ItemLocation& location, std::unique_ptr<Item>& item);

	bool deleteItem(const MapCoord& mapCoord)
	{
		std::unique_ptr<Item> nullItem;
		return setItem(mapCoord, nullItem);
	}
	bool deleteItem(const ItemCoordInventory& itemCoord)
	{
		std::unique_ptr<Item> nullItem;
		return setItem(itemCoord, nullItem);
	}
	bool deleteItem(const ItemLocation& location)
	{
		std::unique_ptr<Item> nullItem;
		return setItem(location, nullItem);
	}

	const std::vector<Quest>& Quests() const noexcept { return quests; };
	void addQuest(Quest quest_);
	void deleteQuest(const std::string& questId);
	void setQuestState(const std::string& questId, int state) noexcept;

	void setExperiencePoints(const std::vector<uint32_t>& experiencePoints_)
	{
		experiencePoints = experiencePoints_;
	}

	uint32_t getExperienceFromLevel(uint32_t level) const;
	uint32_t getLevelFromExperience(uint32_t experience) const;

	const char* getPropertyName(uint16_t hash16) const;
	void setPropertyName(uint16_t hash16, const std::string& prop);
};
