#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser::Actions
{
	std::shared_ptr<Action> parseDrawableAddToPosition(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableAddToSize(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableAnchor(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableAnchorSizeX(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableAnchorSizeY(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableAnchorToFocused(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableBringToFront(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableCenter(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableCenterOnMouseX(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableCenterOnMouseY(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableDelete(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableExecuteAction(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableHorizontalAnchorToFocused(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableMoveX(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableMoveY(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableMoveStepX(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableMoveStepY(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableResizeX(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableResizeY(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableResizeOnMouseX(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableResizeOnMouseY(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableResizeToPositionX(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableResizeToPositionY(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableSendToBack(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableSetAction(Game& game, const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableSetPosition(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableSetPositionX(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableSetPositionY(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableSetSize(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableSetSizeX(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableSetSizeY(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseDrawableVerticalAnchorToFocused(const rapidjson::Value& elem);
}
