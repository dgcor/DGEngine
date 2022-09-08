#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser::Actions
{
	std::shared_ptr<Action> parseShaderSetGameShader(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseShaderSetBool(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseShaderSetColor(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseShaderSetFloat(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseShaderSetVec2(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseShaderSetVec3(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseShadersetVec4(const rapidjson::Value& elem);
}
