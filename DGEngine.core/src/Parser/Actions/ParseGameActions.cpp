#include "ParseGameActions.h"
#include "Game/Actions/ActGame.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<Action> parseGameAddToProperty(const Value& elem)
	{
		return std::make_shared<ActGameAddToProperty>(
			getStringViewKey(elem, "property"),
			getVariableKey(elem, "value"));
	}

	std::shared_ptr<Action> parseGameClearPlayingSounds()
	{
		return std::make_shared<ActGameClearPlayingSounds>();
	}

	std::shared_ptr<Action> parseGameClose()
	{
		return std::make_shared<ActGameClose>();
	}

	std::shared_ptr<Action> parseGameDraw()
	{
		return std::make_shared<ActGameDraw>();
	}

	std::shared_ptr<Action> parseGameEnableInput(const Value& elem)
	{
		return std::make_shared<ActGameEnableInput>(getBoolKey(elem, "enable", true));
	}

	std::shared_ptr<Action> parseGameLoad(const Value& elem)
	{
		return std::make_shared<ActGameLoad>(
			getStringViewKey(elem, "file"),
			getStringViewKey(elem, "mainFile", "main.json"));
	}

	std::shared_ptr<Action> parseGameSetGamma(const Value& elem)
	{
		return std::make_shared<ActGameSetGamma>(getVariableKey(elem, "gamma"));
	}

	std::shared_ptr<Action> parseGameSetMusicVolume(const Value& elem)
	{
		return std::make_shared<ActGameSetMusicVolume>(getVariableKey(elem, "volume"));
	}

	std::shared_ptr<Action> parseGameSetProperty(const Value& elem)
	{
		auto action = std::make_shared<ActGameSetProperty>(
			getStringViewKey(elem, "property"),
			getVariableKey(elem, "value"));

		if (elem.HasMember("propRange"sv) == true)
		{
			action->setPropRange(getVector2iKey<sf::Vector2i>(elem, "propRange"));
		}
		if (elem.HasMember("valueRange"sv) == true)
		{
			action->setValueRange(getVector2iKey<sf::Vector2i>(elem, "valueRange"));
		}
		return action;
	}

	std::shared_ptr<Action> parseGameSetSoundVolume(const Value& elem)
	{
		return std::make_shared<ActGameSetSoundVolume>(getVariableKey(elem, "volume"));
	}
}
