#pragma once

#include "BaseAnimation.h"
#include "LevelObjectClass.h"
#include <list>
#include <memory>
#include "PairXY.h"
#include "Queryable.h"
#include "Save/SaveProperties.h"
#include "SFML/CompositeSprite.h"
#include <string_view>
#include "Variable.h"

class Game;
class Level;
class LevelMap;

class LevelObject : public Queryable
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
	bool getLevelObjProp(const uint16_t propHash16,
		const std::string_view prop, Variable& var) const;

	PairFloat getCenterMapPosition(const PairFloat& mapPos);

	void getMinMaxMapPosition(const PairFloat& mapPos,
		PairFloat& minMapPos, PairFloat& maxMapPos);

	bool hasValidState() const noexcept;
	bool getCurrentTexture(TextureInfo& ti) const;
	void updateDrawPosition(const LevelMap& map);
	void updateDrawPosition(const LevelMap& map, const PairFloat& mapPos);
	void updateSpriteDrawPosition();
	void updateHover(Game& game, Level& level, std::weak_ptr<LevelObject> thisPtr);
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

	const sf::Vector2f& getBasePosition() const noexcept { return basePosition; }
	const sf::Vector2f& getAnchorPosition() const noexcept { return anchorPosition; }
	bool updateTexture();

	const sf::Vector2f& Position() const { return sprite.getPosition(); }
	sf::Vector2f Size() const { return sprite.getSize(); }
	const PairFloat& MapPosition() const noexcept { return mapPosition; }

	// sets map position and updates the object and map
	virtual bool MapPosition(LevelMap& map, const PairFloat& pos);
	bool MapPosition(Level& level, const PairFloat& pos);
	// sets map position and updates the object and map
	virtual bool move(LevelMap& map, const PairFloat& pos);
	bool move(Level& level, const PairFloat& pos);
	// removes this object from the map
	bool remove(LevelMap& map) const;

	// Game

	uint8_t getDefaultLight() const noexcept { return class_->getLightSource().light; }
	LightSource getDefaultLightSource() const noexcept { return class_->getLightSource(); }

	uint8_t getLight() const noexcept { return lightSource.light; }
	LightSource getLightSource() const noexcept { return lightSource; }

	void executeAction(Game& game) const
	{
		class_->executeAction(game, str2int16("action"));
	}

	virtual bool Passable() const = 0;

	bool hasPalette() const noexcept { return sprite.hasPalette(); }
	void setPalette(const std::shared_ptr<Palette>& palette) noexcept { sprite.setPalette(palette); }

	bool Hoverable() const noexcept { return enableHover; }
	void Hoverable(bool hoverable) noexcept { enableHover = hoverable; }

	void setColor(const sf::Color& color) { sprite.setColor(color); }
	void setOutline(const sf::Color& outline, const sf::Color& ignore) noexcept
	{
		sprite.setOutline(outline, ignore);
	}
	void setOutlineOnHover(bool outlineOnHover_) noexcept { outlineOnHover = outlineOnHover_; }

	const LevelObjectClass* getBaseClass() const { return class_; }

	PairInt8 getCellSize() const noexcept { return cellSize; }
	void setCellSize(PairInt8 cellSize_) noexcept { cellSize = cellSize_; }

	const std::string& getId() const { return id; }
	const std::string& getClassId() const { return class_->Id(); }
	virtual const std::string_view getType() const = 0;

	// serialize this object.
	// serializeObj - currently is a RapidJson writer class
	virtual void serialize(void* serializeObj, Save::Properties& props,
		const Game& game, const Level& level) const = 0;

	void draw(sf::RenderTarget& target, sf::Shader* spriteShader,
		SpriteShaderCache& cache) const
	{
		sprite.draw(target, spriteShader, cache);
	}

	// Update
	virtual void update(Game& game, Level& level, std::weak_ptr<LevelObject> thisPtr) = 0;

	virtual void setProperty(const std::string_view prop, const Variable& val) = 0;
};
