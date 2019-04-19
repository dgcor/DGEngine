#include "Level.h"
#include "Game.h"
#include "GameHashes.h"
#include "GameUtils.h"
#include "Player.h"
#include "SimpleLevelObject.h"
#include "Utils/EasingFunctions.h"
#include "Utils/Utils.h"

void Level::Init(const Game& game, LevelMap map_,
	const std::vector<LevelLayer>& levelLayers_,
	int32_t tileWidth, int32_t tileHeight,
	int32_t indexToDrawObjects)
{
	map = std::move(map_);
	clickedObject = nullptr;
	hoverObject = nullptr;

	defaultLayerInfo.tileWidth = std::max(tileWidth, 2);
	defaultLayerInfo.tileHeight = std::max(tileHeight, 2);
	defaultLayerInfo.blockWidth = defaultLayerInfo.tileWidth / 2;
	defaultLayerInfo.blockHeight = defaultLayerInfo.tileHeight / 2;
	defaultLayerInfo.visible = true;

	auto mapVisible = automapLayerInfo.visible;
	automapLayerInfo = defaultLayerInfo;
	automapLayerInfo.visible = mapVisible;

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

	map.setDefaultTileSize(defaultLayerInfo.tileWidth, defaultLayerInfo.tileHeight);

	setCurrentMapPosition(PairFloat(-1.f, -1.f), false);

	game.recreateRenderTexture(levelTexture);

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

void Level::addLayer(const ColorLevelLayer& layer,
	const sf::FloatRect& viewportOffset, bool automap)
{
	levelLayers.push_back({ layer, viewportOffset, automap });
}

void Level::addLayer(const TextureLevelLayer& layer,
	const sf::FloatRect& viewportOffset, bool automap)
{
	levelLayers.push_back({ layer, viewportOffset, automap });
}

void Level::setAutomap(const TilesetLevelLayer& automapLayer, int tileWidth,
	int tileHeight, const sf::FloatRect& viewportOffset)
{
	automapLayerInfo.tileWidth = std::max(tileWidth, 2);
	automapLayerInfo.tileHeight = std::max(tileHeight, 2);
	automapLayerInfo.blockWidth = automapLayerInfo.tileWidth / 2;
	automapLayerInfo.blockHeight = automapLayerInfo.tileHeight / 2;

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
	viewPortNeedsUpdate = true;
}

void Level::setCurrentMapPosition(const PairFloat& mapPos, bool smooth) noexcept
{
	currentMapPosition = mapPos;
	setCurrentMapViewCenter(map.toDrawCoord(currentMapPosition), smooth);
}

void Level::setCurrentMapViewCenter(const sf::Vector2f& coord_, bool smooth) noexcept
{
	if (currentMapViewCenter != coord_)
	{
		currentMapViewCenter = coord_;
		currentMapViewCenterX.set(coord_.x, smooth == true ? 1.f : 0.f);
		currentMapViewCenterY.set(coord_.y, smooth == true ? 1.f : 0.f);
		updateCurrentAutomapViewCenter();
	}
}

void Level::updateCurrentAutomapViewCenter() noexcept
{
	currentAutomapViewCenter.x = std::round((currentMapViewCenter.x * (float)AutomapTileWidth()) / (float)TileWidth());
	currentAutomapViewCenter.y = std::round((currentMapViewCenter.y * (float)AutomapTileHeight()) / (float)TileHeight());
}

bool Level::updateCurrentMapViewCenter(bool smooth) noexcept
{
	if (followCurrentPlayer == true && currentPlayer != nullptr)
	{
		currentMapPosition = currentPlayer->MapPosition();
		setCurrentMapViewCenter(currentPlayer->getBasePosition(), smooth);
		return true;
	}
	return false;
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

void Level::setAutomapPosition(const sf::Vector2f& position) noexcept
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
		automapView.setPosition(automapPosition);
	}
	updateAutomapRelativePosition();
}

void Level::Position(const sf::Vector2f& position) noexcept
{
	view.setPosition(position);
	updateAutomapRelativePosition();
}

void Level::updateAutomapRelativePosition()
{
	if (automapRelativeCoords == true)
	{
		auto position = view.getPosition();
		position.x = std::round(position.x + view.getSize().x * automapPosition.x);
		position.y = std::round(position.y + view.getSize().y * automapPosition.y);
		automapView.setPosition(position);
	}
	viewPortNeedsUpdate = true;
}

void Level::setAutomapSize(const sf::Vector2f& size)
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
		automapView.setSize(automapSize);
	}
	updateAutomapRelativeSize();
}

void Level::Size(const sf::Vector2f& size)
{
	view.setSize(size);
	updateAutomapRelativePosition();
	updateAutomapRelativeSize();
}

void Level::updateAutomapRelativeSize()
{
	if (automapRelativeCoords == true)
	{
		auto size = view.getSize();
		size.x = std::round(size.x * automapSize.x);
		size.y = std::round(size.y * automapSize.y);
		automapView.setSize(size);
	}
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
	zoomValue.set(factor, smooth == true ? 1.f : 0.f);
}

void::Level::updateZoom(const Game& game)
{
	zoomValue.update(game.getElapsedTime().asSeconds());
	view.setZoom(1.f / zoomValue.get());
	view.updateViewport(game);
	updateVisibleArea();
}

void Level::addLevelObject(std::unique_ptr<LevelObject> obj)
{
	auto mapPos = obj->MapPosition();
	addLevelObject(std::move(obj), mapPos);
}

void Level::addLevelObject(std::unique_ptr<LevelObject> obj, const PairFloat& mapCoord)
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

void Level::draw(const Game& game, sf::RenderTarget& target) const
{
	if (visible == false)
	{
		return;
	}

	auto origView = target.getView();

	levelTexture.clear();

	SpriteShaderCache spriteCache;

	for (size_t i = 0; i < levelLayers.size(); i++)
	{
		const auto& levelLayer = levelLayers[i];

		if (holdsNullLevelLayer(levelLayer.layer) == true)
		{
			continue;
		}
		if (levelLayer.automap == false)
		{
			if (defaultLayerInfo.visible == false)
			{
				continue;
			}
		}
		else
		{
			if (automapLayerInfo.visible == false)
			{
				continue;
			}
		}

		const auto& layerInfo = (levelLayer.automap == false ? defaultLayerInfo : automapLayerInfo);
		const auto& layerView = (levelLayer.automap == false ? view : automapView);

		if (levelLayer.viewportOffset == sf::FloatRect(0.f, 0.f, 0.f, 0.f))
		{
			levelTexture.setView(layerView.getView());
		}
		else
		{
			auto newView = layerView;
			newView.setPosition(
				{ newView.getPosition().x + levelLayer.viewportOffset.left,
				newView.getPosition().y + levelLayer.viewportOffset.top }
			);
			newView.setSize(
				{ newView.getSize().x + levelLayer.viewportOffset.width,
				newView.getSize().y + levelLayer.viewportOffset.height }
			);
			newView.updateViewport(game);
			levelTexture.setView(newView.getView());
		}

		if (holdsTilesetLevelLayer(levelLayer.layer) == true)
		{
			const auto& layer = std::get<TilesetLevelLayer>(levelLayer.layer);
			layer.draw(levelTexture, layerInfo, spriteCache,
				game.Shaders().Sprite, *this,
				i == indexToDrawLevelObjects,
				levelLayer.automap
			);
		}
		else if (holdsTextureLevelLayer(levelLayer.layer) == true)
		{
			const auto& layer = std::get<TextureLevelLayer>(levelLayer.layer);
			layer.draw(levelTexture, layerInfo, layerView.getCenter());
		}
		else if (holdsColorLevelLayer(levelLayer.layer) == true)
		{
			const auto& layer = std::get<ColorLevelLayer>(levelLayer.layer);
			layer.draw(levelTexture, layerInfo);
		}
	}

	levelTexture.display();

	sf::RenderStates states(sf::RenderStates::Default);
	sf::Sprite levelSprite(levelTexture.getTexture());
	if (shader != nullptr)
	{
		states.shader = shader;
		shader->setUniform("elapsedTime", game.getTotalElapsedTime().asSeconds());
		if (hasMouseInside == true)
		{
			shader->setUniform("mousePosition", sf::Glsl::Vec2(
				(game.MousePositionf().x - view.getPosition().x) / view.getSize().x,
				(game.MousePositionf().y - view.getPosition().y) / view.getSize().y
			));
		}
		shader->setUniform("textureSize", sf::Glsl::Vec2(
			view.getSize().x,
			view.getSize().y
		));
	}
	target.draw(levelSprite, states);

	target.setView(origView);
}

void Level::updateSize(const Game& game)
{
	view.updateSize(game);
	automapView.updateSize(game);
	updateAutomapRelativePosition();
	updateAutomapRelativeSize();
	updateVisibleArea();
	game.recreateRenderTexture(levelTexture);
}

void Level::updateVisibleArea()
{
	updateLayerInfoVisibleArea(defaultLayerInfo, view);
	updateLayerInfoVisibleArea(automapLayerInfo, automapView);
	updateTilesetLayersVisibleArea();
}

void Level::updateLayerInfoVisibleArea(LevelLayerInfo& layerInfo, View2& layerView)
{
	const auto& viewCenter = layerView.getCenter();
	const auto& viewSize = layerView.getVisibleSize();

	layerInfo.visibleRect = sf::FloatRect(
		viewCenter.x - std::round(viewSize.x / 2),
		viewCenter.y - std::round(viewSize.y / 2),
		std::round(viewSize.x),
		std::round(viewSize.y)
	);
}

void Level::updateTilesetLayersVisibleArea()
{
	for (auto& levelLayer : levelLayers)
	{
		if (holdsTilesetLevelLayer(levelLayer.layer) == false)
		{
			continue;
		}
		auto& layer = std::get<TilesetLevelLayer>(levelLayer.layer);
		const auto& layerInfo = (levelLayer.automap == false ? defaultLayerInfo : automapLayerInfo);
		layer.updateVisibleArea(layerInfo, map);
	}
}

void Level::updateMouse(const Game& game)
{
	mousePositionf = view.getPosition(game.MousePositionf());
	auto mousePositionf2 = mousePositionf;
	mousePositionf2.x -= (float)map.DefaultBlockWidth();
	mousePositionf2.y -= (float)map.DefaultBlockHeight();
	mapCoordOverMouse = map.toMapCoord(mousePositionf2);
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

	for (auto& layer : levelLayers)
	{
		if (holdsTilesetLevelLayer(layer.layer) == true)
		{
			auto tiles = std::get<TilesetLevelLayer>(layer.layer).tiles.get();
			if (tiles != nullptr)
			{
				tiles->update(game.getElapsedTime());
			}
		}
		if (holdsTextureLevelLayer(layer.layer) == true)
		{
			std::get<TextureLevelLayer>(layer.layer).update(game.getElapsedTime());
		}
	}

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
	if (currentMapPosition.x == -1.f &&
		currentMapPosition.y == -1.f)
	{
		if (updateCurrentMapViewCenter(false) == false)
		{
			setCurrentMapPosition(
				{ map.MapSizef().x / 2.f, map.MapSizef().y / 2.f },
				false
			);
		}
	}
	if (smoothMovement == true)
	{
		auto elapsedSeconds = game.getElapsedTime().asSeconds();
		currentMapViewCenterX.update(elapsedSeconds);
		currentMapViewCenterY.update(elapsedSeconds);
	}
	updateCurrentMapViewCenter(smoothMovement);
	auto newViewCenterX = std::round(currentMapViewCenterX.get());
	auto newViewCenterY = std::round(currentMapViewCenterY.get());
	if (view.getCenter().x != newViewCenterX ||
		view.getCenter().y != newViewCenterY ||
		viewPortNeedsUpdate == true)
	{
		view.setCenter(newViewCenterX, newViewCenterY);
		updateCurrentAutomapViewCenter();
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
		var = Variable(hasAutomap());
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
		var = Variable(automapLayerInfo.visible);
		return true;
	case str2int16("zoom"):
		var = Variable((double)zoomValue.getFinal());
		return true;
	case str2int16("zoomPercentage"):
		var = Variable((int64_t)(std::round(zoomValue.getFinal() * 100.f)));
		return true;
	default:
		return getUIObjProp(propHash, props.second, var);
	}
	return false;
}

bool Level::hasAutomap() const noexcept
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

std::vector<std::variant<const Queryable*, Variable>> Level::getQueryableList(
	const std::string_view prop) const
{
	std::vector<std::variant<const Queryable*, Variable>> queriableList;

	auto props = Utils::splitStringIn2(prop, '.');
	if (props.first.empty() == false)
	{
		if (props.first == "quests")
		{
			for (const auto& quest : quests)
			{
				queriableList.push_back({ &quest });
			}
		}
		else
		{
			auto player = getPlayerOrCurrent(std::string(props.first));
			if (player == nullptr)
			{
				player = getCurrentPlayer();
			}
			if (player != nullptr)
			{
				return player->getQueryableList(props.second);
			}
		}
	}
	return queriableList;
}

LevelObject* Level::parseLevelObjectIdOrMapPosition(
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

void Level::setCurrentPlayer(Player* player_) noexcept
{
	currentPlayer = player_;
	updateCurrentMapViewCenter(false);
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

Item* Level::getItem(const PairFloat& mapCoord) const noexcept
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
		return getItem(std::get<PairFloat>(location));
	}
	else
	{
		return getItem(std::get<ItemCoordInventory>(location), player);
	}
}

std::unique_ptr<Item> Level::removeItem(const PairFloat& mapCoord)
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
		return removeItem(std::get<PairFloat>(location));
	}
	else
	{
		return removeItem(std::get<ItemCoordInventory>(location), player);
	}
}

bool Level::setItem(const PairFloat& mapCoord, std::unique_ptr<Item>& item)
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
		return setItem(std::get<PairFloat>(location), item);
	}
	else
	{
		return setItem(std::get<ItemCoordInventory>(location), item);
	}
}

bool Level::deleteItem(const PairFloat& mapCoord)
{
	std::unique_ptr<Item> nullItem;
	return setItem(mapCoord, nullItem);
}

bool Level::deleteItem(const ItemCoordInventory& itemCoord)
{
	std::unique_ptr<Item> nullItem;
	return setItem(itemCoord, nullItem);
}

bool Level::deleteItem(const ItemLocation& location)
{
	std::unique_ptr<Item> nullItem;
	return setItem(location, nullItem);
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
