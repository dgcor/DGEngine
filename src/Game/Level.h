#pragma once

#include "Actions/Action.h"
#include "Classifier.h"
#include "InputEvent.h"
#include "ItemLocation.h"
#include "LevelLayer.h"
#include "LevelMap.h"
#include "LevelObjectClass.h"
#include "LevelSurface.h"
#include "Quest.h"
#include "Save/SaveLevel.h"
#include "UIObject.h"
#include <unordered_map>
#include "Utils/EasedValue.h"
#include "Utils/FixedArray.h"

class Panel;
class Player;

struct LevelDrawable
{
	std::string id;
	std::weak_ptr<Panel> drawable;
	std::weak_ptr<LevelObject> anchorTo;
	sf::Vector2f offset;
	float visibleRectOffset{ -1 };
};

class Level : public UIObject
{
private:
	LevelSurface surface;
	LevelSurface automapSurface;

	FixedArray<LevelLayer, 8> levelLayers;

	uint16_t indexToDrawLevelObjects{ 0 };
	int16_t automapPlayerDirectionBaseIndex{ -1 };

	sf::Shader* shader{ nullptr };
	sf::Vector2f automapPosition{ 0.f, 0.f };
	sf::Vector2f automapSize{ 1.f, 1.f };
	bool automapRelativeCoords{ true };
	bool viewNeedsUpdate{ false };
	EasedValuef zoomValue{ 1.f };

	LevelMap map;

	std::vector<LevelDrawable> drawables;

	sf::Vector2f mousePositionf;
	bool hasMouseInside{ false };

	PairFloat mapCoordOverMouse;
	PairFloat currentMapPosition;

	EasedValuef currentMapViewCenterX;
	EasedValuef currentMapViewCenterY;
	sf::Vector2f currentMapViewCenter;
	sf::Vector2f currentAutomapViewCenter;
	bool smoothMovement{ false };

	std::string id;
	std::string name;
	std::string path;

	std::shared_ptr<Action> leftAction;
	std::shared_ptr<Action> rightAction;
	std::shared_ptr<Action> hoverEnterAction;
	std::shared_ptr<Action> hoverLeaveAction;
	std::shared_ptr<Action> scrollDownAction;
	std::shared_ptr<Action> scrollUpAction;

	PairFloat clickedMapPosition;

	std::vector<std::shared_ptr<LevelObject>> levelObjects;
	std::unordered_map<std::string, std::shared_ptr<LevelObject>> levelObjectIds;

	std::weak_ptr<LevelObject> clickedObject;
	std::weak_ptr<LevelObject> hoverObject;
	std::weak_ptr<Player> currentPlayer;

	std::unordered_map<std::string, std::unique_ptr<Classifier>> classifiers;
	std::unordered_map<std::string, std::unique_ptr<LevelObjectClass>> levelObjectClasses;

	bool followCurrentPlayer{ true };

	bool pause{ false };
	bool visible{ true };
	bool enableHover{ true };
	InputEvent captureInputEvents{ InputEvent::All };

	std::vector<Quest> quests;
	std::vector<uint32_t> experiencePoints;
	std::unordered_map<uint16_t, std::string> propertyNames;

	static const LevelCell& get(int32_t x, int32_t y, const Level& level) noexcept
	{
		return level.map[x][y];
	}

	void updateAutomapRelativePosition();
	void updateAutomapRelativeSize();
	void updateCurrentAutomapViewCenter() noexcept;

	void setCurrentMapPosition(const PairFloat& mapPos, bool smooth) noexcept;
	void setCurrentMapViewCenter(const sf::Vector2f& coord_, bool smooth) noexcept;

	void addLevelObject(std::shared_ptr<LevelObject> obj, const PairFloat& mapCoord);

	// clears the clickedObject, hoverObject, currentPlayer if they're pointing to the given object.
	void clearCache(const LevelObject* obj) noexcept;

	// Removes level object from level. Object still needs to be deleted from map.
	// Returns the removed object.
	template <class T>
	std::shared_ptr<T> removeLevelObject(const LevelObject* obj)
	{
		std::shared_ptr<T> oldObj;
		for (auto it = levelObjects.begin(); it != levelObjects.end(); ++it)
		{
			if (it->get() == obj)
			{
				oldObj = std::move(std::dynamic_pointer_cast<T>(*it));
				if (oldObj->getId().empty() == false)
				{
					levelObjectIds.erase(oldObj->getId());
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

	void setLevelDrawablePosition(LevelDrawable& obj, Panel& panelObj);
	void updateDrawables(Game& game);
	void updateLevelObjectPositions();
	void updateMouse(const Game& game);
	void updateTilesetLayersVisibleArea();
	void updateZoom(const Game& game);

	void onMouseButtonPressed(Game& game);
	void onMouseScrolled(Game& game);
	void onTouchBegan(Game& game);

	friend void Save::save(const std::string_view filePath,
		Save::Properties& props, const Game& game, const Level& level);
	friend void Save::serialize(void* serializeObj,
		Save::Properties& props, const Game& game, const Level& level);

public:
	void Init(const Game& game, LevelMap map_,
		const std::vector<LevelLayer>& levelLayers_,
		int32_t tileWidth, int32_t tileHeight, int32_t indexToDrawObjects);
	void Init();

	void setShader(sf::Shader* shader_) noexcept { shader = shader_; }

	void addLayer(const ColorLevelLayer& layer,
		const sf::FloatRect& viewportOffset, bool automap);

	void addLayer(const TextureLevelLayer& layer,
		const sf::FloatRect& viewportOffset, bool automap);

	void setAutomap(const TilesetLevelLayer& automapLayer, int tileWidth,
		int tileHeight, const sf::FloatRect& viewportOffset);

	void addDrawable(LevelDrawable obj);
	Panel* getDrawable(size_t idx) const;
	LevelDrawable* getLevelDrawable(const std::string& id);
	size_t getItemCount() const noexcept { return drawables.size(); }

	Misc::Helper2D<const Level, const LevelCell&, int32_t> operator[] (int32_t x) const noexcept
	{
		return Misc::Helper2D<const Level, const LevelCell&, int32_t>(*this, x, get);
	}

	const sf::Vector2f& MousePositionf() const noexcept { return mousePositionf; }
	bool HasMouseInside() const noexcept { return hasMouseInside; }

	LevelMap& Map() noexcept { return map; }
	const LevelMap& Map() const noexcept { return map; }

	int32_t Width() const noexcept { return map.MapSizei().x; }
	int32_t Height() const noexcept { return map.MapSizei().y; }

	const sf::Vector2f& getCurrentMapViewCenter() const noexcept
	{
		return currentMapViewCenter;
	}
	const sf::Vector2f& getCurrentAutomapViewCenter() const noexcept
	{
		return currentAutomapViewCenter;
	}

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
	std::weak_ptr<LevelObject> getLevelObjectPtr(const std::string id) const;

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

	void executeHoverEnterAction(Game& game);
	void executeHoverLeaveAction(Game& game);

	const PairFloat& getClickedMapPosition() const noexcept { return clickedMapPosition; }

	LevelObject* getClickedObject() const noexcept { return clickedObject.lock().get(); }
	void setClickedObject(std::weak_ptr<LevelObject> object) noexcept { clickedObject = object; }

	LevelObject* getHoverObject() const noexcept { return hoverObject.lock().get(); }
	void setHoverObject(std::weak_ptr<LevelObject> object) noexcept { hoverObject = object; }

	virtual std::shared_ptr<Action> getAction(uint16_t nameHash16) const noexcept;
	virtual bool setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept;

	virtual Anchor getAnchor() const noexcept { return surface.getAnchor(); }
	virtual void setAnchor(const Anchor anchor) noexcept { surface.setAnchor(anchor); }

	Anchor getAutomapAnchor() const noexcept { return automapSurface.getAnchor(); }
	void setAutomapAnchor(const Anchor anchor) noexcept { automapSurface.setAnchor(anchor); }

	virtual void updateSize(const Game& game);

	virtual const sf::Vector2f& DrawPosition() const noexcept { return surface.Position(); }
	virtual const sf::Vector2f& Position() const noexcept { return surface.Position(); }
	virtual void Position(const sf::Vector2f& position) noexcept;
	virtual sf::Vector2f Size() const noexcept { return surface.Size(); }
	virtual void Size(const sf::Vector2f& size);

	bool getAutomapRelativeCoords() const noexcept { return automapRelativeCoords; }
	void setAutomapRelativeCoords(bool relative) noexcept { automapRelativeCoords = relative; }

	const sf::Vector2f& getAutomapPosition() const noexcept { return automapPosition; }
	const sf::Vector2f& getAutomapSize() const noexcept { return automapSize; }

	// if position is relative to level, between [0, 100]
	void setAutomapPosition(const sf::Vector2f& position) noexcept;
	// if position is relative to level, between [0, 100]
	void setAutomapSize(const sf::Vector2f& size);

	float Zoom() const noexcept { return zoomValue.getFinal(); }
	void Zoom(float factor, bool smooth = false) noexcept;

	void updateView() { viewNeedsUpdate = true; }

	void addLevelObject(std::shared_ptr<LevelObject> obj);

	// Deletes level object by id. If id is empty, no action is performed.
	void deleteLevelObjectById(const std::string_view id);

	// Deletes level object by class id. If id is empty, no action is performed.
	void deleteLevelObjectByClass(const std::string_view classId);

	const PairFloat& getMapCoordOverMouse() const noexcept { return mapCoordOverMouse; }

	void move(const PairFloat& mapPos, bool smooth)
	{
		setCurrentMapPosition(mapPos, smooth);
	}
	void move(bool smooth)
	{
		setCurrentMapViewCenter(mousePositionf, smooth);
	}

	void setSmoothMovement(bool smooth) noexcept { smoothMovement = smooth; }

	Player* getCurrentPlayer() const noexcept { return currentPlayer.lock().get(); }
	void setCurrentPlayer(std::weak_ptr<Player> player_) noexcept;

	bool FollowCurrentPlayer() const noexcept { return followCurrentPlayer; }
	void FollowCurrentPlayer(bool follow) noexcept { followCurrentPlayer = follow; }

	// only updates if level view is following current player
	bool updateCurrentMapViewCenter(bool smooth) noexcept;

	int32_t TileWidth() const noexcept { return surface.tileWidth; }
	int32_t TileHeight() const noexcept { return surface.tileHeight; }

	int32_t AutomapTileWidth() const noexcept { return automapSurface.tileWidth; }
	int32_t AutomapTileHeight() const noexcept { return automapSurface.tileHeight; }

	virtual bool Pause() const noexcept { return pause; }
	virtual void Pause(bool pause_) noexcept { pause = pause_; }

	virtual bool Visible() const noexcept { return visible; }
	virtual void Visible(bool visible_) noexcept { visible = visible_; }

	bool EnableHover() const noexcept { return enableHover; }
	void EnableHover(bool enable_) noexcept { enableHover = enable_; }

	int16_t getAutomapPlayerDirectionBaseIndex() const noexcept
	{
		return automapPlayerDirectionBaseIndex;
	}

	// setting a negative value will disable drawing player direction in the automap
	void setAutomapPlayerDirectionBaseIndex(int16_t index) noexcept
	{
		automapPlayerDirectionBaseIndex = index;
	}

	bool hasAutomap() const noexcept;

	bool ShowAutomap() const noexcept { return automapSurface.visible; }
	void ShowAutomap(bool show_) noexcept { automapSurface.visible = show_; }

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

	virtual void draw(const Game& game, sf::RenderTarget& target) const;
	virtual void update(Game& game);
	virtual bool getProperty(const std::string_view prop, Variable& var) const;
	virtual const Queryable* getQueryable(const std::string_view prop) const;

	std::vector<std::variant<const Queryable*, Variable>> getQueryableList(
		const std::string_view prop) const;

	Item* getItem(const PairFloat& mapCoord) const noexcept;
	Item* getItem(const ItemCoordInventory& itemCoord) const;
	Item* getItem(const ItemCoordInventory& itemCoord, Player*& player) const;
	Item* getItem(const ItemLocation& location) const;
	Item* getItem(const ItemLocation& location, Player*& player) const;

	std::shared_ptr<Item> removeItem(const PairFloat& mapCoord);
	std::shared_ptr<Item> removeItem(const ItemCoordInventory& itemCoord);
	std::shared_ptr<Item> removeItem(const ItemCoordInventory& itemCoord, Player*& player);
	std::shared_ptr<Item> removeItem(const ItemLocation& location);
	std::shared_ptr<Item> removeItem(const ItemLocation& location, Player*& player);

	bool setItem(const PairFloat& mapCoord, std::shared_ptr<Item>& item);
	bool setItem(const ItemCoordInventory& itemCoord, std::shared_ptr<Item>& item);
	bool setItem(const ItemLocation& location, std::shared_ptr<Item>& item);

	bool deleteItem(const PairFloat& mapCoord);
	bool deleteItem(const ItemCoordInventory& itemCoord);
	bool deleteItem(const ItemLocation& location);

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

	bool hasSpell(const std::string& id) const;
	Spell* getSpell(const std::string& id) const;

	uint32_t getExperienceFromLevel(uint32_t level) const;
	uint32_t getLevelFromExperience(uint32_t experience) const;

	const std::string_view getPropertyName(uint16_t hash16) const;
	void setPropertyName(uint16_t hash16, const std::string_view prop);
};
