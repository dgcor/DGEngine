#include "ParseKeyboard.h"
#include "Game.h"
#include "ParseAction.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseKey(Game& game, const Value& elem)
	{
		std::shared_ptr<Action> action;
		if (elem["action"].IsNull() == false)
		{
			action = parseAction(game, elem["action"]);
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
#ifdef __ANDROID__
		evt.key.system = false;
#else
		evt.key.system = getBoolKey(elem, "system");
#endif
		const auto& keyElem = elem["key"];

		if (keyElem.IsArray() == true)
		{
			for (const auto& arrVal : keyElem)
			{
				auto keyCode = Parser::getKeyCodeVal(arrVal);
				if (keyCode != sf::Keyboard::Key::Unknown)
				{
					evt.key.code = keyCode;
					game.Resources().setInputAction(evt, action);
				}
			}
		}
		else
		{
			auto keyCode = Parser::getKeyCodeVal(keyElem);
			if (keyCode != sf::Keyboard::Key::Unknown)
			{
				evt.key.code = keyCode;
				game.Resources().setInputAction(evt, action);
			}
		}
	}

	void parseCompositeKey(Game& game, const Value& elem)
	{
		std::shared_ptr<Action> action;
		if (elem["action"].IsNull() == false)
		{
			action = parseAction(game, elem["action"]);
		}

		CompositeInputEvent compositeEvt;

		const auto& keyElem = elem["compositeKey"];

		if (keyElem.IsArray() == true)
		{
			InputEvent evt;
			evt.type = InputType::Keyboard;
			for (const auto& arrVal : keyElem)
			{
				auto keyCode = Parser::getKeyCodeVal(arrVal);
				if (keyCode == sf::Keyboard::Key::Unknown)
				{
					return;
				}
				evt.value = keyCode;
				compositeEvt.events.push_back(evt);
			}
		}
		else
		{
			InputEvent evt;
			evt.type = InputType::Keyboard;
			auto keyCode = Parser::getKeyCodeVal(keyElem);
			if (keyCode == sf::Keyboard::Key::Unknown)
			{
				return;
			}
			evt.value = keyCode;
			compositeEvt.events.push_back(evt);
		}
		if (compositeEvt.events.empty() == false)
		{
			game.Resources().setInputAction(compositeEvt, action);
		}
	}

	void parseKeyboard(Game& game, const Value& elem)
	{
		if (elem.HasMember("key") == true)
		{
			parseKey(game, elem);
		}
		else if (elem.HasMember("compositeKey") == true)
		{
			parseCompositeKey(game, elem);
		}
	}
}
