#include "SimpleLevelObject.h"
#include "Game.h"
#include "Game/Level.h"

SimpleLevelObject::SimpleLevelObject(const LevelObjectClass* class__) : class_(class__)
{
	if (class__->getTexture() != nullptr)
	{
		base.sprite.setTexture(*class__->getTexture());
		base.sprite.setTextureRect(class__->getTextureRect());
	}
	else
	{
		base.texturePack = class__->getTexturePack();
		base.animation.textureIndexRange = class__->getTextureIndexRange();
		base.animation.frameTime = class__->getFrameTime();
		base.animation.animType = class__->getAnimationType();
		base.hoverCellSize = 1;
		base.updateTexture();
	}
	action = class__->getAction(str2int16("action"));

	for (const auto& prop : class_->Defaults())
	{
		setIntByHash(prop.first, prop.second);
	}
}

bool SimpleLevelObject::getTexture(size_t textureNumber, TextureInfo& ti) const
{
	switch (textureNumber)
	{
	case 0:
		return base.getTexture(ti);
	default:
		return false;
	}
}

void SimpleLevelObject::executeAction(Game& game) const
{
	if (action != nullptr)
	{
		game.Events().addBack(action);
	}
}

void SimpleLevelObject::update(Game& game, Level& level)
{
	base.updateHover(game, level, this);

	const auto& rect = base.sprite.getTextureRect();
	if (rect.width > 0 && rect.height > 0)
	{
		base.updateDrawPosition(level);
	}
	if (base.hasValidState() == true)
	{
		base.animation.update(game.getElapsedTime());
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
	switch (propHash)
	{
	case str2int16("type"):
		var = Variable(std::string("levelObject"));
		break;
	case str2int16("id"):
		var = Variable(id);
		break;
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
		if (class_->getFullName(*this, name) == false &&
			name.empty() == true)
		{
			name = SimpleName();
		}
		for (size_t i = 0; i < descriptions.size(); i++)
		{
			class_->getDescription(i, *this, descriptions[i]);
		}
	}
}
