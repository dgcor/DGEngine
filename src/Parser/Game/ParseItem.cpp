#include "ParseItem.h"
#include "Game/Item.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	std::shared_ptr<Item> parseItemObj(Game& game,
		const Level& level, const Value& elem)
	{
		if (isValidString(elem, "class") == false)
		{
			return nullptr;
		}

		auto class_ = level.getItemClass(elem["class"].GetString());
		if (class_ == nullptr)
		{
			return nullptr;
		}

		auto item = std::make_shared<Item>(class_);

		item->applyDefaults();

		item->Hoverable(getBoolKey(elem, "enableHover", true));

		if (isValidString(elem, "name") == true)
		{
			item->Name(elem["name"].GetString());
		}
		else
		{
			item->Name(class_->Name());
		}

		if (elem.HasMember("properties") == true)
		{
			const auto& props = elem["properties"];
			if (props.IsObject() == true)
			{
				for (auto it = props.MemberBegin(); it != props.MemberEnd(); ++it)
				{
					item->setItemProperty(it->name.GetString(),
						getMinMaxIntVal<int16_t>(it->value));
				}
			}
		}

		item->updateFullName();
		item->updateDescriptions();

		return item;
	}

	void parseItem(Game& game, const Value& elem)
	{
		auto level = game.Resources().getLevel(getStringKey(elem, "idLevel"));
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
