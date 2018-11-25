#pragma once

#include "Action.h"
#include "FadeInOut.h"
#include "Game.h"
#include <memory>
#include <string>

class ActPaletteReplace : public Action
{
private:
	std::string idDstPal;
	std::string idSrcPal;
	size_t srcStart;
	size_t size;
	size_t dstStart;

public:
	ActPaletteReplace(const std::string& idDstPal_, const std::string& idSrcPal_,
		size_t srcStart_, size_t size_, size_t dstStart_) noexcept
		: idDstPal(idDstPal_), idSrcPal(idSrcPal_), srcStart(srcStart_),
		size(size_), dstStart(dstStart_) {}

	virtual bool execute(Game& game)
	{
		auto dstPal = game.Resources().getPalette(idDstPal);
		auto srcPal = game.Resources().getPalette(idSrcPal);
		if (dstPal != nullptr && srcPal != nullptr)
		{
			dstPal->replaceRange(*srcPal, srcStart, size, dstStart);
		}
		return false;
	}
};

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
