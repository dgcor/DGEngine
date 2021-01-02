#pragma once

#include "Action.h"
#include "Game.h"

class ActPaletteReplace : public Action
{
private:
	std::string idDstPal;
	std::string idSrcPal;
	size_t srcStart;
	size_t size;
	size_t dstStart;
	int step;
	bool stepReplace;

public:
	ActPaletteReplace(const std::string_view idDstPal_, const std::string_view idSrcPal_,
		size_t srcStart_, size_t size_, size_t dstStart_, bool stepReplace_) noexcept
		: idDstPal(idDstPal_), idSrcPal(idSrcPal_), srcStart(srcStart_),
		size(size_), dstStart(dstStart_), step((int)size_), stepReplace(stepReplace_) {}

	bool execute(Game& game) override
	{
		auto dstPal = game.Resources().getPalette(idDstPal);
		auto srcPal = game.Resources().getPalette(idSrcPal);
		if (dstPal != nullptr && srcPal != nullptr)
		{
			if (stepReplace == false)
			{
				dstPal->replaceRange(*srcPal, srcStart, size, dstStart);
			}
			else
			{
				if (step > 0)
				{
					step--;
					dstPal->replaceRange(*srcPal, srcStart, size - (size_t)step, dstStart);
				}
				if (step > 0)
				{
					return false;
				}
			}
		}
		return true;
	}
};

class ActPaletteShiftLeft : public Action
{
private:
	std::string id;
	size_t shift;
	std::pair<size_t, size_t> range;

public:
	ActPaletteShiftLeft(const std::string_view id_, size_t shift_,
		const std::pair<size_t, size_t>& range_) noexcept
		: id(id_), shift(shift_), range(range_) {}

	bool execute(Game& game) override
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
	ActPaletteShiftRight(const std::string_view id_, size_t shift_,
		const std::pair<size_t, size_t>& range_) noexcept
		: id(id_), shift(shift_), range(range_) {}

	bool execute(Game& game) override
	{
		auto palette = game.Resources().getPalette(id);
		if (palette != nullptr)
		{
			palette->shiftRight(shift, range.first, range.second);
		}
		return false;
	}
};
