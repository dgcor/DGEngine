#include "TilesetLevelLayer.h"
#include "Level.h"
#include "LevelSurface.h"
#include "Player.h"
#include "SFML/VertexArray2.h"

void TilesetLevelLayer::updateVisibleArea(const LevelSurface& surface, const LevelMap& map)
{
	// subtract 1 tile to adjust for coordinate conversion
	// add max pillar height to bottom corners to avoid clipping pillars

	constexpr static auto maxPillarHeight = 256.f;

	sf::Vector2f TL{
		surface.visibleRect.left - (float)(surface.tileWidth * surface.subTiles),
		surface.visibleRect.top - (float)(surface.tileHeight * surface.subTiles)
	};
	sf::Vector2f TR{
		TL.x + surface.visibleRect.width + (float)(surface.tileWidth * surface.subTiles),
		TL.y
	};
	sf::Vector2f BL{
		TL.x,
		TL.y + surface.visibleRect.height + maxPillarHeight + (float)(surface.tileHeight * surface.subTiles)
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

static void addTile(std::vector<sf::Vertex>& vertices,
	float curX, float curY, const sf::IntRect& textureRect)
{
	auto vertIdx = vertices.size();
	vertices.resize(vertices.size() + 6);

	// triangle 1

	// top left
	vertices[vertIdx].position.x = curX;
	vertices[vertIdx].position.y = curY;
	vertices[vertIdx].texCoords.x = (float)textureRect.left;
	vertices[vertIdx].texCoords.y = (float)textureRect.top;
	vertIdx++;

	// top right
	vertices[vertIdx].position.x = curX + (float)textureRect.width;
	vertices[vertIdx].position.y = curY;
	vertices[vertIdx].texCoords.x = (float)textureRect.left + (float)textureRect.width;
	vertices[vertIdx].texCoords.y = (float)textureRect.top;
	vertIdx++;

	// bottom left
	vertices[vertIdx].position.x = curX;
	vertices[vertIdx].position.y = curY + (float)textureRect.height;
	vertices[vertIdx].texCoords.x = (float)textureRect.left;
	vertices[vertIdx].texCoords.y = (float)textureRect.top + (float)textureRect.height;
	vertIdx++;

	// triangle 2

	// top right
	vertices[vertIdx] = vertices[vertIdx - 2];
	vertIdx++;

	// bottom left
	vertices[vertIdx] = vertices[vertIdx - 2];
	vertIdx++;

	// bottom right
	vertices[vertIdx].position.x = curX + (float)textureRect.width;
	vertices[vertIdx].position.y = curY + (float)textureRect.height;
	vertices[vertIdx].texCoords.x = (float)textureRect.left + (float)textureRect.width;
	vertices[vertIdx].texCoords.y = (float)textureRect.top + (float)textureRect.height;
}

void TilesetLevelLayer::draw(const LevelSurface& surface,
	SpriteShaderCache& spriteCache, GameShader* spriteShader,
	const Level& level, bool drawLevelObjects, bool isAutomap) const
{
	VertexArray2 vertexLayer;
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

	const sf::Texture* tilesetTexture = nullptr;
	if (tiles != nullptr &&
		drawLevelObjects == false)
	{
		tilesetTexture = tiles->getTexture();
	}
	if (tilesetTexture != nullptr)
	{
		auto reserve = (visibleEnd.x - visibleStart.x) * (visibleEnd.y - visibleStart.y) * 6;
		vertexLayer.vertices.reserve(reserve);
	}

	const auto& map = level.Map();
	PairInt32 mapPos;
	for (mapPos.x = visibleStart.x; mapPos.x < visibleEnd.x; mapPos.x++)
	{
		for (mapPos.y = visibleStart.y; mapPos.y < visibleEnd.y; mapPos.y++)
		{
			int32_t index;
			if (map.isMapCoordValid(mapPos) == false)
			{
				index = outOfBoundsTile.getTileIndex(mapPos.x, mapPos.y);
			}
			else
			{
				index = map[mapPos].getTileIndex(layerIdx);

				if (drawLevelObjects == true)
				{
					for (const auto& drawObj : map[mapPos])
					{
						if (drawObj != nullptr)
						{
							surface.draw(*drawObj, spriteShader, spriteCache);
						}
					}
					if (tiles == nullptr ||
						surface.visible == false)
					{
						continue;
					}
				}
			}
			while (index >= 0 && tiles->get((uint32_t)index, ti) == true)
			{
				auto drawPos = map.toDrawCoord(mapPos, surface.blockWidth, surface.blockHeight);
				drawPos += ti.offset;
				tileRect.left = drawPos.x;
				tileRect.top = drawPos.y;
				tileRect.width = (float)ti.textureRect.width;
				tileRect.height = (float)ti.textureRect.height;
				if (surface.visibleRect.intersects(tileRect) == true)
				{
					if (tilesetTexture == nullptr)
					{
						sprite.setPosition(drawPos);
						sprite.setTexture(ti, true);
						surface.draw(sprite, spriteShader, spriteCache);
					}
					else
					{
						addTile(vertexLayer.vertices, drawPos.x, drawPos.y, ti.textureRect);
					}
				}
				index = ti.nextIndex;
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
		auto direction = (uint32_t)level.getCurrentPlayer()->getDirection();
		auto index = (uint32_t)level.getAutomapPlayerDirectionBaseIndex() + direction;
		if (direction < (uint32_t)PlayerDirection::All &&
			tiles->get(index, ti) == true)
		{
			auto drawPos = level.getCurrentAutomapViewCenter() + ti.offset;
			drawPos.x -= (float)surface.blockWidth;
			drawPos.y -= (float)surface.blockHeight;
			tileRect.left = drawPos.x;
			tileRect.top = drawPos.y;
			tileRect.width = (float)ti.textureRect.width;
			tileRect.height = (float)ti.textureRect.height;
			if (surface.visibleRect.intersects(tileRect) == true)
			{
				if (tilesetTexture == nullptr)
				{
					sprite.setPosition(drawPos);
					sprite.setTexture(ti, true);
					surface.draw(sprite, spriteShader, spriteCache);
				}
				else
				{
					addTile(vertexLayer.vertices, drawPos.x, drawPos.y, ti.textureRect);
				}
			}
		}
	}

	if (tilesetTexture != nullptr)
	{
		surface.draw(vertexLayer, tilesetTexture, tiles->getPalette().get(), spriteShader);
	}
}
