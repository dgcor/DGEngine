#include "LevelBase.h"
#include "Game/Game.h"
#include "Utils/Utils.h"

LevelBase::LevelBase()
{
	automapSurface.visible = false;
}

void LevelBase::Init(const Game& game, LevelMap map_,
	const std::vector<LevelLayer>& levelLayers_, int32_t tileWidth,
	int32_t tileHeight, uint32_t subTiles, int32_t indexToDrawObjects)
{
	map = std::move(map_);
	levelObjects.clearClickedObject();
	levelObjects.clearHoverObject();

	surface.tileWidth = std::max(tileWidth, 2);
	surface.tileHeight = std::max(tileHeight, 2);
	surface.blockWidth = surface.tileWidth / 2;
	surface.blockHeight = surface.tileHeight / 2;
	surface.subTiles = std::clamp(subTiles, 1u, 8u);

	automapSurface.tileWidth = surface.tileWidth;
	automapSurface.tileHeight = surface.tileHeight;
	automapSurface.blockWidth = surface.blockWidth;
	automapSurface.blockHeight = surface.blockHeight;
	automapSurface.subTiles = surface.subTiles;

	levelLayers.clear();
	for (const auto& layer : levelLayers_)
	{
		if (holdsTilesetLevelLayer(layer.layer) == true &&
			std::get<TilesetLevelLayer>(layer.layer).tiles == nullptr)
		{
			continue;
		}
		levelLayers.push_back(layer);
	}
	indexToDrawLevelObjects = 0;
	int32_t numTilesetLayers = 0;
	for (size_t i = 0; i < levelLayers.size(); i++)
	{
		if (holdsTilesetLevelLayer(levelLayers[i].layer) == true)
		{
			numTilesetLayers++;
			if (indexToDrawObjects < 0 &&
				numTilesetLayers <= 2)
			{
				indexToDrawLevelObjects = (uint16_t)i;
			}
			else
			{
				break;
			}
		}
	}
	if (indexToDrawObjects >= 0)
	{
		indexToDrawLevelObjects = indexToDrawObjects;
	}
	if (numTilesetLayers == 0 &&
		levelLayers.full() == false)
	{
		levelLayers.push_back({ TilesetLevelLayer(), {}, false });
	}

	map.setDefaultTileSize(surface.tileWidth, surface.tileHeight, surface.subTiles);

	setCurrentMapPosition(PairFloat(-1.f, -1.f), false);

	surface.init(game);
	automapSurface.init(game);
	updateTilesetLayersVisibleArea();

	levelObjects.updatePositions(map);
	viewNeedsUpdate = true;
}

void LevelBase::Init()
{
	levelObjects.clearClickedObject();
	levelObjects.clearHoverObject();

	levelObjects.updatePositions(map);
	viewNeedsUpdate = true;
}

void LevelBase::Id(const std::string_view id_)
{
	id = id_;
	inputManager.Init(id);
}

void LevelBase::addLayer(const ColorLevelLayer& layer, const sf::FloatRect& viewportOffset, bool automap)
{
	levelLayers.push_back({ layer, viewportOffset, automap });
}

void LevelBase::addLayer(const TextureLevelLayer& layer, const sf::FloatRect& viewportOffset, bool automap)
{
	levelLayers.push_back({ layer, viewportOffset, automap });
}

void LevelBase::setAutomap(const TilesetLevelLayer& automapLayer, int tileWidth, int tileHeight, const sf::FloatRect& viewportOffset)
{
	automapSurface.tileWidth = std::max(tileWidth, 2);
	automapSurface.tileHeight = std::max(tileHeight, 2);
	automapSurface.blockWidth = automapSurface.tileWidth / 2;
	automapSurface.blockHeight = automapSurface.tileHeight / 2;

	bool updated = false;
	for (auto& layer : levelLayers)
	{
		if (layer.automap == true &&
			holdsTilesetLevelLayer(layer.layer) == true)
		{
			layer.layer = automapLayer;
			layer.viewportOffset = viewportOffset;
			updated = true;
			break;
		}
	}
	if (updated == false)
	{
		levelLayers.push_back({ automapLayer, viewportOffset, true });
	}
	viewNeedsUpdate = true;
}

void LevelBase::setCurrentMapPosition(const PairFloat& mapPos, bool smooth) noexcept
{
	currentMapPosition = mapPos;
	setCurrentMapViewCenter(map.toDrawCoord(currentMapPosition), smooth);
}

void LevelBase::setCurrentMapViewCenter(const sf::Vector2f& coord_, bool smooth) noexcept
{
	if (currentMapViewCenter != coord_)
	{
		currentMapViewCenter = coord_;
		currentMapViewCenterX.set(coord_.x, smooth == true ? 1.f : 0.f);
		currentMapViewCenterY.set(coord_.y, smooth == true ? 1.f : 0.f);
		updateCurrentAutomapViewCenter();
	}
}

void LevelBase::updateCurrentAutomapViewCenter() noexcept
{
	currentAutomapViewCenter.x = std::round((currentMapViewCenter.x * (float)AutomapTileWidth()) / (float)TileWidth());
	currentAutomapViewCenter.y = std::round((currentMapViewCenter.y * (float)AutomapTileHeight()) / (float)TileHeight());
}

void LevelBase::executeHoverEnterAction(Game& game)
{
	game.Events().tryAddBack(hoverEnterAction);
}

void LevelBase::executeHoverLeaveAction(Game& game)
{
	game.Events().tryAddFront(hoverLeaveAction);
}

void LevelBase::setAutomapPosition(const sf::Vector2f& position) noexcept
{
	if (automapRelativeCoords == true)
	{
		automapPosition.x = std::clamp(position.x / 100.f, 0.f, 1.f);
		automapPosition.y = std::clamp(position.y / 100.f, 0.f, 1.f);
	}
	else
	{
		automapPosition.x = (float)position.x;
		automapPosition.y = (float)position.y;
		automapSurface.Position(automapPosition);
	}
	updateAutomapRelativePosition();
}

void LevelBase::Position(const sf::Vector2f& position) noexcept
{
	surface.Position(position);
	updateAutomapRelativePosition();
}

void LevelBase::LightRadius(float lightRadius_) noexcept
{
	lightRadius = std::clamp(lightRadius_, 4.f, 256.f);
}

void LevelBase::updateAutomapRelativePosition()
{
	if (automapRelativeCoords == true)
	{
		auto position = surface.Position();
		position.x = std::round(position.x + surface.Size().x * automapPosition.x);
		position.y = std::round(position.y + surface.Size().y * automapPosition.y);
		automapSurface.Position(position);
	}
	viewNeedsUpdate = true;
}

void LevelBase::setAutomapSize(const sf::Vector2f& size)
{
	if (automapRelativeCoords == true)
	{
		automapSize.x = std::clamp(size.x / 100.f, 0.f, 1.f);
		automapSize.y = std::clamp(size.y / 100.f, 0.f, 1.f);
	}
	else
	{
		automapSize.x = std::max((float)size.x, 0.f);
		automapSize.y = std::max((float)size.y, 0.f);
		automapSurface.Size(automapSize);
	}
	updateAutomapRelativeSize();
}

void LevelBase::Size(const sf::Vector2f& size)
{
	surface.Size(size);
	updateAutomapRelativePosition();
	updateAutomapRelativeSize();
}

void LevelBase::updateAutomapRelativeSize()
{
	if (automapRelativeCoords == true)
	{
		auto size = surface.Size();
		size.x = std::round(size.x * automapSize.x);
		size.y = std::round(size.y * automapSize.y);
		automapSurface.Size(size);
	}
	viewNeedsUpdate = true;
}

void LevelBase::Zoom(float factor, bool smooth) noexcept
{
	if (factor > LevelSurface::ZoomMax)
	{
		factor = LevelSurface::ZoomMax;
	}
	else if (factor < LevelSurface::ZoomMin)
	{
		factor = LevelSurface::ZoomMin;
	}
	zoomValue.set(factor, smooth == true ? 1.f : 0.f);
}

void LevelBase::updateZoom(const Game& game)
{
	zoomValue.update(game.getElapsedTime().asSeconds());
	auto newZoom = 1.f / zoomValue.get();
	if (surface.updateZoom(game, newZoom) == true)
	{
		updateTilesetLayersVisibleArea();
	}
}

void LevelBase::addLevelObject(std::shared_ptr<LevelObject> obj)
{
	levelObjects.add(map, obj);
}

void LevelBase::updateSize(const Game& game)
{
	surface.updateSize(game);
	updateAutomapRelativePosition();
	updateAutomapRelativeSize();
	automapSurface.updateSize(game);
	updateTilesetLayersVisibleArea();
}

void LevelBase::updateTilesetLayersVisibleArea()
{
	for (auto& levelLayer : levelLayers)
	{
		if (holdsTilesetLevelLayer(levelLayer.layer) == false)
		{
			continue;
		}
		auto& layer = std::get<TilesetLevelLayer>(levelLayer.layer);
		const auto& layerSurface = (levelLayer.automap == false ? surface : automapSurface);
		layer.updateVisibleArea(layerSurface, map);
	}
}

void LevelBase::updateMouse(const Game& game)
{
	mousePositionf = surface.getPosition(game.MousePositionf());
	auto mousePositionf2 = mousePositionf;
	mousePositionf2.x -= (float)map.DefaultBlockWidth();
	mousePositionf2.y -= (float)map.DefaultBlockHeight();
	mapCoordOverMouse = map.toMapCoord(mousePositionf2);
}

void LevelBase::updateLights()
{
	map.updateLights(levelObjects.Objects(), currentMapViewCenter);
	lights.clear();
	for (const auto& light : map.AllLights())
	{
		auto radius = (float)light.lightSource.radius * lightRadius;
		GradientCircle circle(radius, 9);
		circle.setInnerColor(sf::Color(0, 0, 0, 0));
		circle.setOuterColor(sf::Color(0, 0, 0, light.lightSource.light));
		circle.setOrigin(radius, radius);
		circle.setPosition(light.drawPos);
		lights.push_back(std::move(circle));
	}
}

bool LevelBase::hasAutomap() const noexcept
{
	for (const auto& layer : levelLayers | std::views::reverse)
	{
		if (layer.automap == true)
		{
			return true;
		}
	}
	return false;
}

LevelObject* LevelBase::parseLevelObjectIdOrMapPosition(const std::string_view str, std::string_view& props) const
{
	auto strPair = Utils::splitStringIn2(str, '.');
	auto strPair2 = Utils::splitStringIn2(strPair.first, ',');
	if (strPair2.second.empty() == false)
	{
		PairInt32 mapPos(Utils::strtou(strPair2.first), Utils::strtou(strPair2.second));
		if (map.isMapCoordValid(mapPos) == true)
		{
			props = strPair.second;
			return map[mapPos].getObject<LevelObject>();
		}
	}
	else
	{
		auto simpleObj = levelObjects.get<LevelObject>(strPair2.first);
		if (simpleObj != nullptr)
		{
			props = strPair.second;
			return simpleObj;
		}
	}
	return nullptr;
}

const std::string_view LevelBase::getPropertyName(uint16_t hash16) const
{
	const auto elem = propertyNames.find(hash16);
	if (elem != propertyNames.cend())
	{
		return elem->second;
	}
	return {};
}

void LevelBase::setPropertyName(uint16_t hash16, const std::string_view prop)
{
	propertyNames[hash16] = std::string(prop);
}
