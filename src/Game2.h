#pragma once

#include "Game.h"

class Game2 : public Game
{
public:
	bool getGameProperty(const std::string_view prop1,
		const std::string_view prop2, Variable& var) const override;

	void setGameProperty(const std::string_view prop, const Variable& val) override;

	bool getQueryableList(const std::string_view prop1, const std::string_view prop2,
		std::vector<std::variant<const Queryable*, Variable>>& queryableList) const override;
};
