#include "ParseAction2.h"
#include "Parser/Actions/ParseItemActions.h"
#include "Parser/Actions/ParseLevelActions.h"
#include "Parser/Actions/ParseLevelObjectActions.h"
#include "Parser/Actions/ParsePlayerActions.h"
#include "Parser/Actions/ParseQuestActions.h"
#include "Utils/StringHash.h"

namespace Parser2
{
	using namespace rapidjson;

	std::shared_ptr<Action> parseActionElem(Game& game, uint16_t nameHash16, const Value& elem)
	{
		switch (nameHash16)
		{
		case str2int16("item.addCursor"):
		{
			return Actions::parseItemAddCursor(elem);
		}
		case str2int16("item.addQuantity"):
		{
			return Actions::parseItemAddQuantity(elem);
		}
		case str2int16("item.delete"):
		{
			return Actions::parseItemDelete(elem);
		}
		case str2int16("item.drop"):
		{
			return Actions::parseItemDrop(elem);
		}
		case str2int16("item.executeAction"):
		{
			return Actions::parseItemExecuteAction(elem);
		}
		case str2int16("item.loadFromLevel"):
		{
			return Actions::parseItemLoadFromLevel(game, elem);
		}
		case str2int16("item.move"):
		{
			return Actions::parseItemMove(elem);
		}
		case str2int16("item.setProperty"):
		{
			return Actions::parseItemSetProperty(elem);
		}
		case str2int16("item.trade"):
		{
			return Actions::parseItemTrade(game, elem);
		}
		case str2int16("item.update"):
		{
			return Actions::parseItemUpdate(elem);
		}
		case str2int16("item.use"):
		{
			return Actions::parseItemUse(elem);
		}
		case str2int16("level.addLayer"):
		{
			return Actions::parseLevelAddLayer(elem);
		}
		case str2int16("level.anchorDrawable"):
		{
			return Actions::parseLevelAnchorDrawable(elem);
		}
		case str2int16("level.clearAllObjects"):
		{
			return Actions::parseLevelClearAllObjects(elem);
		}
		case str2int16("level.clearItems"):
		{
			return Actions::parseLevelClearItems(elem);
		}
		case str2int16("level.clearLevelObjects"):
		{
			return Actions::parseLevelClearLevelObjects(elem);
		}
		case str2int16("level.clearPlayerClasses"):
		{
			return Actions::parseLevelClearPlayerClasses(elem);
		}
		case str2int16("level.clearPlayers"):
		{
			return Actions::parseLevelClearPlayers(elem);
		}
		case str2int16("level.clearPlayerTextures"):
		{
			return Actions::parseLevelClearPlayerTextures(elem);
		}
		case str2int16("level.clearQuests"):
		{
			return Actions::parseLevelClearQuests(elem);
		}
		case str2int16("level.enableHover"):
		{
			return Actions::parseLevelEnableHover(elem);
		}
		case str2int16("level.move"):
		{
			return Actions::parseLevelMove(elem);
		}
		case str2int16("level.moveToClick"):
		{
			return Actions::parseLevelMoveToClick(elem);
		}
		case str2int16("level.moveToPlayer"):
		{
			return Actions::parseLevelMoveToPlayer(elem);
		}
		case str2int16("level.pause"):
		{
			return Actions::parseLevelPause(elem);
		}
		case str2int16("level.save"):
		{
			return Actions::parseLevelSave(elem);
		}
		case str2int16("level.setAutomap"):
		{
			return Actions::parseLevelSetAutomap(elem);
		}
		case str2int16("level.setAutomapPosition"):
		{
			return Actions::parseLevelSetAutomapPosition(elem);
		}
		case str2int16("level.setAutomapSize"):
		{
			return Actions::parseLevelSetAutomapSize(elem);
		}
		case str2int16("level.setShader"):
		{
			return Actions::parseLevelSetShader(elem);
		}
		case str2int16("level.setSmoothMovement"):
		{
			return Actions::parseLevelSetSmoothMovement(elem);
		}
		case str2int16("level.showAutomap"):
		{
			return Actions::parseLevelShowAutomap(elem);
		}
		case str2int16("level.updateDrawables"):
		{
			return Actions::parseLevelUpdateDrawables(elem);
		}
		case str2int16("level.zoom"):
		{
			return Actions::parseLevelZoom(elem);
		}
		case str2int16("level.zoomDrawables"):
		{
			return Actions::parseLevelZoomDrawables(elem);
		}
		case str2int16("levelObject.delete"):
		{
			return Actions::parseLevelObjDelete(elem);
		}
		case str2int16("levelObject.deleteByClass"):
		{
			return Actions::parseLevelObjDeleteByClass(elem);
		}
		case str2int16("levelObject.executeAction"):
		{
			return Actions::parseLevelObjExecuteAction(elem);
		}
		case str2int16("levelObject.setOutline"):
		{
			return Actions::parseLevelObjSetOutline(elem);
		}
		case str2int16("levelObject.setPalette"):
		{
			return Actions::parseLevelObjSetPalette(elem);
		}
		case str2int16("player.addItemQuantity"):
		{
			return Actions::parsePlayerAddItemQuantity(elem);
		}
		case str2int16("player.addToProperty"):
		{
			return Actions::parsePlayerAddToProperty(elem);
		}
		case str2int16("player.move"):
		{
			return Actions::parsePlayerMove(elem);
		}
		case str2int16("player.removeItemQuantity"):
		{
			return Actions::parsePlayerRemoveItemQuantity(elem);
		}
		case str2int16("player.selectSpell"):
		{
			return Actions::parsePlayerSelectSpell(elem);
		}
		case str2int16("player.setDefaultSpeed"):
		{
			return Actions::parsePlayerSetDefaultSpeed(elem);
		}
		case str2int16("player.setDirection"):
		{
			return Actions::parsePlayerSetDirection(elem);
		}
		case str2int16("player.setProperty"):
		{
			return Actions::parsePlayerSetProperty(elem);
		}
		case str2int16("player.setRestStatus"):
		{
			return Actions::parsePlayerSetRestStatus(elem);
		}
		case str2int16("player.setTextureIndex"):
		{
			return Actions::parsePlayerSetTextureIndex(elem);
		}
		case str2int16("player.walk"):
		{
			return Actions::parsePlayerWalk(elem);
		}
		case str2int16("player.walkToClick"):
		{
			return Actions::parsePlayerWalkToClick(elem);
		}
		case str2int16("quest.add"):
		{
			return Actions::parseQuestAdd(game, elem);
		}
		case str2int16("quest.delete"):
		{
			return Actions::parseQuestDelete(elem);
		}
		case str2int16("quest.setState"):
		{
			return Actions::parseQuestSetState(elem);
		}
		default:
			return nullptr;
		}
	}
}
