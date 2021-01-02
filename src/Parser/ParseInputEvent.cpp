#include "ParseInputEvent.h"
#include "Game.h"
#include "ParseAction.h"
#include "Utils/ParseUtils.h"
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	sf::Event updateKeyEvent(sf::Event evt)
	{
		switch (evt.key.code)
		{
		case sf::Keyboard::LControl:
		case sf::Keyboard::RControl:
			evt.key.control = true;
			break;
		case sf::Keyboard::LShift:
		case sf::Keyboard::RShift:
			evt.key.shift = true;
			break;
		case sf::Keyboard::LAlt:
		case sf::Keyboard::RAlt:
			evt.key.alt = true;
			break;
		case sf::Keyboard::LSystem:
		case sf::Keyboard::RSystem:
			evt.key.system = true;
			break;
		default:
			break;
		}
		return evt;
	}

	void parseActionKey(Game& game, const Value& elem)
	{
		if (elem.HasMember("key"sv) == false)
		{
			return;
		}

		std::shared_ptr<Action> action;
		if (elem["action"sv].IsNull() == false)
		{
			action = getActionVal(game, elem["action"sv]);
		}

		sf::Event evt;
		evt.type = sf::Event::KeyPressed;
		if (getBoolKey(elem, "keyUp") == true)
		{
			evt.type = sf::Event::KeyReleased;
		}
		evt.key.alt = getBoolKey(elem, "alt");
		evt.key.control = getBoolKey(elem, "control");
		evt.key.shift = getBoolKey(elem, "shift");
		evt.key.system = getBoolKey(elem, "system");

		const auto& keyElem = elem["key"sv];

		if (keyElem.IsArray() == true)
		{
			for (const auto& arrVal : keyElem)
			{
				auto keyCode = Parser::getKeyCodeVal(arrVal);
				if (keyCode != sf::Keyboard::Key::Unknown)
				{
					evt.key.code = keyCode;
					game.Resources().setInputAction(updateKeyEvent(evt), action);
				}
			}
		}
		else
		{
			auto keyCode = Parser::getKeyCodeVal(keyElem);
			if (keyCode != sf::Keyboard::Key::Unknown)
			{
				evt.key.code = keyCode;
				game.Resources().setInputAction(updateKeyEvent(evt), action);
			}
		}
	}

	void parseEventKey(Game& game, const Value& elem)
	{
		if (elem.HasMember("key"sv) == false)
		{
			return;
		}

		uint16_t actionHash{ 0 };
		if (isValidString(elem, "event") == true)
		{
			actionHash = str2int16(elem["event"sv].GetStringView());
		}

		const auto& keyElem = elem["key"sv];

		if (keyElem.IsArray() == true)
		{
			InputEvent evt;
			evt.type = InputType::Keyboard;
			for (const auto& arrVal : keyElem)
			{
				auto keyCode = Parser::getKeyCodeVal(arrVal);
				if (keyCode != sf::Keyboard::Key::Unknown)
				{
					evt.value = keyCode;
					game.Resources().setInputEvent(evt, actionHash);
				}
			}
		}
		else
		{
			InputEvent evt;
			evt.type = InputType::Keyboard;
			auto keyCode = Parser::getKeyCodeVal(keyElem);
			if (keyCode != sf::Keyboard::Key::Unknown)
			{
				evt.value = keyCode;
				game.Resources().setInputEvent(evt, actionHash);
			}
		}
	}

	void parseInputEvent(Game& game, const Value& elem)
	{
		if (elem.HasMember("action"sv) == true)
		{
			parseActionKey(game, elem);
		}
		else if (elem.HasMember("event"sv) == true)
		{
			parseEventKey(game, elem);
		}
	}
}
