#include "ParseKeyboard.h"
#include "ParseAction.h"
#include "ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseKeyboard(Game& game, const Value& elem)
	{
		if (elem.HasMember("key"))
		{
			std::shared_ptr<Action> action;
			if (elem["action"].IsNull() == false)
			{
				action = parseAction(game, elem["action"]);
			}

			sf::Event::KeyEvent evt;
			evt.alt = getBool(elem, "alt");
			evt.control = getBool(elem, "control");
			evt.shift = getBool(elem, "shift");
			evt.system = getBool(elem, "system");

			const auto& keyElem = elem["key"];

			if (keyElem.IsArray() == true)
			{
				for (const auto& arrKey : keyElem)
				{
					auto keyCode = Parser::getKeyCode(arrKey);
					if (keyCode != sf::Keyboard::Key::Unknown)
					{
						evt.code = keyCode;
						game.Resources().setKeyboardAction(evt, action);
					}
				}
			}
			else
			{
				auto keyCode = Parser::getKeyCode(keyElem);
				if (keyCode != sf::Keyboard::Key::Unknown)
				{
					evt.code = keyCode;
					game.Resources().setKeyboardAction(evt, action);
				}
			}
		}
	}
}
