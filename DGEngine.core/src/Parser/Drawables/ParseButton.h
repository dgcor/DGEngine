#pragma once

#include "Json/JsonParser.h"
#include <memory>

class BitmapButton;
class Button;
class StringButton;
class Game;

namespace Parser
{
	std::shared_ptr<BitmapButton> parseBitmapButton(Game& game, const rapidjson::Value& elem);

	std::shared_ptr<StringButton> parseStringButton(Game& game, const rapidjson::Value& elem);

	std::shared_ptr<Button> parseButtonObj(Game& game, const rapidjson::Value& elem);

	void parseButtonProperties(Game& game, const rapidjson::Value& elem, Button& button);

	void parseButtonActions(Game& game, const rapidjson::Value& elem, Button& button);

	std::shared_ptr<Button> getButtonObj(Game& game, const rapidjson::Value& elem);

	void parseButton(Game& game, const rapidjson::Value& elem);
}
