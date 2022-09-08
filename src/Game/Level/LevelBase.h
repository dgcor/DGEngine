#pragma once

#include "Game/InputEvent.h"
#include "Game/Level/LevelInputManager.h"
#include "Game/Quest/Quest.h"
#include "Game/UIObject.h"
#include "LevelDrawableManager.h"
#include "LevelLayer.h"
#include "LevelObjectManager.h"
#include "LevelSurface.h"
#include "SFML/GradientCircle.h"
#include "Utils/EasedValue.h"
#include "Utils/FixedArray.h"

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

	LevelDrawableManager drawables;
	std::vector<GradientCircle> lights;

	sf::Vector2f mousePositionf;
	bool hasMouseInside{ false };

	PairFloat mapCoordOverMouse;
	PairFloat currentMapPosition;
	PairFloat clickedMapPosition;

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

	LevelObjectManager levelObjects;

	bool followCurrentPlayer{ true };

	bool pause{ false };
	bool visible{ true };
	bool enableHover{ true };
	InputEventType captureInputEvents{ InputEventType::All };

	std::vector<Quest> quests;
	std::vector<uint32_t> experiencePoints;
	std::unordered_map<uint16_t, std::string> propertyNames;

	int epoch{ 0 };

	LevelInputManager inputManager;

	static auto& get(int32_t x, int32_t y, const LevelBase& level) noexcept { return level.map[x][y]; }

	void updateAutomapRelativePosition();
	void updateAutomapRelativeSize();
	void updateCurrentAutomapViewCenter() noexcept;

	void setCurrentMapPosition(const PairFloat& mapPos, bool smooth) noexcept;
	void setCurrentMapViewCenter(const sf::Vector2f& coord_, bool smooth) noexcept;

	LevelObject* parseLevelObjectIdOrMapPosition(const std::string_view str, std::string_view& props) const;

	void updateLights();
	void updateMouse(const Game& game);
	void updateTilesetLayersVisibleArea();
	void updateZoom(const Game& game);

public:
	LevelBase();

	void Init(const Game& game, LevelMap map_,
		const std::vector<LevelLayer>& levelLayers_, int32_t tileWidth,
		int32_t tileHeight, uint32_t subTiles, int32_t indexToDrawObjects);
	void Init();

	void setShader(GameShader* shader) noexcept { gameShader = shader; }

	void addLayer(const ColorLevelLayer& layer, const sf::FloatRect& viewportOffset, bool automap);

	void addLayer(const TextureLevelLayer& layer, const sf::FloatRect& viewportOffset, bool automap);

	void setAutomap(const TilesetLevelLayer& automapLayer, int tileWidth, int tileHeight, const sf::FloatRect& viewportOffset);
	
	auto operator[] (int32_t x) const noexcept { return Misc::Helper2D(*this, x, get); }

	auto& MousePositionf() const noexcept { return mousePositionf; }
	bool HasMouseInside() const noexcept { return hasMouseInside; }

	auto& Map() noexcept { return map; }
	auto& Map() const noexcept { return map; }

	auto Width() const noexcept { return map.MapSizei().x; }
	auto Height() const noexcept { return map.MapSizei().y; }

	auto& getCurrentMapViewCenter() const noexcept { return currentMapViewCenter; }
	auto& getCurrentAutomapViewCenter() const noexcept { return currentAutomapViewCenter; }

	auto& Drawables() noexcept { return drawables; }
	auto& Drawables() const noexcept { return drawables; }

	auto& LevelObjects() noexcept { return levelObjects; }
	auto& LevelObjects() const noexcept { return levelObjects; }

	void Id(const std::string_view id_);
	void Name(const std::string_view name_) { name = name_; }
	void Path(const std::string_view path_) { path = path_; }

	void executeHoverEnterAction(Game& game);
	void executeHoverLeaveAction(Game& game);

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

	auto& getAutomapPosition() const noexcept { return automapPosition; }
	auto& getAutomapSize() const noexcept { return automapSize; }

	// if position is relative to level, between [0, 100]
	void setAutomapPosition(const sf::Vector2f& position) noexcept;
	// if position is relative to level, between [0, 100]
	void setAutomapSize(const sf::Vector2f& size);

	auto Zoom() const noexcept { return zoomValue.getFinal(); }
	void Zoom(float factor, bool smooth = false) noexcept;

	bool ZoomDrawables() const noexcept { return zoomDrawables; }
	void ZoomDrawables(bool val) noexcept { zoomDrawables = val; }

	void updateView() { viewNeedsUpdate = true; }

	void addLevelObject(std::shared_ptr<LevelObject> obj);

	auto& getMapCoordOverMouse() const noexcept { return mapCoordOverMouse; }
	auto& getClickedMapPosition() const noexcept { return clickedMapPosition; }

	void move(const PairFloat& mapPos, bool smooth) { setCurrentMapPosition(mapPos, smooth); }
	void move(bool smooth) { setCurrentMapViewCenter(mousePositionf, smooth); }

	void setSmoothMovement(bool smooth) noexcept { smoothMovement = smooth; }

	auto TileWidth() const noexcept { return surface.tileWidth; }
	auto TileHeight() const noexcept { return surface.tileHeight; }
	auto SubTiles() const noexcept { return surface.subTiles; }

	auto AutomapTileWidth() const noexcept { return automapSurface.tileWidth; }
	auto AutomapTileHeight() const noexcept { return automapSurface.tileHeight; }
	auto AutomapSubTiles() const noexcept { return automapSurface.subTiles; }

	bool Pause() const noexcept { return pause; }
	void Pause(bool pause_) noexcept { pause = pause_; }

	bool Visible() const noexcept override { return visible; }
	void Visible(bool visible_) noexcept override { visible = visible_; }

	bool EnableHover() const noexcept { return enableHover; }
	void EnableHover(bool enable) noexcept { enableHover = enable; }

	auto getAutomapPlayerDirectionBaseIndex() const noexcept { return automapPlayerDirectionBaseIndex; }

	// setting a negative value will disable drawing player direction in the automap
	void setAutomapPlayerDirectionBaseIndex(int16_t index) noexcept { automapPlayerDirectionBaseIndex = index; }

	bool hasAutomap() const noexcept;

	bool ShowAutomap() const noexcept { return automapSurface.visible; }
	void ShowAutomap(bool show) noexcept { automapSurface.visible = show; }

	auto getCaptureInputEvents() const noexcept { return captureInputEvents; }
	void setCaptureInputEvents(InputEventType e) noexcept { captureInputEvents = e; }

	const std::string_view getPropertyName(uint16_t hash16) const;
	void setPropertyName(uint16_t hash16, const std::string_view prop);
};
