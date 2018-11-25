#pragma once

#include "Actions/Action.h"
#include "BaseLevelObject.h"
#include "LevelObjectClass.h"
#include <memory>
#include "Save/SaveSimpleLevelObject.h"
#include "Utils/FixedMap.h"

class SimpleLevelObject : public LevelObject
{
private:
	typedef FixedMap<uint16_t, LevelObjValue, 4> LevelObjectProperties;

	const LevelObjectClass* class_;

	BaseLevelObject base;

	std::shared_ptr<Action> action;

	std::string id;
	mutable std::string name;
	mutable std::array<std::string, 2> descriptions;

	std::string text1;
	std::string text2;

	mutable bool updateNameAndDescr{ true };

	LevelObjectProperties properties;

	void updateNameAndDescriptions() const;

	friend void Save::serialize(void* serializeObj, Save::Properties& props,
		const Game& game, const Level& level, const SimpleLevelObject& obj);

public:
	SimpleLevelObject(const LevelObjectClass* class__);

	virtual const sf::Vector2f& Position() const { return base.sprite.getPosition(); }
	virtual sf::Vector2f Size() const { return base.getSize(); }

	virtual const MapCoord& MapPosition() const noexcept { return base.mapPosition; }
	virtual void MapPosition(const MapCoord& pos) noexcept { base.mapPosition = pos; }

	virtual bool getTexture(size_t textureNumber, TextureInfo& ti) const;

	virtual void executeAction(Game& game) const;
	virtual bool getNumberProp(const std::string_view prop, Number32& value) const noexcept { return false; }
	virtual bool Passable() const noexcept { return true; }
	virtual void setColor(const sf::Color& color) { base.sprite.setColor(color); }
	virtual void setOutline(const sf::Color& outline, const sf::Color& ignore) noexcept
	{
		base.sprite.setOutline(outline, ignore);
	}
	virtual void setOutlineOnHover(bool outlineOnHover_) noexcept { base.outlineOnHover = outlineOnHover_; }
	virtual void setPalette(const std::shared_ptr<Palette>& palette) noexcept { base.sprite.setPalette(palette); }
	virtual bool hasPalette() const noexcept { return base.sprite.hasPalette(); }

	virtual bool Hoverable() const noexcept { return base.enableHover; }
	virtual void Hoverable(bool hoverable) noexcept { base.enableHover = hoverable; }

	virtual void serialize(void* serializeObj, Save::Properties& props,
		const Game& game, const Level& level) const
	{
		Save::serialize(serializeObj, props, game, level, *this);
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (base.sprite.getTexture() != nullptr)
		{
			target.draw(base.sprite, states);
		}
	}
	virtual void update(Game& game, Level& level);

	virtual bool getProperty(const std::string_view prop, Variable& var) const;
	virtual void setProperty(const std::string_view prop, const Variable& val) noexcept {}

	virtual std::string_view getId() const { return id; }
	virtual std::string_view getClassId() const { return class_->Id(); }

	bool hasIntByHash(uint16_t propHash) const noexcept;
	bool hasInt(const std::string_view prop) const noexcept;

	LevelObjValue getIntByHash(uint16_t propHash) const;
	LevelObjValue getInt(const std::string_view prop) const;

	bool getIntByHash(uint16_t propHash, LevelObjValue& value) const;
	bool getInt(const std::string_view prop, LevelObjValue& value) const;
	void setIntByHash(uint16_t propHash, LevelObjValue value);
	void setInt(const std::string_view prop, LevelObjValue value);

	sf::Time getFrameTime() const noexcept { return base.animation.frameTime; }
	void setFrameTime(sf::Time time) noexcept { base.animation.frameTime = time; }

	void setTextureRect(const sf::IntRect& rectangle) { base.sprite.setTextureRect(rectangle); }

	const std::string& Id() const noexcept { return id; }
	const std::string& Name() const;
	const std::string& SimpleName() const noexcept { return class_->Name(); }
	const std::string& Text1() const noexcept { return text1; }
	const std::string& Text2() const noexcept { return text2; }
	void Id(const std::string_view id_) { id = id_; }
	void Name(const std::string_view name_) { name = name_; }
	void Text1(const std::string_view text1_) { text1 = text1_; }
	void Text2(const std::string_view text2_) { text2 = text2_; }
};
