#include "Level.h"
#include "Game.h"
#include "GameHashes.h"
#include "GameUtils.h"
#include "Panel.h"
#include "Player.h"
#include "SimpleLevelObject.h"
#include "Utils/Utils.h"

void Level::Init(const Game& game, LevelMap map_,
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
	automapSurface.visible = false;

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

void Level::Init()
{
	clickedObject.reset();
	hoverObject.reset();

	updateLevelObjectPositions();
	viewNeedsUpdate = true;
}

void Level::Id(const std::string_view id_)
{
	id = id_;
	moveUpEventHash = str2int16(id + ".up");
	moveDownEventHash = str2int16(id + ".down");
	moveLeftEventHash = str2int16(id + ".left");
	moveRightEventHash = str2int16(id + ".right");
	doActionEventHash = str2int16(id + ".doAction");
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

void Level::addDrawable(LevelDrawable obj)
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

Panel* Level::getDrawable(size_t idx) const
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

LevelDrawable* Level::getLevelDrawable(const std::string_view id)
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
	if (followCurrentPlayer == true)
	{
		if (auto obj = currentPlayer.lock())
		{
			currentMapPosition = obj->MapPosition();
			setCurrentMapViewCenter(obj->getBasePosition(), smooth);
			return true;
		}
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
		automapSurface.Position(automapPosition);
	}
	updateAutomapRelativePosition();
}

void Level::Position(const sf::Vector2f& position) noexcept
{
	surface.Position(position);
	updateAutomapRelativePosition();
}

void Level::LightRadius(float lightRadius_) noexcept
{
	lightRadius = std::clamp(lightRadius_, 4.f, 256.f);
}

void Level::updateAutomapRelativePosition()
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
		automapSurface.Size(automapSize);
	}
	updateAutomapRelativeSize();
}

void Level::Size(const sf::Vector2f& size)
{
	surface.Size(size);
	updateAutomapRelativePosition();
	updateAutomapRelativeSize();
}

void Level::updateAutomapRelativeSize()
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

void Level::Zoom(float factor, bool smooth) noexcept
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

void Level::updateZoom(const Game& game)
{
	zoomValue.update(game.getElapsedTime().asSeconds());
	auto newZoom = 1.f / zoomValue.get();
	if (surface.updateZoom(game, newZoom) == true)
	{
		updateTilesetLayersVisibleArea();
	}
}

void Level::addLevelObject(std::shared_ptr<LevelObject> obj)
{
	auto mapPos = obj->MapPosition();
	addLevelObject(std::move(obj), mapPos);
}

void Level::addLevelObject(std::shared_ptr<LevelObject> obj, const PairFloat& mapCoord)
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

void Level::clearCache(const LevelObject* obj) noexcept
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

	// layers

	surface.clear(sf::Color::Black);
	automapSurface.clear(sf::Color::Transparent);

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
			if (surface.visible == false)
			{
				continue;
			}
		}
		else
		{
			if (automapSurface.visible == false)
			{
				continue;
			}
		}

		auto& layerSurface = (levelLayer.automap == false ? surface : automapSurface);

		layerSurface.updateDrawView(levelLayer.viewportOffset);

		if (holdsTilesetLevelLayer(levelLayer.layer) == true)
		{
			const auto& layer = std::get<TilesetLevelLayer>(levelLayer.layer);
			layer.draw(layerSurface, spriteCache,
				game.Shaders().Sprite, *this,
				i == indexToDrawLevelObjects,
				levelLayer.automap
			);
		}
		else if (holdsTextureLevelLayer(levelLayer.layer) == true)
		{
			const auto& layer = std::get<TextureLevelLayer>(levelLayer.layer);
			layer.draw(layerSurface);
		}
		else if (holdsColorLevelLayer(levelLayer.layer) == true)
		{
			const auto& layer = std::get<ColorLevelLayer>(levelLayer.layer);
			layer.draw(layerSurface);
		}
	}

	// lighting

	if (lights.empty() == false && map.getDefaultLight() < 255)
	{
#if defined(SFML_BLENDMODE_MIN_MAX)
		const static sf::BlendMode lightBlend(
			sf::BlendMode::Zero, sf::BlendMode::One, sf::BlendMode::Add,
			sf::BlendMode::One, sf::BlendMode::Zero, sf::BlendMode::Min
		);
#else
		const static sf::BlendMode lightBlend(
			sf::BlendMode::Zero, sf::BlendMode::One, sf::BlendMode::Add,
			sf::BlendMode::DstAlpha, sf::BlendMode::Zero, sf::BlendMode::Add
		);
#endif

		sf::RenderStates lightStates;
		lightStates.blendMode = lightBlend;

		for (const auto& light : lights)
		{
			surface.draw(light, lightStates);
		}
	}

	auto surfaceStates(sf::RenderStates::Default);
	if (gameShader != nullptr)
	{
		auto shader = gameShader->shader.get();
		surfaceStates.shader = shader;
		for (auto uniformHash : gameShader->uniforms)
		{
			switch (uniformHash)
			{
			case str2int16("elapsedTime"):
			{
				shader->setUniform("elapsedTime",game.getTotalElapsedTime().asSeconds());
				break;
			}
			case str2int16("mousePosition"):
			{
				if (hasMouseInside == true)
				{
					shader->setUniform("mousePosition", sf::Glsl::Vec2(
						(game.MousePositionf().x - surface.Position().x) /
						surface.Size().x,
						(game.MousePositionf().y - surface.Position().y) /
						surface.Size().y
					));
				}
				break;
			}
			case str2int16("textureSize"):
			{
				shader->setUniform("textureSize", sf::Glsl::Vec2(
					surface.Size().x,
					surface.Size().y
				));
				break;
			}
			case str2int16("visibleRect"):
			{
				shader->setUniform("visibleRect", sf::Glsl::Vec4(
					surface.visibleRect.left,
					surface.visibleRect.top,
					surface.visibleRect.width,
					surface.visibleRect.height
				));
				break;
			}
			case str2int16("defaultLight"):
			{
				shader->setUniform("defaultLight",(float)(255 - map.getDefaultLight()) / 255);
				break;
			}
			default:
				break;
			}
		}
	}
	surface.draw(target, surfaceStates);

	// level drawables

	if (drawables.empty() == false)
	{
		surface.clear(sf::Color::Transparent);
		bool hasDrawn = false;
		for (const auto& item : drawables)
		{
			if (auto obj = item.drawable.lock())
			{
				hasDrawn |= surface.draw(game, *obj);
			}
		}
		if (hasDrawn == true)
		{
			surface.draw(target);
		}
	}

	// automap

	automapSurface.draw(target, sf::RenderStates::Default);

	target.setView(origView);
}

void Level::updateSize(const Game& game)
{
	surface.updateSize(game);
	updateAutomapRelativePosition();
	updateAutomapRelativeSize();
	automapSurface.updateSize(game);
	updateTilesetLayersVisibleArea();
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
		const auto& layerSurface = (levelLayer.automap == false ? surface : automapSurface);
		layer.updateVisibleArea(layerSurface, map);
	}
}

void Level::updateMouse(const Game& game)
{
	mousePositionf = surface.getPosition(game.MousePositionf());
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

void Level::processInputEvents(Game& game)
{
	if (game.Resources().hasActiveInputEvents() == false)
	{
		return;
	}
	if (auto player = currentPlayer.lock())
	{
		if (player->blockWalk() == true)
		{
			return;
		}
		PlayerDirection direction(PlayerDirection::All);
		if (game.Resources().hasActiveInputEvents({ moveUpEventHash, moveLeftEventHash }) == true)
		{
			direction = PlayerDirection::BackLeft;
		}
		else if (game.Resources().hasActiveInputEvents({ moveUpEventHash, moveRightEventHash }) == true)
		{
			direction = PlayerDirection::BackRight;
		}
		else if (game.Resources().hasActiveInputEvents({ moveDownEventHash, moveLeftEventHash }) == true)
		{
			direction = PlayerDirection::FrontLeft;
		}
		else if (game.Resources().hasActiveInputEvents({ moveDownEventHash, moveRightEventHash }) == true)
		{
			direction = PlayerDirection::FrontRight;
		}
		else if (game.Resources().hasActiveInputEvents({ moveUpEventHash }) == true)
		{
			direction = PlayerDirection::Back;
		}
		else if (game.Resources().hasActiveInputEvents({ moveDownEventHash }) == true)
		{
			direction = PlayerDirection::Front;
		}
		else if (game.Resources().hasActiveInputEvents({ moveLeftEventHash }) == true)
		{
			direction = PlayerDirection::Left;
		}
		else if (game.Resources().hasActiveInputEvents({ moveRightEventHash }) == true)
		{
			direction = PlayerDirection::Right;
		}
		bool doAction = game.Resources().hasActiveInputEvents({ doActionEventHash });
		player->Walk(map, direction, doAction);
	}
}

void Level::updateLights()
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

void Level::update(Game& game)
{
	if (visible == false)
	{
		return;
	}
	if (viewNeedsUpdate == true)
	{
		surface.updateView(game);
		automapSurface.updateView(game);
		updateTilesetLayersVisibleArea();
	}
	if (pause == true)
	{
		return;
	}

	updateZoom(game);
	updateMouse(game);
	updateLights();
	processInputEvents(game);

	epoch++;

	for (auto& layer : levelLayers)
	{
		if (holdsTilesetLevelLayer(layer.layer) == true)
		{
			auto tiles = std::get<TilesetLevelLayer>(layer.layer).tiles.get();
			if (tiles != nullptr)
			{
				tiles->update(epoch, game.getElapsedTime());
			}
		}
		if (holdsTextureLevelLayer(layer.layer) == true)
		{
			std::get<TextureLevelLayer>(layer.layer).update(game.getElapsedTime());
		}
	}

	sf::FloatRect rect(surface.Position(), surface.Size());
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
		if (captureInputEvents != InputEventType::None)
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
		obj->update(game, *this, obj);
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
	if (surface.getCenter().x != newViewCenterX ||
		surface.getCenter().y != newViewCenterY ||
		viewNeedsUpdate == true)
	{
		surface.setCenter(newViewCenterX, newViewCenterY);
		updateCurrentAutomapViewCenter();
		automapSurface.setCenter(currentAutomapViewCenter);
		updateTilesetLayersVisibleArea();
	}
	if (viewNeedsUpdate == true)
	{
		viewNeedsUpdate = false;
	}

	updateDrawables(game);
}

void Level::updateDrawables(Game& game)
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

void Level::setLevelDrawablePosition(LevelDrawable& obj, Panel& panelObj)
{
	if (auto anchorTo = obj.anchorTo.lock())
	{
		const sf::Vector2f& drawPos = panelObj.DrawPosition();
		sf::Vector2f drawSize = panelObj.Size();
		auto newPos = anchorTo->getAnchorPosition() + obj.offset;
		auto drawPosOffset = drawPos - panelObj.Position();
		auto newDrawPos = newPos + drawPosOffset;
		if (obj.visibleRectOffset >= 0.f)
		{
			if (newDrawPos.x < surface.visibleRect.left + obj.visibleRectOffset)
			{
				newPos.x = surface.visibleRect.left + obj.visibleRectOffset - drawPosOffset.x;
			}
			if (newDrawPos.y < surface.visibleRect.top + obj.visibleRectOffset)
			{
				newPos.y = surface.visibleRect.top + obj.visibleRectOffset - drawPosOffset.y;
			}
			newDrawPos += drawSize;
			if (newDrawPos.x > surface.visibleRect.left +
				surface.visibleRect.width - obj.visibleRectOffset)
			{
				newPos.x = surface.visibleRect.left + surface.visibleRect.width
					- obj.visibleRectOffset - drawSize.x - drawPosOffset.x;
			}
			if (newDrawPos.y > surface.visibleRect.top +
				surface.visibleRect.height - obj.visibleRectOffset)
			{
				newPos.y = surface.visibleRect.top + surface.visibleRect.height
					- obj.visibleRectOffset - drawSize.y - drawPosOffset.y;
			}
		}
		panelObj.Position(newPos);
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
		if (auto obj = clickedObject.lock())
		{
			return obj->getProperty(props.second, var);
		}
		break;
	}
	case str2int16("currentPlayer"):
	{
		if (auto obj = currentPlayer.lock())
		{
			return obj->getProperty(props.second, var);
		}
		break;
	}
	case str2int16("hasAutomap"):
		var = Variable(hasAutomap());
		return true;
	case str2int16("hasCurrentPlayer"):
		var = Variable((currentPlayer.expired() == false));
		return true;
	case str2int16("hasQuest"):
		var = Variable(hasQuest(props.second));
		return true;
	case str2int16("hoverObject"):
	{
		if (auto obj = hoverObject.lock())
		{
			return obj->getProperty(props.second, var);
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
		auto player = getLevelObject<Player>(props2.first);
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
		var = Variable(automapSurface.visible);
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
		queryable = clickedObject.lock().get();
		break;
	}
	case str2int16("currentPlayer"):
	{
		queryable = currentPlayer.lock().get();
		break;
	}
	break;
	case str2int16("hoverObject"):
	{
		queryable = hoverObject.lock().get();
		break;
	}
	break;
	case str2int16("player"):
	{
		props = Utils::splitStringIn2(props.second, '.');
		queryable = getLevelObject<Player>(props.first);
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
			auto player = getPlayerOrCurrent(props.first);
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
		auto simpleObj = getLevelObject<SimpleLevelObject>(strPair2.first);
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

LevelObject* Level::getLevelObject(const std::string_view id) const
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

std::weak_ptr<LevelObject> Level::getLevelObjectPtr(const std::string_view id) const
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

Player* Level::getPlayerOrCurrent(const std::string_view id) const noexcept
{
	if (id.empty() == true)
	{
		return currentPlayer.lock().get();
	}
	return getLevelObject<Player>(id);
}

void Level::setCurrentPlayer(std::weak_ptr<Player> player_) noexcept
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

std::shared_ptr<Item> Level::removeItem(const PairFloat& mapCoord)
{
	auto item = map.removeLevelObject<Item>(mapCoord);
	if (item != nullptr)
	{
		item->clearMapPosition();
		return removeLevelObject<Item>(item);
	}
	return nullptr;
}

std::shared_ptr<Item> Level::removeItem(const ItemCoordInventory& itemCoord)
{
	Player* player;
	return removeItem(itemCoord, player);
}

std::shared_ptr<Item> Level::removeItem(const ItemCoordInventory& itemCoord, Player*& player)
{
	player = getPlayerOrCurrent(itemCoord.getPlayerId());
	if (player != nullptr)
	{
		if (itemCoord.isSelectedItem() == true)
		{
			return player->SelectedItem(nullptr);
		}
		std::shared_ptr<Item> nullItem;
		auto& inventory = player->getInventory(itemCoord.getInventoryIdx());
		if (itemCoord.isCoordXY() == true)
		{
			std::shared_ptr<Item> oldItem;
			inventory.set(itemCoord.getItemXY(), nullItem, oldItem);
			return oldItem;
		}
		else
		{
			std::shared_ptr<Item> oldItem;
			inventory.set(itemCoord.getItemIdx(), nullItem, oldItem);
			return oldItem;
		}
	}
	return nullptr;
}

std::shared_ptr<Item> Level::removeItem(const ItemLocation& location)
{
	Player* player;
	return removeItem(location, player);
}

std::shared_ptr<Item> Level::removeItem(const ItemLocation& location, Player*& player)
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

bool Level::setItem(const PairFloat& mapCoord, std::shared_ptr<Item>& item)
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
	if (mapCell.PassableIgnoreObject(currentPlayer.lock().get()) == true
		&& oldItem == nullptr)
	{
		addLevelObject(std::move(item), mapCoord);
		return true;
	}
	return false;
}

bool Level::setItem(const ItemCoordInventory& itemCoord, std::shared_ptr<Item>& item)
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

bool Level::setItem(const ItemLocation& location, std::shared_ptr<Item>& item)
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
	std::shared_ptr<Item> nullItem;
	return setItem(mapCoord, nullItem);
}

bool Level::deleteItem(const ItemCoordInventory& itemCoord)
{
	std::shared_ptr<Item> nullItem;
	return setItem(itemCoord, nullItem);
}

bool Level::deleteItem(const ItemLocation& location)
{
	std::shared_ptr<Item> nullItem;
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

bool Level::hasSpell(const std::string_view id) const
{
	return getClass<Spell>(id) != nullptr;
}

Spell* Level::getSpell(const std::string_view id) const
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
