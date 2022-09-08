#pragma once

#include "Game/Action.h"
#include "Game/LevelObject/LevelObject.h"
#include <memory>
#include "SimpleLevelObjectClass.h"
#include "Utils/FixedMap.h"
#include "Utils/UnorderedStringMap.h"

class SimpleLevelObject : public LevelObject
{
private:
	friend class SimpleLevelObjectLevelObject;
	friend class SimpleLevelObjectSave;

	mutable std::string name;
	mutable std::array<std::string, 2> descriptions;

	std::string text1;
	std::string text2;

	mutable bool updateNameAndDescr{ true };

	FixedMap<uint16_t, LevelObjValue, 4> properties;

	void updateNameAndDescriptions() const;

public:
	SimpleLevelObject(const SimpleLevelObjectClass* class__);

	void serialize(const Level& level, void* serializeObj, const UnorderedStringMap<Variable>& props) const;

	constexpr auto Class() const noexcept { return (const SimpleLevelObjectClass*)class_; }

	bool Passable() const noexcept override { return true; }

	const std::string_view getType() const override { return "levelObject"; }

	void update(Game& game, Level& level, const std::shared_ptr<LevelObject>& thisPtr) override;

	bool getNumber(const std::string_view prop, Number32& value) const override;

	bool getProperty(const std::string_view prop, Variable& var) const override;

	bool getNumberByHash(const Queryable& owner, uint16_t propHash, LevelObjValue& value) const override;

	bool setNumberByHash(uint16_t propHash, const Number32& value) override;

	bool getIntByHash(uint16_t propHash, LevelObjValue& value) const;
	bool setIntByHash(uint16_t propHash, LevelObjValue value);

	auto getFrameTime() const noexcept { return animation.elapsedTime.timeout; }
	void setFrameTime(sf::Time time) noexcept { animation.elapsedTime.timeout = time; }

	void setTextureRect(const sf::IntRect& rectangle) { sprite.setTextureRect(rectangle); }

	auto& Id() const noexcept { return id; }
	const std::string& Name() const;
	auto& SimpleName() const noexcept { return Class()->Name(); }
	auto& Text1() const noexcept { return text1; }
	auto& Text2() const noexcept { return text2; }
	void Id(const std::string_view id_) { id = id_; }
	void Name(const std::string_view name_) { name = name_; }
	void Text1(const std::string_view text1_) { text1 = text1_; }
	void Text2(const std::string_view text2_) { text2 = text2_; }
};
