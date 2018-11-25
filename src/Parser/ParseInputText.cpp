#include "ParseInputText.h"
#include "InputText.h"
#include "ParseAction.h"
#include "ParseText.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseInputText(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false)
		{
			return;
		}
		std::string id(elem["id"].GetString());
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
			inputText->setRegex(elem["regex"].GetString());
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

		game.Resources().addDrawable(id, inputText, getStringViewKey(elem, "resource"));
	}
}
