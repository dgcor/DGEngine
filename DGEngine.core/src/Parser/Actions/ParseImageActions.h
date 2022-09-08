#pragma once

#include "Game/Action.h"
#include "Json/JsonParser.h"
#include <memory>

namespace Parser::Actions
{
	std::shared_ptr<Action> parseImageEnableOutline(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseImageInverseResizeX(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseImageInverseResizeY(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseImageSetOutline(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseImageSetPalette(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseImageSetTexture(const rapidjson::Value& elem);

	std::shared_ptr<Action> parseImageSetTextureRect(const rapidjson::Value& elem);
}
