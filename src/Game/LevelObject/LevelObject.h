#pragma once

#include "Game/BaseAnimation.h"
#include "Game/Variable.h"
#include "LevelObjectClass.h"
#include "LevelObjectQueryable.h"
#include <list>
#include <memory>
#include "SFML/CompositeSprite.h"
#include <string_view>
#include "Utils/PairXY.h"

class Game;
class Level;
class LevelMap;

class LevelObject : public LevelObjectQueryable
{
protected:
	const LevelObjectClass* class_{ nullptr };

	CompositeSprite sprite;
	sf::Vector2f basePosition;
	sf::Vector2f anchorPosition;
	bool absoluteOffset{ false };
	float tileBlockHeight{ 0.f };
	PairFloat mapPosition{ -1.f, -1.f };

	BaseAnimation animation;
	LightSource lightSource;
	PairInt8 cellSize;

	bool enableHover{ true };
	bool hovered{ false };
	bool outlineOnHover{ true };

	std::list<std::shared_ptr<Action>> queuedActions;

	std::string id;

	// gets common getProperty properties;
	bool getLevelObjProp(const uint16_t propHash16, const std::string_view prop, Variable& var) const;

	PairFloat getCenterMapPosition(const PairFloat& mapPos);

	void getMinMaxMapPosition(const PairFloat& mapPos, PairFloat& minMapPos, PairFloat& maxMapPos);

	bool hasValidState() const noexcept;
	bool getCurrentTexture(TextureInfo& ti) const;
	void updateDrawPosition(const LevelMap& map);
	void updateDrawPosition(const LevelMap& map, const PairFloat& mapPos);
	void updateSpriteDrawPosition();
	void updateHover(Game& game, Level& level, const std::shared_ptr<LevelObject>& thisPtr);
	bool updateMapPositionBack(LevelMap& map, const PairFloat pos);
	bool updateMapPositionFront(LevelMap& map, const PairFloat pos);

	void queueAction(const std::shared_ptr<Action>& action);
	void queueAction(const LevelObjectClass& class_, uint16_t nameHash16);
	void processQueuedActions(Game& game);

	// sets map position without updating the object or level
	void MapPosition(const PairFloat& pos) noexcept { mapPosition = pos; }

public:
	LevelObject(const LevelObjectClass* class__) : class_(class__) {}
	~LevelObject() override = default;

	auto& getBasePosition() const noexcept { return basePosition; }
	auto& getAnchorPosition() const noexcept { return anchorPosition; }
	bool updateTexture();

	auto& Position() const { return sprite.getPosition(); }
	auto Size() const { return sprite.getSize(); }
	auto& MapPosition() const noexcept { return mapPosition; }

	// removes this object from the map
	bool remove(LevelMap& map) const;

	// sets map position and updates the object and map
	virtual bool MapPosition(LevelMap& map, const PairFloat& pos);

	// sets map position and updates the object and map
	virtual bool move(LevelMap& map, const PairFloat& pos);

	// Game

	virtual bool Passable() const = 0;

	virtual const std::string_view getType() const = 0;

	auto getDefaultLight() const noexcept { return class_->getLightSource().light; }
	auto getDefaultLightSource() const noexcept { return class_->getLightSource(); }

	auto getLight() const noexcept { return lightSource.light; }
	auto getLightRadius() const noexcept { return lightSource.radius; }
	auto getLightSource() const noexcept { return lightSource; }

	void setLight(uint8_t light) noexcept { lightSource.light = light; }
	void setLightRadius(uint8_t radius) noexcept { lightSource.radius = radius; }
	void setLightSource(LightSource lightSource_) noexcept { lightSource = lightSource_; }

	void executeAction(Game& game) const;

	bool hasPalette() const noexcept { return sprite.hasPalette(); }
	void setPalette(const std::shared_ptr<Palette>& palette) noexcept { sprite.setPalette(palette); }

	bool Hoverable() const noexcept { return enableHover; }
	void Hoverable(bool hoverable) noexcept { enableHover = hoverable; }

	void setColor(const sf::Color& color) { sprite.setColor(color); }
	void setOutline(const sf::Color& outline, const sf::Color& ignore) noexcept { sprite.setOutline(outline, ignore); }
	void setOutlineOnHover(bool outlineOnHover_) noexcept { outlineOnHover = outlineOnHover_; }

	auto getBaseClass() const { return class_; }

	auto getCellSize() const noexcept { return cellSize; }
	void setCellSize(PairInt8 cellSize_) noexcept { cellSize = cellSize_; }

	auto& getId() const { return id; }
	auto& getClassId() const { return class_->Id(); }

	void draw(sf::RenderTarget& target, GameShader* spriteShader, SpriteShaderCache& cache) const
	{
		sprite.draw(target, spriteShader, cache);
	}

	virtual void update(Game& game, Level& level, const std::shared_ptr<LevelObject>& thisPtr) = 0;

	bool getTexture(uint32_t textureNumber, TextureInfo& ti) const override;
};
