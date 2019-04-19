#include "LevelMap.h"
#include "PathFinder.h"
#include "Utils/EasingFunctions.h"

LevelMap::LevelMap(const std::string_view tilFileName, const std::string_view solFileName,
	int32_t width_, int32_t height_, int16_t defaultTile)
	: tileSet(tilFileName), sol(solFileName)
{
	resize(width_, height_);
	clear(defaultTile);
}

LevelMap::LevelMap(int32_t width_, int32_t height_, int16_t defaultTile)
{
	resize(width_, height_);
	clear(defaultTile);
}

void LevelMap::resize(int32_t width_, int32_t height_)
{
	mapSizei.x = std::clamp(width_, 0, (int32_t)std::numeric_limits<uint16_t>::max());
	mapSizei.y = std::clamp(height_, 0, (int32_t)std::numeric_limits<uint16_t>::max());
	mapSizef.x = (float)mapSizei.x;
	mapSizef.y = (float)mapSizei.y;

	cells.resize(mapSizei.x * mapSizei.y, {});
}

void LevelMap::clear(int16_t defaultTile)
{
	if (defaultTile < 0)
	{
		cells.assign(cells.size(), {});
	}
	else
	{
		if ((size_t)defaultTile < tileSet.size())
		{
			cells.assign(cells.size(), {});

			const auto& defaultTileBlock = tileSet[defaultTile];
			for (int32_t j = 0; j < mapSizei.y; j++)
			{
				for (int32_t i = 0; i < mapSizei.x; i++)
				{
					auto tileIdx = defaultTileBlock.getTileIndex(i, j);
					(*this)[i][j].setTileIndex(0, tileIdx);
					(*this)[i][j].setTileIndex(LevelCell::SolLayer, sol.get(tileIdx));
				}
			}
		}
		else
		{
			cells.assign(cells.size(), { defaultTile });
		}
	}
};

void LevelMap::setDefaultTileSize(int32_t tileWidth_, int32_t tileHeight_) noexcept
{
	defaultTileWidth = tileWidth_;
	defaultTileHeight = tileHeight_;
	defaultBlockWidth = std::max(1, tileWidth_ / 2);
	defaultBlockHeight = std::max(1, tileHeight_ / 2);
}

uint8_t LevelMap::getTileLight(size_t layer, const LevelCell& cell) const
{
	auto tileIndex = cell.getTileIndex(layer);
	if (tileIndex < 0)
	{
		return 0;
	}
	return lightMap.get((size_t)tileIndex);
}

void LevelMap::addLight(PairInt32 lightPos, LightSource lightSource)
{
	if (lightSource.maxLight == 0 ||
		lightSource.minLight > lightSource.maxLight)
	{
		return;
	}
	pendingLights.push_back({ lightPos, lightSource, false });
}

void LevelMap::addLight(PairFloat lightPos, LightSource lightSource)
{
	addLight(PairInt32((int32_t)lightPos.x, (int32_t)lightPos.y), lightSource);
}

void LevelMap::removeLight(PairInt32 lightPos, LightSource lightSource)
{
	if (lightSource.maxLight == 0 ||
		lightSource.minLight > lightSource.maxLight)
	{
		return;
	}
	pendingLights.push_back({ lightPos, lightSource, true });
}

void LevelMap::removeLight(PairFloat lightPos, LightSource lightSource)
{
	removeLight(PairInt32((int32_t)lightPos.x, (int32_t)lightPos.y), lightSource);
}

void LevelMap::doLight(PairInt32 lightPos, LightSource ls,
	const std::function<void(LevelCell*, uint8_t)>& doLightFunc)
{
	if (defaultSource.maxLight == 255 ||
		ls.maxLight == 0 ||
		ls.minLight > ls.maxLight)
	{
		return;
	}

	// limit real radius
	int32_t radius = std::min(128, (int32_t)ls.radius);
	int32_t radiusSquared = radius * radius;
	double range = ((double)ls.maxLight - (double)ls.minLight);

	PairInt32 mapPosStart(lightPos.x - radius, lightPos.y - radius);
	PairInt32 mapPosEnd(lightPos.x + radius + 1, lightPos.y + radius + 1);

	mapPosStart.x = std::max(mapPosStart.x, 0);
	mapPosStart.y = std::max(mapPosStart.y, 0);
	mapPosEnd.x = std::min(mapPosEnd.x, mapSizei.x);
	mapPosEnd.y = std::min(mapPosEnd.y, mapSizei.y);

	auto easingFunc = EasingFunctions::easeLinear<double>;

	switch (ls.easing)
	{
	default:
	case LightEasing::Linear:
		easingFunc = EasingFunctions::easeLinear<double>;
		break;
	case LightEasing::Sine:
		easingFunc = EasingFunctions::easeInSine<double>;
		break;
	case LightEasing::Quad:
		easingFunc = EasingFunctions::easeInQuad<double>;
		break;
	case LightEasing::Cubic:
		easingFunc = EasingFunctions::easeInCubic<double>;
		break;
	case LightEasing::Quart:
		easingFunc = EasingFunctions::easeInQuart<double>;
		break;
	case LightEasing::Quint:
		easingFunc = EasingFunctions::easeInQuint<double>;
		break;
	case LightEasing::Expo:
		easingFunc = EasingFunctions::easeInExpo<double>;
		break;
	case LightEasing::Circ:
		easingFunc = EasingFunctions::easeInCirc<double>;
		break;
	}

	PairInt32 mapPos;

	for (mapPos.y = mapPosStart.y; mapPos.y < mapPosEnd.y; mapPos.y++)
	{
		for (mapPos.x = mapPosStart.x; mapPos.x < mapPosEnd.x; mapPos.x++)
		{
			auto dist_y = mapPos.y - lightPos.y;
			auto dist_x = mapPos.x - lightPos.x;
			auto xxyy = (dist_x * dist_x) + (dist_y * dist_y);
			if (xxyy <= radiusSquared)
			{
				double distance = std::sqrt(xxyy);
				auto easedLight = easingFunc(
					distance,
					(double)ls.maxLight,
					-range,
					(double)ls.radius);
				auto light = (uint8_t)std::round(easedLight);
				doLightFunc(&(*this)[mapPos], light);
			}
		}
	}
}

void LevelMap::doDefaultLight(PairInt32 lightPos, LightSource lightSource)
{
	static std::function<void(LevelCell*, uint8_t)> func = &LevelCell::setDefaultLight;
	doLight(lightPos, lightSource, func);
}

void LevelMap::doLight(PairInt32 lightPos, LightSource lightSource)
{
	static std::function<void(LevelCell*, uint8_t)> func = &LevelCell::addLight;
	doLight(lightPos, lightSource, func);
}

void LevelMap::undoLight(PairInt32 lightPos, LightSource lightSource)
{
	static std::function<void(LevelCell*, uint8_t)> func = &LevelCell::subtractLight;
	doLight(lightPos, lightSource, func);
}

void LevelMap::initLights()
{
	for (auto& cell : cells)
	{
		cell.clearLights(defaultSource.maxLight);
	}
	if (defaultSource.maxLight == 255)
	{
		return;
	}
	LightSource ls = defaultSource;
	for (int j = 0; j < mapSizei.y; j++)
	{
		for (int i = 0; i < mapSizei.x; i++)
		{
			auto& cell = (*this)[i][j];
			ls.maxLight = lightMap.get(cell.getTileIndex(0));
			doDefaultLight({i, j}, ls);
			for (auto& obj : cell)
			{
				auto lightSource = obj->getLightSource();
				addLight(obj->MapPosition(), lightSource);
			}
		}
	}
}

void LevelMap::updateLights()
{
	if (pendingLights.empty() == true)
	{
		return;
	}
	for (auto& l : pendingLights)
	{
		if (l.remove == false)
		{
			doLight(l.mapPos, l.lightSource);
		}
		else
		{
			undoLight(l.mapPos, l.lightSource);
		}
	}
	pendingLights.clear();
}

void LevelMap::setTileSetAreaUseSol(int32_t x, int32_t y, const Dun& dun)
{
	auto dWidth = dun.Width() * 2;
	auto dHeight = dun.Height() * 2;
	for (size_t j = 0; j < dHeight; j++)
	{
		for (size_t i = 0; i < dWidth; i++)
		{
			size_t xDunIndex = i;
			size_t xTilIndex = 0;
			if ((xDunIndex % 2) != 0)
			{
				xDunIndex--;
				xTilIndex = 1;
			}
			xDunIndex /= 2;

			size_t yDunIndex = j;
			size_t yTilIndex = 0;
			if ((yDunIndex % 2) != 0)
			{
				yDunIndex--;
				yTilIndex = 1;
			}
			yDunIndex /= 2;

			int32_t dunIndex = dun[xDunIndex][yDunIndex];
			if (dunIndex < 0 || (size_t)dunIndex >= tileSet.size())
			{
				continue;
			}

			int16_t tileIndex = 0;
			if (xTilIndex)
			{
				if (yTilIndex)
					tileIndex = tileSet[dunIndex].get<3>(); // bottom
				else
					tileIndex = tileSet[dunIndex].get<1>(); // left
			}
			else
			{
				if (yTilIndex)
					tileIndex = tileSet[dunIndex].get<2>(); // right
				else
					tileIndex = tileSet[dunIndex].get<0>(); // top
			}

			auto cellX = x + (int32_t)i;
			auto cellY = y + (int32_t)j;

			if (cellX < 0 || cellX >= mapSizei.x ||
				cellY < 0 || cellY >= mapSizei.y)
			{
				continue;
			}

			auto& cell = cells[cellX + (cellY * mapSizei.x)];

			cell.setTileIndex(0, tileIndex);
			cell.setTileIndex(LevelCell::SolLayer, sol.get(tileIndex));
		}
	}
}

void LevelMap::setSimpleAreaUseSol(size_t layer, int32_t x, int32_t y, const Dun& dun)
{
	for (size_t j = 0; j < dun.Height(); j++)
	{
		for (size_t i = 0; i < dun.Width(); i++)
		{
			auto cellX = x + (int32_t)i;
			auto cellY = y + (int32_t)j;

			if (cellX < 0 || cellX >= mapSizei.x ||
				cellY < 0 || cellY >= mapSizei.y)
			{
				continue;
			}

			auto& cell = cells[(size_t)(cellX + (cellY * mapSizei.x))];

			auto tileIndex = dun[i][j];
			cell.setTileIndex(layer, tileIndex);
			cell.setTileIndex(LevelCell::SolLayer, (tileIndex >= 0 ? sol.get(tileIndex) : 0));
		}
	}
}

void LevelMap::setSimpleArea(size_t layer, int32_t x, int32_t y,
	const Dun& dun, bool normalizeSolLayer)
{
	if (layer > LevelCell::NumberOfLayers)
	{
		return;
	}
	for (size_t j = 0; j < dun.Height(); j++)
	{
		for (size_t i = 0; i < dun.Width(); i++)
		{
			auto cellX = x + (int32_t)i;
			auto cellY = y + (int32_t)j;

			if (cellX < 0 || cellX >= mapSizei.x ||
				cellY < 0 || cellY >= mapSizei.y)
			{
				continue;
			}

			auto& cell = cells[(size_t)(cellX + (cellY * mapSizei.x))];

			auto tileIndex = dun[i][j];
			if (layer == LevelCell::SolLayer &&
				normalizeSolLayer == true)
			{
				tileIndex = (tileIndex != 0 ? 1 : 0);
			}
			cell.setTileIndex(layer, tileIndex);
		}
	}
}

bool LevelMap::isLayerUsed(size_t layer) const noexcept
{
	for (const auto& elem : cells)
	{
		if (elem.getTileIndex(layer) >= 0)
		{
			return true;
		}
	}
	return false;
}

bool LevelMap::isMapCoordValid(int32_t x, int32_t y) const noexcept
{
	return x >= 0 && x < mapSizei.x &&
		y >= 0 && y < mapSizei.y;
}

bool LevelMap::isMapCoordValid(const PairInt32& mapCoord) const noexcept
{
	return isMapCoordValid(mapCoord.x, mapCoord.y);
}

bool LevelMap::isMapCoordValid(float x, float y) const noexcept
{
	return x >= 0.f && x < mapSizef.x &&
		y >= 0.f && y < mapSizef.y;
}

bool LevelMap::isMapCoordValid(const PairFloat& mapCoord) const noexcept
{
	return isMapCoordValid(mapCoord.x, mapCoord.y);
}

PairFloat LevelMap::toMapCoord(const sf::Vector2f& drawCoord) const noexcept
{
	return toMapCoord(drawCoord, defaultBlockWidth, defaultBlockHeight);
}

PairFloat LevelMap::toMapCoord(const sf::Vector2f& drawCoord,
	int32_t blockWidth, int32_t blockHeight) const noexcept
{
	return PairFloat(
		std::round((drawCoord.x / blockWidth + drawCoord.y / blockHeight) / 2),
		std::round((drawCoord.y / blockHeight - (drawCoord.x / blockWidth)) / 2)
	);
}

TileBlock LevelMap::getTileBlock(int16_t index) const
{
	if (tileSet.empty() == true)
	{
		return { index };
	}
	else if (index >= 0 && (size_t)index < tileSet.size())
	{
		return tileSet[index];
	}
	return {};
}

bool LevelMap::addLevelObject(std::unique_ptr<LevelObject> obj)
{
	return obj->MapPosition(*this, obj->MapPosition());
}

bool LevelMap::removeLevelObject(const LevelObject* obj)
{
	return obj->remove(*this);
}

std::vector<PairFloat> LevelMap::getPath(const PairFloat& a, const PairFloat& b) const
{
	std::vector<PairFloat> path;

	if (a == b)
	{
		path.push_back(a);
		return path;
	}

	MapSearchNode start(*this, (int16_t)a.x, (int16_t)a.y, PlayerDirection::All);
	MapSearchNode end(*this, (int16_t)b.x, (int16_t)b.y, PlayerDirection::All);
	MapSearchNode endOrig(end);

	if (end.IsValid() == false)
	{
		return path;
	}
	if (end.IsPassable() == false)
	{
		if (((*this)[b]).hasObjects() == true)
		{
			if (start.GoalDistanceEstimateC(end) == 1.f ||
				getNearestPassableEndNode(*this, start, end) == false)
			{
				path.push_back(b);
				return path;
			}
			if ((*this)[end.x][end.y].PassableIgnoreObject() == false)
			{
				return path;
			}
		}
		else
		{
			return path;
		}
	}

	PathFinder pathFinder(this);
	pathFinder.SetStartAndGoalStates(start, end);

	unsigned int SearchState;
	do
	{
		SearchState = pathFinder.SearchStep();
		if (pathFinder.GetStepCount() == PathFinder::MaxNodes)
		{
			pathFinder.CancelSearch();
		}
	} while (SearchState == PathFinder::SEARCH_STATE_SEARCHING);

	if (SearchState == PathFinder::SEARCH_STATE_SUCCEEDED)
	{
		if (endOrig.IsPassable() == false)
		{
			path.push_back(PairFloat((float)endOrig.x, (float)endOrig.y));
		}
		auto node = pathFinder.GetSolutionEnd();
		while (true)
		{
			if (node == nullptr)
			{
				break;
			}
			path.push_back(PairFloat((float)node->x, (float)node->y));
			node = pathFinder.GetSolutionPrev();
		};
		pathFinder.FreeSolutionNodes();
	}
	pathFinder.EnsureMemoryFreed();

	return path;
}

std::string LevelMap::toCSV(bool zeroBasedIndex) const
{
	std::string str;
	int16_t inc = (zeroBasedIndex == true ? 0 : 1);

	for (int j = 0; j < mapSizei.y; j++)
	{
		for (int i = 0; i < mapSizei.x; i++)
		{
			str += Utils::toString((*this)[i][j].getTileIndex(0) + inc) + ",";
		}
		str += "\n";
	}
	str.pop_back();
	str.pop_back();
	return str;
}
