#include "ParsePaletteActions.h"
#include "Game/Actions/ActPalette.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;

	std::shared_ptr<Action> parsePaletteReplace(const Value& elem)
	{
		return std::make_shared<ActPaletteReplace>(
			getStringViewKey(elem, "id"),
			getStringViewKey(elem, "idSource"),
			getUIntKey(elem, "srcStart", 0),
			getUIntKey(elem, "size", 256),
			getUIntKey(elem, "dstStart", 0),
			getBoolKey(elem, "stepReplace"));
	}

	std::shared_ptr<Action> parsePaletteShiftLeft(const Value& elem)
	{
		return std::make_shared<ActPaletteShiftLeft>(
			getStringViewKey(elem, "id"),
			getUIntKey(elem, "shift", 1),
			getVector2uKey<std::pair<size_t, size_t>>(elem, "range", { 0, 256 }));
	}

	std::shared_ptr<Action> parsePaletteShiftRight(const Value& elem)
	{
		return std::make_shared<ActPaletteShiftRight>(
			getStringViewKey(elem, "id"),
			getUIntKey(elem, "shift", 1),
			getVector2uKey<std::pair<size_t, size_t>>(elem, "range", { 0, 256 }));
	}
}
