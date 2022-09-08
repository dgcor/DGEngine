#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser::Actions
{
	std::shared_ptr<Action> parseGameAddToProperty(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseGameClearPlayingSounds();

	std::shared_ptr<Action> parseGameClose();

	std::shared_ptr<Action> parseGameDraw();

	std::shared_ptr<Action> parseGameEnableInput(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseGameLoad(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseGameSetGamma(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseGameSetMusicVolume(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseGameSetProperty(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseGameSetSoundVolume(const rapidjson::Value& elem);
}
