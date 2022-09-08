#pragma once

#include "LevelDrawable.h"
#include <vector>

class Game;
class Level;

class LevelDrawableManager
{
private:
	std::vector<LevelDrawable> drawables;

	void setPosition(const Level& level, LevelDrawable& obj, Panel& panelObj);

public:
	auto begin() noexcept { return drawables.begin(); }
	auto end() noexcept { return drawables.end(); }
	auto begin() const noexcept { return drawables.begin(); }
	auto end() const noexcept { return drawables.end(); }
	auto cbegin() const noexcept { return drawables.cbegin(); }
	auto cend() const noexcept { return drawables.cend(); }
	auto rbegin() noexcept { return drawables.rbegin(); }
	auto rend() noexcept { return drawables.rend(); }
	auto rbegin() const noexcept { return drawables.rbegin(); }
	auto rend() const noexcept { return drawables.rend(); }
	auto crbegin() const noexcept { return drawables.crbegin(); }
	auto crend() const noexcept { return drawables.crend(); }

	auto empty() const noexcept { return drawables.empty(); }
	auto size() const noexcept { return drawables.size(); }

	void add(const Level& level, LevelDrawable obj);

	LevelDrawable* get(const std::string_view id);

	std::shared_ptr<Panel> getPanel(size_t idx) const;

	void update(const Level& level, Game& game);
};
