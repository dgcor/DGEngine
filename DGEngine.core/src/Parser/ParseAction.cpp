#include "ParseAction.h"
#include "Game/Actions/ActionList.h"
#include "Hooks.h"
#include "Parser/Actions/ParseActionActions.h"
#include "Parser/Actions/ParseAnimationActions.h"
#include "Parser/Actions/ParseAudioActions.h"
#include "Parser/Actions/ParseButtonActions.h"
#include "Parser/Actions/ParseConditionActions.h"
#include "Parser/Actions/ParseCursorActions.h"
#include "Parser/Actions/ParseDrawableActions.h"
#include "Parser/Actions/ParseEventActions.h"
#include "Parser/Actions/ParseFadeActions.h"
#include "Parser/Actions/ParseFocusActions.h"
#include "Parser/Actions/ParseFontActions.h"
#include "Parser/Actions/ParseGameActions.h"
#include "Parser/Actions/ParseImageActions.h"
#include "Parser/Actions/ParseInputTextActions.h"
#include "Parser/Actions/ParseIOActions.h"
#include "Parser/Actions/ParseLoadActions.h"
#include "Parser/Actions/ParseLoadingScreenActions.h"
#include "Parser/Actions/ParseMenuActions.h"
#include "Parser/Actions/ParseMountActions.h"
#include "Parser/Actions/ParseMovieActions.h"
#include "Parser/Actions/ParsePaletteActions.h"
#include "Parser/Actions/ParseRandomActions.h"
#include "Parser/Actions/ParseResourceActions.h"
#include "Parser/Actions/ParseScrollableActions.h"
#include "Parser/Actions/ParseShaderActions.h"
#include "Parser/Actions/ParseSoundActions.h"
#include "Parser/Actions/ParseTextActions.h"
#include "Parser/Actions/ParseVariableActions.h"
#include "Parser/Actions/ParseVisibilityActions.h"
#include "Utils/ParseUtils.h"
#include "Utils/StringHash.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	std::shared_ptr<Action> parseActionElem(Game& game, const Value& elem)
	{
		if (isValidString(elem, "name") == false)
		{
			return nullptr;
		}
		auto nameHash = str2int16(elem["name"sv].GetStringView());
		switch (nameHash)
		{
		case str2int16("=="):
		{
			return Actions::parseIfCondition(str2int16("=="), game, elem);
		}
		case str2int16("!="):
		{
			return Actions::parseIfCondition(str2int16("!="), game, elem);
		}
		case str2int16(">"):
		{
			return Actions::parseIfCondition(str2int16(">"), game, elem);
		}
		case str2int16(">="):
		{
			return Actions::parseIfCondition(str2int16(">="), game, elem);
		}
		case str2int16("<"):
		{
			return Actions::parseIfCondition(str2int16("<"), game, elem);
		}
		case str2int16("<="):
		{
			return Actions::parseIfCondition(str2int16("<="), game, elem);
		}
		case str2int16("action.set"):
		{
			return Actions::parseActionSet(game, elem);
		}
		case str2int16("animation.pause"):
		{
			return Actions::parseAnimationPause(elem);
		}
		case str2int16("animation.set"):
		{
			return Actions::parseAnimationSetAnimation(elem);
		}
		case str2int16("animation.setRefresh"):
		{
			return Actions::parseAnimationSetRefresh(elem);
		}
		case str2int16("audio.delete"):
		{
			return Actions::parseAudioDelete(elem);
		}
		case str2int16("audio.pause"):
		{
			return Actions::parseAudioPause(elem);
		}
		case str2int16("audio.pauseAll"):
		{
			return Actions::parseAudioPauseAll();
		}
		case str2int16("audio.play"):
		{
			return Actions::parseAudioPlay(elem);
		}
		case str2int16("audio.resumeAll"):
		{
			return Actions::parseAudioResumeAll(elem);
		}
		case str2int16("audio.seek"):
		{
			return Actions::parseAudioSeek(elem);
		}
		case str2int16("audio.setVolume"):
		{
			return Actions::parseAudioSetVolume(elem);
		}
		case str2int16("audio.stop"):
		{
			return Actions::parseAudioStop(elem);
		}
		case str2int16("audio.stopAll"):
		{
			return Actions::parseAudioStopAll();
		}
		case str2int16("button.click"):
		{
			return Actions::parseButtonClick(elem);
		}
		case str2int16("button.enable"):
		{
			return Actions::parseButtonEnable(elem);
		}
		case str2int16("button.rightClick"):
		{
			return Actions::parseButtonRightClick(elem);
		}
		case str2int16("button.setColor"):
		{
			return Actions::parseButtonSetColor(elem);
		}
		case str2int16("condition"):
		{
			return Actions::parseIfCondition(str2int16(getStringViewKey(elem, "condition")), game, elem);
		}
		case str2int16("cursor.centerOnDrawable"):
		{
			return Actions::parseCursorCenterOnDrawable(elem);
		}
		case str2int16("cursor.enableOutline"):
		{
			return Actions::parseCursorEnableOutline(elem);
		}
		case str2int16("cursor.pop"):
		{
			return Actions::parseCursorPop(elem);
		}
		case str2int16("cursor.setColor"):
		{
			return Actions::parseCursorSetColor(elem);
		}
		case str2int16("cursor.setOutline"):
		{
			return Actions::parseCursorSetOutline(elem);
		}
		case str2int16("cursor.setPalette"):
		{
			return Actions::parseCursorSetPalette(elem);
		}
		case str2int16("cursor.setPosition"):
		{
			return Actions::parseCursorSetPosition(elem);
		}
		case str2int16("cursor.show"):
		{
			return Actions::parseCursorShow(elem);
		}
		case str2int16("dir.copy"):
		{
			return Actions::parseDirCopy(elem);
		}
		case str2int16("dir.create"):
		{
			return Actions::parseDirCreate(elem);
		}
		case str2int16("drawable.addToPosition"):
		case str2int16("drawable.move"):
		{
			return Actions::parseDrawableAddToPosition(elem);
		}
		case str2int16("drawable.addToSize"):
		{
			return Actions::parseDrawableAddToSize(elem);
		}
		case str2int16("drawable.anchor"):
		{
			return Actions::parseDrawableAnchor(elem);
		}
		case str2int16("drawable.anchorSizeX"):
		{
			return Actions::parseDrawableAnchorSizeX(elem);
		}
		case str2int16("drawable.anchorSizeY"):
		{
			return Actions::parseDrawableAnchorSizeY(elem);
		}
		case str2int16("drawable.anchorToFocused"):
		{
			return Actions::parseDrawableAnchorToFocused(elem);
		}
		case str2int16("drawable.bringToFront"):
		{
			return Actions::parseDrawableBringToFront(elem);
		}
		case str2int16("drawable.center"):
		{
			return Actions::parseDrawableCenter(elem);
		}
		case str2int16("drawable.centerOnMouseX"):
		{
			return Actions::parseDrawableCenterOnMouseX(elem);
		}
		case str2int16("drawable.centerOnMouseY"):
		{
			return Actions::parseDrawableCenterOnMouseY(elem);
		}
		case str2int16("drawable.delete"):
		{
			return Actions::parseDrawableDelete(elem);
		}
		case str2int16("drawable.executeAction"):
		{
			return Actions::parseDrawableExecuteAction(elem);
		}
		case str2int16("drawable.horizontalAnchorToFocused"):
		{
			return Actions::parseDrawableHorizontalAnchorToFocused(elem);
		}
		case str2int16("drawable.moveX"):
		{
			return Actions::parseDrawableMoveX(elem);
		}
		case str2int16("drawable.moveY"):
		{
			return Actions::parseDrawableMoveY(elem);
		}
		case str2int16("drawable.moveStepX"):
		{
			return Actions::parseDrawableMoveStepX(elem);
		}
		case str2int16("drawable.moveStepY"):
		{
			return Actions::parseDrawableMoveStepY(elem);
		}
		case str2int16("drawable.resizeX"):
		{
			return Actions::parseDrawableResizeX(elem);
		}
		case str2int16("drawable.resizeY"):
		{
			return Actions::parseDrawableResizeY(elem);
		}
		case str2int16("drawable.resizeOnMouseX"):
		{
			return Actions::parseDrawableResizeOnMouseX(elem);
		}
		case str2int16("drawable.resizeOnMouseY"):
		{
			return Actions::parseDrawableResizeOnMouseY(elem);
		}
		case str2int16("drawable.resizeToPositionX"):
		{
			return Actions::parseDrawableResizeToPositionX(elem);
		}
		case str2int16("drawable.resizeToPositionY"):
		{
			return Actions::parseDrawableResizeToPositionY(elem);
		}
		case str2int16("drawable.sendToBack"):
		{
			return Actions::parseDrawableSendToBack(elem);
		}
		case str2int16("drawable.setAction"):
		{
			return Actions::parseDrawableSetAction(game, elem);
		}
		case str2int16("drawable.setPosition"):
		{
			return Actions::parseDrawableSetPosition(elem);
		}
		case str2int16("drawable.setPositionX"):
		{
			return Actions::parseDrawableSetPositionX(elem);
		}
		case str2int16("drawable.setPositionY"):
		{
			return Actions::parseDrawableSetPositionY(elem);
		}
		case str2int16("drawable.setSize"):
		{
			return Actions::parseDrawableSetSize(elem);
		}
		case str2int16("drawable.setSizeX"):
		{
			return Actions::parseDrawableSetSizeX(elem);
		}
		case str2int16("drawable.setSizeY"):
		{
			return Actions::parseDrawableSetSizeY(elem);
		}
		case str2int16("drawable.toggleVisible"):
		{
			return Actions::parseToggleVisible(elem);
		}
		case str2int16("drawable.verticalAnchorToFocused"):
		{
			return Actions::parseDrawableVerticalAnchorToFocused(elem);
		}
		case str2int16("drawable.visible"):
		{
			return Actions::parseSetVisible(elem);
		}
		case str2int16("event.add"):
		{
			return Actions::parseEventAdd(game, elem);
		}
		case str2int16("event.delete"):
		{
			return Actions::parseEventDelete(elem);
		}
		case str2int16("event.deleteAll"):
		{
			return Actions::parseEventDeleteAll();
		}
		case str2int16("event.resetTime"):
		{
			return Actions::parseEventResetTime(elem);
		}
		case str2int16("file.copy"):
		{
			return Actions::parseFileCopy(elem);
		}
		case str2int16("focus.add"):
		{
			return Actions::parseFocusAdd(elem);
		}
		case str2int16("focus.click"):
		{
			return Actions::parseFocusClick(elem);
		}
		case str2int16("focus.moveDown"):
		{
			return Actions::parseFocusMoveDown();
		}
		case str2int16("focus.moveUp"):
		{
			return Actions::parseFocusMoveUp();
		}
		case str2int16("focus.rightClick"):
		{
			return Actions::parseFocusRightClick(elem);
		}
		case str2int16("focus.set"):
		{
			return Actions::parseFocusSet(elem);
		}
		case str2int16("focus.update"):
		{
			return Actions::parseFocusUpdate(elem);
		}
		case str2int16("font.setColor"):
		case str2int16("font.setPalette"):
		{
			return Actions::parseFontSetPaletteOrColor(elem);
		}
		case str2int16("game.addToProperty"):
		{
			return Actions::parseGameAddToProperty(elem);
		}
		case str2int16("game.clearPlayingSounds"):
		{
			return Actions::parseGameClearPlayingSounds();
		}
		case str2int16("game.close"):
		{
			return Actions::parseGameClose();
		}
		case str2int16("game.draw"):
		{
			return Actions::parseGameDraw();
		}
		case str2int16("game.enableInput"):
		{
			return Actions::parseGameEnableInput(elem);
		}
		case str2int16("game.fadeIn"):
		{
			return Actions::parseGameFadeIn(game, elem);
		}
		case str2int16("game.fadeOut"):
		{
			return Actions::parseGameFadeOut(game, elem);
		}
		case str2int16("game.load"):
		{
			return Actions::parseGameLoad(elem);
		}
		case str2int16("game.setGamma"):
		{
			return Actions::parseGameSetGamma(elem);
		}
		case str2int16("game.setMusicVolume"):
		{
			return Actions::parseGameSetMusicVolume(elem);
		}
		case str2int16("game.setProperty"):
		{
			return Actions::parseGameSetProperty(elem);
		}
		case str2int16("game.setShader"):
		{
			return Actions::parseShaderSetGameShader(elem);
		}
		case str2int16("game.setSoundVolume"):
		{
			return Actions::parseGameSetSoundVolume(elem);
		}
		case str2int16("if"):
		{
			return Actions::parseMultiIfCondition(game, elem);
		}
		case str2int16("if.equal"):
		{
			return Actions::parseIfCondition(str2int16("=="), game, elem);
		}
		case str2int16("if.greater"):
		{
			return Actions::parseIfCondition(str2int16(">"), game, elem);
		}
		case str2int16("if.inList"):
		{
			return Actions::parseInListCondition(game, elem);
		}
		case str2int16("if.lower"):
		{
			return Actions::parseIfCondition(str2int16("<"), game, elem);
		}
		case str2int16("if.notEqual"):
		{
			return Actions::parseIfCondition(str2int16("!="), game, elem);
		}
		case str2int16("if.regex"):
		{
			return Actions::parseIfCondition(str2int16("regex"), game, elem);
		}
		case str2int16("image.enableOutline"):
		{
			return Actions::parseImageEnableOutline(elem);
		}
		case str2int16("image.inverseResizeX"):
		{
			return Actions::parseImageInverseResizeX(elem);
		}
		case str2int16("image.inverseResizeY"):
		{
			return Actions::parseImageInverseResizeY(elem);
		}
		case str2int16("image.setOutline"):
		{
			return Actions::parseImageSetOutline(elem);
		}
		case str2int16("image.setPalette"):
		{
			return Actions::parseImageSetPalette(elem);
		}
		case str2int16("image.setTexture"):
		{
			return Actions::parseImageSetTexture(elem);
		}
		case str2int16("image.setTextureRect"):
		{
			return Actions::parseImageSetTextureRect(elem);
		}
		case str2int16("inputText.click"):
		{
			return Actions::parseInputTextClick(elem);
		}
		case str2int16("io.delete"):
		{
			return Actions::parseIODelete(elem);
		}
		case str2int16("io.deleteAll"):
		{
			return Actions::parseIODeleteAll(elem);
		}
		case str2int16("load"):
		{
			return Actions::parseLoad(elem);
		}
		case str2int16("loadingScreen.clear"):
		{
			return Actions::parseLoadingScreenClear();
		}
		case str2int16("loadingScreen.setProgress"):
		{
			return Actions::parseLoadingScreenSetProgress(elem);
		}
		case str2int16("loadJson"):
		{
			return Actions::parseLoadJson(elem);
		}
		case str2int16("loadRandom"):
		{
			return Actions::parseLoadRandom(elem);
		}
		case str2int16("menu.appendText"):
		{
			return Actions::parseMenuAppendText(elem);
		}
		case str2int16("menu.click"):
		{
			return Actions::parseMenuClick(elem);
		}
		case str2int16("menu.clickVisible"):
		{
			return Actions::parseMenuClickVisible(elem);
		}
		case str2int16("menu.moveScrollbar"):
		{
			return Actions::parseMenuMoveScrollbar(elem);
		}
		case str2int16("menu.rightClick"):
		{
			return Actions::parseMenuRightClick(elem);
		}
		case str2int16("menu.rightClickVisible"):
		{
			return Actions::parseMenuRightClickVisible(elem);
		}
		case str2int16("menu.setColor"):
		{
			return Actions::parseMenuSetColor(elem);
		}
		case str2int16("menu.setFont"):
		{
			return Actions::parseMenuSetFont(elem);
		}
		case str2int16("menu.setIndex"):
		{
			return Actions::parseMenuSetIndex(elem);
		}
		case str2int16("menu.setText"):
		{
			return Actions::parseMenuSetText(elem);
		}
		case str2int16("mount"):
		{
			return Actions::parseMount(elem);
		}
		case str2int16("movie.pause"):
		{
			return Actions::parseMoviePause(elem);
		}
		case str2int16("movie.play"):
		{
			return Actions::parseMoviePlay(elem);
		}
		case str2int16("palette.replace"):
		{
			return Actions::parsePaletteReplace(elem);
		}
		case str2int16("palette.shiftLeft"):
		{
			return Actions::parsePaletteShiftLeft(elem);
		}
		case str2int16("palette.shiftRight"):
		{
			return Actions::parsePaletteShiftRight(elem);
		}
		case str2int16("random"):
		{
			return Actions::parseRandom(game, elem);
		}
		case str2int16("randomList"):
		{
			return Actions::parseRandomList(game, elem);
		}
		case str2int16("resource.add"):
		{
			return Actions::parseResourceAdd(elem);
		}
		case str2int16("resource.bringToFront"):
		{
			return Actions::parseResourceBringToFront(elem);
		}
		case str2int16("resource.ignore"):
		{
			return Actions::parseResourceIgnore(elem);
		}
		case str2int16("resource.pop"):
		{
			return Actions::parseResourcePop(elem);
		}
		case str2int16("resource.popAll"):
		{
			return Actions::parseResourcePopAll(elem);
		}
		case str2int16("scrollable.setSpeed"):
		{
			return Actions::parseScrollableSetSpeed(elem);
		}
		case str2int16("shader.setBool"):
		{
			return Actions::parseShaderSetBool(elem);
		}
		case str2int16("shader.setColor"):
		{
			return Actions::parseShaderSetColor(elem);
		}
		case str2int16("shader.setFloat"):
		{
			return Actions::parseShaderSetFloat(elem);
		}
		case str2int16("shader.setVec2"):
		{
			return Actions::parseShaderSetVec2(elem);
		}
		case str2int16("shader.setVec3"):
		{
			return Actions::parseShaderSetVec3(elem);
		}
		case str2int16("shader.setVec4"):
		{
			return Actions::parseShadersetVec4(elem);
		}
		case str2int16("sound.loadPlay"):
		{
			return Actions::parseSoundLoadPlay(elem);
		}
		case str2int16("sound.play"):
		{
			return Actions::parseSoundPlay(elem);
		}
		case str2int16("switch"):
		{
			return Actions::parseSwitchCondition(game, elem);
		}
		case str2int16("text.appendText"):
		{
			return Actions::parseTextAppendText(elem);
		}
		case str2int16("text.setColor"):
		{
			return Actions::parseTextSetColor(elem);
		}
		case str2int16("text.setFont"):
		{
			return Actions::parseTextSetFont(elem);
		}
		case str2int16("text.setSpacing"):
		{
			return Actions::parseTextSetSpacing(elem);
		}
		case str2int16("text.setText"):
		{
			return Actions::parseTextSetText(elem);
		}
		case str2int16("unmount"):
		{
			return Actions::parseUnmount(elem);
		}
		case str2int16("unmountAll"):
		{
			return Actions::parseUnmountAll();
		}
		case str2int16("variable.add"):
		{
			return Actions::parseVariableAdd(elem);
		}
		case str2int16("variable.clear"):
		{
			return Actions::parseVariableClear(elem);
		}
		case str2int16("variable.save"):
		{
			return Actions::parseVariableSave(elem);
		}
		case str2int16("variable.set"):
		{
			return Actions::parseVariableSet(elem);
		}
		case str2int16("variable.setIfNull"):
		{
			return Actions::parseVariableSetIfNull(elem);
		}
		default:
		{
			if (Hooks::ParseActionElem != nullptr)
			{
				return Hooks::ParseActionElem(game, nameHash, elem);
			}
			else
			{
				return nullptr;
			}
		}
		}
	}

	std::shared_ptr<Action> getActionVal(Game& game, const Value& elem)
	{
		if (elem.IsNull() == true)
		{
			return nullptr;
		}
		else if (isValidString(elem) == true)
		{
			if (elem.GetString()[0] != '#')
			{
				return game.Resources().getAction(elem.GetStringView());
			}
			else
			{
				return game.getQueryAction(elem.GetStringView().substr(1));
			}
		}
		else if (elem.IsObject() == true)
		{
			return parseActionElem(game, elem);
		}
		else if (elem.IsArray() == true)
		{
			auto actionList = std::make_shared<ActionList>();
			bool hasActions = false;
			for (const auto& val : elem)
			{
				auto action = getActionVal(game, val);
				if (action != nullptr)
				{
					actionList->add(action);
					hasActions = true;
				}
			}
			if (hasActions == true)
			{
				return actionList;
			}
		}
		return nullptr;
	}

	std::shared_ptr<Action> getActionKey(Game& game, const Value& elem, const std::string_view key)
	{
		if (elem.HasMember(key) == true)
		{
			return getActionVal(game, elem[key]);
		}
		return nullptr;
	}

	void parseActionAndExecute(Game& game, const Value& elem)
	{
		if (elem.IsNull() == true)
		{
			return;
		}
		else if (elem.IsString() == true)
		{
			auto action = game.Resources().getAction(elem.GetStringView());
			if (action != nullptr)
			{
				action->execute(game);
			}
			return;
		}
		else if (elem.IsObject() == true)
		{
			auto action = parseActionElem(game, elem);
			if (action != nullptr)
			{
				action->execute(game);
			}
			return;
		}
		else if (elem.IsArray() == true)
		{
			for (const auto& val : elem)
			{
				auto action = getActionVal(game, val);
				if (action != nullptr)
				{
					action->execute(game);
				}
			}
		}
	}
}
