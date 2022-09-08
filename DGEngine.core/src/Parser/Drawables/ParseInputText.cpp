#include "ParseInputText.h"
#include "Game/Drawables/InputText.h"
#include "ParseDrawable.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"
#include "ParseText.h"
#include "Utils/StringHash.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<InputText> getInputTextObj(Game& game, const Value& elem)
	{
		auto text = getDrawableTextObj(game, elem);
		if (text == nullptr)
		{
			return nullptr;
		}
		auto inputText = std::make_shared<InputText>(std::move(text));

		if (elem.HasMember("minLength"sv) == true)
		{
			inputText->setMinLength(getUIntVal(elem["minLength"sv]));
		}
		if (elem.HasMember("maxLength"sv) == true)
		{
			inputText->setMaxLength(getUIntVal(elem["maxLength"sv]));
		}
		if (elem.HasMember("minValue"sv) == true)
		{
			inputText->setMinValue(getVariableVal(elem["minValue"sv]));
		}
		if (elem.HasMember("maxValue"sv) == true)
		{
			inputText->setMaxValue(getVariableVal(elem["maxValue"sv]));
		}
		if (isValidString(elem, "regex") == true)
		{
			inputText->setRegex(elem["regex"sv].GetStringView());
		}

		if (elem.HasMember("onClick"sv))
		{
			inputText->setAction(str2int16("click"), getActionVal(game, elem["onClick"sv]));
		}
		if (elem.HasMember("onChange"sv))
		{
			inputText->setAction(str2int16("change"), getActionVal(game, elem["onChange"sv]));
		}
		if (elem.HasMember("onMinLength"sv))
		{
			inputText->setAction(str2int16("minLength"), getActionVal(game, elem["onMinLength"sv]));
		}
		return inputText;
	}

	static std::shared_ptr<UIObject> parseInputTextFunc(Game& game, const Value& elem)
	{
		return getInputTextObj(game, elem);
	}

	void parseInputText(Game& game, const Value& elem)
	{
		parseValidIdAndAddDrawable(game, elem, parseInputTextFunc);
	}
}
