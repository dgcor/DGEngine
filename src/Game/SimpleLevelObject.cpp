#include "SimpleLevelObject.h"
#include "Game.h"
#include "Game/Level.h"

SimpleLevelObject::SimpleLevelObject(const SimpleLevelObjectClass* class__) : LevelObject(class__)
{
	if (class__->getTexture() != nullptr)
	{
		sprite.setTexture(*class__->getTexture());
		sprite.setTextureRect(class__->getTextureRect());
	}
	else
	{
		texturePack = class__->getTexturePack();
		animation.textureIndexRange = class__->getTextureIndexRange();
		animation.frameTime = class__->getFrameTime();
		animation.animType = class__->getAnimationType();
		hoverCellSize = 1;
		updateTexture();
	}

	for (const auto& prop : class__->Defaults())
	{
		setIntByHash(prop.first, prop.second);
	}
}

bool SimpleLevelObject::getTexture(size_t textureNumber, TextureInfo& ti) const
{
	switch (textureNumber)
	{
	case 0:
		return getCurrentTexture(ti);
	default:
		return false;
	}
}

void SimpleLevelObject::update(Game& game, Level& level)
{
	updateHover(game, level);

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

const std::string& SimpleLevelObject::Name() const
{
	updateNameAndDescriptions();
	return name;
}

bool SimpleLevelObject::getProperty(const std::string_view prop, Variable& var) const
{
	if (prop.empty() == true)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	if (getLevelObjProp(propHash, props.second, var) == true)
	{
		return true;
	}
	switch (propHash)
	{
	case str2int16("name"):
		var = Variable(Name());
		break;
	case str2int16("simpleName"):
		var = Variable(SimpleName());
		break;
	case str2int16("text1"):
		var = Variable(text1);
		break;
	case str2int16("text2"):
		var = Variable(text2);
		break;
	case str2int16("d"):
	case str2int16("description"):
	{
		updateNameAndDescriptions();
		size_t idx = Utils::strtou(props.second);
		if (idx >= descriptions.size())
		{
			idx = 0;
		}
		var = Variable(descriptions[idx]);
		break;
	}
	case str2int16("eval"):
		var = Variable((int64_t)Formula::evalString(props.second, *this));
		break;
	case str2int16("evalMin"):
		var = Variable((int64_t)Formula::evalMinString(props.second, *this));
		break;
	case str2int16("evalMax"):
		var = Variable((int64_t)Formula::evalMaxString(props.second, *this));
		break;
	case str2int16("evalf"):
		var = Variable(Formula::evalString(props.second, *this));
		break;
	case str2int16("evalMinf"):
		var = Variable(Formula::evalMinString(props.second, *this));
		break;
	case str2int16("evalMaxf"):
		var = Variable(Formula::evalMaxString(props.second, *this));
		break;
	default:
	{
		LevelObjValue value;
		if (getIntByHash(propHash, value) == true)
		{
			var = Variable((int64_t)value);
			break;
		}
		else
		{
			return false;
		}
	}
	}
	return true;
}

bool SimpleLevelObject::hasIntByHash(uint16_t propHash) const noexcept
{
	return properties.hasValue(propHash);
}

bool SimpleLevelObject::hasInt(const std::string_view prop) const noexcept
{
	return hasIntByHash(str2int16(prop));
}

LevelObjValue SimpleLevelObject::getIntByHash(uint16_t propHash) const
{
	LevelObjValue value = 0;
	getIntByHash(propHash, value);
	return value;
}

LevelObjValue SimpleLevelObject::getInt(const std::string_view prop) const
{
	LevelObjValue value = 0;
	getInt(prop, value);
	return value;
}

bool SimpleLevelObject::getIntByHash(uint16_t propHash, LevelObjValue& value) const
{
	return properties.getValue(propHash, value);
}

bool SimpleLevelObject::getInt(const std::string_view prop, LevelObjValue& value) const
{
	return getIntByHash(str2int16(prop), value);
}

void SimpleLevelObject::setIntByHash(uint16_t propHash, LevelObjValue value)
{
	properties.setValue(propHash, value);
	updateNameAndDescr = true;
}

void SimpleLevelObject::setInt(const std::string_view prop, LevelObjValue value)
{
	setIntByHash(str2int16(prop), value);
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
