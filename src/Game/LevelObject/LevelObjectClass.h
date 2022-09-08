#pragma once

#include "Game/Action.h"
#include "Game/LightSource.h"
#include <memory>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <vector>

class LevelObjectClass
{
protected:
	std::vector<std::pair<uint16_t, std::shared_ptr<Action>>> actions;

	std::string id;
	uint16_t idHash16{ 0 };

	LightSource lightSource;
	sf::Vector2f anchorOffset;

public:
	virtual ~LevelObjectClass() = default;

	void Id(const std::string_view id_);
	auto& Id() const noexcept { return id; }
	auto IdHash16() const noexcept { return idHash16; }

	auto getLightSource() const noexcept { return lightSource; }
	void setLightSource(LightSource lightSource_) noexcept { lightSource = lightSource_; }

	auto& getAnchorOffset() const noexcept { return anchorOffset; }
	void setAnchorOffset(const sf::Vector2f& offset_) noexcept { anchorOffset = offset_; }

	std::shared_ptr<Action> getAction(uint16_t nameHash16) const noexcept;
	void setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action_);

	void executeAction(Game& game, uint16_t nameHash16, bool executeNow = false) const;
};
