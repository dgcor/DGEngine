#pragma once

#include "Json/JsonParser.h"
#include <memory>
#include <SFML/System/Vector2.hpp>

class Game;
class ResourceManager;
class UIObject;

namespace Parser
{
	// sets anchor, position, size and visible
	void parseDrawableProperties(Game& game, const rapidjson::Value& elem, UIObject& obj);

	// sets anchor, position, size and visible
	void parseDrawableProperties(Game& game, const rapidjson::Value& elem, UIObject& obj, sf::Vector2f& size);

	// adds a drawable to an existing panel, if the panel element is present
	bool parseDrawablePanel(const ResourceManager& resources, const rapidjson::Value& elem,
		const std::shared_ptr<UIObject>& drawable);

	void parseDrawablePanelAndAdd(ResourceManager& resources, const rapidjson::Value& elem,
		const std::string_view id, const std::shared_ptr<UIObject>& obj);

	typedef std::shared_ptr<UIObject>(*ParseDrawableFuncPtr)(Game&, const rapidjson::Value&);

	void parseValidIdAndAddDrawable(Game& game, const rapidjson::Value& elem, const ParseDrawableFuncPtr parseDrawableFunc);

	void parseValidIdFileAndAddDrawable(Game& game, const rapidjson::Value& elem, const ParseDrawableFuncPtr parseDrawableFunc);
}
