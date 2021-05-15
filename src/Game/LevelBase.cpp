#include "LevelBase.h"
#include "Game.h"
#include "Panel.h"
#include "Player.h"
#include "SimpleLevelObject.h"
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
	clickedObject.reset();
	hoverObject.reset();

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

	updateLevelObjectPositions();
	viewNeedsUpdate = true;
}

void LevelBase::Init()
{
	clickedObject.reset();
	hoverObject.reset();

	updateLevelObjectPositions();
	viewNeedsUpdate = true;
}

void LevelBase::Id(const std::string_view id_)
{
	id = id_;
	moveUpEventHash = str2int16(id + ".up");
	moveDownEventHash = str2int16(id + ".down");
	moveLeftEventHash = str2int16(id + ".left");
	moveRightEventHash = str2int16(id + ".right");
	doActionEventHash = str2int16(id + ".doAction");
}

void LevelBase::addLayer(const ColorLevelLayer& layer,
	const sf::FloatRect& viewportOffset, bool automap)
{
	levelLayers.push_back({ layer, viewportOffset, automap });
}

void LevelBase::addLayer(const TextureLevelLayer& layer,
	const sf::FloatRect& viewportOffset, bool automap)
{
	levelLayers.push_back({ layer, viewportOffset, automap });
}

void LevelBase::setAutomap(const TilesetLevelLayer& automapLayer, int tileWidth,
	int tileHeight, const sf::FloatRect& viewportOffset)
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

void LevelBase::addDrawable(LevelDrawable obj)
{
	for (const auto& drawable : drawables)
	{
		if (drawable.id == obj.id)
		{
			return;
		}
	}
	if (auto drawableObj = obj.drawable.lock())
	{
		setLevelDrawablePosition(obj, *drawableObj);
	}
	drawables.push_back(obj);
}

Panel* LevelBase::getDrawable(size_t idx) const
{
	if (idx < drawables.size())
	{
		if (auto obj = drawables[idx].drawable.lock())
		{
			return obj.get();
		}
	}
	return nullptr;
}

LevelDrawable* LevelBase::getLevelDrawable(const std::string_view id)
{
	for (auto& obj : drawables)
	{
		if (obj.id == id)
		{
			return &obj;
		}
	}
	return nullptr;
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

void LevelBase::updateLevelObjectPositions()
{
	for (const auto& obj : levelObjects)
	{
		const auto& mapPosition = obj->MapPosition();
		if (map.isMapCoordValid(mapPosition) == true)
		{
			obj->MapPosition(map, mapPosition);
		}
	}
}

std::shared_ptr<Action> LevelBase::getAction(uint16_t nameHash16) const noexcept
{
	switch (nameHash16)
	{
	case str2int16("click"):
	case str2int16("leftClick"):
		return leftAction;
	case str2int16("rightClick"):
		return rightAction;
	case str2int16("hoverEnter"):
		return hoverEnterAction;
	case str2int16("hoverLeave"):
		return hoverLeaveAction;
	case str2int16("scrollDown"):
		return scrollDownAction;
	case str2int16("scrollUp"):
		return scrollUpAction;
	default:
		return nullptr;
	}
}

bool LevelBase::setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept
{
	switch (nameHash16)
	{
	case str2int16("click"):
	case str2int16("leftClick"):
		leftAction = action;
		break;
	case str2int16("rightClick"):
		rightAction = action;
		break;
	case str2int16("hoverEnter"):
		hoverEnterAction = action;
		break;
	case str2int16("hoverLeave"):
		hoverLeaveAction = action;
		break;
	case str2int16("scrollDown"):
		scrollDownAction = action;
		break;
	case str2int16("scrollUp"):
		scrollUpAction = action;
		break;
	default:
		return false;
	}
	return true;
}

void LevelBase::executeHoverEnterAction(Game& game)
{
	if (hoverEnterAction != nullptr)
	{
		game.Events().addBack(hoverEnterAction);
	}
}

void LevelBase::executeHoverLeaveAction(Game& game)
{
	if (hoverLeaveAction != nullptr)
	{
		game.Events().addFront(hoverLeaveAction);
	}
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
	auto mapPos = obj->MapPosition();
	addLevelObject(std::move(obj), mapPos);
}

void LevelBase::addLevelObject(std::shared_ptr<LevelObject> obj, const PairFloat& mapCoord)
{
	if (obj->getId().empty() == false)
	{
		if (levelObjectIds.find(obj->getId()) != levelObjectIds.end())
		{
			return;
		}
		levelObjectIds[obj->getId()] = obj;
	}
	obj->MapPosition(map, mapCoord);
	levelObjects.push_back(obj);
}

void LevelBase::clearCache(const LevelObject* obj) noexcept
{
	if (clickedObject.lock().get() == obj)
	{
		clickedObject.reset();
	}
	if (hoverObject.lock().get() == obj)
	{
		hoverObject.reset();
	}
	if (currentPlayer.lock().get() == obj)
	{
		currentPlayer.reset();
	}
}

void LevelBase::deleteLevelObjectById(const std::string_view id)
{
	if (id.empty() == true)
	{
		return;
	}
	for (auto it = levelObjects.begin(); it != levelObjects.end(); ++it)
	{
		if ((*it)->getId() == id)
		{
			auto obj = it->get();
			obj->remove(map);
			clearCache(obj);
			if (obj->getId().empty() == false)
			{
				levelObjectIds.erase(obj->getId());
			}
			levelObjects.erase(it);
			break;
		}
	}
}

void LevelBase::deleteLevelObjectByClass(const std::string_view classId)
{
	if (classId.empty() == true)
	{
		return;
	}
	for (auto it = levelObjects.begin(); it != levelObjects.end(); ++it)
	{
		if ((*it)->getClassId() == classId)
		{
			auto obj = it->get();
			obj->remove(map);
			clearCache(obj);
			if (obj->getId().empty() == false)
			{
				levelObjectIds.erase(obj->getId());
			}
			levelObjects.erase(it);
			break;
		}
	}
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

void LevelBase::onMouseButtonPressed(Game& game)
{
	game.clearMousePressed();
	switch (game.MousePress().button)
	{
	case sf::Mouse::Left:
	{
		clickedMapPosition = getMapCoordOverMouse();
		clickedObject.reset();
		if (leftAction != nullptr)
		{
			game.Events().addBack(leftAction);
		}
	}
	break;
	case sf::Mouse::Right:
	{
		if (rightAction != nullptr)
		{
			game.Events().addBack(rightAction);
		}
	}
	break;
	default:
		break;
	}
}

void LevelBase::onMouseScrolled(Game& game)
{
	game.clearMouseScrolled();
	if (game.MouseScroll().delta < 0.f)
	{
		if (scrollDownAction != nullptr)
		{
			game.Events().addBack(scrollDownAction);
		}
	}
	else
	{
		if (scrollUpAction != nullptr)
		{
			game.Events().addBack(scrollUpAction);
		}
	}
}

void LevelBase::onTouchBegan(Game& game)
{
	game.clearTouchBegan();
	switch (game.TouchBegan().finger)
	{
	case 0:
	{
		clickedMapPosition = getMapCoordOverMouse();
		clickedObject.reset();
		if (leftAction != nullptr)
		{
			game.Events().addBack(leftAction);
		}
	}
	break;
	case 1:
	{
		if (rightAction != nullptr)
		{
			game.Events().addBack(rightAction);
		}
	}
	break;
	default:
		break;
	}
}

void LevelBase::updateLights()
{
	map.updateLights(levelObjects, currentMapViewCenter);
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

void LevelBase::updateDrawables(Game& game)
{
	for (auto it = drawables.rbegin(); it != drawables.rend();)
	{
		auto obj = it->drawable.lock();
		if (obj == nullptr)
		{
			++it;
			it = std::reverse_iterator(drawables.erase(it.base()));
		}
		else
		{
			setLevelDrawablePosition(*it, *obj);
			obj->update(game);
			++it;
		}
	}
}

void LevelBase::setLevelDrawablePosition(LevelDrawable& obj, Panel& panelObj)
{
	if (auto anchorTo = obj.anchorTo.lock())
	{
		auto& drawPos = panelObj.DrawPosition();
		auto drawSize = panelObj.Size();
		sf::Vector2f newPos;
		sf::FloatRect visibleRect;

		if (zoomDrawables == true)
		{
			newPos = anchorTo->getAnchorPosition() + obj.offset;
			if (surface.getZoom() > 1.f)
			{
				newPos.y += std::round((drawSize.y / surface.getZoom()) - drawSize.y);
			}
			visibleRect = surface.visibleRect;
		}
		else
		{
			newPos = surface.getDrawPosition(anchorTo->getAnchorPosition() + obj.offset);
			newPos.y += std::round((drawSize.y / surface.getZoom()) - drawSize.y);
			visibleRect = sf::FloatRect(surface.Position(), surface.Size());
		}

		if (obj.visibleRectOffset >= 0.f)
		{
			auto drawPosOffset = drawPos - panelObj.Position();
			auto newDrawPos = newPos + drawPosOffset;

			if (newDrawPos.x < visibleRect.left + obj.visibleRectOffset)
			{
				newPos.x = visibleRect.left + obj.visibleRectOffset - drawPosOffset.x;
			}
			if (newDrawPos.y < visibleRect.top + obj.visibleRectOffset)
			{
				newPos.y = visibleRect.top + obj.visibleRectOffset - drawPosOffset.y;
			}
			newDrawPos += drawSize;
			if (newDrawPos.x > visibleRect.left +
				visibleRect.width - obj.visibleRectOffset)
			{
				newPos.x = visibleRect.left + visibleRect.width
					- obj.visibleRectOffset - drawSize.x - drawPosOffset.x;
			}
			if (newDrawPos.y > visibleRect.top +
				visibleRect.height - obj.visibleRectOffset)
			{
				newPos.y = visibleRect.top + visibleRect.height
					- obj.visibleRectOffset - drawSize.y - drawPosOffset.y;
			}
		}
		panelObj.Position(newPos);
	}
}

bool LevelBase::hasAutomap() const noexcept
{
	for (const auto& layer : reverse(levelLayers))
	{
		if (layer.automap == true)
		{
			return true;
		}
	}
	return false;
}

LevelObject* LevelBase::parseLevelObjectIdOrMapPosition(
	const std::string_view str, std::string_view& props) const
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
		auto simpleObj = getLevelObject<SimpleLevelObject>(strPair2.first);
		if (simpleObj != nullptr)
		{
			props = strPair.second;
			return simpleObj;
		}
	}
	return nullptr;
}

void LevelBase::clearAllLevelObjects()
{
	for (const auto& obj : levelObjects)
	{
		const auto& mapPos = obj->MapPosition();
		if (map.isMapCoordValid(mapPos) == true)
		{
			map[mapPos].removeObject(obj.get());
		}
	}
	levelObjects.clear();
	levelObjectIds.clear();
}

LevelObject* LevelBase::getLevelObject(const std::string_view id) const
{
	if (id.empty() == true)
	{
		return nullptr;
	}
	auto it = levelObjectIds.find(sv2str(id));
	if (it != levelObjectIds.cend())
	{
		return it->second.get();
	}
	return nullptr;
}

LevelObject* LevelBase::getLevelObjectByClass(const std::string_view classId) const noexcept
{
	if (classId.empty() == true)
	{
		return nullptr;
	}
	for (const auto& obj : levelObjects)
	{
		if (obj->getClassId() == classId)
		{
			return obj.get();
		}
	}
	return nullptr;
}

std::weak_ptr<LevelObject> LevelBase::getLevelObjectPtr(const std::string_view id) const
{
	if (id.empty() == true)
	{
		return {};
	}
	switch (str2int16(id))
	{
	case str2int16("clickedObject"):
		return clickedObject;
	case str2int16("currentPlayer"):
		return currentPlayer;
	case str2int16("hoverObject"):
		return hoverObject;
	default:
		break;
	}
	auto it = levelObjectIds.find(sv2str(id));
	if (it != levelObjectIds.cend())
	{
		return it->second;
	}
	return {};
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
