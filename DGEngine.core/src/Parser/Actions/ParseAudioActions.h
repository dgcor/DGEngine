#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser::Actions
{
	std::shared_ptr<Action> parseAudioDelete(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseAudioPause(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseAudioPauseAll();

	std::shared_ptr<Action> parseAudioPlay(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseAudioResumeAll(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseAudioSeek(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseAudioSetVolume(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseAudioStop(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseAudioStopAll();
}
