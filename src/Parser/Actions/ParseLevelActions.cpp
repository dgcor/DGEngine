#include "ParseLevelActions.h"
#include "Game/Actions/ActLevel.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser2::Actions
{
	using namespace Parser;
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<Action> parseLevelAddLayer(const Value& elem)
	{
		if (elem.HasMember("color"sv) == true)
		{
			return std::make_shared<ActLevelAddColorLayer>(
				getStringViewKey(elem, "level"),
				getColorKey(elem, "color"),
				getFloatRectKey(elem, "offset"),
				getBoolKey(elem, "automap"));
		}
		else
		{
			return std::make_shared<ActLevelAddTextureLayer>(
				getStringViewKey(elem, "level"),
				getStringViewKey(elem, "texture"),
				getIntRectKey(elem, "textureRect"),
				getFloatRectKey(elem, "offset"),
				getBoolKey(elem, "automap"));
		}
	}

	std::shared_ptr<Action> parseLevelAnchorDrawable(const Value& elem)
	{
		return std::make_shared<ActLevelAnchorDrawable>(
			getStringViewKey(elem, "level"),
			getStringViewKey(elem, "drawable"),
			getStringViewKey(elem, "anchorTo"),
			getVector2fKey<sf::Vector2f>(elem, "offset"));
	}

	std::shared_ptr<Action> parseLevelClearAllObjects(const Value& elem)
	{
		return std::make_shared<ActLevelClearAllObjects>(
			getStringViewKey(elem, "level"),
			getStringVectorKey(elem, "exclude"));
	}

	std::shared_ptr<Action> parseLevelClearItems(const Value& elem)
	{
		return std::make_shared<ActLevelClearItems>(
			getStringViewKey(elem, "level"),
			getStringVectorKey(elem, "exclude"));
	}

	std::shared_ptr<Action> parseLevelClearLevelObjects(const Value& elem)
	{
		return std::make_shared<ActLevelClearLevelObjects>(
			getStringViewKey(elem, "level"),
			getStringVectorKey(elem, "exclude"));
	}

	std::shared_ptr<Action> parseLevelClearPlayerClasses(const Value& elem)
	{
		return std::make_shared<ActLevelClearPlayerClasses>(
			getStringViewKey(elem, "level"));
	}

	std::shared_ptr<Action> parseLevelClearPlayers(const Value& elem)
	{
		return std::make_shared<ActLevelClearPlayers>(
			getStringViewKey(elem, "level"),
			getStringVectorKey(elem, "exclude"));
	}

	std::shared_ptr<Action> parseLevelClearPlayerTextures(const Value& elem)
	{
		return std::make_shared<ActLevelClearPlayerTextures>(
			getStringViewKey(elem, "level"));
	}

	std::shared_ptr<Action> parseLevelClearQuests(const Value& elem)
	{
		return std::make_shared<ActLevelClearQuests>(
			getStringViewKey(elem, "level"));
	}

	std::shared_ptr<Action> parseLevelEnableHover(const Value& elem)
	{
		return std::make_shared<ActLevelEnableHover>(
			getStringViewKey(elem, "level"),
			getBoolKey(elem, "enable", true));
	}

	std::shared_ptr<Action> parseLevelMove(const Value& elem)
	{
		return std::make_shared<ActLevelMove>(
			getStringViewKey(elem, "level"),
			getVector2UnsignedNumberKey<PairFloat, float>(elem, "position"),
			getBoolKey(elem, "smooth"));
	}

	std::shared_ptr<Action> parseLevelMoveToClick(const Value& elem)
	{
		return std::make_shared<ActLevelMoveToClick>(
			getStringViewKey(elem, "level"),
			getBoolKey(elem, "smooth"));
	}

	std::shared_ptr<Action> parseLevelMoveToPlayer(const Value& elem)
	{
		return std::make_shared<ActLevelMoveToPlayer>(
			getStringViewKey(elem, "level"),
			getStringViewKey(elem, "player"),
			getBoolKey(elem, "smooth"));
	}

	std::shared_ptr<Action> parseLevelPause(const Value& elem)
	{
		return std::make_shared<ActLevelPause>(
			getStringViewKey(elem, "level"),
			getBoolKey(elem, "pause", true));
	}

	std::shared_ptr<Action> parseLevelSave(const Value& elem)
	{
		return std::make_shared<ActLevelSave>(
			getStringViewKey(elem, "level"),
			getStringViewKey(elem, "file"),
			getVariablesMapKey(elem, "properties"));
	}

	std::shared_ptr<Action> parseLevelSetAutomap(const Value& elem)
	{
		return std::make_shared<ActLevelSetAutomap>(
			getStringViewKey(elem, "level"),
			getStringViewKey(elem, "automap"),
			getVector2uKey<std::pair<uint32_t, uint32_t>>(elem, "tileSize", { 64u, 32u }),
			(uint16_t)getUIntKey(elem, "index"),
			getFloatRectKey(elem, "offset"));
	}

	std::shared_ptr<Action> parseLevelSetAutomapPosition(const Value& elem)
	{
		return std::make_shared<ActLevelSetAutomapPosition>(
			getStringViewKey(elem, "level"),
			getVector2fKey<sf::Vector2f>(elem, "position"));
	}

	std::shared_ptr<Action> parseLevelSetAutomapSize(const Value& elem)
	{
		return std::make_shared<ActLevelSetAutomapSize>(
			getStringViewKey(elem, "level"),
			getVector2fKey<sf::Vector2f>(elem, "size", { 100, 100 }));
	}

	std::shared_ptr<Action> parseLevelSetShader(const Value& elem)
	{
		return std::make_shared<ActLevelSetShader>(
			getStringViewKey(elem, "level"),
			getStringViewKey(elem, "shader"));
	}

	std::shared_ptr<Action> parseLevelSetSmoothMovement(const Value& elem)
	{
		return std::make_shared<ActLevelSetSmoothMovement>(
			getStringViewKey(elem, "level"),
			getBoolKey(elem, "smooth"));
	}

	std::shared_ptr<Action> parseLevelShowAutomap(const Value& elem)
	{
		return std::make_shared<ActLevelShowAutomap>(
			getStringViewKey(elem, "level"),
			getBoolKey(elem, "show", true));
	}

	std::shared_ptr<Action> parseLevelUpdateDrawables(const Value& elem)
	{
		return std::make_shared<ActLevelUpdateDrawables>(getStringViewKey(elem, "level"));
	}

	std::shared_ptr<Action> parseLevelZoom(const Value& elem)
	{
		return std::make_shared<ActLevelZoom>(
			getStringViewKey(elem, "level"),
			getIntKey(elem, "zoom", 100),
			getBoolKey(elem, "relative"),
			getBoolKey(elem, "smooth"));
	}

	std::shared_ptr<Action> parseLevelZoomDrawables(const Value& elem)
	{
		return std::make_shared<ActLevelZoomDrawables>(
			getStringViewKey(elem, "level"),
			getBoolKey(elem, "zoomDrawables", true));
	}
}
