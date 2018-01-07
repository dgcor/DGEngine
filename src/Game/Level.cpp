#include "Level.h"
#include "Game.h"
#include "GameUtils.h"
#include "Shaders.h"
#include "Utils.h"

void Level::Init(const LevelMap& map_,
	std::shared_ptr<TexturePack>& tilesBottom_,
	std::shared_ptr<TexturePack>& tilesTop_)
{
	map = map_;
	tilesBottom = tilesBottom_;
	tilesTop = tilesTop_;
	currentMapPosition = MapCoord(map.Width() / 2, map.Height() / 2);
	hoverObject = nullptr;

	sf::Vector2i textureSize;
	if (tilesBottom_ != nullptr &&
		tilesBottom_->getTextureSize(textureSize) == true)
	{
		tileWidth = textureSize.x;
		tileHeight = textureSize.y;
	}
	else
	{
		tileWidth = 64;
		tileHeight = 32;
	}
	if (tilesTop_ != nullptr &&
		tilesTop_->getTextureSize(textureSize) == true)
	{
		pillarHeight = textureSize.y;
	}
	else
	{
		pillarHeight = 256;
	}
	map.setTileSize(tileWidth, tileHeight);
}

std::shared_ptr<Action> Level::getAction(uint16_t nameHash16)
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

bool Level::setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action)
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

void Level::Zoom(float factor, bool smooth)
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

// Quart ease out function
// http://easings.net/
// t is the current time.
// b is the beginning value of the property.
// c is the change between the beginning and destination value of the property.
// d is the final time.
static float easeOutFunction(float t, float b, float c, float d)
{
	t = t / d - 1;
	return -c * (t*t*t*t - 1) + b;
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

		currentZoomFactor = easeOutFunction(
			zoomStepStart,
			startZoomFactor,
			diffZoomFactor,
			zoomStepStop);
	}

	view.setZoom(1.f / currentZoomFactor);

	view.updateViewport(game);
}

void Level::deleteLevelObject(const LevelObject* obj)
{
	levelObjects.erase(
		std::remove_if(levelObjects.begin(),
			levelObjects.end(),
			[&](const std::shared_ptr<LevelObject>& p) { return p.get() == obj; }),
		levelObjects.end());

	if (clickedObject == obj)
	{
		clickedObject = nullptr;
	}
	if (hoverObject == obj)
	{
		hoverObject = nullptr;
	}
}

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (visible == false)
	{
		return;
	}

	auto origView = target.getView();
	target.setView(view.getView());

	sf::Sprite sprite;
	const sf::Texture* texture;
	sf::IntRect textureRect;
	const sf::Texture* palette = nullptr;

	auto viewCenter = target.getView().getCenter();
	auto viewSize = target.getView().getSize();

	sf::FloatRect drawRect(viewCenter.x - (viewSize.x / 2) - tileWidth,
		viewCenter.y - (viewSize.y / 2) - pillarHeight,
		viewSize.x + tileWidth, viewSize.y + pillarHeight);

	auto tiles = tilesBottom.get();
	if (tiles != nullptr)
	{
		if (tiles->isIndexed() == true &&
			tiles->getPalette() != nullptr &&
			Shaders::supportsPalettes() == true)
		{
			palette = &tiles->getPalette()->texture;
		}
		if (palette != nullptr)
		{
			states.shader = &Shaders::Palette;
			Shaders::Palette.setUniform("palette", *palette);
		}
		auto newPillarHeight = pillarHeight - tileHeight;

		for (Coord x = 0; x < map.Width(); x++)
		{
			for (Coord y = 0; y < map.Height(); y++)
			{
				auto index = map[x][y].MinIndex();
				if (index < 0)
				{
					continue;
				}
				auto coords = map.getCoord(MapCoord(x, y));
				coords.y += newPillarHeight;

				if (drawRect.contains(coords) == true)
				{
					if (tiles->get((size_t)index, &texture, textureRect) == true)
					{
						sprite.setTexture(*texture);
						sprite.setTextureRect(textureRect);
						sprite.setPosition(coords);
						target.draw(sprite, states);
					}
				}
			}
		}
	}

	palette = nullptr;
	tiles = tilesTop.get();

	if (tiles != nullptr &&
		tiles->isIndexed() == true &&
		tiles->getPalette() != nullptr &&
		Shaders::supportsPalettes() == true)
	{
		palette = &tiles->getPalette()->texture;
	}
	if (palette != nullptr)
	{
		states.shader = &Shaders::Palette;
		Shaders::Palette.setUniform("palette", *palette);
	}
	else
	{
		states.shader = NULL;
	}

	for (Coord x = 0; x < map.Width(); x++)
	{
		for (Coord y = 0; y < map.Height(); y++)
		{
			auto coords = map.getCoord(MapCoord(x, y));

			if (drawRect.contains(coords) == true)
			{
				bool drewObj = false;
				for (const auto& drawObj : map[x][y])
				{
					if (drawObj != nullptr)
					{
						target.draw(*drawObj, states);
						drewObj = true;
					}
				}
				if (drewObj == true &&
					palette != nullptr)
				{
					Shaders::Palette.setUniform("palette", *palette);
				}
				if (tiles != nullptr)
				{
					size_t index = map[x][y].MinIndex();
					if (index < 0)
					{
						continue;
					}
					if (tiles->get((size_t)index, &texture, textureRect) == true)
					{
						sprite.setTexture(*texture);
						sprite.setTextureRect(textureRect);
						sprite.setPosition(coords);
						target.draw(sprite, states);
					}
				}
			}
		}
	}
	target.setView(origView);
}

void Level::updateSize(const Game& game)
{
	view.updateSize(game);
}

void Level::updateMouse(const Game& game)
{
	mousePositionf = view.getPosition(game.MousePositionf());
	mapCoordOverMouse = map.getTile(mousePositionf);
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
	if (updateView == true)
	{
		updateView = false;
		view.updateSize(game);
		view.updateViewport(game);
	}
	if (pause == true)
	{
		return;
	}

	updateZoom(game);
	updateMouse(game);

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
		if (captureInputEvents == true)
		{
			game.clearInputEvents();
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

	for (auto& player : players)
	{
		player->update(game, *this);
	}

	if (followCurrentPlayer == true && currentPlayer != nullptr)
	{
		currentMapPosition = currentPlayer->MapPosition();
		view.setCenter(currentPlayer->getBasePosition(*this));
	}
}

bool Level::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first.c_str());
	switch (propHash)
	{
	case str2int16("clickedObject"):
	{
		if (clickedObject != nullptr)
		{
			return clickedObject->getProperty(props.second, var);
		}
	}
	case str2int16("currentPlayer"):
	{
		if (currentPlayer != nullptr)
		{
			return currentPlayer->getProperty(props.second, var);
		}
	}
	break;
	case str2int16("hoverObject"):
	{
		if (hoverObject != nullptr)
		{
			return hoverObject->getProperty(props.second, var);
		}
	}
	break;
	case str2int16("name"):
		var = Variable(name);
		return true;
		break;
	case str2int16("player"):
	{
		auto props2 = Utils::splitStringIn2(props.second, '.');
		for (const auto& player : players)
		{
			if (player->Id() == props2.first)
			{
				return player->getProperty(props2.second, var);
			}
		}
	}
	break;
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
	}
	case str2int16("zoom"):
		var = Variable((double)stopZoomFactor);
		return true;
		break;
	case str2int16("zoomPercentage"):
		var = Variable((int64_t)(std::roundf(stopZoomFactor * 100.f)));
		return true;
		break;
	default:
		return GameUtils::getUIObjProp(*this, propHash, props.second, var);
	}
	return false;
}

const Queryable* Level::getQueryable(const std::string& prop) const
{
	if (prop.empty() == true)
	{
		return this;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first.c_str());
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
		for (const auto& player : players)
		{
			if (player->Id() == props.first)
			{
				queryable = player.get();
				break;
			}
		}
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

Player* Level::getPlayer(const std::string& id) const
{
	for (auto& player : players)
	{
		if (player->Id() == id)
		{
			return player.get();
		}
	}
	return nullptr;
}

Player* Level::getPlayerOrCurrent(const std::string& id) const
{
	if (id.empty() == true)
	{
		return currentPlayer;
	}
	return getPlayer(id);
}

void Level::clearPlayerClasses(size_t clearIdx)
{
	if (clearIdx < playerClasses.size())
	{
		auto removeIfFunc = [&](const auto& cls) {
			auto it = std::find_if(players.begin(), players.end(), [&](const auto& plr) {
				return plr->getPlayerClass() == cls.second.get();
			});
			return it == players.end();
		};

		playerClasses.erase(
			std::remove_if(playerClasses.begin() + clearIdx,
				playerClasses.end(),
				removeIfFunc),
			playerClasses.end());
	}
}

void Level::clearPlayerTextures()
{
	for (const auto& playerClass : playerClasses)
	{
		auto plrClassPtr = playerClass.second.get();
		bool classIsUsed = false;
		for (const auto& player : players)
		{
			if (plrClassPtr == player->getPlayerClass())
			{
				classIsUsed = true;
				break;
			}
		}
		if (classIsUsed == false)
		{
			plrClassPtr->clearTextures();
		}
	}
}

void Level::clearPlayers(size_t clearIdx)
{
	if (clearIdx < players.size())
	{
		players.erase(players.begin() + clearIdx, players.end());
	}
	for (const auto& player : players)
	{
		if (player.get() == currentPlayer)
		{
			return;
		}
	}
	currentPlayer = nullptr;
}

std::shared_ptr<Item> Level::getItem(const MapCoord& mapCoord) const
{
	return map[mapCoord.x][mapCoord.y].getObject<Item>();
}

std::shared_ptr<Item> Level::getItem(const ItemCoordInventory& itemCoord) const
{
	Player* player;
	return getItem(itemCoord, player);
}

std::shared_ptr<Item> Level::getItem(const ItemCoordInventory& itemCoord, Player*& player) const
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

std::shared_ptr<Item> Level::getItem(const ItemLocation& location) const
{
	Player* player;
	return getItem(location, player);
}

std::shared_ptr<Item> Level::getItem(const ItemLocation& location, Player*& player) const
{
	if (std::holds_alternative<MapCoord>(location) == true)
	{
		player = nullptr;
		return getItem(std::get<MapCoord>(location));
	}
	else
	{
		return getItem(std::get<ItemCoordInventory>(location), player);
	}
}

bool Level::setItem(const MapCoord& mapCoord, const std::shared_ptr<Item>& item)
{
	auto& mapCell = map[mapCoord.x][mapCoord.y];
	auto oldItem = mapCell.getObject<Item>();
	if (item == nullptr)
	{
		if (oldItem != nullptr)
		{
			deleteLevelObject(oldItem.get());
			mapCell.deleteObject(oldItem.get());
		}
		return true;
	}
	if (mapCell.Passable() == true
		&& oldItem == nullptr)
	{
		item->MapPosition(mapCoord);
		item->updateDrawPosition(*this);
		mapCell.addFront(item);
		addLevelObject(item);
		return true;
	}
	return false;
}

bool Level::setItem(const ItemCoordInventory& itemCoord, const std::shared_ptr<Item>& item)
{
	auto player = getPlayerOrCurrent(itemCoord.getPlayerId());
	if (player != nullptr)
	{
		if (itemCoord.isSelectedItem() == true)
		{
			player->SelectedItem(item);
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

bool Level::setItem(const ItemLocation& location, const std::shared_ptr<Item>& item)
{
	if (std::holds_alternative<MapCoord>(location) == true)
	{
		return setItem(std::get<MapCoord>(location), item);
	}
	else
	{
		return setItem(std::get<ItemCoordInventory>(location), item);
	}
}

void Level::addQuest(const Quest& quest_)
{
	for (const auto& quest : quests)
	{
		if (quest.Id() == quest_.Id())
		{
			return;
		}
	}
	quests.push_back(quest_);
}

void Level::deleteQuest(const std::string& questId)
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

void Level::setQuestState(const std::string& questId, int state)
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

void Level::updateLevelObjectPositions()
{
	for (auto& obj : levelObjects)
	{
		const auto& mapPosition = obj->MapPosition();
		map[mapPosition.x][mapPosition.y].addBack(obj);
	}
	for (auto& obj : players)
	{
		const auto& mapPosition = obj->MapPosition();
		map[mapPosition.x][mapPosition.y].addBack(obj);
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
