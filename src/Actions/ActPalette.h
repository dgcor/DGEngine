#pragma once

#include "Action.h"
#include "FadeInOut.h"
#include "Game.h"
#include <memory>
#include <string>

class ActPaletteShiftLeft : public Action
{
private:
	std::string id;
	size_t shift;
	std::pair<size_t, size_t> range;

public:
	ActPaletteShiftLeft(const std::string& id_, size_t shift_,
		const std::pair<size_t, size_t>& range_) noexcept
		: id(id_), shift(shift_), range(range_) {}

	virtual bool execute(Game& game)
	{
		auto palette = game.Resources().getPalette(id);
		if (palette != nullptr)
		{
			palette->shiftLeft(shift, range.first, range.second);
		}
		return false;
	}
};

class ActPaletteShiftRight : public Action
{
private:
	std::string id;
	size_t shift;
	std::pair<size_t, size_t> range;

public:
	ActPaletteShiftRight(const std::string& id_, size_t shift_,
		const std::pair<size_t, size_t>& range_) noexcept
		: id(id_), shift(shift_), range(range_) {}

	virtual bool execute(Game& game)
	{
		auto palette = game.Resources().getPalette(id);
		if (palette != nullptr)
		{
			palette->shiftRight(shift, range.first, range.second);
		}
		return false;
	}
};
