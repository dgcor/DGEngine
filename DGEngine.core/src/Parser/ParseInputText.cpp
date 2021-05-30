#include "ParseInputText.h"
#include <cassert>
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

	void parseInputText(Game& game, const Value& elem,
		const getInputTextObjFuncPtr getInputTextObjFunc)
	{
		assert(getInputTextObjFunc != nullptr);

		if (isValidString(elem, "id") == false)
		{
			return;
		}
		auto id = elem["id"sv].GetStringView();
		if (isValidId(id) == false)
		{
			return;
		}

		auto inputText = getInputTextObjFunc(game, elem);
		if (inputText == nullptr)
		{
			return;
		}

		bool manageObjDrawing = true;
		if (isValidString(elem, "panel") == true)
		{
			auto panelId = getStringViewVal(elem["panel"sv]);
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

	void parseInputText(Game& game, const Value& elem)
	{
		parseInputText(game, elem, getInputTextObj);
	}
}
