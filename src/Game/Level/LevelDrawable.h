#pragma once

#include <memory>
#include <SFML/System/Vector2.hpp>
#include <string>

class LevelObject;
class Panel;

struct LevelDrawable
{
	std::string id;
	std::weak_ptr<Panel> drawable;
	std::weak_ptr<LevelObject> anchorTo;
	sf::Vector2f offset;
	float visibleRectOffset{ -1 };
};
