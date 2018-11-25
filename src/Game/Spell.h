#pragma once

#include "Actions/Action.h"
#include "Queryable.h"
#include "Save/SaveItem.h"
#include "SpellClass.h"
#include "Utils/FixedMap.h"

class Player;

class Spell : public Queryable
{
private:
	typedef FixedMap<uint16_t, LevelObjValue, 4> SpellProperties;

	const SpellClass* class_;

	mutable bool updateDescr{ true };

	mutable std::array<std::string, 5> descriptions;

	SpellProperties properties;

	void updateDescriptions() const;

public:
	using iterator = SpellProperties::iterator;
	using const_iterator = SpellProperties::const_iterator;
	using reverse_iterator = SpellProperties::reverse_iterator;
	using const_reverse_iterator = SpellProperties::const_reverse_iterator;

	iterator begin() noexcept { return properties.begin(); }
	iterator end() noexcept { return properties.end(); }
	const_iterator begin() const noexcept { return properties.begin(); }
	const_iterator end() const noexcept { return properties.end(); }
	const_iterator cbegin() const noexcept { return properties.cbegin(); }
	const_iterator cend() const noexcept { return properties.cend(); }
	reverse_iterator rbegin() noexcept { return properties.rbegin(); }
	reverse_iterator rend() noexcept { return properties.rend(); }
	const_reverse_iterator rbegin() const noexcept { return properties.rbegin(); }
	const_reverse_iterator rend() const noexcept { return properties.rend(); }
	const_reverse_iterator crbegin() const noexcept { return properties.crbegin(); }
	const_reverse_iterator crend() const noexcept { return properties.crend(); }

	Spell(const SpellClass* class__);

	virtual bool getProperty(const std::string_view prop, Variable& var) const;
	virtual bool getTexture(size_t textureNumber, TextureInfo& ti) const;

	const SpellClass* Class() const noexcept { return class_; }

	bool hasIntByHash(uint16_t propHash) const noexcept;
	bool hasInt(const std::string_view prop) const noexcept;

	LevelObjValue getIntByHash(uint16_t propHash) const;
	LevelObjValue getInt(const std::string_view prop) const;

	bool getIntByHash(uint16_t propHash, LevelObjValue& value) const;
	bool getInt(const std::string_view prop, LevelObjValue& value) const;
	void setIntByHash(uint16_t propHash, LevelObjValue value);
	void setInt(const std::string_view prop, LevelObjValue value);

	void applyDefaults();

	const std::string& Name() const noexcept { return class_->Name(); }
	const std::string& SpellType() const noexcept { return class_->Type(); }
};
