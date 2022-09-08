#include "ParseLevelMap.h"
#include "Game/Game.h"
#include "Game/Level/Level.h"
#include "Game/Level/LevelMap.h"
#include "ParseLevelLayer.h"
#include "Parser/Utils/ParseUtils.h"
#include "Parser/Utils/ParseUtilsGameKey.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	void parseLevelAutomap(Game& game, Level& level, const Value& elem)
	{
		if (isValidString(elem, "automap") == true)
		{
			auto automap = game.Resources().getTexturePack(elem["automap"sv].GetStringView());
			if (automap != nullptr)
			{
				auto index = getLayerIndex(elem);
				auto defaultTile = level.Map().getTileBlock((int16_t)getIntKey(elem, "outOfBoundsTile", -1));
				auto automapTileSize = getVector2uKey<std::pair<uint32_t, uint32_t>>(
					elem, "automapTileSize", std::make_pair(64u, 32u));

				level.setAutomap(
					{ automap, index, defaultTile },
					automapTileSize.first,
					automapTileSize.second,
					getFloatRectKey(elem, "automapOffset")
				);
			}
		}
		if (elem.HasMember("automapRelativeCoords"sv) == true)
		{
			level.setAutomapRelativeCoords(getBoolVal(elem["automapRelativeCoords"sv]));
		}
		if (elem.HasMember("automapAnchor"sv) == true)
		{
			level.setAutomapAnchor(getAnchorVal(elem["automapAnchor"sv]));
		}
		if (elem.HasMember("automapSize"sv) == true)
		{
			auto size = getVector2fVal<sf::Vector2f>(elem["automapSize"sv], { 100.f, 100.f });
			level.setAutomapSize(size);
		}
		if (elem.HasMember("automapPosition"sv) == true)
		{
			auto anchor = level.getAutomapAnchor();
			auto size = level.getAutomapSize();
			auto pos = getVector2fVal<sf::Vector2f>(elem["automapPosition"sv]);
			if (level.getAutomapRelativeCoords() == false &&
				game.RefSize() != game.DrawRegionSize())
			{
				GameUtils::setAnchorPosSize(anchor, pos, size, game.RefSize(), game.DrawRegionSize());
			}
			level.setAutomapPosition(pos);
		}
		if (elem.HasMember("automapPlayerDirectionIndex"sv) == true)
		{
			auto index = (int16_t)getIntVal(elem["automapPlayerDirectionIndex"sv], -1);
			level.setAutomapPlayerDirectionBaseIndex(index);
		}
		if (elem.HasMember("showAutomap"sv) == true)
		{
			level.ShowAutomap(getBoolVal(elem["showAutomap"sv]));
		}
	}
}
