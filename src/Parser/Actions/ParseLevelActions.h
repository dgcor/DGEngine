#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser2::Actions
{
	std::shared_ptr<Action> parseLevelAddLayer(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLevelAnchorDrawable(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLevelClearAllObjects(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLevelClearItems(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLevelClearLevelObjects(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLevelClearPlayerClasses(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLevelClearPlayers(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLevelClearPlayerTextures(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLevelClearQuests(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLevelEnableHover(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLevelMove(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLevelMoveToClick(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLevelMoveToPlayer(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLevelPause(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLevelSave(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLevelSetAutomap(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLevelSetAutomapPosition(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLevelSetAutomapSize(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLevelSetShader(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLevelSetSmoothMovement(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLevelShowAutomap(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLevelUpdateDrawables(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLevelZoom(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseLevelZoomDrawables(const rapidjson::Value& elem);
}
