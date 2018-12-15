#pragma once

#include "Actions/Action.h"
#include "Classifier.h"
#include "InputEvent.h"
#include "ItemClass.h"
#include "ItemLocation.h"
#include "LevelMap.h"
#include "LevelObjectClass.h"
#include <memory>
#include "Palette.h"
#include "Player.h"
#include "PlayerClass.h"
#include "Quest.h"
#include "Save/SaveLevel.h"
#include "SFML/View2.h"
#include "Sol.h"
#include <string>
#include "TexturePacks/TexturePack.h"
#include "UIObject.h"
#include <unordered_map>
#include <utility>

class Level : public UIObject
{
private:
	View2 view{ true };
	View2 automapView{ true };
	sf::Vector2f automapRelativePosition{ 0.f, 0.f };	// relative to this UIObject
	sf::Vector2f automapRelativeSize{ 1.f, 1.f };
	bool viewPortNeedsUpdate{ false };
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
	sf::Vector2f currentAutomapViewCenter;

	std::string id;
	std::string name;
	std::string path;

	struct LevelLayer
	{
		std::shared_ptr<TexturePack> tiles;
		View2* view{ nullptr };
		sf::Color background{ sf::Color::Transparent };
		sf::FloatRect visibleRect;
		MapCoord visibleStart;
		MapCoord visibleEnd;
		int32_t tileWidth{ 0 };		// same size for all layers except for automap
		int32_t tileHeight{ 0 };
		int32_t blockWidth{ 0 };	// a tile is 4 blocks
		int32_t blockHeight{ 0 };
		bool visible{ false };		// currently, only used for automap
	};

public:
	static constexpr size_t NumberOfLayers = LevelCell::NumberOfLayers - 1;
	static constexpr size_t AutomapLayer = NumberOfLayers - 1;

private:
	std::array<LevelLayer, NumberOfLayers> levelLayers;

	int32_t automapPlayerDirectionBaseIndex{ -1 };

	std::shared_ptr<Action> leftAction;
	std::shared_ptr<Action> rightAction;
	std::shared_ptr<Action> hoverEnterAction;
	std::shared_ptr<Action> hoverLeaveAction;
	std::shared_ptr<Action> scrollDownAction;
	std::shared_ptr<Action> scrollUpAction;

	MapCoord clickedMapPosition;

	std::vector<std::unique_ptr<LevelObject>> levelObjects;
	std::unordered_map<std::string, LevelObject*> levelObjectIds;

	LevelObject* clickedObject{ nullptr };
	LevelObject* hoverObject{ nullptr };
	Player* currentPlayer{ nullptr };

	std::unordered_map<std::string, std::unique_ptr<Classifier>> classifiers;
	std::unordered_map<std::string, std::unique_ptr<LevelObjectClass>> levelObjectClasses;

	bool followCurrentPlayer{ true };

	bool pause{ false };
	bool visible{ true };
	InputEvent captureInputEvents{ InputEvent::All };

	std::vector<Quest> quests;
	std::vector<uint32_t> experiencePoints;
	std::unordered_map<uint16_t, std::string> propertyNames;

	static const LevelCell& get(Coord x, Coord y, const Level& level) noexcept
	{
		return level.map[x][y];
	}

	void updateAutomapPosition(sf::Vector2f position);
	void updateAutomapSize(sf::Vector2f position);

	void setCurrentMapViewCenter(const sf::Vector2f& coord_);

	void setCurrentMapPosition(const MapCoord& mapPos)
	{
		currentMapPosition = mapPos;
		setCurrentMapViewCenter(map.getCoord(currentMapPosition));
	}

	void addLevelObject(std::unique_ptr<LevelObject> obj, const MapCoord& mapCoord);

	// clears the clickedObject, hoverObject, currentPlayer if they're pointing to the given object.
	void clearCache(const LevelObject* obj) noexcept;

	// Removes level object from level. Object still needs to be deleted from map.
	// Returns the removed object.
	template <class T>
	std::unique_ptr<T> removeLevelObject(const LevelObject* obj)
	{
		std::unique_ptr<T> oldObj;
		for (auto it = levelObjects.begin(); it != levelObjects.end(); ++it)
		{
			if (it->get() == obj)
			{
				auto oldObjPtr = dynamic_cast<T*>(it->get());
				it->release();
				oldObj.reset(oldObjPtr);
				if (oldObjPtr->getId().empty() == false)
				{
					levelObjectIds.erase(oldObjPtr->getId());
				}
				levelObjects.erase(it);
				break;
			}
		}
		clearCache(obj);
		return oldObj;
	}

	LevelObject* parseLevelObjectIdOrMapPosition(
		const std::string_view str, std::string_view& props) const;

	void setTileLayer(size_t layer, View2* view_,
		std::shared_ptr<TexturePack> tiles,
		int tileWidth, int tileHeight, bool visible_);

	void updateLevelObjectPositions();
	void updateMouse(const Game& game);
	void updateVisibleArea();
	void updateZoom(const Game& game);

	void onMouseButtonPressed(Game& game);
	void onMouseScrolled(Game& game);
	void onTouchBegan(Game& game);

	friend void Save::save(const std::string_view filePath,
		Save::Properties& props, const Game& game, const Level& level);
	friend void Save::serialize(void* serializeObj,
		Save::Properties& props, const Game& game, const Level& level);

public:
	void Init(LevelMap map_,
		const std::vector<std::shared_ptr<TexturePack>>& texturePackLayers,
		int32_t tileWidth, int32_t tileHeight);
	void Init();

	void setAutomap(std::shared_ptr<TexturePack> tiles, int tileWidth, int tileHeight);

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

	void Id(const std::string_view id_) { id = id_; }
	void Name(const std::string_view name_) { name = name_; }
	void Path(const std::string_view path_) { path = path_; }

	void clearAllLevelObjects();

	template <class T>
	void clearLevelObjects()
	{
		auto it = levelObjects.begin();
		while (it != levelObjects.end())
		{
			if (dynamic_cast<T*>(it->get()) != nullptr)
			{
				const auto& mapPos = (*it)->MapPosition();
				if (map.isMapCoordValid(mapPos) == true)
				{
					map[mapPos].removeObject(it->get());
				}
				if ((*it)->getId().empty() == false)
				{
					levelObjectIds.erase((*it)->getId());
				}
				clearCache(it->get());
				it = levelObjects.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	template <class T>
	void clearLevelObjects(const std::vector<std::string>& excludeIds)
	{
		auto it = levelObjects.begin();
		while (it != levelObjects.end())
		{
			if (dynamic_cast<T*>(it->get()) != nullptr
				&& std::find(excludeIds.begin(), excludeIds.end(),
				(*it)->getId()) == excludeIds.end())
			{
				const auto& mapPos = (*it)->MapPosition();
				if (map.isMapCoordValid(mapPos) == true)
				{
					map[mapPos].removeObject(it->get());
				}
				if ((*it)->getId().empty() == false)
				{
					levelObjectIds.erase((*it)->getId());
				}
				clearCache(it->get());
				it = levelObjects.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	template <class T>
	T* getLevelObject(const std::string id) const
	{
		return dynamic_cast<T*>(getLevelObject(id));
	}

	LevelObject* getLevelObject(const std::string id) const;
	LevelObject* getLevelObjectByClass(const std::string_view classId) const noexcept;

	void addClassifier(const std::string key, std::unique_ptr<Classifier> obj)
	{
		classifiers.insert(std::make_pair(key, std::move(obj)));
	}

	Classifier* getClassifier(const std::string& key) const
	{
		auto it = classifiers.find(key);
		if (it != classifiers.end())
		{
			return it->second.get();
		}
		return nullptr;
	}

	bool hasClass(const std::string& key) const
	{
		return levelObjectClasses.find(key) != levelObjectClasses.end();
	}

	void addClass(const std::string key, std::unique_ptr<LevelObjectClass> obj)
	{
		levelObjectClasses.insert(std::make_pair(key, std::move(obj)));
	}

	template <class T>
	T* getClass(const std::string& key) const
	{
		auto it = levelObjectClasses.find(key);
		if (it != levelObjectClasses.end())
		{
			return dynamic_cast<T*>(it->second.get());
		}
		return nullptr;
	}

	Player* getPlayerOrCurrent(const std::string id) const noexcept;

	// doesn't clear currently used player classes
	void clearPlayerClasses();
	// doesn't clear currently used player classes
	void clearPlayerTextures() noexcept;

	void resetView()
	{
		view.reset();
		automapView.reset();
	}

	void executeHoverEnterAction(Game& game);
	void executeHoverLeaveAction(Game& game);

	const MapCoord& getClickedMapPosition() const noexcept { return clickedMapPosition; }

	LevelObject* getClickedObject() const noexcept { return clickedObject; }
	void setClickedObject(LevelObject* object) noexcept { clickedObject = object; }

	LevelObject* getHoverObject() const noexcept { return hoverObject; }
	void setHoverObject(LevelObject* object) noexcept { hoverObject = object; }

	virtual std::shared_ptr<Action> getAction(uint16_t nameHash16) const noexcept;
	virtual bool setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept;

	virtual void setAnchor(const Anchor anchor) noexcept
	{
		view.setAnchor(anchor);
		automapView.setAnchor(anchor);
	}
	virtual void updateSize(const Game& game);

	virtual const sf::Vector2f& DrawPosition() const noexcept { return view.getPosition(); }
	virtual const sf::Vector2f& Position() const noexcept { return view.getPosition(); }
	virtual void Position(const sf::Vector2f& position) noexcept;
	virtual sf::Vector2f Size() const noexcept { return view.getSize(); }
	virtual void Size(const sf::Vector2f& size);

	// between [0, 100]
	void setAutomapRelativePosition(const sf::Vector2i& position) noexcept;
	// between [0, 100]
	void setAutomapRelativeSize(const sf::Vector2i& size);

	void setAutomapBackgroundColor(sf::Color color) { levelLayers[AutomapLayer].background = color; }

	float Zoom() const noexcept { return stopZoomFactor; }
	void Zoom(float factor, bool smooth = false) noexcept;

	void updateViewport(const Game& game)
	{
		view.updateViewport(game);
		automapView.updateViewport(game);
	}

	void addLevelObject(std::unique_ptr<LevelObject> obj);

	// Deletes level object by id. If id is empty, no action is performed.
	void deleteLevelObjectById(const std::string_view id);

	// Deletes level object by class id. If id is empty, no action is performed.
	void deleteLevelObjectByClass(const std::string_view classId);

	MapCoord getMapCoordOverMouse() const noexcept { return mapCoordOverMouse; }

	void move(const MapCoord& mapPos)
	{
		setCurrentMapPosition(mapPos);
	}
	void move() noexcept
	{
		setCurrentMapViewCenter(mousePositionf);
	}

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

	int32_t TileWidth() const noexcept { return levelLayers[0].tileWidth; }
	int32_t TileHeight() const noexcept { return levelLayers[0].tileHeight; }

	int32_t AutomapTileWidth() const noexcept { return levelLayers[AutomapLayer].tileWidth; }
	int32_t AutomapTileHeight() const noexcept { return levelLayers[AutomapLayer].tileHeight; }

	virtual bool Pause() const noexcept { return pause; }
	virtual void Pause(bool pause_) noexcept { pause = pause_; }

	virtual bool Visible() const noexcept { return visible; }
	virtual void Visible(bool visible_) noexcept { visible = visible_; }

	// setting a negative value will disable drawing player direction in the automap
	void setAutomapPlayerDirectionBaseIndex(int32_t index) noexcept
	{
		automapPlayerDirectionBaseIndex = index;
	}

	bool ShowAutomap() const noexcept { return levelLayers[AutomapLayer].visible; }
	void ShowAutomap(bool show_) noexcept { levelLayers[AutomapLayer].visible = show_; }

	InputEvent getCaptureInputEvents() const noexcept { return captureInputEvents; }
	void setCaptureInputEvents(InputEvent e) noexcept { captureInputEvents = e; }

	void save(const std::string_view filePath,
		Save::Properties& props, const Game& game) const
	{
		Save::save(filePath, props, game, *this);
	}
	virtual void serialize(void* serializeObj,
		Save::Properties& props, const Game& game, const Level& level)
	{
		Save::serialize(serializeObj, props, game, *this);
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void update(Game& game);
	virtual bool getProperty(const std::string_view prop, Variable& var) const;
	virtual const Queryable* getQueryable(const std::string_view prop) const;

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

	// returns the remaining quantity to add/remove. 0 if all quantity was added.
	LevelObjValue addItemQuantity(const ItemLocation& location, LevelObjValue amount);

	const std::vector<Quest>& Quests() const noexcept { return quests; };
	void addQuest(Quest quest_);
	void clearQuests();
	void deleteQuest(const std::string_view questId);
	bool hasQuest(const std::string_view questId) const noexcept;
	void setQuestState(const std::string_view questId, int state) noexcept;

	void setExperiencePoints(const std::vector<uint32_t>& experiencePoints_)
	{
		experiencePoints = experiencePoints_;
	}

	uint32_t getExperienceFromLevel(uint32_t level) const;
	uint32_t getLevelFromExperience(uint32_t experience) const;

	const std::string_view getPropertyName(uint16_t hash16) const;
	void setPropertyName(uint16_t hash16, const std::string_view prop);
};
