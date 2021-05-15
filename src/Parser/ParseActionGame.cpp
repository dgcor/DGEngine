#include "ParseActionGame.h"
#include "Actions/ActItem.h"
#include "Actions/ActLevel.h"
#include "Actions/ActLevelObject.h"
#include "Actions/ActPlayer.h"
#include "Actions/ActQuest.h"
#include "ParseAction.h"
#include "Parser/Game/ParseQuest.h"
#include "Utils/ParseUtils.h"
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<Action> parseActionGameElem(Game& game,
		uint16_t nameHash16, const Value& elem)
	{
		switch (nameHash16)
		{
		case str2int16("item.addCursor"):
		{
			return std::make_shared<ActItemAddCursor>(
				getStringViewKey(elem, "level"),
				getItemLocationVal(elem));
		}
		case str2int16("item.addQuantity"):
		{
			return std::make_shared<ActItemAddQuantity>(
				getStringViewKey(elem, "level"),
				getItemLocationVal(elem),
				getVariableKey(elem, "value"),
				getBoolKey(elem, "remove"));
		}
		case str2int16("item.delete"):
		{
			return std::make_shared<ActItemDelete>(
				getStringViewKey(elem, "level"),
				getItemLocationVal(elem));
		}
		case str2int16("item.drop"):
		{
			return std::make_shared<ActItemDrop>(
				getStringViewKey(elem, "level"),
				getItemCoordInventoryVal(elem));
		}
		case str2int16("item.executeAction"):
		{
			return std::make_shared<ActItemExecuteAction>(
				getStringViewKey(elem, "level"),
				getItemLocationVal(elem),
				str2int16(getStringViewKey(elem, "action")));
		}
		case str2int16("item.loadFromLevel"):
		{
			auto action = std::make_shared<ActItemLoadFromLevel>(
				getStringViewKey(elem, "level"),
				getItemCoordInventoryVal(elem),
				getBoolKey(elem, "splitIntoMultiple"));

			if (elem.HasMember("position"sv) == true)
			{
				action->setInventoryPosition(getInventoryPositionVal(elem["position"sv]));
			}
			if (elem.HasMember("onInventoryFull"sv) == true)
			{
				action->setInventoryFullAction(getActionVal(game, elem["onInventoryFull"sv]));
			}
			return action;
		}
		case str2int16("item.move"):
		{
			return std::make_shared<ActItemMove>(
				getStringViewKey(elem, "level"),
				getItemLocationKey(elem, "from"),
				getItemLocationKey(elem, "to"));
		}
		case str2int16("item.setProperty"):
		{
			return std::make_shared<ActItemSetProperty>(
				getItemLocationVal(elem),
				getStringViewKey(elem, "level"),
				getStringViewKey(elem, "property"),
				getVariableKey(elem, "value"));
		}
		case str2int16("item.trade"):
		{
			auto action = std::make_shared<ActItemTrade>(
				getStringViewKey(elem, "level"),
				getStringViewKey(elem, "player"),
				getItemCoordInventoryKey(elem, "item"),
				getInventoryPositionKey(elem, "position"));

			if (elem.HasMember("onInventoryFull"sv) == true)
			{
				action->setInventoryFullAction(getActionVal(game, elem["onInventoryFull"sv]));
			}
			return action;
		}
		case str2int16("item.update"):
		{
			return std::make_shared<ActItemUpdate>(
				getStringViewKey(elem, "level"),
				getItemCoordInventoryVal(elem));
		}
		case str2int16("item.use"):
		{
			return std::make_shared<ActItemUse>(
				getStringViewKey(elem, "level"),
				getItemCoordInventoryVal(elem));
		}
		case str2int16("level.addLayer"):
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
		case str2int16("level.anchorDrawable"):
		{
			return std::make_shared<ActLevelAnchorDrawable>(
				getStringViewKey(elem, "level"),
				getStringViewKey(elem, "drawable"),
				getStringViewKey(elem, "anchorTo"),
				getVector2fKey<sf::Vector2f>(elem, "offset"));
		}
		case str2int16("level.clearAllObjects"):
		{
			return std::make_shared<ActLevelClearAllObjects>(
				getStringViewKey(elem, "level"),
				getStringVectorKey(elem, "exclude"));
		}
		case str2int16("level.clearItems"):
		{
			return std::make_shared<ActLevelClearItems>(
				getStringViewKey(elem, "level"),
				getStringVectorKey(elem, "exclude"));
		}
		case str2int16("level.clearLevelObjects"):
		{
			return std::make_shared<ActLevelClearLevelObjects>(
				getStringViewKey(elem, "level"),
				getStringVectorKey(elem, "exclude"));
		}
		case str2int16("level.clearPlayerClasses"):
		{
			return std::make_shared<ActLevelClearPlayerClasses>(
				getStringViewKey(elem, "level"));
		}
		case str2int16("level.clearPlayers"):
		{
			return std::make_shared<ActLevelClearPlayers>(
				getStringViewKey(elem, "level"),
				getStringVectorKey(elem, "exclude"));
		}
		case str2int16("level.clearPlayerTextures"):
		{
			return std::make_shared<ActLevelClearPlayerTextures>(
				getStringViewKey(elem, "level"));
		}
		case str2int16("level.clearQuests"):
		{
			return std::make_shared<ActLevelClearQuests>(
				getStringViewKey(elem, "level"));
		}
		case str2int16("level.enableHover"):
		{
			return std::make_shared<ActLevelEnableHover>(
				getStringViewKey(elem, "level"),
				getBoolKey(elem, "enable", true));
		}
		case str2int16("level.move"):
		{
			return std::make_shared<ActLevelMove>(
				getStringViewKey(elem, "level"),
				getVector2UnsignedNumberKey<PairFloat, float>(elem, "position"),
				getBoolKey(elem, "smooth"));
		}
		case str2int16("level.moveToClick"):
		{
			return std::make_shared<ActLevelMoveToClick>(
				getStringViewKey(elem, "level"),
				getBoolKey(elem, "smooth"));
		}
		case str2int16("level.moveToPlayer"):
		{
			return std::make_shared<ActLevelMoveToPlayer>(
				getStringViewKey(elem, "level"),
				getStringViewKey(elem, "player"),
				getBoolKey(elem, "smooth"));
		}
		case str2int16("level.pause"):
		{
			return std::make_shared<ActLevelPause>(
				getStringViewKey(elem, "level"),
				getBoolKey(elem, "pause", true));
		}
		case str2int16("level.save"):
		{
			return std::make_shared<ActLevelSave>(
				getStringViewKey(elem, "level"),
				getStringViewKey(elem, "file"),
				getVariablesMapKey(elem, "properties"));
		}
		case str2int16("level.setAutomap"):
		{
			return std::make_shared<ActLevelSetAutomap>(
				getStringViewKey(elem, "level"),
				getStringViewKey(elem, "automap"),
				getVector2uKey<std::pair<uint32_t, uint32_t>>(elem, "tileSize", { 64u, 32u }),
				(uint16_t)getUIntKey(elem, "index"),
				getFloatRectKey(elem, "offset"));
		}
		case str2int16("level.setAutomapPosition"):
		{
			return std::make_shared<ActLevelSetAutomapPosition>(
				getStringViewKey(elem, "level"),
				getVector2fKey<sf::Vector2f>(elem, "position"));
		}
		case str2int16("level.setAutomapSize"):
		{
			return std::make_shared<ActLevelSetAutomapSize>(
				getStringViewKey(elem, "level"),
				getVector2fKey<sf::Vector2f>(elem, "size", { 100, 100 }));
		}
		case str2int16("level.setShader"):
		{
			return std::make_shared<ActLevelSetShader>(
				getStringViewKey(elem, "level"),
				getStringViewKey(elem, "shader"));
		}
		case str2int16("level.setSmoothMovement"):
		{
			return std::make_shared<ActLevelSetSmoothMovement>(
				getStringViewKey(elem, "level"),
				getBoolKey(elem, "smooth"));
		}
		case str2int16("level.showAutomap"):
		{
			return std::make_shared<ActLevelShowAutomap>(
				getStringViewKey(elem, "level"),
				getBoolKey(elem, "show", true));
		}
		case str2int16("level.updateDrawables"):
		{
			return std::make_shared<ActLevelUpdateDrawables>(getStringViewKey(elem, "level"));
		}
		case str2int16("level.zoom"):
		{
			return std::make_shared<ActLevelZoom>(
				getStringViewKey(elem, "level"),
				getIntKey(elem, "zoom", 100),
				getBoolKey(elem, "relative"),
				getBoolKey(elem, "smooth"));
		}
		case str2int16("level.zoomDrawables"):
		{
			return std::make_shared<ActLevelZoomDrawables>(
				getStringViewKey(elem, "level"),
				getBoolKey(elem, "zoomDrawables", true));
		}
		case str2int16("levelObject.delete"):
		{
			return std::make_shared<ActLevelObjDelete>(
				getStringViewKey(elem, "level"),
				getStringViewKey(elem, "object"));
		}
		case str2int16("levelObject.deleteByClass"):
		{
			return std::make_shared<ActLevelObjDeleteByClass>(
				getStringViewKey(elem, "level"),
				getStringViewKey(elem, "class"));
		}
		case str2int16("levelObject.executeAction"):
		{
			return std::make_shared<ActLevelObjExecuteAction>(
				getStringViewKey(elem, "level"),
				getStringViewKey(elem, "object"));
		}
		case str2int16("levelObject.setOutline"):
		{
			return std::make_shared<ActLevelObjSetOutline>(
				getStringViewKey(elem, "level"),
				getColorKey(elem, "outline", sf::Color::Transparent),
				getColorKey(elem, "ignore", sf::Color::Transparent));
		}
		case str2int16("levelObject.setPalette"):
		{
			return std::make_shared<ActLevelObjSetPalette>(
				getStringViewKey(elem, "level"),
				getStringViewKey(elem, "palette"),
				getColorKey(elem, "color", sf::Color::White));
		}
		case str2int16("player.addItemQuantity"):
		{
			return std::make_shared<ActPlayerAddItemQuantity>(
				getStringViewKey(elem, "player"),
				getStringViewKey(elem, "level"),
				getStringViewKey(elem, "itemClass"),
				getInventoryPositionKey(elem, "position"),
				getVariableKey(elem, "value"),
				getBoolKey(elem, "remove"));
		}
		case str2int16("player.addToProperty"):
		{
			return std::make_shared<ActPlayerAddToProperty>(
				getStringViewKey(elem, "player"),
				getStringViewKey(elem, "level"),
				getStringViewKey(elem, "property"),
				getVariableKey(elem, "value"),
				getBoolKey(elem, "remove"));
		}
		case str2int16("player.move"):
		{
			return std::make_shared<ActPlayerMove>(
				getStringViewKey(elem, "player"),
				getStringViewKey(elem, "level"),
				getVector2UnsignedNumberKey<PairFloat, float>(elem, "position"),
				getBoolKey(elem, "resetDirection"),
				getBoolKey(elem, "smooth"));
		}
		case str2int16("player.removeItemQuantity"):
		{
			return std::make_shared<ActPlayerAddItemQuantity>(
				getStringViewKey(elem, "player"),
				getStringViewKey(elem, "level"),
				getStringViewKey(elem, "itemClass"),
				getInventoryPositionKey(elem, "position"),
				getVariableKey(elem, "value"),
				true);
		}
		case str2int16("player.selectSpell"):
		{
			return std::make_shared<ActPlayerSelectSpell>(
				getStringViewKey(elem, "player"),
				getStringViewKey(elem, "level"),
				getStringViewKey(elem, "spell"));
		}
		case str2int16("player.setDefaultSpeed"):
		{
			auto speed = getPlayerAnimationSpeedVal(elem);
			if (elem.HasMember("animation"sv) == false)
			{
				speed.animation = sf::Time::Zero;
			}
			if (elem.HasMember("walk"sv) == false)
			{
				speed.walk = sf::Time::Zero;
			}
			return std::make_shared<ActPlayerSetDefaultSpeed>(
				getStringViewKey(elem, "player"),
				getStringViewKey(elem, "level"),
				speed);
		}
		case str2int16("player.setDirection"):
		{
			return std::make_shared<ActPlayerSetDirection>(
				getStringViewKey(elem, "player"),
				getStringViewKey(elem, "level"),
				getPlayerDirectionKey(elem, "direction"));
		}
		case str2int16("player.setProperty"):
		{
			return std::make_shared<ActPlayerSetProperty>(
				getStringViewKey(elem, "player"),
				getStringViewKey(elem, "level"),
				getStringViewKey(elem, "property"),
				getVariableKey(elem, "value"));
		}
		case str2int16("player.setRestStatus"):
		{
			return std::make_shared<ActPlayerSetRestStatus>(
				getStringViewKey(elem, "player"),
				getStringViewKey(elem, "level"),
				(uint8_t)getUIntKey(elem, "status"));
		}
		case str2int16("player.setTextureIndex"):
		{
			return std::make_shared<ActPlayerSetTextureIndex>(
				getStringViewKey(elem, "player"),
				getStringViewKey(elem, "level"),
				getUIntKey(elem, "index"));
		}
		case str2int16("player.walk"):
		{
			return std::make_shared<ActPlayerWalk>(
				getStringViewKey(elem, "player"),
				getStringViewKey(elem, "level"),
				getPlayerDirectionKey(elem, "direction"),
				getBoolKey(elem, "executeAction"));
		}
		case str2int16("player.walkToClick"):
		{
			return std::make_shared<ActPlayerWalkToClick>(
				getStringViewKey(elem, "player"),
				getStringViewKey(elem, "level"),
				getBoolKey(elem, "executeAction", true));
		}
		case str2int16("quest.add"):
		{
			if (elem.HasMember("quest"sv) == true)
			{
				auto quest = parseQuestObj(game, elem["quest"sv]);
				if (isValidId(quest.Id()) == false)
				{
					return nullptr;
				}
				return std::make_shared<ActQuestAdd>(
					getStringViewKey(elem, "level"),
					std::move(quest));
			}
			return nullptr;
		}
		case str2int16("quest.delete"):
		{
			return std::make_shared<ActQuestDelete>(
				getStringViewKey(elem, "level"),
				getStringViewKey(elem, "quest"));
		}
		case str2int16("quest.setState"):
		{
			return std::make_shared<ActQuestSetState>(
				getStringViewKey(elem, "level"),
				getStringViewKey(elem, "quest"),
				getIntKey(elem, "state"));
		}
		default:
			return nullptr;
		}
	}
}
