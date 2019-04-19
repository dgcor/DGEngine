#include "TilesetLevelLayer.h"
#include "Level.h"
#include "Player.h"

void TilesetLevelLayer::updateVisibleArea(const LevelLayerInfo& layerInfo, const LevelMap& map)
{
	// subtract 1 tile to adjust for coordinate conversion
	// add max pillar height to bottom corners to avoid clipping pillars

	constexpr static auto maxPillarHeight = 256.f;

	sf::Vector2f TL{
		layerInfo.visibleRect.left - (float)layerInfo.tileWidth,
		layerInfo.visibleRect.top - (float)layerInfo.tileHeight
	};
	sf::Vector2f TR{ TL.x + layerInfo.visibleRect.width + layerInfo.tileWidth, TL.y };
	sf::Vector2f BL{
		TL.x,
		TL.y + layerInfo.visibleRect.height + maxPillarHeight + layerInfo.tileHeight
	};
	sf::Vector2f BR{ TR.x, BL.y };

	auto mapTL = map.toMapCoord(TL, layerInfo.blockWidth, layerInfo.blockHeight);
	auto mapTR = map.toMapCoord(TR, layerInfo.blockWidth, layerInfo.blockHeight);
	auto mapBL = map.toMapCoord(BL, layerInfo.blockWidth, layerInfo.blockHeight);
	auto mapBR = map.toMapCoord(BR, layerInfo.blockWidth, layerInfo.blockHeight);

	visibleStart.x = (int32_t)mapTL.x;
	visibleEnd.x = (int32_t)mapBR.x;
	visibleStart.y = (int32_t)mapTR.y;
	visibleEnd.y = (int32_t)mapBL.y;
}

void TilesetLevelLayer::draw(sf::RenderTexture& levelTexture,
	const LevelLayerInfo& layerInfo, SpriteShaderCache& spriteCache,
	sf::Shader* spriteShader, const Level& level,
	bool drawLevelObjects, bool isAutomap) const
{
	Sprite2 sprite;
	TextureInfo ti;
	sf::FloatRect tileRect;

	if (layerInfo.visible == false ||
		tiles == nullptr)
	{
		if (drawLevelObjects == false)
		{
			return;
		}
	}
	const auto& map = level.Map();
	PairInt32 mapPos;
	for (mapPos.x = visibleStart.x; mapPos.x < visibleEnd.x; mapPos.x++)
	{
		for (mapPos.y = visibleStart.y; mapPos.y < visibleEnd.y; mapPos.y++)
		{
			uint8_t light = 255;
			int16_t index;
			if (map.isMapCoordValid(mapPos) == false)
			{
				if (isAutomap == false)
				{
					light = map.getDefaultLight();
				}
				index = outOfBoundsTile.getTileIndex(mapPos.x, mapPos.y);
			}
			else
			{
				if (isAutomap == false)
				{
					light = std::max(map[mapPos].getDefaultLight(), map[mapPos].getCurrentLight());
				}
				index = map[mapPos].getTileIndex(layerIdx);

				if (drawLevelObjects == true)
				{
					for (const auto& drawObj : map[mapPos])
					{
						if (drawObj != nullptr)
						{
							auto objLight = std::max(drawObj->getLight(), light);
							drawObj->draw(levelTexture, spriteShader, spriteCache, objLight);
						}
					}
					if (tiles == nullptr ||
						layerInfo.visible == false)
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
				auto drawPos = map.toDrawCoord(mapPos, layerInfo.blockWidth, layerInfo.blockHeight);
				drawPos += ti.offset;
				tileRect.left = drawPos.x;
				tileRect.top = drawPos.y;
				tileRect.width = (float)ti.textureRect.width;
				tileRect.height = (float)ti.textureRect.height;
				if (layerInfo.visibleRect.intersects(tileRect) == true)
				{
					sprite.setTexture(ti, true);
					sprite.setPosition(drawPos);
					sprite.draw(levelTexture, spriteShader, spriteCache, light);
				}
			}
		}
	}

	// draw player direction in automap, if enabled (baseIndex >= 0)
	if (isAutomap == true &&
		layerInfo.visible == true &&
		level.getAutomapPlayerDirectionBaseIndex() >= 0 &&
		level.getCurrentPlayer() != nullptr &&
		tiles != nullptr)
	{
		auto direction = (size_t)level.getCurrentPlayer()->getDirection();
		auto index = (size_t)level.getAutomapPlayerDirectionBaseIndex() + direction;
		if (direction < (size_t)PlayerDirection::All &&
			tiles->get(index, ti) == true)
		{
			auto drawPos = level.getCurrentAutomapViewCenter();
			drawPos.x -= (float)layerInfo.blockWidth;
			drawPos.y -= (float)layerInfo.blockHeight;
			drawPos += ti.offset;
			tileRect.left = drawPos.x;
			tileRect.top = drawPos.y;
			tileRect.width = (float)ti.textureRect.width;
			tileRect.height = (float)ti.textureRect.height;
			if (layerInfo.visibleRect.intersects(tileRect) == true)
			{
				sprite.setTexture(ti, true);
				sprite.setPosition(drawPos);
				sprite.draw(levelTexture, spriteShader, spriteCache);
			}
		}
	}
}
