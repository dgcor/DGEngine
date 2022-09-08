#include "ParseItem.h"
#include "Game/Game.h"
#include "Game/Item/Item.h"
#include "Game/Level/Level.h"
#include "Parser/LevelObject/ParseLevelObjectCommon.h"
#include "Parser/Utils/ParseUtils.h"
#include "Parser/Utils/ParseUtilsGameKey.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<Item> parseItemObj(Level& level, const Value& elem)
	{
		if (isValidString(elem, "class") == false)
		{
			return nullptr;
		}

		auto class_ = level.LevelObjects().getClass<ItemClass>(elem["class"sv].GetStringView());
		if (class_ == nullptr)
		{
			return nullptr;
		}

		auto item = std::make_shared<Item>(class_);

		parseLevelObjectProperties(*item, level, elem);

		auto outline = getColorKey(elem, "outline", class_->Outline());
		auto outlineIgnore = getColorKey(elem, "outlineIgnore", class_->OutlineIgnore());
		item->setOutline(outline, outlineIgnore);
		item->setOutlineOnHover(getBoolKey(elem, "outlineOnHover", true));

		return item;
	}

	void parseItem(Game& game, const Value& elem)
	{
		auto level = game.Resources().getLevel<Level>(getStringViewKey(elem, "level"));
		if (level == nullptr)
		{
			return;
		}

		auto item = parseItemObj(*level, elem);
		if (item == nullptr)
		{
			return;
		}

		auto itemLocation = getItemLocationVal(elem);

		level->setItem(itemLocation, item);
	}
}
