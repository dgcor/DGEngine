#include "ParseInputText.h"
#include "Game.h"
#include "InputText.h"
#include "Panel.h"
#include "ParseAction.h"
#include "ParseText.h"
#include "Utils/ParseUtils.h"
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseInputText(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false)
		{
			return;
		}
		auto id = elem["id"].GetStringStr();
		if (isValidId(id) == false)
		{
			return;
		}

		auto text = parseDrawableTextObj(game, elem);
		auto inputText = std::make_shared<InputText>(std::move(text));

		if (elem.HasMember("minLength") == true)
		{
			inputText->setMinLength(getUIntVal(elem["minLength"]));
		}
		if (elem.HasMember("maxLength") == true)
		{
			inputText->setMaxLength(getUIntVal(elem["maxLength"]));
		}
		if (elem.HasMember("minValue") == true)
		{
			inputText->setMinValue(getVariableVal(elem["minValue"]));
		}
		if (elem.HasMember("maxValue") == true)
		{
			inputText->setMaxValue(getVariableVal(elem["maxValue"]));
		}
		if (isValidString(elem, "regex") == true)
		{
			inputText->setRegex(elem["regex"].GetStringStr());
		}

		if (elem.HasMember("onClick"))
		{
			inputText->setAction(str2int16("click"), parseAction(game, elem["onClick"]));
		}
		if (elem.HasMember("onChange"))
		{
			inputText->setAction(str2int16("change"), parseAction(game, elem["onChange"]));
		}
		if (elem.HasMember("onMinLength"))
		{
			inputText->setAction(str2int16("minLength"), parseAction(game, elem["onMinLength"]));
		}

		bool manageObjDrawing = true;
		if (isValidString(elem, "panel") == true)
		{
			std::string panelId = getStringVal(elem["panel"]);
			auto panel = game.Resources().getDrawable<Panel>(panelId);
			if (panel != nullptr)
			{
				panel->addDrawable(inputText);
				manageObjDrawing = false;
			}
		}
		game.Resources().addDrawable(
			id, inputText, manageObjDrawing, getStringViewKey(elem, "resource")
		);
	}
}
