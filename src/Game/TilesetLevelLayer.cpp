#include "TilesetLevelLayer.h"
#include "Level.h"
#include "LevelSurface.h"
#include "Player.h"

void TilesetLevelLayer::updateVisibleArea(const LevelSurface& surface, const LevelMap& map)
{
	// subtract 1 tile to adjust for coordinate conversion
	// add max pillar height to bottom corners to avoid clipping pillars

	constexpr static auto maxPillarHeight = 256.f;

	sf::Vector2f TL{
		surface.visibleRect.left - (float)surface.tileWidth,
		surface.visibleRect.top - (float)surface.tileHeight
	};
	sf::Vector2f TR{ TL.x + surface.visibleRect.width + surface.tileWidth, TL.y };
	sf::Vector2f BL{
		TL.x,
		TL.y + surface.visibleRect.height + maxPillarHeight + surface.tileHeight
	};
	sf::Vector2f BR{ TR.x, BL.y };

	auto mapTL = map.toMapCoord(TL, surface.blockWidth, surface.blockHeight);
	auto mapTR = map.toMapCoord(TR, surface.blockWidth, surface.blockHeight);
	auto mapBL = map.toMapCoord(BL, surface.blockWidth, surface.blockHeight);
	auto mapBR = map.toMapCoord(BR, surface.blockWidth, surface.blockHeight);

	visibleStart.x = (int32_t)mapTL.x;
	visibleEnd.x = (int32_t)mapBR.x;
	visibleStart.y = (int32_t)mapTR.y;
	visibleEnd.y = (int32_t)mapBL.y;
}

void TilesetLevelLayer::draw(const LevelSurface& surface,
	SpriteShaderCache& spriteCache, sf::Shader* spriteShader,
	const Level& level, bool drawLevelObjects, bool isAutomap) const
{
	Sprite2 sprite;
	TextureInfo ti;
	sf::FloatRect tileRect;

	if (surface.visible == false ||
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
							surface.draw(*drawObj, spriteShader, spriteCache, objLight);
						}
					}
					if (tiles == nullptr ||
						surface.visible == false)
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
				auto drawPos = map.toDrawCoord(mapPos, surface.blockWidth, surface.blockHeight);
				drawPos += ti.offset;
				tileRect.left = drawPos.x;
				tileRect.top = drawPos.y;
				tileRect.width = (float)ti.textureRect.width;
				tileRect.height = (float)ti.textureRect.height;
				if (surface.visibleRect.intersects(tileRect) == true)
				{
					sprite.setTexture(ti, true);
					sprite.setPosition(drawPos);
					surface.draw(sprite, spriteShader, spriteCache, light);
				}
			}
		}
	}

	// draw player direction in automap, if enabled (baseIndex >= 0)
	if (isAutomap == true &&
		surface.visible == true &&
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
			drawPos.x -= (float)surface.blockWidth;
			drawPos.y -= (float)surface.blockHeight;
			drawPos += ti.offset;
			tileRect.left = drawPos.x;
			tileRect.top = drawPos.y;
			tileRect.width = (float)ti.textureRect.width;
			tileRect.height = (float)ti.textureRect.height;
			if (surface.visibleRect.intersects(tileRect) == true)
			{
				sprite.setTexture(ti, true);
				sprite.setPosition(drawPos);
				surface.draw(sprite, spriteShader, spriteCache);
			}
		}
	}
}
