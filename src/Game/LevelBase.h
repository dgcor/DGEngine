#pragma once

#include "Actions/Action.h"
#include "Classifier.h"
#include "InputEvent.h"
#include "LevelLayer.h"
#include "LevelMap.h"
#include "LevelObjectClass.h"
#include "LevelSurface.h"
#include "Quest.h"
#include "SFML/GradientCircle.h"
#include "UIObject.h"
#include "Utils/EasedValue.h"
#include "Utils/FixedArray.h"
#include "Utils/UnorderedStringMap.h"

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

class LevelBase : public UIObject
{
protected:
	LevelSurface surface;
	LevelSurface automapSurface;

	FixedArray<LevelLayer, 8> levelLayers;

	uint16_t indexToDrawLevelObjects{ 0 };
	int16_t automapPlayerDirectionBaseIndex{ -1 };

	GameShader* gameShader{ nullptr };
	float lightRadius{ 64.f };
	sf::Vector2f automapPosition{ 0.f, 0.f };
	sf::Vector2f automapSize{ 1.f, 1.f };
	bool automapRelativeCoords{ true };
	bool viewNeedsUpdate{ false };
	EasedValuef zoomValue{ 1.f };
	bool zoomDrawables{ false };

	LevelMap map;

	std::vector<LevelDrawable> drawables;
	std::vector<GradientCircle> lights;

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
	UnorderedStringMap<std::shared_ptr<LevelObject>> levelObjectIds;

	std::weak_ptr<LevelObject> clickedObject;
	std::weak_ptr<LevelObject> hoverObject;
	std::weak_ptr<Player> currentPlayer;

	UnorderedStringMap<std::unique_ptr<Classifier>> classifiers;
	UnorderedStringMap<std::unique_ptr<LevelObjectClass>> levelObjectClasses;

	bool followCurrentPlayer{ true };

	bool pause{ false };
	bool visible{ true };
	bool enableHover{ true };
	InputEventType captureInputEvents{ InputEventType::All };

	std::vector<Quest> quests;
	std::vector<uint32_t> experiencePoints;
	std::unordered_map<uint16_t, std::string> propertyNames;

	int epoch{ 0 };

	uint16_t moveUpEventHash{ 0 };
	uint16_t moveDownEventHash{ 0 };
	uint16_t moveLeftEventHash{ 0 };
	uint16_t moveRightEventHash{ 0 };
	uint16_t doActionEventHash{ 0 };

	static const LevelCell& get(int32_t x, int32_t y, const LevelBase& level) noexcept
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
	void updateLevelObjectPositions();
	void updateLights();
	void updateMouse(const Game& game);
	void updateTilesetLayersVisibleArea();
	void updateZoom(const Game& game);

	void onMouseButtonPressed(Game& game);
	void onMouseScrolled(Game& game);
	void onTouchBegan(Game& game);

public:
	LevelBase();

	void Init(const Game& game, LevelMap map_,
		const std::vector<LevelLayer>& levelLayers_, int32_t tileWidth,
		int32_t tileHeight, uint32_t subTiles, int32_t indexToDrawObjects);
	void Init();

	void setShader(GameShader* shader) noexcept { gameShader = shader; }

	void addLayer(const ColorLevelLayer& layer,
		const sf::FloatRect& viewportOffset, bool automap);

	void addLayer(const TextureLevelLayer& layer,
		const sf::FloatRect& viewportOffset, bool automap);

	void setAutomap(const TilesetLevelLayer& automapLayer, int tileWidth,
		int tileHeight, const sf::FloatRect& viewportOffset);

	void addDrawable(LevelDrawable obj);
	Panel* getDrawable(size_t idx) const;
	LevelDrawable* getLevelDrawable(const std::string_view id);
	size_t getDrawableCount() const noexcept { return drawables.size(); }
	void updateDrawables(Game& game);

	Misc::Helper2D<const LevelBase, const LevelCell&, int32_t> operator[] (int32_t x) const noexcept
	{
		return Misc::Helper2D<const LevelBase, const LevelCell&, int32_t>(*this, x, get);
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

	void Id(const std::string_view id_);
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
	T* getLevelObject(const std::string_view id) const
	{
		return dynamic_cast<T*>(getLevelObject(id));
	}

	LevelObject* getLevelObject(const std::string_view id) const;
	LevelObject* getLevelObjectByClass(const std::string_view classId) const noexcept;
	std::weak_ptr<LevelObject> getLevelObjectPtr(const std::string_view id) const;

	void addClassifier(const std::string_view key, std::unique_ptr<Classifier> obj)
	{
		classifiers.insert(std::make_pair(key, std::move(obj)));
	}

	Classifier* getClassifier(const std::string_view key) const
	{
		auto it = classifiers.find(sv2str(key));
		if (it != classifiers.end())
		{
			return it->second.get();
		}
		return nullptr;
	}

	bool hasClass(const std::string_view key) const
	{
		return levelObjectClasses.find(sv2str(key)) != levelObjectClasses.end();
	}

	void addClass(const std::string_view key, std::unique_ptr<LevelObjectClass> obj)
	{
		levelObjectClasses.insert(std::make_pair(key, std::move(obj)));
	}

	template <class T>
	T* getClass(const std::string_view key) const
	{
		auto it = levelObjectClasses.find(sv2str(key));
		if (it != levelObjectClasses.end())
		{
			return dynamic_cast<T*>(it->second.get());
		}
		return nullptr;
	}

	void executeHoverEnterAction(Game& game);
	void executeHoverLeaveAction(Game& game);

	const PairFloat& getClickedMapPosition() const noexcept { return clickedMapPosition; }

	LevelObject* getClickedObject() const noexcept { return clickedObject.lock().get(); }
	void setClickedObject(std::weak_ptr<LevelObject> object) noexcept { clickedObject = object; }

	LevelObject* getHoverObject() const noexcept { return hoverObject.lock().get(); }
	void setHoverObject(std::weak_ptr<LevelObject> object) noexcept { hoverObject = object; }

	std::shared_ptr<Action> getAction(uint16_t nameHash16) const noexcept override;
	bool setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept override;

	Anchor getAnchor() const noexcept override { return surface.getAnchor(); }
	void setAnchor(const Anchor anchor) noexcept override { surface.setAnchor(anchor); }

	Anchor getAutomapAnchor() const noexcept { return automapSurface.getAnchor(); }
	void setAutomapAnchor(const Anchor anchor) noexcept { automapSurface.setAnchor(anchor); }

	void updateSize(const Game& game) override;

	const sf::Vector2f& DrawPosition() const noexcept override { return surface.Position(); }
	const sf::Vector2f& Position() const noexcept override { return surface.Position(); }
	void Position(const sf::Vector2f& position) noexcept override;
	sf::Vector2f Size() const noexcept override { return surface.Size(); }
	void Size(const sf::Vector2f& size) override;

	float LightRadius() const noexcept { return lightRadius; }
	void LightRadius(float lightRadius_) noexcept;

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

	bool ZoomDrawables() const noexcept { return zoomDrawables; }
	void ZoomDrawables(bool val) noexcept { zoomDrawables = val; }

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

	int32_t TileWidth() const noexcept { return surface.tileWidth; }
	int32_t TileHeight() const noexcept { return surface.tileHeight; }
	uint32_t SubTiles() const noexcept { return surface.subTiles; }

	int32_t AutomapTileWidth() const noexcept { return automapSurface.tileWidth; }
	int32_t AutomapTileHeight() const noexcept { return automapSurface.tileHeight; }
	uint32_t AutomapSubTiles() const noexcept { return automapSurface.subTiles; }

	bool Pause() const noexcept { return pause; }
	void Pause(bool pause_) noexcept { pause = pause_; }

	bool Visible() const noexcept override { return visible; }
	void Visible(bool visible_) noexcept override { visible = visible_; }

	bool EnableHover() const noexcept { return enableHover; }
	void EnableHover(bool enable) noexcept { enableHover = enable; }

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
	void ShowAutomap(bool show) noexcept { automapSurface.visible = show; }

	InputEventType getCaptureInputEvents() const noexcept { return captureInputEvents; }
	void setCaptureInputEvents(InputEventType e) noexcept { captureInputEvents = e; }

	const std::string_view getPropertyName(uint16_t hash16) const;
	void setPropertyName(uint16_t hash16, const std::string_view prop);
};
