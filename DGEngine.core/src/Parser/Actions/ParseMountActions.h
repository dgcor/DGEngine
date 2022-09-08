#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser::Actions
{
	std::shared_ptr<Action> parseMount(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseUnmount(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseUnmountAll();
}
