#include "ParseAction.h"
#include "Actions/ActAction.h"
#include "Actions/ActAnimation.h"
#include "Actions/ActAudio.h"
#include "Actions/ActButton.h"
#include "Actions/ActCondition.h"
#include "Actions/ActCursor.h"
#include "Actions/ActDrawable.h"
#include "Actions/ActEvent.h"
#include "Actions/ActFade.h"
#include "Actions/ActFocus.h"
#include "Actions/ActFont.h"
#include "Actions/ActGame.h"
#include "Actions/ActImage.h"
#include "Actions/ActInputText.h"
#include "Actions/ActIO.h"
#include "Actions/ActiontList.h"
#include "Actions/ActLoad.h"
#include "Actions/ActLoadingScreen.h"
#include "Actions/ActMenu.h"
#include "Actions/ActMount.h"
#include "Actions/ActMovie.h"
#include "Actions/ActPalette.h"
#include "Actions/ActRandom.h"
#include "Actions/ActResource.h"
#include "Actions/ActScrollable.h"
#include "Actions/ActShader.h"
#include "Actions/ActSound.h"
#include "Actions/ActText.h"
#include "Actions/ActVariable.h"
#include "Actions/ActVisibility.h"
#include "Hooks.h"
#include "Json/JsonUtils.h"
#include "ParseCondition.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	template <class T>
	std::shared_ptr<Action> parseSetTextHelper(Game& game, const Value& elem)
	{
		std::shared_ptr<T> action;
		if (elem.HasMember("binding"sv) == true)
		{
			action = std::make_shared<T>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "format"),
				getStringVectorKey(elem, "binding"));
		}
		else if (elem.HasMember("query"sv) == true)
		{
			action = std::make_shared<T>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "text"),
				getStringViewKey(elem, "query"));
		}
		else
		{
			auto textOp = TextUtils::TextOp::Replace;
			if (getBoolKey(elem, "set") == true)
			{
				textOp = TextUtils::TextOp::Set;
			}
			action = std::make_shared<T>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "text"),
				textOp);
		}
		if (getBoolKey(elem, "replaceAll") == true)
		{
			action->ReplaceAll();
		}
		if (getBoolKey(elem, "removeEmptyLines") == true)
		{
			action->RemoveEmptyLines();
		}
		if (getBoolKey(elem, "trim") == true)
		{
			action->Trim();
		}
		return action;
	}

	template <class T>
	std::shared_ptr<Action> parseSetMenuTextHelper(Game& game, const Value& elem)
	{
		auto index = getUIntKey(elem, "index");
		std::shared_ptr<T> action;
		if (elem.HasMember("binding"sv) == true)
		{
			action = std::make_shared<T>(
				getStringViewKey(elem, "id"),
				index,
				getStringViewKey(elem, "format"),
				getStringVectorKey(elem, "binding"));
		}
		else if (elem.HasMember("query"sv) == true)
		{
			action = std::make_shared<T>(
				getStringViewKey(elem, "id"),
				index,
				getStringViewKey(elem, "text"),
				getStringViewKey(elem, "query"));
		}
		else
		{
			auto textOp = TextUtils::TextOp::Replace;
			if (getBoolKey(elem, "set") == true)
			{
				textOp = TextUtils::TextOp::Set;
			}
			else if (getBoolKey(elem, "replaceAll") == true)
			{
				textOp = TextUtils::TextOp::ReplaceAll;
			}
			action = std::make_shared<T>(
				getStringViewKey(elem, "id"),
				index,
				getStringViewKey(elem, "text"),
				textOp);
		}
		if (getBoolKey(elem, "removeEmptyLines") == true)
		{
			action->RemoveEmptyLines();
		}
		if (getBoolKey(elem, "trim") == true)
		{
			action->Trim();
		}
		return action;
	}

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
			return getIfCondition(str2int16("=="), game, elem);
		}
		case str2int16("!="):
		{
			return getIfCondition(str2int16("!="), game, elem);
		}
		case str2int16(">"):
		{
			return getIfCondition(str2int16(">"), game, elem);
		}
		case str2int16(">="):
		{
			return getIfCondition(str2int16(">="), game, elem);
		}
		case str2int16("<"):
		{
			return getIfCondition(str2int16("<"), game, elem);
		}
		case str2int16("<="):
		{
			return getIfCondition(str2int16("<="), game, elem);
		}
		case str2int16("action.set"):
		{
			return std::make_shared<ActActionSet>(
				getStringViewKey(elem, "id"),
				getActionKey(game, elem, "action"));
		}
		case str2int16("animation.pause"):
		{
			return std::make_shared<ActAnimationPause>(
				getStringViewKey(elem, "id"),
				getBoolKey(elem, "pause", true),
				getBoolKey(elem, "reset"));
		}
		case str2int16("animation.set"):
		{
			return std::make_shared<ActAnimationSetAnimation>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "texturePack"),
				getIntKey(elem, "group", -1),
				getIntKey(elem, "direction", -1),
				getTimeKey(elem, "refresh"),
				getBoolKey(elem, "reset", true),
				getBoolKey(elem, "updateAnimationType"));
		}
		case str2int16("animation.setRefresh"):
		{
			return std::make_shared<ActAnimationSetRefresh>(
				getStringViewKey(elem, "id"),
				getTimeKey(elem, "refresh"));
		}
		case str2int16("audio.delete"):
		{
			return std::make_shared<ActAudioDelete>(getStringViewKey(elem, "id"));
		}
		case str2int16("audio.pause"):
		{
			return std::make_shared<ActAudioPause>(getStringViewKey(elem, "id"));
		}
		case str2int16("audio.pauseAll"):
		{
			return std::make_shared<ActAudioPauseAll>();
		}
		case str2int16("audio.play"):
		{
			auto action = std::make_shared<ActAudioPlay>(
				getStringViewKey(elem, "id"),
				getVariableKey(elem, "volume"),
				getBoolKey(elem, "clear"));

			if (elem.HasMember("loop"sv) == true)
			{
				action->setLoop(getBoolVal(elem["loop"sv]));
			}
			return action;
		}
		case str2int16("audio.resumeAll"):
		{
			return std::make_shared<ActAudioResumeAll>(getBoolKey(elem, "restart"));
		}
		case str2int16("audio.seek"):
		{
			return std::make_shared<ActAudioSeek>(
				getStringViewKey(elem, "id"),
				getTimeKey(elem, "time"));
		}
		case str2int16("audio.setVolume"):
		{
			return std::make_shared<ActAudioSetVolume>(
				getStringViewKey(elem, "id"),
				getVariableKey(elem, "volume"));
		}
		case str2int16("audio.stop"):
		{
			return std::make_shared<ActAudioStop>(getStringViewKey(elem, "id"));
		}
		case str2int16("audio.stopAll"):
		{
			return std::make_shared<ActAudioStopAll>();
		}
		case str2int16("button.click"):
		{
			return std::make_shared<ActButtonClick>(
				getStringViewKey(elem, "id"),
				getBoolKey(elem, "playSound", true));
		}
		case str2int16("button.enable"):
		{
			return std::make_shared<ActButtonEnable>(
				getStringViewKey(elem, "id"),
				getBoolKey(elem, "enable", true));
		}
		case str2int16("button.rightClick"):
		{
			return std::make_shared<ActButtonRightClick>(
				getStringViewKey(elem, "id"),
				getBoolKey(elem, "playSound", true));
		}
		case str2int16("button.setColor"):
		{
			return std::make_shared<ActButtonSetColor>(
				getStringViewKey(elem, "id"),
				getColorKey(elem, "color", sf::Color::White));
		}
		case str2int16("condition"):
		{
			return getIfCondition(
				str2int16(getStringViewKey(elem, "condition")),
				game, elem);
		}
		case str2int16("cursor.centerOnDrawable"):
		{
			return std::make_shared<ActCursorCenterOnDrawable>(
				getStringViewKey(elem, "id"));
		}
		case str2int16("cursor.enableOutline"):
		{
			return std::make_shared<ActCursorEnableOutline>(
				getBoolKey(elem, "enable", true));
		}
		case str2int16("cursor.pop"):
		{
			return std::make_shared<ActCursorPop>(getBoolKey(elem, "popAll"));
		}
		case str2int16("cursor.setColor"):
		{
			return std::make_shared<ActCursorSetColor>(
				getColorKey(elem, "color", sf::Color::White));
		}
		case str2int16("cursor.setOutline"):
		{
			return std::make_shared<ActCursorSetOutline>(
				getColorKey(elem, "outline", sf::Color::Transparent),
				getColorKey(elem, "ignore", sf::Color::Transparent));
		}
		case str2int16("cursor.setPalette"):
		{
			return std::make_shared<ActCursorSetPalette>(
				getStringViewKey(elem, "palette"),
				getColorKey(elem, "color", sf::Color::White));
		}
		case str2int16("cursor.setPosition"):
		{
			return std::make_shared<ActCursorSetPosition>(
				getVector2iKey<sf::Vector2i>(elem, "position"));
		}
		case str2int16("cursor.show"):
		{
			return std::make_shared<ActCursorShow>(getBoolKey(elem, "show"));
		}
		case str2int16("dir.copy"):
		{
			return std::make_shared<ActDirCopy>(
				getStringViewKey(elem, "source"),
				getStringViewKey(elem, "destination"));
		}
		case str2int16("dir.create"):
		{
			return std::make_shared<ActDirCreate>(getStringViewKey(elem, "file"));
		}
		case str2int16("drawable.addToPosition"):
		case str2int16("drawable.move"):
		{
			return std::make_shared<ActDrawableAddToPosition>(
				getStringViewKey(elem, "id"),
				getVector2fKey<sf::Vector2f>(elem, "offset"));
		}
		case str2int16("drawable.addToSize"):
		{
			return std::make_shared<ActDrawableAddToSize>(
				getStringViewKey(elem, "id"),
				getVector2fKey<sf::Vector2f>(elem, "offset"));
		}
		case str2int16("drawable.anchor"):
		{
			return std::make_shared<ActDrawableAnchor>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "idAnchor"),
				getAnchorKey(elem, "anchor"),
				getVector2fKey<sf::Vector2f>(elem, "offset"));
		}
		case str2int16("drawable.anchorSizeX"):
		{
			return std::make_shared<ActDrawableAnchorSizeXY>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "idAnchorTo"),
				(float)getIntKey(elem, "offset"),
				false);
		}
		case str2int16("drawable.anchorSizeY"):
		{
			return std::make_shared<ActDrawableAnchorSizeXY>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "idAnchorTo"),
				(float)getIntKey(elem, "offset"),
				true);
		}
		case str2int16("drawable.anchorToFocused"):
		{
			return std::make_shared<ActDrawableAnchorToFocused>(
				getStringViewKey(elem, "id"),
				getAnchorKey(elem, "anchor"),
				getVector2fKey<sf::Vector2f>(elem, "offset"));
		}
		case str2int16("drawable.bringToFront"):
		{
			return std::make_shared<ActDrawableBringToFront>(getStringViewKey(elem, "id"));
		}
		case str2int16("drawable.center"):
		{
			return std::make_shared<ActDrawableCenter>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "idCenterOn"),
				getVector2fKey<sf::Vector2f>(elem, "offset"));
		}
		case str2int16("drawable.centerOnMouseX"):
		{
			return std::make_shared<ActDrawableCenterOnMouseX>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "idAnchorTo"),
				getUIntKey(elem, "range"),
				getVariableKey(elem, "steps"));
		}
		case str2int16("drawable.centerOnMouseY"):
		{
			return std::make_shared<ActDrawableCenterOnMouseY>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "idAnchorTo"),
				getUIntKey(elem, "range"),
				getVariableKey(elem, "steps"));
		}
		case str2int16("drawable.delete"):
		{
			return std::make_shared<ActDrawableDelete>(getStringViewKey(elem, "id"));
		}
		case str2int16("drawable.executeAction"):
		{
			return std::make_shared<ActDrawableExecuteAction>(
				getStringViewKey(elem, "id"),
				str2int16(getStringViewKey(elem, "action")));
		}
		case str2int16("drawable.horizontalAnchorToFocused"):
		{
			return std::make_shared<ActDrawableHorizontalAnchorToFocused>(
				getStringViewKey(elem, "id"),
				(float)getIntKey(elem, "offset"));
		}
		case str2int16("drawable.moveX"):
		{
			return std::make_shared<ActDrawableMoveX>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "idAnchorTo"),
				getUIntKey(elem, "range"),
				getVariableKey(elem, "position"),
				getUIntKey(elem, "min"),
				getUIntKey(elem, "max"),
				getVariableKey(elem, "steps"));
		}
		case str2int16("drawable.moveY"):
		{
			return std::make_shared<ActDrawableMoveY>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "idAnchorTo"),
				getUIntKey(elem, "range"),
				getVariableKey(elem, "position"),
				getUIntKey(elem, "min"),
				getUIntKey(elem, "max"),
				getVariableKey(elem, "steps"));
		}
		case str2int16("drawable.moveStepX"):
		{
			return std::make_shared<ActDrawableMoveStepX>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "idAnchorTo"),
				getUIntKey(elem, "range"),
				getVariableKey(elem, "steps"),
				getIntKey(elem, "stepOffset"));
		}
		case str2int16("drawable.moveStepY"):
		{
			return std::make_shared<ActDrawableMoveStepY>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "idAnchorTo"),
				getUIntKey(elem, "range"),
				getVariableKey(elem, "steps"),
				getIntKey(elem, "stepOffset"));
		}
		case str2int16("drawable.resizeX"):
		{
			return std::make_shared<ActDrawableResizeXY>(
				getStringViewKey(elem, "id"),
				getVariableKey(elem, "size"),
				getVariableKey(elem, "inputRangeMin"),
				getVariableKey(elem, "inputRangeMax"),
				getVector2iKey<sf::Vector2i>(elem, "range"),
				false);
		}
		case str2int16("drawable.resizeY"):
		{
			return std::make_shared<ActDrawableResizeXY>(
				getStringViewKey(elem, "id"),
				getVariableKey(elem, "size"),
				getVariableKey(elem, "inputRangeMin"),
				getVariableKey(elem, "inputRangeMax"),
				getVector2iKey<sf::Vector2i>(elem, "range"),
				true);
		}
		case str2int16("drawable.resizeOnMouseX"):
		{
			return std::make_shared<ActDrawableResizeOnMouseX>(
				getStringViewKey(elem, "id"),
				getVector2fKey<sf::Vector2f>(elem, "range"));
		}
		case str2int16("drawable.resizeOnMouseY"):
		{
			return std::make_shared<ActDrawableResizeOnMouseY>(
				getStringViewKey(elem, "id"),
				getVector2fKey<sf::Vector2f>(elem, "range"));
		}
		case str2int16("drawable.resizeToPositionX"):
		{
			return std::make_shared<ActDrawableResizeToPositionX>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "idToPosition"),
				(float)getIntKey(elem, "offset"));
		}
		case str2int16("drawable.resizeToPositionY"):
		{
			return std::make_shared<ActDrawableResizeToPositionY>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "idToPosition"),
				(float)getIntKey(elem, "offset"));
		}
		case str2int16("drawable.setAction"):
		{
			return std::make_shared<ActDrawableSetAction>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "event"),
				getActionKey(game, elem, "action"));
		}
		case str2int16("drawable.sendToBack"):
		{
			return std::make_shared<ActDrawableSendToBack>(getStringViewKey(elem, "id"));
		}
		case str2int16("drawable.setPosition"):
		{
			return std::make_shared<ActDrawableSetPosition>(
				getStringViewKey(elem, "id"),
				getVector2fKey<sf::Vector2f>(elem, "position"),
				getVector2fKey<sf::Vector2f>(elem, "offset"),
				getBoolKey(elem, "relativeCoords", true));
		}
		case str2int16("drawable.setPositionX"):
		{
			return std::make_shared<ActDrawableSetPositionXY>(
				getStringViewKey(elem, "id"),
				(float)getIntKey(elem, "position"),
				false);
		}
		case str2int16("drawable.setPositionY"):
		{
			return std::make_shared<ActDrawableSetPositionXY>(
				getStringViewKey(elem, "id"),
				(float)getIntKey(elem, "position"),
				true);
		}
		case str2int16("drawable.setSize"):
		{
			return std::make_shared<ActDrawableSetSize>(
				getStringViewKey(elem, "id"),
				getVector2fKey<sf::Vector2f>(elem, "size"));
		}
		case str2int16("drawable.setSizeX"):
		{
			return std::make_shared<ActDrawableSetSizeXY>(
				getStringViewKey(elem, "id"),
				(float)getIntKey(elem, "size"),
				false);
		}
		case str2int16("drawable.setSizeY"):
		{
			return std::make_shared<ActDrawableSetSizeXY>(
				getStringViewKey(elem, "id"),
				(float)getIntKey(elem, "size"),
				true);
		}
		case str2int16("drawable.toggleVisible"):
		{
			return std::make_shared<ActToggleVisible>(getStringViewKey(elem, "id"));
		}
		case str2int16("drawable.verticalAnchorToFocused"):
		{
			return std::make_shared<ActDrawableVerticalAnchorToFocused>(
				getStringViewKey(elem, "id"),
				(float)getIntKey(elem, "offset"));
		}
		case str2int16("drawable.visible"):
		{
			return std::make_shared<ActSetVisible>(
				getStringViewKey(elem, "id"),
				getBoolKey(elem, "visible", true));
		}
		case str2int16("event.add"):
		{
			return std::make_shared<ActEventAdd>(
				getStringViewKey(elem, "id"),
				getActionKey(game, elem, "action"),
				getTimeKey(elem, "time"),
				getBoolKey(elem, "addToFront"));
		}
		case str2int16("event.delete"):
		{
			return std::make_shared<ActEventDelete>(getStringViewKey(elem, "id"));
		}
		case str2int16("event.deleteAll"):
		{
			return std::make_shared<ActEventDeleteAll>();
		}
		case str2int16("event.resetTime"):
		{
			return std::make_shared<ActEventResetTime>(getStringViewKey(elem, "id"));
		}
		case str2int16("file.copy"):
		{
			return std::make_shared<ActFileCopy>(
				getStringViewKey(elem, "dir"),
				getStringVectorKey(elem, "file"),
				getStringViewKey(elem, "writeFile"),
				getStringViewKey(elem, "nullText"),
				getReplaceVarsKey(elem, "replace"));
		}
		case str2int16("focus.add"):
		{
			return std::make_shared<ActFocusAdd>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "resource"),
				getBoolKey(elem, "focus", true));
		}
		case str2int16("focus.click"):
		{
			return std::make_shared<ActFocusClick>(getBoolKey(elem, "playSound", true));
		}
		case str2int16("focus.moveDown"):
		{
			return std::make_shared<ActFocusMoveDown>();
		}
		case str2int16("focus.moveUp"):
		{
			return std::make_shared<ActFocusMoveUp>();
		}
		case str2int16("focus.rightClick"):
		{
			return std::make_shared<ActFocusRightClick>(getBoolKey(elem, "playSound", true));
		}
		case str2int16("focus.set"):
		{
			return std::make_shared<ActFocusSet>(
				getStringViewKey(elem, "id"),
				getBoolKey(elem, "focus", true));
		}
		case str2int16("focus.update"):
		{
			return std::make_shared<ActFocusUpdate>();
		}
		case str2int16("font.setColor"):
		case str2int16("font.setPalette"):
		{
			return std::make_shared<ActFontSetPaletteOrColor>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "palette"),
				getColorKey(elem, "color", sf::Color::White));
		}
		case str2int16("game.addToProperty"):
		{
			return std::make_shared<ActGameAddToProperty>(
				getStringViewKey(elem, "property"),
				getVariableKey(elem, "value"));
		}
		case str2int16("game.clearPlayingSounds"):
		{
			return std::make_shared<ActGameClearPlayingSounds>();
		}
		case str2int16("game.close"):
		{
			return std::make_shared<ActGameClose>();
		}
		case str2int16("game.draw"):
		{
			return std::make_shared<ActGameDraw>();
		}
		case str2int16("game.enableInput"):
		{
			return std::make_shared<ActGameEnableInput>(getBoolKey(elem, "enable", true));
		}
		case str2int16("game.fadeIn"):
		{
			auto action = std::make_shared<ActGameFade>(
				getColorKey(elem, "color"),
				false,
				getBoolKey(elem, "enableInput", true),
				(uint8_t)getUIntKey(elem, "fade", 25),
				getTimeKey(elem, "refresh", sf::milliseconds(15)));

			if (elem.HasMember("action"sv))
			{
				action->setAction(getActionVal(game, elem["action"sv]));
			}
			return action;
		}
		case str2int16("game.fadeOut"):
		{
			auto action = std::make_shared<ActGameFade>(
				getColorKey(elem, "color", sf::Color::Transparent),
				true,
				getBoolKey(elem, "enableInput"),
				(uint8_t)getUIntKey(elem, "fade", 25),
				getTimeKey(elem, "refresh", sf::milliseconds(15)));

			if (elem.HasMember("action"sv))
			{
				action->setAction(getActionVal(game, elem["action"sv]));
			}
			return action;
		}
		case str2int16("game.load"):
		{
			return std::make_shared<ActGameLoad>(
				getStringViewKey(elem, "file"),
				getStringViewKey(elem, "mainFile", "main.json"));
		}
		case str2int16("game.pauseOnFocusLoss"):
		{
			return std::make_shared<ActGamePauseOnFocusLoss>(getBoolKey(elem, "pause", true));
		}
		case str2int16("game.setGamma"):
		{
			return std::make_shared<ActGameSetGamma>(getVariableKey(elem, "gamma"));
		}
		case str2int16("game.setMusicVolume"):
		{
			return std::make_shared<ActGameSetMusicVolume>(getVariableKey(elem, "volume"));
		}
		case str2int16("game.setProperty"):
		{
			auto action = std::make_shared<ActGameSetProperty>(
				getStringViewKey(elem, "property"),
				getVariableKey(elem, "value"));

			if (elem.HasMember("propRange"sv) == true)
			{
				action->setPropRange(getVector2iKey<sf::Vector2i>(elem, "propRange"));
			}
			if (elem.HasMember("valueRange"sv) == true)
			{
				action->setValueRange(getVector2iKey<sf::Vector2i>(elem, "valueRange"));
			}
			return action;
		}
		case str2int16("game.setShader"):
		{
			return std::make_shared<ActShaderSetGameShader>(
				getStringViewKey(elem, "shader"),
				getStringViewKey(elem, "gameShader"));
		}
		case str2int16("game.setSoundVolume"):
		{
			return std::make_shared<ActGameSetSoundVolume>(getVariableKey(elem, "volume"));
		}
		case str2int16("if"):
		{
			return getMultiIfCondition(game, elem);
		}
		case str2int16("if.equal"):
		{
			return getIfCondition(str2int16("=="), game, elem);
		}
		case str2int16("if.greater"):
		{
			return getIfCondition(str2int16(">"), game, elem);
		}
		case str2int16("if.inList"):
		{
			return getInListCondition(game, elem);
		}
		case str2int16("if.lower"):
		{
			return getIfCondition(str2int16("<"), game, elem);
		}
		case str2int16("if.notEqual"):
		{
			return getIfCondition(str2int16("!="), game, elem);
		}
		case str2int16("if.regex"):
		{
			return getIfCondition(str2int16("regex"), game, elem);
		}
		case str2int16("image.enableOutline"):
		{
			return std::make_shared<ActImageEnableOutline>(
				getStringViewKey(elem, "id"),
				getBoolKey(elem, "enable", true));
		}
		case str2int16("image.inverseResizeX"):
		{
			return std::make_shared<ActImageInverseResizeXY>(
				getStringViewKey(elem, "id"),
				getVariableKey(elem, "size"),
				getVariableKey(elem, "inputRangeMin"),
				getVariableKey(elem, "inputRangeMax"),
				getVector2iKey<sf::Vector2i>(elem, "range"),
				false);
		}
		case str2int16("image.inverseResizeY"):
		{
			return std::make_shared<ActImageInverseResizeXY>(
				getStringViewKey(elem, "id"),
				getVariableKey(elem, "size"),
				getVariableKey(elem, "inputRangeMin"),
				getVariableKey(elem, "inputRangeMax"),
				getVector2iKey<sf::Vector2i>(elem, "range"),
				true);
		}
		case str2int16("image.setOutline"):
		{
			return std::make_shared<ActImageSetOutline>(
				getStringViewKey(elem, "id"),
				getColorKey(elem, "outline", sf::Color::Transparent),
				getColorKey(elem, "ignore", sf::Color::Transparent));
		}
		case str2int16("image.setPalette"):
		{
			return std::make_shared<ActImageSetPalette>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "palette"),
				getColorKey(elem, "color", sf::Color::White));
		}
		case str2int16("image.setTexture"):
		{
			if (elem.HasMember("texture"sv) == true)
			{
				return std::make_shared<ActImageSetTexture>(
					getStringViewKey(elem, "id"),
					getStringViewKey(elem, "texture"),
					getBoolKey(elem, "resetRect"));
			}
			else if (elem.HasMember("texturePack"sv) == true)
			{
				return std::make_shared<ActImageSetTextureFromPack>(
					getStringViewKey(elem, "id"),
					getStringViewKey(elem, "texturePack"),
					getUIntKey(elem, "index"));
			}
			else
			{
				return std::make_shared<ActImageSetTextureFromQueryable>(
					getStringViewKey(elem, "id"),
					getStringViewKey(elem, "query"),
					getUIntKey(elem, "index"));
			}
		}
		case str2int16("image.setTextureRect"):
		{
			return std::make_shared<ActImageSetTextureRect>(
				getStringViewKey(elem, "id"),
				getIntRectKey(elem, "rect"));
		}
		case str2int16("inputText.click"):
		{
			return std::make_shared<ActInputTextClick>(getStringViewKey(elem, "id"));
		}
		case str2int16("io.delete"):
		{
			return std::make_shared<ActIODelete>(getStringViewKey(elem, "file"));
		}
		case str2int16("io.deleteAll"):
		{
			return std::make_shared<ActIODeleteAll>(
				getStringViewKey(elem, "file"),
				getBoolKey(elem, "deleteRoot", true));
		}
		case str2int16("load"):
		{
			return std::make_shared<ActLoad>(getStringVectorKey(elem, "file"));
		}
		case str2int16("loadRandom"):
		{
			return std::make_shared<ActLoadRandom>(
				getStringVectorKey(elem, "file"),
				getStringViewKey(elem, "endsWith", ".json"));
		}
		case str2int16("loadingScreen.clear"):
		{
			return std::make_shared<ActLoadingScreenClear>();
		}
		case str2int16("loadingScreen.setProgress"):
		{
			return std::make_shared<ActLoadingScreenSetProgress>(getIntKey(elem, "progress"));
		}
		case str2int16("loadJson"):
		{
			std::string json;
			if (elem.HasMember("json"sv) == true)
			{
				if (elem["json"sv].IsString())
				{
					json = getStringVal(elem["json"sv]);
				}
				else
				{
					json = JsonUtils::jsonToString(elem["json"sv]);
				}
			}
			else if (isValidString(elem, "file") == true)
			{
				json = FileUtils::readText(elem["file"sv].GetString());
			}
			if (isValidArray(elem, "args") == true)
			{
				return std::make_shared<ActLoadJson>(
					json,
					getStringVectorKey(elem, "args"));
			}
			else
			{
				return std::make_shared<ActLoadJson>(json);
			}
		}
		case str2int16("menu.appendText"):
		{
			return parseSetMenuTextHelper<ActMenuAppendText>(game, elem);
		}
		case str2int16("menu.click"):
		{
			return std::make_shared<ActMenuClick>(
				getStringViewKey(elem, "id"),
				getUIntKey(elem, "index"),
				getBoolKey(elem, "playSound", true));
		}
		case str2int16("menu.clickVisible"):
		{
			return std::make_shared<ActMenuClickVisible>(
				getStringViewKey(elem, "id"),
				getUIntKey(elem, "index"),
				getBoolKey(elem, "playSound", true));
		}
		case str2int16("menu.moveScrollbar"):
		{
			return std::make_shared<ActMenuMoveScrollbar>(
				getStringViewKey(elem, "idMenu"),
				getStringViewKey(elem, "idScrollbar"),
				getStringViewKey(elem, "idAnchorTo"),
				getUIntKey(elem, "range"),
				getBoolKey(elem, "focus"));
		}
		case str2int16("menu.rightClick"):
		{
			return std::make_shared<ActMenuRightClick>(
				getStringViewKey(elem, "id"),
				getUIntKey(elem, "index"),
				getBoolKey(elem, "playSound", true));
		}
		case str2int16("menu.rightClickVisible"):
		{
			return std::make_shared<ActMenuRightClickVisible>(
				getStringViewKey(elem, "id"),
				getUIntKey(elem, "index"),
				getBoolKey(elem, "playSound", true));
		}
		case str2int16("menu.setColor"):
		{
			return std::make_shared<ActMenuSetColor>(
				getStringViewKey(elem, "id"),
				getUIntKey(elem, "index"),
				getColorKey(elem, "color", sf::Color::White));
		}
		case str2int16("menu.setFont"):
		{
			return std::make_shared<ActMenuSetFont>(
				getStringViewKey(elem, "id"),
				getUIntKey(elem, "index"),
				getStringViewKey(elem, "font"));
		}
		case str2int16("menu.setIndex"):
		{
			return std::make_shared<ActMenuSetIndex>(
				getStringViewKey(elem, "id"),
				getVariableKey(elem, "index"),
				getBoolKey(elem, "focus"));
		}
		case str2int16("menu.setText"):
		{
			return parseSetMenuTextHelper<ActMenuSetText>(game, elem);
		}
		case str2int16("mount"):
		{
			return std::make_shared<ActMount>(
				getStringViewKey(elem, "file"),
				getStringViewKey(elem, "mount"),
				getBoolKey(elem, "append", true),
				getBoolKey(elem, "useSaveDir"));
		}
		case str2int16("movie.pause"):
		{
			return std::make_shared<ActMoviePause>(getStringViewKey(elem, "id"));
		}
		case str2int16("movie.play"):
		{
			return std::make_shared<ActMoviePlay>(getStringViewKey(elem, "id"));
		}
		case str2int16("palette.replace"):
		{
			return std::make_shared<ActPaletteReplace>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "idSource"),
				getUIntKey(elem, "srcStart", 0),
				getUIntKey(elem, "size", 256),
				getUIntKey(elem, "dstStart", 0),
				getBoolKey(elem, "stepReplace"));
		}
		case str2int16("palette.shiftLeft"):
		{
			return std::make_shared<ActPaletteShiftLeft>(
				getStringViewKey(elem, "id"),
				getUIntKey(elem, "shift", 1),
				getVector2uKey<std::pair<size_t, size_t>>(elem, "range", { 0, 256 }));
		}
		case str2int16("palette.shiftRight"):
		{
			return std::make_shared<ActPaletteShiftRight>(
				getStringViewKey(elem, "id"),
				getUIntKey(elem, "shift", 1),
				getVector2uKey<std::pair<size_t, size_t>>(elem, "range", { 0, 256 }));
		}
		case str2int16("randomList"):
		{
			auto actionList = std::make_shared<ActRandomList>();
			bool hasActions = false;
			if (elem.HasMember("actions"sv) == true &&
				elem["actions"sv].IsArray() == true)
			{
				for (const auto& val : elem["actions"sv])
				{
					auto action = getActionVal(game, val);
					if (action != nullptr)
					{
						actionList->add(action);
						hasActions = true;
					}
				}
			}
			if (hasActions == false)
			{
				return nullptr;
			}
			return actionList;
		}
		case str2int16("random"):
		{
			return std::make_shared<ActRandom>(
				getFloatKey(elem, "percentage", 0.5),
				getActionKey(game, elem, "action1"),
				getActionKey(game, elem, "action2"));
		}
		case str2int16("resource.add"):
		{
			auto id = getStringViewKey(elem, "id");
			if (isValidId(id) == false)
			{
				return nullptr;
			}
			auto action = std::make_shared<ActResourceAdd>(id);
			if (elem.HasMember("ignorePrevious"sv) == true)
			{
				action->setIgnorePrevious(
					getIgnoreResourceVal(elem["ignorePrevious"sv]));
			}
			return action;
		}
		case str2int16("resource.ignore"):
		{
			return std::make_shared<ActResourceIgnore>(
				getStringViewKey(elem, "id"),
				getIgnoreResourceKey(elem, "ignore", IgnoreResource::Draw | IgnoreResource::Update));
		}
		case str2int16("resource.bringToFront"):
		{
			return std::make_shared<ActResourceBringToFront>(getStringViewKey(elem, "id"));
		}
		case str2int16("resource.pop"):
		{
			return std::make_shared<ActResourcePop>(
				getStringViewKey(elem, "id"),
				getIgnoreResourceKey(elem, "ignorePrevious"));
		}
		case str2int16("resource.popAll"):
		{
			return std::make_shared<ActResourcePopAll>(
				getStringViewKey(elem, "id"),
				getBoolKey(elem, "popBase"),
				getIgnoreResourceKey(elem, "ignorePrevious"));
		}
		case str2int16("scrollable.setSpeed"):
		{
			return std::make_shared<ActScrollableSetSpeed>(
				getStringViewKey(elem, "id"),
				getTimeKey(elem, "speed"));
		}
		case str2int16("shader.setBool"):
		{
			return std::make_shared<ActShaderSetUniform<bool>>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "key"),
				getBoolKey(elem, "value"));
		}
		case str2int16("shader.setColor"):
		{
			return std::make_shared<ActShaderSetUniform<sf::Glsl::Vec4>>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "key"),
				sf::Glsl::Vec4(getColorKey(elem, "value")));
		}
		case str2int16("shader.setFloat"):
		{
			return std::make_shared<ActShaderSetUniform<float>>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "key"),
				getFloatKey(elem, "value"));
		}
		case str2int16("shader.setVec2"):
		{
			return std::make_shared<ActShaderSetUniform<sf::Glsl::Vec2>>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "key"),
				getVector2fKey<sf::Glsl::Vec2>(elem, "value"));
		}
		case str2int16("shader.setVec3"):
		{
			return std::make_shared<ActShaderSetUniform<sf::Glsl::Vec3>>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "key"),
				getVector3fKey<sf::Glsl::Vec3>(elem, "value"));
		}
		case str2int16("shader.setVec4"):
		{
			return std::make_shared<ActShaderSetUniform<sf::Glsl::Vec4>>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "key"),
				getVector4fKey<sf::Glsl::Vec4>(elem, "value"));
		}
		case str2int16("sound.loadPlay"):
		{
			return std::make_shared<ActSoundLoadPlay>(
				getStringViewKey(elem, "file"),
				getVariableKey(elem, "volume"),
				getTimeKey(elem, "seek"),
				getBoolKey(elem, "unique"));
		}
		case str2int16("sound.play"):
		{
			return std::make_shared<ActSoundPlay>(
				getStringViewKey(elem, "id"),
				getVariableKey(elem, "volume"),
				getTimeKey(elem, "seek"),
				getBoolKey(elem, "unique"));
		}
		case str2int16("switch"):
		{
			return getSwitchCondition(game, elem);
		}
		case str2int16("text.appendText"):
		{
			return parseSetTextHelper<ActTextAppendText>(game, elem);
		}
		case str2int16("text.setColor"):
		{
			return std::make_shared<ActTextSetColor>(
				getStringViewKey(elem, "id"),
				getColorKey(elem, "color", sf::Color::White));
		}
		case str2int16("text.setFont"):
		{
			return std::make_shared<ActTextSetFont>(
				getStringViewKey(elem, "id"),
				getStringViewKey(elem, "font"));
		}
		case str2int16("text.setSpacing"):
		{
			auto action = std::make_shared<ActTextSetSpacing>(
				getStringViewKey(elem, "id"));
			if (elem.HasMember("horizontal"sv) == true)
			{
				action->setHorizontalSpaceOffset(getIntVal(elem["horizontal"sv]));
			}
			if (elem.HasMember("vertical"sv) == true)
			{
				action->setVerticalSpaceOffset(getIntVal(elem["vertical"sv]));
			}
			return action;
		}
		case str2int16("text.setText"):
		{
			return parseSetTextHelper<ActTextSetText>(game, elem);
		}
		case str2int16("unmount"):
		{
			return std::make_shared<ActUnmount>(
				getStringViewKey(elem, "file"),
				getBoolKey(elem, "useSaveDir"));
		}
		case str2int16("unmountAll"):
		{
			return std::make_shared<ActUnmountAll>();
		}
		case str2int16("variable.add"):
		{
			return std::make_shared<ActVariableAdd>(
				getStringViewKey(elem, "key"),
				getVariableKey(elem, "value"));
		}
		case str2int16("variable.clear"):
		{
			return std::make_shared<ActVariableClear>(getStringViewKey(elem, "key"));
		}
		case str2int16("variable.save"):
		{
			return std::make_shared<ActVariableSave>(
				getStringViewKey(elem, "file"),
				getStringVectorKey(elem, "vars"));
		}
		case str2int16("variable.set"):
		{
			if (elem.HasMember("values"sv) == true &&
				elem["values"sv].IsObject() == true)
			{
				return std::make_shared<ActVariablesSet>(
					getVariables(elem["values"sv]),
					getBoolKey(elem, "resolveValue", true));
			}
			auto key = getStringViewKey(elem, "key");
			if (isValidId(key) == false)
			{
				return nullptr;
			}
			return std::make_shared<ActVariableSet>(
				key,
				getVariableKey(elem, "value"),
				getBoolKey(elem, "resolveValue", true));
		}
		case str2int16("variable.setIfNull"):
		{
			auto key = getStringViewKey(elem, "key");
			if (isValidId(key) == false)
			{
				return nullptr;
			}
			return std::make_shared<ActVariableSetIfNull>(
				key,
				getVariableKey(elem, "value"),
				getBoolKey(elem, "resolveValue", true));
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

	std::shared_ptr<Action> getActionKey(Game& game, const Value& elem, const char* key)
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
