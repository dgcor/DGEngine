#include "ParseButton.h"
#include "Game/Drawables/BitmapButton.h"
#include "Game/Drawables/BitmapText.h"
#include "Game/Drawables/Panel.h"
#include "Game/Drawables/StringButton.h"
#include "Game/Drawables/StringText.h"
#include "Game/Game.h"
#include "Game/Utils/GameUtils.h"
#include "ParseDrawable.h"
#include "ParseImage.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"
#include "ParseText.h"
#include "Utils/StringHash.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<BitmapButton> parseBitmapButton(Game& game, const Value& elem)
	{
		return getImageObj<BitmapButton>(game, elem);
	}

	std::shared_ptr<StringButton> parseStringButton(Game& game, const Value& elem)
	{
		return getBindableTextObj<StringButton>(game, elem);
	}

	std::shared_ptr<Button> parseButtonObj(Game& game, const Value& elem)
	{
		if (elem.HasMember("font"sv) == false)
		{
			return parseBitmapButton(game, elem);
		}
		else
		{
			return parseStringButton(game, elem);
		}
	}

	void parseButtonProperties(Game& game, const Value& elem, Button& button)
	{
		if (getBoolKey(elem, "focus") == true)
		{
			button.focusEnabled(getBoolKey(elem, "focusOnClick", true));
		}
		button.enable(getBoolKey(elem, "enable", true));
		button.setClickUp(getBoolKey(elem, "clickUp"));
		button.setCaptureInputEvents(getInputEventTypeKey(elem, "captureInputEvents"));

		if (isValidString(elem, "sound"))
		{
			button.setClickSound(game.Resources().getSoundBuffer(elem["sound"sv].GetStringView()));
		}
		if (isValidString(elem, "focusSound"))
		{
			button.setFocusSound(game.Resources().getSoundBuffer(elem["focusSound"sv].GetStringView()));
		}
	}

	void parseButtonActions(Game& game, const Value& elem, Button& button)
	{
		if (elem.HasMember("onClick"sv))
		{
			button.setAction(str2int16("click"), getActionVal(game, elem["onClick"sv]));
		}
		if (elem.HasMember("onRightClick"sv))
		{
			button.setAction(str2int16("rightClick"), getActionVal(game, elem["onRightClick"sv]));
		}
		if (elem.HasMember("onDoubleClick"sv))
		{
			button.setAction(str2int16("doubleClick"), getActionVal(game, elem["onDoubleClick"sv]));
		}
		if (elem.HasMember("onClickDrag"sv))
		{
			button.setAction(str2int16("clickDrag"), getActionVal(game, elem["onClickDrag"sv]));
		}
		if (elem.HasMember("onClickIn"sv))
		{
			button.setAction(str2int16("clickIn"), getActionVal(game, elem["onClickIn"sv]));
		}
		if (elem.HasMember("onClickOut"sv))
		{
			button.setAction(str2int16("clickOut"), getActionVal(game, elem["onClickOut"sv]));
		}
		if (elem.HasMember("onFocus"sv))
		{
			button.setAction(str2int16("focus"), getActionVal(game, elem["onFocus"sv]));
		}
		if (elem.HasMember("onHoverEnter"sv))
		{
			button.setAction(str2int16("hoverEnter"), getActionVal(game, elem["onHoverEnter"sv]));
		}
		if (elem.HasMember("onHoverLeave"sv))
		{
			button.setAction(str2int16("hoverLeave"), getActionVal(game, elem["onHoverLeave"sv]));
		}
	}

	std::shared_ptr<Button> getButtonObj(Game& game, const Value& elem)
	{
		auto button = parseButtonObj(game, elem);
		if (button != nullptr)
		{
			parseButtonProperties(game, elem, *button);
			parseButtonActions(game, elem, *button);

			if (button->isFocusEnabled() == true)
			{
				game.Resources().addFocused(button, getStringViewKey(elem, "resource"));
			}
		}
		return button;
	}

	static std::shared_ptr<UIObject> parseButtonFunc(Game& game, const Value& elem)
	{
		return getButtonObj(game, elem);
	}

	void parseButton(Game& game, const Value& elem)
	{
		parseValidIdAndAddDrawable(game, elem, parseButtonFunc);
	}
}
