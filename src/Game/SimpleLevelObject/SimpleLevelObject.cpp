#include "SimpleLevelObject.h"
#include "Game/Game.h"
#include "Game/Level/Level.h"
#include "SimpleLevelObjectLevelObject.h"
#include "SimpleLevelObjectSave.h"

SimpleLevelObject::SimpleLevelObject(const SimpleLevelObjectClass* class__) : LevelObject(class__)
{
	if (class__->getTexture() != nullptr)
	{
		sprite.setTexture(*class__->getTexture());
		sprite.setTextureRect(class__->getTextureRect());
	}
	else
	{
		animation.setTexturePack(class__->getTexturePack());
		animation.textureIndexRange = class__->getTextureIndexRange();
		animation.elapsedTime = class__->getFrameTime();
		animation.animType = class__->getAnimationType();
		updateTexture();
	}
	lightSource = class__->getLightSource();
	cellSize = class__->getCellSize();

	for (const auto& prop : class__->Defaults())
	{
		setIntByHash(prop.first, prop.second);
	}
}

void SimpleLevelObject::serialize(const Level& level, void* serializeObj, const UnorderedStringMap<Variable>& props) const
{
	SimpleLevelObjectSave::serialize(*this, level, serializeObj, props);
}

const std::string& SimpleLevelObject::Name() const
{
	updateNameAndDescriptions();
	return name;
}

bool SimpleLevelObject::getNumber(const std::string_view prop, Number32& value) const
{
	return SimpleLevelObjectLevelObject::getNumber(*this, prop, value);
}

bool SimpleLevelObject::getProperty(const std::string_view prop, Variable& var) const
{
	return SimpleLevelObjectLevelObject::getProperty(*this, prop, var);
}

bool SimpleLevelObject::getNumberByHash(const Queryable& owner, uint16_t propHash, LevelObjValue& value) const
{
	return properties.getValue(propHash, value);
}

bool SimpleLevelObject::getIntByHash(uint16_t propHash, LevelObjValue& value) const
{
	return properties.getValue(propHash, value);
}

bool SimpleLevelObject::setNumberByHash(uint16_t propHash, const Number32& value)
{
	return setIntByHash(propHash, value.getInt32());
}

bool SimpleLevelObject::setIntByHash(uint16_t propHash, LevelObjValue value)
{
	if (properties.setValue(propHash, value) == true)
	{
		updateNameAndDescr = true;
		return true;
	}
	return false;
}

void SimpleLevelObject::update(Game& game, Level& level, const std::shared_ptr<LevelObject>& thisPtr)
{
	updateHover(game, level, thisPtr);

	const auto& rect = sprite.getTextureRect();
	if (rect.width > 0 && rect.height > 0)
	{
		updateDrawPosition(level.Map());
	}
	if (hasValidState() == true)
	{
		animation.update(game.getElapsedTime());
	}
}

void SimpleLevelObject::updateNameAndDescriptions() const
{
	if (updateNameAndDescr == true)
	{
		updateNameAndDescr = false;
		if (Class()->getFullName(*this, name) == false &&
			name.empty() == true)
		{
			name = SimpleName();
		}
		for (size_t i = 0; i < descriptions.size(); i++)
		{
			Class()->getDescription(i, *this, descriptions[i]);
		}
	}
}
