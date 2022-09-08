#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser2::Actions
{
	std::shared_ptr<Action> parsePlayerAddItemQuantity(const rapidjson::Value& elem);

	std::shared_ptr<Action> parsePlayerAddToProperty(const rapidjson::Value& elem);

	std::shared_ptr<Action> parsePlayerMove(const rapidjson::Value& elem);

	std::shared_ptr<Action> parsePlayerRemoveItemQuantity(const rapidjson::Value& elem);

	std::shared_ptr<Action> parsePlayerSelectSpell(const rapidjson::Value& elem);

	std::shared_ptr<Action> parsePlayerSetDefaultSpeed(const rapidjson::Value& elem);

	std::shared_ptr<Action> parsePlayerSetDirection(const rapidjson::Value& elem);

	std::shared_ptr<Action> parsePlayerSetProperty(const rapidjson::Value& elem);

	std::shared_ptr<Action> parsePlayerSetRestStatus(const rapidjson::Value& elem);

	std::shared_ptr<Action> parsePlayerSetTextureIndex(const rapidjson::Value& elem);

	std::shared_ptr<Action> parsePlayerWalk(const rapidjson::Value& elem);

	std::shared_ptr<Action> parsePlayerWalkToClick(const rapidjson::Value& elem);
}
