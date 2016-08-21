#include "ParseInputText.h"
#include "InputText.h"
#include "ParseAction.h"
#include "ParseText.h"
#include "ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseInputText(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false)
		{
			return;
		}

		auto text = parseDrawableTextObj(game, elem);
		auto inputText = std::make_shared<InputText>(std::move(text),
			getUIntKey(elem, "minSize"), getUIntKey(elem, "maxSize"));

		if (elem.HasMember("onClick"))
		{
			inputText->setActionEnter(parseAction(game, elem["onClick"]));
		}

		if (elem.HasMember("onChange"))
		{
			inputText->setActionChange(parseAction(game, elem["onChange"]));
		}

		if (elem.HasMember("onMinSize"))
		{
			inputText->setActionMinSize(parseAction(game, elem["onMinSize"]));
		}

		inputText->setRegex(getStringKey(elem, "regex"));

		game.Resources().addDrawable(elem["id"].GetString(), inputText);
	}
}
