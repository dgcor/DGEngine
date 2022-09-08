#include "ParseMenu.h"
#include "Game/Drawables/Menu.h"
#include "Game/Utils/GameUtils.h"
#include "ParseButton.h"
#include "ParseDrawable.h"
#include "ParseMenuItem.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"
#include "Utils/StringHash.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<Menu> getMenuObj(Game& game, const Value& elem)
	{
		auto menu = std::make_shared<Menu>();

		auto referenceButton = parseStringButton(game, elem);
		if (referenceButton == nullptr)
		{
			return nullptr;
		}

		parseButtonProperties(game, elem, *referenceButton);
		parseButtonActions(game, elem, *referenceButton);

		if (elem.HasMember("items"sv) == true)
		{
			size_t menuIdx = 0;

			auto relativePos = getBoolKey(elem, "relativeCoords", true);
			auto resource = getStringViewKey(elem, "resource");

			const auto& items = elem["items"sv];
			if (items.IsObject() == true)
			{
				menuIdx = parseMenuItem(game, items, *menu, menuIdx, *referenceButton, relativePos, resource);
			}
			else if (items.IsArray() == true)
			{
				for (const auto& itemVal : elem["items"sv])
				{
					menuIdx = parseMenuItem(game, itemVal, *menu, menuIdx, *referenceButton, relativePos, resource);
				}
			}
		}

		if (referenceButton->isFocusEnabled() == true)
		{
			menu->focusEnabled();
		}

		auto size = getVector2fKey<sf::Vector2f>(elem, "size");
		parseDrawableProperties(game, elem, *menu, size);

		auto pos = menu->Position();
		auto horizAlign = referenceButton->getHorizontalAlign();
		auto vertAlign = referenceButton->getVerticalAlign();

		menu->setVerticalAlign(vertAlign);
		menu->ScrollPosition(GameUtils::getAlignmentPosition(pos, size, horizAlign, vertAlign));
		menu->setVerticalPad(getIntKey(elem, "verticalPad"));
		menu->setVisibleItems(getUIntKey(elem, "visibleItems"));

		if (elem.HasMember("onScrollDown"sv))
		{
			menu->setAction(str2int16("scrollDown"), getActionVal(game, elem["onScrollDown"sv]));
		}
		if (elem.HasMember("onScrollUp"sv))
		{
			menu->setAction(str2int16("scrollUp"), getActionVal(game, elem["onScrollUp"sv]));
		}

		menu->updateVisibleItems();
		menu->calculatePositions();

		return menu;
	}

	static std::shared_ptr<UIObject> parseMenuFunc(Game& game, const Value& elem)
	{
		return getMenuObj(game, elem);
	}

	void parseMenu(Game& game, const Value& elem)
	{
		parseValidIdAndAddDrawable(game, elem, parseMenuFunc);
	}
}
