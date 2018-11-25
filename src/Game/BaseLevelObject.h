#pragma once

#include "BaseAnimation.h"
#include "BaseClassActions.h"
#include "LevelObject.h"
#include <list>
#include "SFML/Sprite2.h"
#include "TexturePacks/TexturePack.h"

struct BaseLevelObject
{
public:
	Sprite2 sprite;
	sf::Vector2f originalPosition;
	sf::Vector2f basePosition;
	sf::Vector2f offset;
	MapCoord mapPosition;

	const TexturePack* texturePack{ nullptr };
	BaseAnimation animation;

	uint8_t hoverCellSize{ 0 };

	bool enableHover{ true };
	bool hovered{ false };
	bool outlineOnHover{ true };

	std::list<std::shared_ptr<Action>> queuedActions;

	sf::Vector2f getSize() const
	{
		return sf::Vector2f((float)sprite.getTextureRect().width, (float)sprite.getTextureRect().height);
	}

	bool hasValidState() const noexcept;
	bool getTexture(TextureInfo& ti) const;
	void updateDrawPosition(const Level& level);
	void updateDrawPosition(const Level& level, const sf::Vector2f& drawPos);
	void updateHover(Game& game, Level& level, LevelObject* levelObj);
	void updateMapPositionBack(Level& level, const MapCoord pos, LevelObject* levelObj);
	void updateMapPositionFront(Level& level, const MapCoord pos, LevelObject* levelObj);
	bool updateTexture();

	void queueAction(const std::shared_ptr<Action>& action);
	void queueAction(const BaseClassActions& class_, uint16_t nameHash16);
	void processQueuedActions(Game& game);

	BaseLevelObject() noexcept {}
	BaseLevelObject(const sf::Texture& texture) : sprite(texture) {}
	BaseLevelObject(const TexturePack& texturePack_,
		const std::pair<size_t, size_t>& textureIndexRange,
		const sf::Time& frameTime, AnimationType type)
		: texturePack(&texturePack_), animation(textureIndexRange, frameTime, type, false) {}
};
