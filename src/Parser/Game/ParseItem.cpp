#include "ParseItem.h"
#include "Game/Item.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	std::unique_ptr<Item> parseItemObj(Game& game,
		Level& level, const Value& elem)
	{
		if (isValidString(elem, "class") == false)
		{
			return nullptr;
		}

		auto class_ = level.getClass<ItemClass>(elem["class"].GetString());
		if (class_ == nullptr)
		{
			return nullptr;
		}

		auto item = std::make_unique<Item>(class_);

		if (elem.HasMember("properties") == true)
		{
			const auto& props = elem["properties"];
			if (props.IsObject() == true)
			{
				for (auto it = props.MemberBegin(); it != props.MemberEnd(); ++it)
				{
					if (it->name.GetStringLength() > 0)
					{
						auto name = getStringViewVal(it->name);
						auto nameHash = str2int16(name);
						level.setPropertyName(nameHash, name);
						item->setIntByHash(nameHash,
							getMinMaxIntVal<LevelObjValue>(it->value));
					}
				}
			}
		}

		auto outline = getColorKey(elem, "outline", class_->Outline());
		auto outlineIgnore = getColorKey(elem, "outlineIgnore", class_->OutlineIgnore());
		item->setOutline(outline, outlineIgnore);
		item->setOutlineOnHover(getBoolKey(elem, "outlineOnHover", true));

		return item;
	}

	void parseItem(Game& game, const Value& elem)
	{
		auto level = game.Resources().getLevel(getStringKey(elem, "level"));
		if (level == nullptr)
		{
			return;
		}

		auto item = parseItemObj(game, *level, elem);
		if (item == nullptr)
		{
			return;
		}

		auto itemLocation = getItemLocationVal(elem);

		level->setItem(itemLocation, item);
	}
}
