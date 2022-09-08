#include "ParseInputEvent.h"
#include "Game/Game.h"
#include "ParseAction.h"
#include "Utils/ParseUtils.h"
#include "Utils/StringHash.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	sf::Event updateKeyEvent(sf::Event evt)
	{
#ifdef _WIN32
		// fixes key presses with these keys under windows
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
#endif
		return evt;
	}

	std::vector<sf::Event> parseInputEvents(const Value& elem)
	{
		std::vector<sf::Event> inputEvents;

		if (elem.HasMember("key"sv) == false &&
			elem.HasMember("scancode"sv) == false)
		{
			return inputEvents;
		}

		sf::Event evt{};
		evt.type = sf::Event::KeyPressed;
		if (getBoolKey(elem, "keyUp") == true)
		{
			evt.type = sf::Event::KeyReleased;
		}
		evt.key.scancode = sf::Keyboard::Scancode::Unknown;
		evt.key.alt = getBoolKey(elem, "alt");
		evt.key.control = getBoolKey(elem, "control");
		evt.key.shift = getBoolKey(elem, "shift");
		evt.key.system = getBoolKey(elem, "system");

		if (elem.HasMember("key"sv) == true)
		{
			const auto& keyElem = elem["key"sv];

			if (keyElem.IsArray() == true)
			{
				for (const auto& arrVal : keyElem)
				{
					auto keyCode = getKeyCodeVal(arrVal);
					if (keyCode != sf::Keyboard::Unknown)
					{
						evt.key.code = keyCode;
						inputEvents.push_back(updateKeyEvent(evt));
					}
				}
			}
			else
			{
				auto keyCode = getKeyCodeVal(keyElem);
				if (keyCode != sf::Keyboard::Unknown)
				{
					evt.key.code = keyCode;
					inputEvents.push_back(updateKeyEvent(evt));
				}
			}
		}

		if (elem.HasMember("scancode") == true)
		{
			evt.key.code = sf::Keyboard::Unknown;

			const auto& scanCodeElem = elem["scancode"sv];

			if (scanCodeElem.IsArray() == true)
			{
				for (const auto& arrVal : scanCodeElem)
				{
					auto scanCode = getScanCodeVal(arrVal);
					if (scanCode != sf::Keyboard::Scancode::Unknown)
					{
						evt.key.scancode = scanCode;
						inputEvents.push_back(evt);
					}
				}
			}
			else
			{
				auto scanCode = getScanCodeVal(scanCodeElem);
				if (scanCode != sf::Keyboard::Scancode::Unknown)
				{
					evt.key.scancode = scanCode;
					inputEvents.push_back(evt);
				}
			}
		}

		return inputEvents;
	}

	std::vector<sf::Event> getInputEvents(const Game& game, const Value& elem)
	{
		std::vector<sf::Event> inputEvents;

		if (elem.HasMember("gameInputEvent"sv) == true)
		{
			const auto& gameInputElem = elem["gameInputEvent"];
			if (gameInputElem.IsString() == true)
			{
				inputEvents = game.GameInputEvents().get(elem["gameInputEvent"].GetStringView());
			}
			else if (gameInputElem.IsArray() == true)
			{
				for (const auto& val : gameInputElem)
				{
					auto vec = game.GameInputEvents().get(getStringViewVal(val));
					inputEvents.insert(inputEvents.end(), vec.begin(), vec.end());
				}
			}
		}
		else
		{
			inputEvents = parseInputEvents(elem);
		}

		return inputEvents;
	}

	void parseActionKey(Game& game, const Value& elem)
	{
		auto inputEvents = getInputEvents(game, elem);
		if (inputEvents.empty() == true)
		{
			return;
		}

		std::shared_ptr<Action> action;
		if (elem["action"sv].IsNull() == false)
		{
			action = getActionVal(game, elem["action"sv]);
		}

		for (const auto& evt : inputEvents)
		{
			game.Resources().setInputAction(evt, action);
		}
	}

	void parseEventKey(Game& game, const Value& elem)
	{
		auto inputEvents = getInputEvents(game, elem);
		if (inputEvents.empty() == true)
		{
			return;
		}

		uint16_t actionHash{ 0 };
		if (isValidString(elem, "event") == true)
		{
			actionHash = str2int16(elem["event"sv].GetStringView());
		}

		for (const auto& evt : inputEvents)
		{
			switch (evt.type)
			{
			case sf::Event::KeyPressed:
			case sf::Event::KeyReleased:
			{
				InputEvent inputEvt;

				if (evt.key.code != sf::Keyboard::Unknown)
				{
					inputEvt.type = InputType::Keyboard;
					inputEvt.value = evt.key.code;
				}
				else
				{
					inputEvt.type = InputType::Scancode;
					inputEvt.value = evt.key.scancode;
				}
				game.Resources().setInputEvent(inputEvt, actionHash);
				break;
			}
			default:
				continue;
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
