#include "ParseKeyboard.h"
#include "ParseAction.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseKeyboard(Game& game, const Value& elem)
	{
		if (elem.HasMember("key") == true)
		{
			std::shared_ptr<Action> action;
			if (elem["action"].IsNull() == false)
			{
				action = parseAction(game, elem["action"]);
			}

			sf::Event::KeyEvent evt;
			evt.alt = getBoolKey(elem, "alt");
			evt.control = getBoolKey(elem, "control");
			evt.shift = getBoolKey(elem, "shift");
#ifdef __ANDROID__
			evt.system = false;
#else
			evt.system = getBoolKey(elem, "system");
#endif
			const auto& keyElem = elem["key"];

			if (keyElem.IsArray() == true)
			{
				for (const auto& arrVal : keyElem)
				{
					auto keyCode = Parser::getKeyCodeVal(arrVal);
					if (keyCode != sf::Keyboard::Key::Unknown)
					{
						evt.code = keyCode;
						game.Resources().setKeyboardAction(evt, action);
					}
				}
			}
			else
			{
				auto keyCode = Parser::getKeyCodeVal(keyElem);
				if (keyCode != sf::Keyboard::Key::Unknown)
				{
					evt.code = keyCode;
					game.Resources().setKeyboardAction(evt, action);
				}
			}
		}
	}
}
