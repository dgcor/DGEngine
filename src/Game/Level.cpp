#include "Level.h"
#include "Game.h"
#include "GameUtils.h"
#include "Game/GameHashes.h"
#include "SimpleLevelObject.h"
#include "Utils/EasingFunctions.h"
#include "Utils/Utils.h"

void Level::Init(LevelMap map_,
	const std::vector<std::shared_ptr<TexturePack>>& texturePackLayers,
	int32_t tileWidth, int32_t tileHeight)
{
	map = std::move(map_);
	clickedObject = nullptr;
	hoverObject = nullptr;

	tileWidth = std::max(tileWidth, 2);
	tileHeight = std::max(tileHeight, 2);

	size_t numLayers = levelLayers.size() - 1;
	for (size_t i = 0; i < numLayers; i++)
	{
		setTileLayer(i, &view, texturePackLayers[i], tileWidth, tileHeight, true);
	}
	setAutomap(nullptr, tileWidth, tileHeight);

	map.setDefaultTileSize(tileWidth, tileHeight);

	setCurrentMapPosition(MapCoord(map.Width() / 2, map.Height() / 2));

	map.initLights();
	updateLevelObjectPositions();
	viewPortNeedsUpdate = true;
}

void Level::Init()
{
	clickedObject = nullptr;
	hoverObject = nullptr;

	map.initLights();
	updateLevelObjectPositions();
	viewPortNeedsUpdate = true;
}

void Level::setTileLayer(size_t layer, View2* view_,
	std::shared_ptr<TexturePack> tiles,
	int tileWidth, int tileHeight, bool visible_)
{
	levelLayers[layer].tiles = tiles;
	levelLayers[layer].view = view_;
	levelLayers[layer].tileWidth = tileWidth;
	levelLayers[layer].tileHeight = tileHeight;
	levelLayers[layer].blockWidth = tileWidth / 2;
	levelLayers[layer].blockHeight = tileHeight / 2;
	levelLayers[layer].visible = visible_;
}

void Level::setAutomap(std::shared_ptr<TexturePack> tiles, int tileWidth, int tileHeight)
{
	tileWidth = std::max(tileWidth, 2);
	tileHeight = std::max(tileHeight, 2);

	setTileLayer(AutomapLayer, &automapView, tiles,
		tileWidth, tileHeight, levelLayers[AutomapLayer].visible);
	viewPortNeedsUpdate = true;
}

void Level::setCurrentMapViewCenter(const sf::Vector2f& coord_)
{
	currentMapViewCenter = coord_;
	currentAutomapViewCenter.x = std::round((coord_.x * (float)AutomapTileWidth()) / (float)TileWidth());
	currentAutomapViewCenter.y = std::round((coord_.y * (float)AutomapTileHeight()) / (float)TileHeight());
}

void Level::updateLevelObjectPositions()
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

std::shared_ptr<Action> Level::getAction(uint16_t nameHash16) const noexcept
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

bool Level::setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action) noexcept
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

void Level::executeHoverEnterAction(Game& game)
{
	if (hoverEnterAction != nullptr)
	{
		game.Events().addBack(hoverEnterAction);
	}
}

void Level::executeHoverLeaveAction(Game& game)
{
	if (hoverLeaveAction != nullptr)
	{
		game.Events().addFront(hoverLeaveAction);
	}
}

void Level::setAutomapRelativePosition(const sf::Vector2i& position) noexcept
{
	automapRelativePosition.x = std::clamp(position.x / 100.f, 0.f, 1.f);
	automapRelativePosition.y = std::clamp(position.y / 100.f, 0.f, 1.f);
	updateAutomapPosition(view.getPosition());
}

void Level::Position(const sf::Vector2f& position) noexcept
{
	view.setPosition(position);
	updateAutomapPosition(position);
}

void Level::updateAutomapPosition(sf::Vector2f position)
{
	position.x = std::round(position.x + view.getSize().x * automapRelativePosition.x);
	position.y = std::round(position.y + view.getSize().y * automapRelativePosition.y);
	automapView.setPosition(position);
	viewPortNeedsUpdate = true;
}

void Level::setAutomapRelativeSize(const sf::Vector2i& size)
{
	automapRelativeSize.x = std::clamp(size.x / 100.f, 0.f, 1.f);
	automapRelativeSize.y = std::clamp(size.y / 100.f, 0.f, 1.f);
	updateAutomapSize(Size());
}

void Level::Size(const sf::Vector2f& size)
{
	view.setSize(size);
	updateAutomapPosition(view.getPosition());
	updateAutomapSize(size);
}

void Level::updateAutomapSize(sf::Vector2f size)
{
	size.x = std::round(size.x * automapRelativeSize.x);
	size.y = std::round(size.y * automapRelativeSize.y);
	automapView.setSize(size);
	viewPortNeedsUpdate = true;
}

void Level::Zoom(float factor, bool smooth) noexcept
{
	if (factor > 2.f)
	{
		factor = 2.f;
	}
	else if (factor < 0.5f)
	{
		factor = 0.5f;
	}
	smoothZoom = smooth;
	if (smooth == false)
	{
		currentZoomFactor = factor;
	}
	else
	{
		zoomStepStart = 0.f;
		startZoomFactor = currentZoomFactor;
		diffZoomFactor = factor - startZoomFactor;
	}
	stopZoomFactor = factor;
	hasNewZoom = true;
}

void::Level::updateZoom(const Game& game)
{
	if (hasNewZoom == false)
	{
		return;
	}
	if (smoothZoom == false)
	{
		hasNewZoom = false;
	}
	else
	{
		zoomStepStart += game.getElapsedTime().asSeconds();

		if (zoomStepStart >= zoomStepStop)
		{
			zoomStepStart = zoomStepStop;
			hasNewZoom = false;
		}

		currentZoomFactor = EasingFunctions::easeOutQuart(
			zoomStepStart,
			startZoomFactor,
			diffZoomFactor,
			zoomStepStop);
	}

	view.setZoom(1.f / currentZoomFactor);

	view.updateViewport(game);
	updateVisibleArea();
}

void Level::addLevelObject(std::unique_ptr<LevelObject> obj)
{
	auto mapPos = obj->MapPosition();
	addLevelObject(std::move(obj), mapPos);
}

void Level::addLevelObject(std::unique_ptr<LevelObject> obj, const MapCoord& mapCoord)
{
	if (obj->getId().empty() == false)
	{
		if (levelObjectIds.find(obj->getId()) != levelObjectIds.end())
		{
			return;
		}
		levelObjectIds[obj->getId()] = obj.get();
	}
	obj->MapPosition(map, mapCoord);
	levelObjects.push_back(std::move(obj));
}

void Level::clearCache(const LevelObject* obj) noexcept
{
	if (clickedObject == obj)
	{
		clickedObject = nullptr;
	}
	if (hoverObject == obj)
	{
		hoverObject = nullptr;
	}
	if (currentPlayer == obj)
	{
		currentPlayer = nullptr;
	}
}

void Level::deleteLevelObjectById(const std::string_view id)
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

void Level::deleteLevelObjectByClass(const std::string_view classId)
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

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (visible == false)
	{
		return;
	}

	auto origView = target.getView();

	Sprite2 sprite;
	TextureInfo ti;
	sf::FloatRect tileRect;
	size_t layerToDrawLevelObjects = 0;

	if (levelLayers[1].tiles != nullptr)
	{
		layerToDrawLevelObjects = 1;
	}
	else if (levelLayers[2].tiles != nullptr)
	{
		layerToDrawLevelObjects = 2;
	}

	for (size_t i = 0; i < levelLayers.size(); i++)
	{
		const auto& layer = levelLayers[i];
		auto tiles = layer.tiles.get();

		if (layer.view == nullptr)
		{
			continue;
		}
		if (layer.visible == false)
		{
			// don't skip layer (level objects are drawn in this layer)
			if (i != layerToDrawLevelObjects)
			{
				continue;
			}
		}
		else
		{
			if (layer.background != sf::Color::Transparent &&
				tiles != nullptr)
			{
				target.setView(origView);
				sf::RectangleShape background(layer.view->getSize());
				background.setPosition(layer.view->getPosition());
				background.setFillColor(layer.background);
				target.draw(background);
			}
		}

		target.setView(layer.view->getView());

		if (tiles == nullptr)
		{
			// don't skip layer (level objects are drawn in this layer)
			if (i != layerToDrawLevelObjects)
			{
				continue;
			}
		}
		MapCoord mapPos;
		for (mapPos.x = layer.visibleStart.x; mapPos.x < layer.visibleEnd.x; mapPos.x++)
		{
			for (mapPos.y = layer.visibleStart.y; mapPos.y < layer.visibleEnd.y; mapPos.y++)
			{
				uint8_t light = 255;
				int16_t index;
				if (map.isMapCoordValid(mapPos) == false)
				{
					if (i != AutomapLayer)
					{
						light = map.getDefaultLight();
					}
					index = map.getOutOfBoundsTileIndex(i, mapPos.x, mapPos.y);
				}
				else
				{
					if (i != AutomapLayer)
					{
						light = std::max(map[mapPos].getDefaultLight(), map[mapPos].getCurrentLight());
					}
					index = map[mapPos].getTileIndex(i);

					if (i == layerToDrawLevelObjects)	// draw level objets in this layer
					{
						for (const auto& drawObj : map[mapPos])
						{
							if (drawObj != nullptr)
							{
								auto objLight = std::max(drawObj->getLight(), light);
								drawObj->draw(target, states, objLight);
							}
						}
						if (tiles == nullptr ||
							layer.visible == false)
						{
							continue;
						}
					}
				}
				if (index < 0 || light == 0)
				{
					continue;
				}
				if (tiles->get((size_t)index, ti) == true)
				{
					auto drawPos = map.getCoord(mapPos, layer.blockWidth, layer.blockHeight);
					drawPos.y -= (float)(ti.textureRect.height - layer.tileHeight);
					drawPos += ti.offset;
					tileRect.left = drawPos.x;
					tileRect.top = drawPos.y;
					tileRect.width = (float)ti.textureRect.width;
					tileRect.height = (float)ti.textureRect.height;
					if (layer.visibleRect.intersects(tileRect) == true)
					{
						sprite.setTexture(ti, true);
						sprite.setPosition(drawPos);
						sprite.draw(target, states, light);
					}
				}
			}
		}
	}

	// draw player direction in automap, if enabled (baseIndex >= 0)
	if (levelLayers[AutomapLayer].tiles != nullptr &&
		levelLayers[AutomapLayer].visible == true &&
		automapPlayerDirectionBaseIndex >= 0 &&
		currentPlayer != nullptr)
	{
		auto direction = (size_t)currentPlayer->getDirection();
		auto index = (size_t)automapPlayerDirectionBaseIndex + direction;
		if (direction < (size_t)PlayerDirection::All &&
			levelLayers[AutomapLayer].tiles->get(index, ti) == true)
		{
			target.setView(levelLayers[AutomapLayer].view->getView());

			auto drawPos = currentAutomapViewCenter;
			drawPos.x -= (float)(ti.textureRect.width / 2);
			drawPos.y -= (float)(ti.textureRect.height / 2);
			drawPos += ti.offset;
			tileRect.left = drawPos.x;
			tileRect.top = drawPos.y;
			tileRect.width = (float)ti.textureRect.width;
			tileRect.height = (float)ti.textureRect.height;
			if (levelLayers[AutomapLayer].visibleRect.intersects(tileRect) == true)
			{
				sprite.setTexture(ti, true);
				sprite.setPosition(drawPos);
				target.draw(sprite, states);
			}
		}
	}

	target.setView(origView);
}

void Level::updateSize(const Game& game)
{
	view.updateSize(game);
	automapView.updateSize(game);
	updateAutomapPosition(view.getPosition());
	updateAutomapSize(view.getSize());
	updateVisibleArea();
}

void Level::updateVisibleArea()
{
	sf::FloatRect visibleRect;
	MapCoord visibleStart;
	MapCoord visibleEnd;
	int tileWidth{ 0 };
	int tileHeight{ 0 };
	View2* layerView{ nullptr };

	for (auto& layer : levelLayers)
	{
		if (layer.tileWidth == 0 &&
			layer.tileHeight == 0)
		{
			continue;
		}
		if (layer.tileWidth != tileWidth ||
			layer.tileHeight != tileHeight ||
			layer.view != layerView)
		{
			tileWidth = layer.tileWidth;
			tileHeight = layer.tileHeight;
			layerView = layer.view;

			const auto& viewCenter = layer.view->getCenter();
			const auto& viewSize = layer.view->getVisibleSize();

			visibleRect = sf::FloatRect(viewCenter.x - std::round(viewSize.x / 2),
				viewCenter.y - std::round(viewSize.y / 2),
				std::round(viewSize.x),
				std::round(viewSize.y));

			sf::Vector2f TL{ visibleRect.left, visibleRect.top };
			sf::Vector2f TR{ TL.x + visibleRect.width + tileWidth, TL.y };
			sf::Vector2f BL{ TL.x, TL.y + visibleRect.height + tileHeight };
			sf::Vector2f BR{ TR.x, BL.y };

			auto mapTL = map.getTile(TL, layer.blockWidth, layer.blockHeight);
			auto mapTR = map.getTile(TR, layer.blockWidth, layer.blockHeight);
			auto mapBL = map.getTile(BL, layer.blockWidth, layer.blockHeight);
			auto mapBR = map.getTile(BR, layer.blockWidth, layer.blockHeight);

			visibleStart.x = mapTL.x - 2;
			visibleEnd.x = mapBR.x + 12;
			visibleStart.y = mapTR.y - 2;
			visibleEnd.y = mapBL.y + 12;
		}
		layer.visibleRect = visibleRect;
		layer.visibleStart = visibleStart;
		layer.visibleEnd = visibleEnd;
	}
}

void Level::updateMouse(const Game& game)
{
	mousePositionf = view.getPosition(game.MousePositionf());
	auto mousePositionf2 = mousePositionf;
	mousePositionf2.y += 224;
	mapCoordOverMouse = map.getTile(mousePositionf2);
}

void Level::onMouseButtonPressed(Game& game)
{
	game.clearMousePressed();
	switch (game.MousePress().button)
	{
	case sf::Mouse::Left:
	{
		clickedMapPosition = getMapCoordOverMouse();
		clickedObject = nullptr;
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

void Level::onMouseScrolled(Game& game)
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

void Level::onTouchBegan(Game& game)
{
	game.clearTouchBegan();
	switch (game.TouchBegan().finger)
	{
	case 0:
	{
		clickedMapPosition = getMapCoordOverMouse();
		clickedObject = nullptr;
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

void Level::update(Game& game)
{
	if (visible == false)
	{
		return;
	}
	if (viewPortNeedsUpdate == true)
	{
		view.updateViewport(game);
		automapView.updateViewport(game);
		updateVisibleArea();
	}
	if (pause == true)
	{
		return;
	}

	updateZoom(game);
	updateMouse(game);
	map.updateLights();

	sf::FloatRect rect(view.getPosition(), view.getSize());
	if (rect.contains(game.MousePositionf()) == true)
	{
		hasMouseInside = true;

		if (game.wasMousePressed() == true)
		{
			onMouseButtonPressed(game);
		}
		if (game.wasMouseScrolled() == true)
		{
			onMouseScrolled(game);
		}
		if (game.hasTouchBegan() == true)
		{
			onTouchBegan(game);
		}
		if (captureInputEvents != InputEvent::None)
		{
			game.clearInputEvents(captureInputEvents);
		}
	}
	else
	{
		hasMouseInside = false;
	}
	for (auto& obj : levelObjects)
	{
		obj->update(game, *this);
	}
	if (followCurrentPlayer == true && currentPlayer != nullptr)
	{
		currentMapPosition = currentPlayer->MapPosition();
		setCurrentMapViewCenter(currentPlayer->getBasePosition());
	}
	if (view.getCenter() != currentMapViewCenter ||
		viewPortNeedsUpdate == true)
	{
		view.setCenter(currentMapViewCenter);
		automapView.setCenter(currentAutomapViewCenter);
		updateVisibleArea();
	}
	if (viewPortNeedsUpdate == true)
	{
		viewPortNeedsUpdate = false;
	}
}

bool Level::getProperty(const std::string_view prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	switch (propHash)
	{
	case str2int16("clickedObject"):
	{
		if (clickedObject != nullptr)
		{
			return clickedObject->getProperty(props.second, var);
		}
		break;
	}
	case str2int16("currentPlayer"):
	{
		if (currentPlayer != nullptr)
		{
			return currentPlayer->getProperty(props.second, var);
		}
		break;
	}
	case str2int16("hasAutomap"):
		var = Variable((levelLayers[AutomapLayer].tiles != nullptr));
		return true;
	case str2int16("hasCurrentPlayer"):
		var = Variable((currentPlayer != nullptr));
		return true;
	case str2int16("hasQuest"):
		var = Variable(hasQuest(props.second));
		return true;
	case str2int16("hoverObject"):
	{
		if (hoverObject != nullptr)
		{
			return hoverObject->getProperty(props.second, var);
		}
		break;
	}
	case str2int16("id"):
		var = Variable(id);
		return true;
	case str2int16("name"):
		var = Variable(name);
		return true;
	case str2int16("path"):
		var = Variable(path);
		return true;
	case str2int16("levelObject"):
	{
		std::string_view props2;
		auto obj = parseLevelObjectIdOrMapPosition(props.second, props2);
		if (obj != nullptr)
		{
			return obj->getProperty(props2, var);
		}
		break;
	}
	case str2int16("player"):
	{
		auto props2 = Utils::splitStringIn2(props.second, '.');
		auto player = getLevelObject<Player>(std::string(props2.first));
		if (player != nullptr)
		{
			return player->getProperty(props2.second, var);
		}
		break;
	}
	case str2int16("quest"):
	{
		auto props2 = Utils::splitStringIn2(props.second, '.');
		for (const auto& quest : quests)
		{
			if (quest.Id() == props2.first)
			{
				return quest.getProperty(props2.second, var);
			}
		}
		break;
	}
	case str2int16("spell"):
	{
		auto props2 = Utils::splitStringIn2(props.second, '.');
		auto spell = getSpell(std::string{ props2.first });
		if (spell != nullptr)
		{
			return spell->getProperty(props2.second, var);
		}
		break;
	}
	case str2int16("showAutomap"):
		var = Variable(levelLayers[AutomapLayer].visible);
		return true;
		break;
	case str2int16("zoom"):
		var = Variable((double)stopZoomFactor);
		return true;
	case str2int16("zoomPercentage"):
		var = Variable((int64_t)(std::roundf(stopZoomFactor * 100.f)));
		return true;
	default:
		return getUIObjProp(propHash, props.second, var);
	}
	return false;
}

const Queryable* Level::getQueryable(const std::string_view prop) const
{
	if (prop.empty() == true)
	{
		return this;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	const Queryable* queryable = nullptr;
	switch (propHash)
	{
	case str2int16("clickedObject"):
	{
		queryable = clickedObject;
		break;
	}
	case str2int16("currentPlayer"):
	{
		queryable = currentPlayer;
		break;
	}
	break;
	case str2int16("hoverObject"):
	{
		queryable = hoverObject;
		break;
	}
	break;
	case str2int16("player"):
	{
		props = Utils::splitStringIn2(props.second, '.');
		queryable = getLevelObject<Player>(std::string(props.first));
	}
	break;
	case str2int16("quest"):
	{
		props = Utils::splitStringIn2(props.second, '.');
		for (const auto& quest : quests)
		{
			if (quest.Id() == props.first)
			{
				queryable = &quest;
				break;
			}
		}
	}
	break;
	default:
		break;
	}
	if (queryable != nullptr &&
		props.second.empty() == false)
	{
		return queryable->getQueryable(props.second);
	}
	return queryable;
}

LevelObject* Level::parseLevelObjectIdOrMapPosition(
	const std::string_view str, std::string_view& props) const
{
	auto strPair = Utils::splitStringIn2(str, '.');
	auto strPair2 = Utils::splitStringIn2(strPair.first, ',');
	if (strPair2.second.empty() == false)
	{
		MapCoord mapPos(Utils::strtou(strPair2.first), Utils::strtou(strPair2.second));
		if (map.isMapCoordValid(mapPos) == true)
		{
			props = strPair.second;
			return map[mapPos].getObject<LevelObject>();
		}
	}
	else
	{
		auto simpleObj = getLevelObject<SimpleLevelObject>(std::string(strPair2.first));
		if (simpleObj != nullptr)
		{
			props = strPair.second;
			return simpleObj;
		}
	}
	return nullptr;
}

void Level::clearAllLevelObjects()
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

LevelObject* Level::getLevelObject(const std::string id) const
{
	if (id.empty() == true)
	{
		return nullptr;
	}
	auto it = levelObjectIds.find(id);
	if (it != levelObjectIds.cend())
	{
		return it->second;
	}
	return nullptr;
}

LevelObject* Level::getLevelObjectByClass(const std::string_view classId) const noexcept
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

Player* Level::getPlayerOrCurrent(const std::string id) const noexcept
{
	if (id.empty() == true)
	{
		return currentPlayer;
	}
	return getLevelObject<Player>(id);
}

void Level::clearPlayerClasses()
{
	for (auto it = levelObjectClasses.begin(); it != levelObjectClasses.end();)
	{
		auto plrClass = dynamic_cast<PlayerClass*>(it->second.get());
		if (plrClass != nullptr)
		{
			bool classBeingUsed = false;
			for (const auto& obj : levelObjects)
			{
				if (obj->getBaseClass() == it->second.get())
				{
					classBeingUsed = true;
					break;
				}
			}
			if (classBeingUsed == false)
			{
				it = levelObjectClasses.erase(it);
				continue;
			}
		}
		++it;
	}
}

void Level::clearPlayerTextures() noexcept
{
	for (const auto& classObj : levelObjectClasses)
	{
		auto plrClass = dynamic_cast<PlayerClass*>(classObj.second.get());
		if (plrClass == nullptr)
		{
			continue;
		}
		bool classIsUsed = false;
		for (const auto& obj : levelObjects)
		{
			if (plrClass == obj->getBaseClass())
			{
				classIsUsed = true;
				break;
			}
		}
		if (classIsUsed == false)
		{
			plrClass->clearTextures();
		}
	}
}

Item* Level::getItem(const MapCoord& mapCoord) const noexcept
{
	if (map.isMapCoordValid(mapCoord) == true)
	{
		return map[mapCoord].getObject<Item>();
	}
	return nullptr;
}

Item* Level::getItem(const ItemCoordInventory& itemCoord) const
{
	Player* player;
	return getItem(itemCoord, player);
}

Item* Level::getItem(const ItemCoordInventory& itemCoord, Player*& player) const
{
	player = getPlayerOrCurrent(itemCoord.getPlayerId());
	if (player != nullptr)
	{
		if (itemCoord.isSelectedItem() == true)
		{
			return player->SelectedItem();
		}
		auto& inventory = player->getInventory(itemCoord.getInventoryIdx());
		if (itemCoord.isCoordXY() == true)
		{
			return inventory.get(itemCoord.getItemXY());
		}
		else
		{
			return inventory.get(itemCoord.getItemIdx());
		}
	}
	return nullptr;
}

Item* Level::getItem(const ItemLocation& location) const
{
	Player* player;
	return getItem(location, player);
}

Item* Level::getItem(const ItemLocation& location, Player*& player) const
{
	if (holdsMapCoord(location) == true)
	{
		player = nullptr;
		return getItem(std::get<MapCoord>(location));
	}
	else
	{
		return getItem(std::get<ItemCoordInventory>(location), player);
	}
}

std::unique_ptr<Item> Level::removeItem(const MapCoord& mapCoord)
{
	auto item = map.removeLevelObject<Item>(mapCoord);
	if (item != nullptr)
	{
		item->clearMapPosition();
		return removeLevelObject<Item>(item);
	}
	return nullptr;
}

std::unique_ptr<Item> Level::removeItem(const ItemCoordInventory& itemCoord)
{
	Player* player;
	return removeItem(itemCoord, player);
}

std::unique_ptr<Item> Level::removeItem(const ItemCoordInventory& itemCoord, Player*& player)
{
	player = getPlayerOrCurrent(itemCoord.getPlayerId());
	if (player != nullptr)
	{
		if (itemCoord.isSelectedItem() == true)
		{
			return player->SelectedItem(nullptr);
		}
		std::unique_ptr<Item> nullItem;
		auto& inventory = player->getInventory(itemCoord.getInventoryIdx());
		if (itemCoord.isCoordXY() == true)
		{
			std::unique_ptr<Item> oldItem;
			inventory.set(itemCoord.getItemXY(), nullItem, oldItem);
			return oldItem;
		}
		else
		{
			std::unique_ptr<Item> oldItem;
			inventory.set(itemCoord.getItemIdx(), nullItem, oldItem);
			return oldItem;
		}
	}
	return nullptr;
}

std::unique_ptr<Item> Level::removeItem(const ItemLocation& location)
{
	Player* player;
	return removeItem(location, player);
}

std::unique_ptr<Item> Level::removeItem(const ItemLocation& location, Player*& player)
{
	if (holdsMapCoord(location) == true)
	{
		player = nullptr;
		return removeItem(std::get<MapCoord>(location));
	}
	else
	{
		return removeItem(std::get<ItemCoordInventory>(location), player);
	}
}

bool Level::setItem(const MapCoord& mapCoord, std::unique_ptr<Item>& item)
{
	if (map.isMapCoordValid(mapCoord) == false)
	{
		return false;
	}
	auto& mapCell = map[mapCoord];
	auto oldItem = mapCell.getObject<Item>();
	if (item == nullptr)
	{
		if (oldItem != nullptr)
		{
			oldItem->remove(map);
			removeLevelObject<Item>(oldItem);
		}
		return true;
	}
	if (mapCell.PassableIgnoreObject(currentPlayer) == true
		&& oldItem == nullptr)
	{
		addLevelObject(std::move(item), mapCoord);
		return true;
	}
	return false;
}

bool Level::setItem(const ItemCoordInventory& itemCoord, std::unique_ptr<Item>& item)
{
	auto player = getPlayerOrCurrent(itemCoord.getPlayerId());
	if (player != nullptr)
	{
		if (itemCoord.isSelectedItem() == true)
		{
			player->SelectedItem(std::move(item));
			return true;
		}
		auto invIdx = itemCoord.getInventoryIdx();
		if (invIdx < player->getInventorySize())
		{
			size_t itemIdx;
			if (itemCoord.isCoordXY() == true)
			{
				itemIdx = player->getInventory(invIdx)
					.getIndex(itemCoord.getItemXY());
			}
			else
			{
				itemIdx = itemCoord.getItemIdx();
			}
			return player->setItem(invIdx, itemIdx, item);
		}
	}
	return false;
}

bool Level::setItem(const ItemLocation& location, std::unique_ptr<Item>& item)
{
	if (holdsMapCoord(location) == true)
	{
		return setItem(std::get<MapCoord>(location), item);
	}
	else
	{
		return setItem(std::get<ItemCoordInventory>(location), item);
	}
}

LevelObjValue Level::addItemQuantity(const ItemLocation& location, LevelObjValue amount)
{
	Player* player;
	auto item = getItem(location, player);
	if (item != nullptr)
	{
		if (amount != 0)
		{
			LevelObjValue newAmount = amount;
			auto newQuant = item->addQuantity(newAmount);

			if (player != nullptr &&
				std::get<ItemCoordInventory>(location).isSelectedItem() == false)
			{
				player->updateItemQuantityCache(
					item->Class()->IdHash16(), (amount - newAmount));
			}
			if (newQuant == 0)
			{
				removeItem(location);
			}
		}
	}
	return amount;
}

void Level::addQuest(Quest quest_)
{
	for (const auto& quest : quests)
	{
		if (quest.Id() == quest_.Id())
		{
			return;
		}
	}
	quests.push_back(std::move(quest_));
}

void Level::clearQuests()
{
	quests.clear();
}

void Level::deleteQuest(const std::string_view questId)
{
	for (auto it = quests.begin(); it != quests.end(); ++it)
	{
		if (it->Id() == questId)
		{
			quests.erase(it);
			return;
		}
	}
}

bool Level::hasQuest(const std::string_view questId) const noexcept
{
	for (auto& quest : quests)
	{
		if (quest.Id() == questId)
		{
			return true;
		}
	}
	return false;
}

void Level::setQuestState(const std::string_view questId, int state) noexcept
{
	for (auto& quest : quests)
	{
		if (quest.Id() == questId)
		{
			quest.State(state);
			return;
		}
	}
}

uint32_t Level::getExperienceFromLevel(uint32_t level) const
{
	if (level < experiencePoints.size())
	{
		return experiencePoints[level];
	}
	else if (experiencePoints.empty() == false)
	{
		return experiencePoints.back();
	}
	return 0;
}

bool Level::hasSpell(const std::string& id) const
{
	return getClass<Spell>(id) != nullptr;
}

Spell* Level::getSpell(const std::string& id) const
{
	return getClass<Spell>(id);
}

uint32_t Level::getLevelFromExperience(uint32_t experience) const
{
	if (experience == 0 || experiencePoints.size() <= 1)
	{
		return 1;
	}
	for (uint32_t i = 1; i < experiencePoints.size(); i++)
	{
		auto exp = experiencePoints[i];
		if (experience <= exp)
		{
			return i;
		}
	}
	return experiencePoints.size();
}

const std::string_view Level::getPropertyName(uint16_t hash16) const
{
	const auto elem = propertyNames.find(hash16);
	if (elem != propertyNames.cend())
	{
		return elem->second;
	}
	return {};
}

void Level::setPropertyName(uint16_t hash16, const std::string_view prop)
{
	propertyNames[hash16] = std::string(prop);
}
