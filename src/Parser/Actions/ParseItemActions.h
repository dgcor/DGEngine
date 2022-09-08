#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser2::Actions
{
	std::shared_ptr<Action> parseItemAddCursor(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseItemAddQuantity(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseItemDelete(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseItemDrop(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseItemExecuteAction(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseItemLoadFromLevel(Game& game, const rapidjson::Value& elem);

	std::shared_ptr<Action> parseItemMove(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseItemSetProperty(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseItemTrade(Game& game, const rapidjson::Value& elem);

	std::shared_ptr<Action> parseItemUpdate(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseItemUse(const rapidjson::Value& elem);
}
