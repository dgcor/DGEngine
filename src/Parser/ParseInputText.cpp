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
		auto inputText = std::make_shared<InputText>(std::move(text),
			getUIntKey(elem, "minSize"), getUIntKey(elem, "maxSize"));

		if (elem.HasMember("onClick"))
		{
			inputText->setAction(str2int16("click"), parseAction(game, elem["onClick"]));
		}
		if (elem.HasMember("onChange"))
		{
			inputText->setAction(str2int16("change"), parseAction(game, elem["onChange"]));
		}
		if (elem.HasMember("onMinSize"))
		{
			inputText->setAction(str2int16("minSize"), parseAction(game, elem["onMinSize"]));
		}

		if (isValidString(elem, "regex") == true)
		{
			inputText->setRegex(elem["regex"].GetString());
		}

		game.Resources().addDrawable(id, inputText);
	}
}
