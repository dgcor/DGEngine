#pragma once

#include "BaseClass.h"
#include "LevelObject.h"
#include <list>
#include "Sprite2.h"
#include "TexturePacks/TexturePack.h"

struct BaseLevelObject
{
private:
	std::shared_ptr<LevelObject> updateMapPosition(
		Level& level, const MapCoord& pos, LevelObject* levelObj);

public:
	Sprite2 sprite;
	MapCoord mapPosition;

	const TexturePack* texturePack{ nullptr };
	size_t textureStartIdx{ 0 };
	size_t textureEndIdx{ 0 };
	size_t currentTextureIdx{ 0 };
	size_t texturePackIdx{ 0 };

	uint8_t hoverCellSize{ 0 };

	bool enableHover{ true };
	bool hovered{ false };
	bool outlineOnHover{ true };

	std::list<std::shared_ptr<Action>> queuedActions;

	sf::Vector2f getSize() const
	{
		return sf::Vector2f((float)sprite.getTextureRect().width, (float)sprite.getTextureRect().height);
	}

	void checkAndUpdateTextureIndex();
	bool hasValidState() const;
	sf::Vector2f getBasePosition(const Level& level) const;
	void updateDrawPosition(const Level& level);
	void updateDrawPosition(const Level& level, sf::Vector2f drawPos);
	void updateHover(Game& game, Level& level, LevelObject* levelObj);
	void updateMapPositionBack(Level& level, const MapCoord& pos, LevelObject* levelObj);
	void updateMapPositionFront(Level& level, const MapCoord& pos, LevelObject* levelObj);
	bool updateTexture();

	void queueAction(const std::shared_ptr<Action>& action);
	void queueAction(const BaseClass& class_, uint16_t nameHash16);
	void processQueuedActions(Game& game);

	BaseLevelObject() {}
	BaseLevelObject(const sf::Texture& texture) : sprite(texture) {}
	BaseLevelObject(const TexturePack& texturePack_,
		const std::pair<size_t, size_t>& frameRange_) : texturePack(&texturePack_),
		textureStartIdx(frameRange_.first), textureEndIdx(frameRange_.second) {}
};
