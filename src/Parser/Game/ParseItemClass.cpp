#include "ParseItemClass.h"
#include "Game/ItemClass.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseItemClass(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false ||
			isValidString(elem, "celTextureDrop") == false ||
			isValidString(elem, "celTextureInventory") == false ||
			isValidString(elem, "celTextureInventoryUnusable") == false)
		{
			return;
		}
		std::string id(elem["id"].GetString());
		if (isValidId(id) == false)
		{
			return;
		}

		auto level = game.Resources().getLevel(getStringKey(elem, "idLevel"));
		if (level == nullptr)
		{
			return;
		}

		auto celTexture = game.Resources().getCelTextureCacheVec(
			elem["celTextureDrop"].GetString());
		if (celTexture == nullptr)
		{
			return;
		}
		auto celDropIdx = (size_t)getUIntKey(elem, "celIndexDrop");
		if (celDropIdx >= celTexture->size())
		{
			return;
		}
		auto celTextureInv = game.Resources().getCelTextureCache(
			elem["celTextureInventory"].GetString());
		if (celTextureInv == nullptr)
		{
			return;
		}
		auto celTextureInvUnusable = game.Resources().getCelTextureCache(
			elem["celTextureInventoryUnusable"].GetString());
		if (celTextureInvUnusable == nullptr)
		{
			return;
		}
		auto celInventoryIdx = (size_t)getUIntKey(elem, "celIndexInventory");
		if (celInventoryIdx >= celTextureInv->size() ||
			celInventoryIdx >= celTextureInvUnusable->size())
		{
			return;
		}

		auto itemClass = std::make_shared<ItemClass>(celTexture, celDropIdx,
			celTextureInv, celTextureInvUnusable, celInventoryIdx);

		itemClass->Name(getStringKey(elem, "name"));
		itemClass->Type(getStringKey(elem, "type"));
		itemClass->Description(getStringKey(elem, "description"));

		if (elem.HasMember("action") == true)
		{
			itemClass->setAction(parseAction(game, elem["action"]));
		}
		if (elem.HasMember("actionDropLevel") == true)
		{
			itemClass->setActionDropLevel(parseAction(game, elem["actionDropLevel"]));
		}
		if (elem.HasMember("actionPickLevel") == true)
		{
			itemClass->setActionPickLevel(parseAction(game, elem["actionPickLevel"]));
		}
		if (elem.HasMember("actionDropInventory") == true)
		{
			itemClass->setActionDropInventory(parseAction(game, elem["actionDropInventory"]));
		}
		if (elem.HasMember("actionPickInventory") == true)
		{
			itemClass->setActionPickInventory(parseAction(game, elem["actionPickInventory"]));
		}

		level->addItemClass(id, itemClass);
	}
}
