#pragma once

#include "Actions/Action.h"
#include "LevelObject.h"
#include <memory>
#include "Save/SaveSimpleLevelObject.h"
#include "SimpleLevelObjectClass.h"
#include "Utils/FixedMap.h"

class SimpleLevelObject : public LevelObject
{
private:
	typedef FixedMap<uint16_t, LevelObjValue, 4> LevelObjectProperties;

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
	SimpleLevelObject(const SimpleLevelObjectClass* class__);

	constexpr const SimpleLevelObjectClass* Class() const noexcept
	{
		return (SimpleLevelObjectClass*)class_;
	}

	virtual bool getTexture(size_t textureNumber, TextureInfo& ti) const;

	virtual bool getNumberProp(const std::string_view prop, Number32& value) const noexcept
	{
		LevelObjValue val;
		bool ret = getInt(prop, val);
		if (ret == true)
		{
			value.setInt32(val);
		}
		return ret;
	}
	virtual bool Passable() const noexcept { return true; }

	virtual void serialize(void* serializeObj, Save::Properties& props,
		const Game& game, const Level& level) const
	{
		Save::serialize(serializeObj, props, game, level, *this);
	}

	virtual void update(Game& game, Level& level);

	virtual bool getProperty(const std::string_view prop, Variable& var) const;
	virtual void setProperty(const std::string_view prop, const Variable& val) noexcept {}

	virtual const std::string_view getType() const { return "levelObject"; }

	bool hasIntByHash(uint16_t propHash) const noexcept;
	bool hasInt(const std::string_view prop) const noexcept;

	LevelObjValue getIntByHash(uint16_t propHash) const;
	LevelObjValue getInt(const std::string_view prop) const;

	bool getIntByHash(uint16_t propHash, LevelObjValue& value) const;
	bool getInt(const std::string_view prop, LevelObjValue& value) const;
	void setIntByHash(uint16_t propHash, LevelObjValue value);
	void setInt(const std::string_view prop, LevelObjValue value);

	sf::Time getFrameTime() const noexcept { return animation.frameTime; }
	void setFrameTime(sf::Time time) noexcept { animation.frameTime = time; }

	void setTextureRect(const sf::IntRect& rectangle) { sprite.setTextureRect(rectangle); }

	const std::string& Id() const noexcept { return id; }
	const std::string& Name() const;
	const std::string& SimpleName() const noexcept { return Class()->Name(); }
	const std::string& Text1() const noexcept { return text1; }
	const std::string& Text2() const noexcept { return text2; }
	void Id(const std::string_view id_) { id = id_; }
	void Name(const std::string_view name_) { name = name_; }
	void Text1(const std::string_view text1_) { text1 = text1_; }
	void Text2(const std::string_view text2_) { text2 = text2_; }
};
