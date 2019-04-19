#include "ParseAction.h"
#include "Actions/ActAction.h"
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
#include "Actions/ActItem.h"
#include "Actions/ActLevel.h"
#include "Actions/ActLevelObject.h"
#include "Actions/ActLoad.h"
#include "Actions/ActLoadingScreen.h"
#include "Actions/ActMenu.h"
#include "Actions/ActMount.h"
#include "Actions/ActMovie.h"
#include "Actions/ActPalette.h"
#include "Actions/ActPlayer.h"
#include "Actions/ActQuest.h"
#include "Actions/ActRandom.h"
#include "Actions/ActResource.h"
#include "Actions/ActShader.h"
#include "Actions/ActSound.h"
#include "Actions/ActText.h"
#include "Actions/ActVariable.h"
#include "Actions/ActVisibility.h"
#include "GameUtils.h"
#include "Json/JsonUtils.h"
#include "ParseCondition.h"
#include "Parser/Game/ParseQuest.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	template <class T>
	std::shared_ptr<Action> parseSetTextHelper(Game& game, const Value& elem)
	{
		std::shared_ptr<T> action;
		if (elem.HasMember("binding") == true)
		{
			action = std::make_shared<T>(
				getStringKey(elem, "id"),
				getStringKey(elem, "format"),
				getStringVectorKey(elem, "binding"));
		}
		else if (elem.HasMember("query") == true)
		{
			action = std::make_shared<T>(
				getStringKey(elem, "id"),
				getStringKey(elem, "text"),
				getStringKey(elem, "query"));
		}
		else
		{
			auto textOp = TextUtils::TextOp::Replace;
			if (getBoolKey(elem, "set") == true)
			{
				textOp = TextUtils::TextOp::Set;
			}
			action = std::make_shared<T>(
				getStringKey(elem, "id"),
				getStringKey(elem, "text"),
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
		if (elem.HasMember("binding") == true)
		{
			action = std::make_shared<T>(
				getStringKey(elem, "id"),
				index,
				getStringKey(elem, "format"),
				getStringVectorKey(elem, "binding"));
		}
		else if (elem.HasMember("query") == true)
		{
			action = std::make_shared<T>(
				getStringKey(elem, "id"),
				index,
				getStringKey(elem, "text"),
				getStringKey(elem, "query"));
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
				getStringKey(elem, "id"),
				index,
				getStringKey(elem, "text"),
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
		if (elem.HasMember("name") == false ||
			elem["name"].IsString() == false)
		{
			return nullptr;
		}
		switch (str2int16(getStringViewVal(elem["name"])))
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
				getStringKey(elem, "id"),
				getActionKey(game, elem, "action"));
		}
		case str2int16("audio.pause"):
		{
			return std::make_shared<ActAudioPause>(getStringKey(elem, "id"));
		}
		case str2int16("audio.pauseAll"):
		{
			return std::make_shared<ActAudioPauseAll>();
		}
		case str2int16("audio.play"):
		{
			auto action = std::make_shared<ActAudioPlay>(
				getStringKey(elem, "id"),
				getVariableKey(elem, "volume"),
				getBoolKey(elem, "clear"));

			if (elem.HasMember("loop") == true)
			{
				action->setLoop(getBoolVal(elem["loop"]));
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
				getStringKey(elem, "id"),
				getTimeKey(elem, "time"));
		}
		case str2int16("audio.setVolume"):
		{
			return std::make_shared<ActAudioSetVolume>(
				getStringKey(elem, "id"),
				getVariableKey(elem, "volume"));
		}
		case str2int16("audio.stop"):
		{
			return std::make_shared<ActAudioStop>(getStringKey(elem, "id"));
		}
		case str2int16("audio.stopAll"):
		{
			return std::make_shared<ActAudioStopAll>();
		}
		case str2int16("button.click"):
		{
			return std::make_shared<ActButtonClick>(
				getStringKey(elem, "id"),
				getBoolKey(elem, "playSound", true));
		}
		case str2int16("button.enable"):
		{
			return std::make_shared<ActButtonEnable>(
				getStringKey(elem, "id"),
				getBoolKey(elem, "enable", true));
		}
		case str2int16("button.setColor"):
		{
			return std::make_shared<ActButtonSetColor>(
				getStringKey(elem, "id"),
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
				getStringKey(elem, "id"));
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
				getStringKey(elem, "palette"),
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
				getStringKey(elem, "source"),
				getStringKey(elem, "destination"));
		}
		case str2int16("dir.create"):
		{
			return std::make_shared<ActDirCreate>(getStringKey(elem, "file"));
		}
		case str2int16("drawable.addToPosition"):
		{
			return std::make_shared<ActDrawableAddToPosition>(
				getStringKey(elem, "id"),
				getVector2fKey<sf::Vector2f>(elem, "offset"));
		}
		case str2int16("drawable.addToSize"):
		{
			return std::make_shared<ActDrawableAddToSize>(
				getStringKey(elem, "id"),
				getVector2fKey<sf::Vector2f>(elem, "offset"));
		}
		case str2int16("drawable.anchor"):
		{
			return std::make_shared<ActDrawableAnchor>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idAnchor"),
				getAnchorKey(elem, "anchor"),
				getVector2fKey<sf::Vector2f>(elem, "offset"));
		}
		case str2int16("drawable.anchorSizeX"):
		{
			return std::make_shared<ActDrawableAnchorSizeXY>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idAnchorTo"),
				(float)getIntKey(elem, "offset"),
				false);
		}
		case str2int16("drawable.anchorSizeY"):
		{
			return std::make_shared<ActDrawableAnchorSizeXY>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idAnchorTo"),
				(float)getIntKey(elem, "offset"),
				true);
		}
		case str2int16("drawable.anchorToFocused"):
		{
			return std::make_shared<ActDrawableAnchorToFocused>(
				getStringKey(elem, "id"),
				getAnchorKey(elem, "anchor"),
				getVector2fKey<sf::Vector2f>(elem, "offset"));
		}
		case str2int16("drawable.center"):
		{
			return std::make_shared<ActDrawableCenter>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idCenterOn"),
				getVector2fKey<sf::Vector2f>(elem, "offset"));
		}
		case str2int16("drawable.centerOnMouseX"):
		{
			return std::make_shared<ActDrawableCenterOnMouseX>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idAnchorTo"),
				getUIntKey(elem, "range"),
				getVariableKey(elem, "steps"));
		}
		case str2int16("drawable.centerOnMouseY"):
		{
			return std::make_shared<ActDrawableCenterOnMouseY>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idAnchorTo"),
				getUIntKey(elem, "range"),
				getVariableKey(elem, "steps"));
		}
		case str2int16("drawable.delete"):
		{
			return std::make_shared<ActDrawableDelete>(getStringKey(elem, "id"));
		}
		case str2int16("drawable.executeAction"):
		{
			return std::make_shared<ActDrawableExecuteAction>(
				getStringKey(elem, "id"),
				str2int16(getStringViewKey(elem, "action")));
		}
		case str2int16("drawable.horizontalAnchorToFocused"):
		{
			return std::make_shared<ActDrawableHorizontalAnchorToFocused>(
				getStringKey(elem, "id"),
				(float)getIntKey(elem, "offset"));
		}
		case str2int16("drawable.moveX"):
		{
			return std::make_shared<ActDrawableMoveX>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idAnchorTo"),
				getUIntKey(elem, "range"),
				getVariableKey(elem, "position"),
				getUIntKey(elem, "min"),
				getUIntKey(elem, "max"),
				getVariableKey(elem, "steps"));
		}
		case str2int16("drawable.moveY"):
		{
			return std::make_shared<ActDrawableMoveY>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idAnchorTo"),
				getUIntKey(elem, "range"),
				getVariableKey(elem, "position"),
				getUIntKey(elem, "min"),
				getUIntKey(elem, "max"),
				getVariableKey(elem, "steps"));
		}
		case str2int16("drawable.moveStepX"):
		{
			return std::make_shared<ActDrawableMoveStepX>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idAnchorTo"),
				getUIntKey(elem, "range"),
				getVariableKey(elem, "steps"),
				getIntKey(elem, "stepOffset"));
		}
		case str2int16("drawable.moveStepY"):
		{
			return std::make_shared<ActDrawableMoveStepY>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idAnchorTo"),
				getUIntKey(elem, "range"),
				getVariableKey(elem, "steps"),
				getIntKey(elem, "stepOffset"));
		}
		case str2int16("drawable.resizeX"):
		{
			return std::make_shared<ActDrawableResizeXY>(
				getStringKey(elem, "id"),
				getVariableKey(elem, "size"),
				getVariableKey(elem, "inputRangeMin"),
				getVariableKey(elem, "inputRangeMax"),
				getVector2iKey<sf::Vector2i>(elem, "range"),
				false);
		}
		case str2int16("drawable.resizeY"):
		{
			return std::make_shared<ActDrawableResizeXY>(
				getStringKey(elem, "id"),
				getVariableKey(elem, "size"),
				getVariableKey(elem, "inputRangeMin"),
				getVariableKey(elem, "inputRangeMax"),
				getVector2iKey<sf::Vector2i>(elem, "range"),
				true);
		}
		case str2int16("drawable.resizeOnMouseX"):
		{
			return std::make_shared<ActDrawableResizeOnMouseX>(
				getStringKey(elem, "id"),
				getVector2fKey<sf::Vector2f>(elem, "range"));
		}
		case str2int16("drawable.resizeOnMouseY"):
		{
			return std::make_shared<ActDrawableResizeOnMouseY>(
				getStringKey(elem, "id"),
				getVector2fKey<sf::Vector2f>(elem, "range"));
		}
		case str2int16("drawable.resizeToPositionX"):
		{
			return std::make_shared<ActDrawableResizeToPositionX>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idToPosition"),
				(float)getIntKey(elem, "offset"));
		}
		case str2int16("drawable.resizeToPositionY"):
		{
			return std::make_shared<ActDrawableResizeToPositionY>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idToPosition"),
				(float)getIntKey(elem, "offset"));
		}
		case str2int16("drawable.setAction"):
		{
			return std::make_shared<ActDrawableSetAction>(
				getStringKey(elem, "id"),
				getStringViewKey(elem, "event"),
				getActionKey(game, elem, "action"));
		}
		case str2int16("drawable.setPosition"):
		{
			return std::make_shared<ActDrawableSetPosition>(
				getStringKey(elem, "id"),
				getVector2fKey<sf::Vector2f>(elem, "position"),
				getVector2fKey<sf::Vector2f>(elem, "offset"),
				getBoolKey(elem, "relativeCoords", true));
		}
		case str2int16("drawable.setPositionX"):
		{
			return std::make_shared<ActDrawableSetPositionXY>(
				getStringKey(elem, "id"),
				(float)getIntKey(elem, "position"),
				false);
		}
		case str2int16("drawable.setPositionY"):
		{
			return std::make_shared<ActDrawableSetPositionXY>(
				getStringKey(elem, "id"),
				(float)getIntKey(elem, "position"),
				true);
		}
		case str2int16("drawable.setSize"):
		{
			return std::make_shared<ActDrawableSetSize>(
				getStringKey(elem, "id"),
				getVector2fKey<sf::Vector2f>(elem, "size"));
		}
		case str2int16("drawable.setSizeX"):
		{
			return std::make_shared<ActDrawableSetSizeXY>(
				getStringKey(elem, "id"),
				(float)getIntKey(elem, "size"),
				false);
		}
		case str2int16("drawable.setSizeY"):
		{
			return std::make_shared<ActDrawableSetSizeXY>(
				getStringKey(elem, "id"),
				(float)getIntKey(elem, "size"),
				true);
		}
		case str2int16("drawable.toggleVisible"):
		{
			return std::make_shared<ActToggleVisible>(getStringKey(elem, "id"));
		}
		case str2int16("drawable.verticalAnchorToFocused"):
		{
			return std::make_shared<ActDrawableVerticalAnchorToFocused>(
				getStringKey(elem, "id"),
				(float)getIntKey(elem, "offset"));
		}
		case str2int16("drawable.visible"):
		{
			return std::make_shared<ActSetVisible>(
				getStringKey(elem, "id"),
				getBoolKey(elem, "visible", true));
		}
		case str2int16("event.add"):
		{
			return std::make_shared<ActEventAdd>(
				getStringKey(elem, "id"),
				getActionKey(game, elem, "action"),
				getTimeKey(elem, "time"),
				getBoolKey(elem, "addToFront"));
		}
		case str2int16("event.delete"):
		{
			return std::make_shared<ActEventDelete>(getStringKey(elem, "id"));
		}
		case str2int16("event.deleteAll"):
		{
			return std::make_shared<ActEventDeleteAll>();
		}
		case str2int16("event.resetTime"):
		{
			return std::make_shared<ActEventResetTime>(getStringKey(elem, "id"));
		}
		case str2int16("file.copy"):
		{
			return std::make_shared<ActFileCopy>(
				getStringKey(elem, "dir"),
				getStringVectorKey(elem, "file"),
				getStringKey(elem, "writeFile"),
				getStringKey(elem, "nullText"),
				getReplaceVarsKey(elem, "replaceVars"));
		}
		case str2int16("focus.add"):
		{
			return std::make_shared<ActFocusAdd>(
				getStringKey(elem, "id"),
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
		case str2int16("focus.set"):
		{
			return std::make_shared<ActFocusSet>(
				getStringKey(elem, "id"),
				getBoolKey(elem, "focus", true));
		}
		case str2int16("focus.update"):
		{
			return std::make_shared<ActFocusUpdate>();
		}
		case str2int16("font.setPalette"):
		{
			return std::make_shared<ActFontSetPalette>(
				getStringKey(elem, "id"),
				getStringKey(elem, "palette"),
				getColorKey(elem, "color", sf::Color::White));
		}
		case str2int16("game.addToProperty"):
		{
			return std::make_shared<ActGameAddToProperty>(
				getStringKey(elem, "property"),
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
				getUIntKey(elem, "fade", 25),
				getTimeKey(elem, "refresh", sf::milliseconds(15)));

			if (elem.HasMember("action"))
			{
				action->setAction(parseAction(game, elem["action"]));
			}
			return action;
		}
		case str2int16("game.fadeOut"):
		{
			auto action = std::make_shared<ActGameFade>(
				getColorKey(elem, "color", sf::Color::Transparent),
				true,
				getBoolKey(elem, "enableInput"),
				getUIntKey(elem, "fade", 25),
				getTimeKey(elem, "refresh", sf::milliseconds(15)));

			if (elem.HasMember("action"))
			{
				action->setAction(parseAction(game, elem["action"]));
			}
			return action;
		}
		case str2int16("game.load"):
		{
			return std::make_shared<ActGameLoad>(
				getStringKey(elem, "file"),
				getStringKey(elem, "mainFile", "main.json"));
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
				getStringKey(elem, "property"),
				getVariableKey(elem, "value"));

			if (elem.HasMember("propRange") == true)
			{
				action->setPropRange(getVector2iKey<sf::Vector2i>(elem, "propRange"));
			}
			if (elem.HasMember("valueRange") == true)
			{
				action->setValueRange(getVector2iKey<sf::Vector2i>(elem, "valueRange"));
			}
			return action;
		}
		case str2int16("game.setShader"):
		{
			return std::make_shared<ActShaderSetGameShader>(
				getStringKey(elem, "shader"),
				getStringKey(elem, "gameShader"));
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
		case str2int16("image.centerTexture"):
		{
			return std::make_shared<ActImageCenterTexture>(getStringKey(elem, "id"));
		}
		case str2int16("image.enableOutline"):
		{
			return std::make_shared<ActImageEnableOutline>(
				getStringKey(elem, "id"),
				getBoolKey(elem, "enable", true));
		}
		case str2int16("image.inverseResizeX"):
		{
			return std::make_shared<ActImageInverseResizeXY>(
				getStringKey(elem, "id"),
				getVariableKey(elem, "size"),
				getVariableKey(elem, "inputRangeMin"),
				getVariableKey(elem, "inputRangeMax"),
				getVector2iKey<sf::Vector2i>(elem, "range"),
				false);
		}
		case str2int16("image.inverseResizeY"):
		{
			return std::make_shared<ActImageInverseResizeXY>(
				getStringKey(elem, "id"),
				getVariableKey(elem, "size"),
				getVariableKey(elem, "inputRangeMin"),
				getVariableKey(elem, "inputRangeMax"),
				getVector2iKey<sf::Vector2i>(elem, "range"),
				true);
		}
		case str2int16("image.setOutline"):
		{
			return std::make_shared<ActImageSetOutline>(
				getStringKey(elem, "id"),
				getColorKey(elem, "outline", sf::Color::Transparent),
				getColorKey(elem, "ignore", sf::Color::Transparent));
		}
		case str2int16("image.setPalette"):
		{
			return std::make_shared<ActImageSetPalette>(
				getStringKey(elem, "id"),
				getStringKey(elem, "palette"),
				getColorKey(elem, "color", sf::Color::White));
		}
		case str2int16("image.setTexture"):
		{
			if (elem.HasMember("texture") == true)
			{
				return std::make_shared<ActImageSetTexture>(
					getStringKey(elem, "id"),
					getStringKey(elem, "texture"),
					getBoolKey(elem, "resetRect"));
			}
			else if (elem.HasMember("texturePack") == true)
			{
				return std::make_shared<ActImageSetTextureFromPack>(
					getStringKey(elem, "id"),
					getStringKey(elem, "texturePack"),
					getUIntKey(elem, "index"),
					getBoolKey(elem, "resetRect", true));
			}
			else
			{
				return std::make_shared<ActImageSetTextureFromQueryable>(
					getStringKey(elem, "id"),
					getStringKey(elem, "query"),
					getUIntKey(elem, "index", 1),
					getBoolKey(elem, "resetRect", true));
			}
		}
		case str2int16("image.setTextureRect"):
		{
			return std::make_shared<ActImageSetTextureRect>(
				getStringKey(elem, "id"),
				getIntRectKey(elem, "rect"));
		}
		case str2int16("inputText.click"):
		{
			return std::make_shared<ActInputTextClick>(getStringKey(elem, "id"));
		}
		case str2int16("io.delete"):
		{
			return std::make_shared<ActIODelete>(getStringKey(elem, "file"));
		}
		case str2int16("io.deleteAll"):
		{
			return std::make_shared<ActIODeleteAll>(
				getStringKey(elem, "file"),
				getBoolKey(elem, "deleteRoot", true));
		}
		case str2int16("item.addCursor"):
		{
			return std::make_shared<ActItemAddCursor>(
				getStringKey(elem, "level"),
				getItemLocationVal(elem));
		}
		case str2int16("item.addQuantity"):
		{
			return std::make_shared<ActItemAddQuantity>(
				getStringKey(elem, "level"),
				getItemLocationVal(elem),
				getVariableKey(elem, "value"),
				getBoolKey(elem, "remove"));
		}
		case str2int16("item.delete"):
		{
			return std::make_shared<ActItemDelete>(
				getStringKey(elem, "level"),
				getItemLocationVal(elem));
		}
		case str2int16("item.drop"):
		{
			return std::make_shared<ActItemDrop>(
				getStringKey(elem, "level"),
				getItemCoordInventoryVal(elem));
		}
		case str2int16("item.executeAction"):
		{
			return std::make_shared<ActItemExecuteAction>(
				getStringKey(elem, "level"),
				getItemLocationVal(elem),
				str2int16(getStringViewKey(elem, "action")));
		}
		case str2int16("item.loadFromLevel"):
		{
			auto action = std::make_shared<ActItemLoadFromLevel>(
				getStringKey(elem, "level"),
				getItemCoordInventoryVal(elem),
				getBoolKey(elem, "splitIntoMultiple"));

			if (elem.HasMember("position") == true)
			{
				action->setInventoryPosition(getInventoryPositionVal(elem["position"]));
			}
			if (elem.HasMember("onInventoryFull") == true)
			{
				action->setInventoryFullAction(parseAction(game, elem["onInventoryFull"]));
			}
			return action;
		}
		case str2int16("item.move"):
		{
			return std::make_shared<ActItemMove>(
				getStringKey(elem, "level"),
				getItemLocationKey(elem, "from"),
				getItemLocationKey(elem, "to"));
		}
		case str2int16("item.setProperty"):
		{
			return std::make_shared<ActItemSetProperty>(
				getItemLocationVal(elem),
				getStringKey(elem, "level"),
				getStringKey(elem, "property"),
				getVariableKey(elem, "value"));
		}
		case str2int16("item.trade"):
		{
			auto action = std::make_shared<ActItemTrade>(
				getStringKey(elem, "level"),
				getStringKey(elem, "player"),
				getItemCoordInventoryKey(elem, "item"),
				getInventoryPositionKey(elem, "position"));

			if (elem.HasMember("onInventoryFull") == true)
			{
				action->setInventoryFullAction(parseAction(game, elem["onInventoryFull"]));
			}
			return action;
		}
		case str2int16("item.update"):
		{
			return std::make_shared<ActItemUpdate>(
				getStringKey(elem, "level"),
				getItemCoordInventoryVal(elem));
		}
		case str2int16("item.use"):
		{
			return std::make_shared<ActItemUse>(
				getStringKey(elem, "level"),
				getItemCoordInventoryVal(elem));
		}
		case str2int16("level.addLayer"):
		{
			if (elem.HasMember("color") == true)
			{
				return std::make_shared<ActLevelAddColorLayer>(
					getStringKey(elem, "level"),
					getColorKey(elem, "color"),
					getFloatRectKey(elem, "offset"),
					getBoolKey(elem, "automap"));
			}
			else
			{
				return std::make_shared<ActLevelAddTextureLayer>(
					getStringKey(elem, "level"),
					getStringKey(elem, "texture"),
					getIntRectKey(elem, "textureRect"),
					getFloatRectKey(elem, "offset"),
					getBoolKey(elem, "automap"));
			}
		}
		case str2int16("level.clearAllObjects"):
		{
			return std::make_shared<ActLevelClearAllObjects>(
				getStringKey(elem, "level"),
				getStringVectorKey(elem, "exclude"));
		}
		case str2int16("level.clearItems"):
		{
			return std::make_shared<ActLevelClearItems>(
				getStringKey(elem, "level"),
				getStringVectorKey(elem, "exclude"));
		}
		case str2int16("level.clearLevelObjects"):
		{
			return std::make_shared<ActLevelClearLevelObjects>(
				getStringKey(elem, "level"),
				getStringVectorKey(elem, "exclude"));
		}
		case str2int16("level.clearPlayerClasses"):
		{
			return std::make_shared<ActLevelClearPlayerClasses>(
				getStringKey(elem, "level"));
		}
		case str2int16("level.clearPlayers"):
		{
			return std::make_shared<ActLevelClearPlayers>(
				getStringKey(elem, "level"),
				getStringVectorKey(elem, "exclude"));
		}
		case str2int16("level.clearPlayerTextures"):
		{
			return std::make_shared<ActLevelClearPlayerTextures>(
				getStringKey(elem, "level"));
		}
		case str2int16("level.clearQuests"):
		{
			return std::make_shared<ActLevelClearQuests>(
				getStringKey(elem, "level"));
		}
		case str2int16("level.enableHover"):
		{
			return std::make_shared<ActLevelEnableHover>(
				getStringKey(elem, "level"),
				getBoolKey(elem, "enable", true));
		}
		case str2int16("level.move"):
		{
			return std::make_shared<ActLevelMove>(
				getStringKey(elem, "level"),
				getVector2UnsignedNumberKey<PairFloat, float>(elem, "position"),
				getBoolKey(elem, "smooth"));
		}
		case str2int16("level.moveToClick"):
		{
			return std::make_shared<ActLevelMoveToClick>(
				getStringKey(elem, "level"),
				getBoolKey(elem, "smooth"));
		}
		case str2int16("level.moveToPlayer"):
		{
			return std::make_shared<ActLevelMoveToPlayer>(
				getStringKey(elem, "level"),
				getStringKey(elem, "player"),
				getBoolKey(elem, "smooth"));
		}
		case str2int16("level.pause"):
		{
			return std::make_shared<ActLevelPause>(
				getStringKey(elem, "level"),
				getBoolKey(elem, "pause", true));
		}
		case str2int16("level.save"):
		{
			return std::make_shared<ActLevelSave>(
				getStringKey(elem, "level"),
				getStringKey(elem, "file"),
				getBoolKey(elem, "saveDefaults"),
				getBoolKey(elem, "saveCurrentPlayer"),
				getBoolKey(elem, "saveQuests"));
		}
		case str2int16("level.setAutomap"):
		{
			return std::make_shared<ActLevelSetAutomap>(
				getStringKey(elem, "level"),
				getStringKey(elem, "automap"),
				getVector2uKey<std::pair<uint32_t, uint32_t>>(elem, "tileSize", { 64u, 32u }),
				(uint16_t)getUIntKey(elem, "index"),
				getFloatRectKey(elem, "offset"));
		}
		case str2int16("level.setAutomapPosition"):
		{
			return std::make_shared<ActLevelSetAutomapPosition>(
				getStringKey(elem, "level"),
				getVector2fKey<sf::Vector2f>(elem, "position"));
		}
		case str2int16("level.setAutomapSize"):
		{
			return std::make_shared<ActLevelSetAutomapSize>(
				getStringKey(elem, "level"),
				getVector2fKey<sf::Vector2f>(elem, "size", { 100, 100 }));
		}
		case str2int16("level.setShader"):
		{
			return std::make_shared<ActLevelSetShader>(
				getStringKey(elem, "level"),
				getStringKey(elem, "shader"));
		}
		case str2int16("level.setSmoothMovement"):
		{
			return std::make_shared<ActLevelSetSmoothMovement>(
				getStringKey(elem, "level"),
				getBoolKey(elem, "smooth"));
		}
		case str2int16("level.showAutomap"):
		{
			return std::make_shared<ActLevelShowAutomap>(
				getStringKey(elem, "level"),
				getBoolKey(elem, "show", true));
		}
		case str2int16("level.zoom"):
		{
			return std::make_shared<ActLevelZoom>(
				getStringKey(elem, "level"),
				getIntKey(elem, "zoom", 100),
				getBoolKey(elem, "relative"),
				getBoolKey(elem, "smooth"));
		}
		case str2int16("levelObject.delete"):
		{
			return std::make_shared<ActLevelObjDelete>(
				getStringKey(elem, "level"),
				getStringKey(elem, "object"));
		}
		case str2int16("levelObject.deleteByClass"):
		{
			return std::make_shared<ActLevelObjDeleteByClass>(
				getStringKey(elem, "level"),
				getStringKey(elem, "class"));
		}
		case str2int16("levelObject.executeAction"):
		{
			return std::make_shared<ActLevelObjExecuteAction>(
				getStringKey(elem, "level"),
				getStringKey(elem, "object"));
		}
		case str2int16("levelObject.setOutline"):
		{
			return std::make_shared<ActLevelObjSetOutline>(
				getStringKey(elem, "level"),
				getColorKey(elem, "outline", sf::Color::Transparent),
				getColorKey(elem, "ignore", sf::Color::Transparent));
		}
		case str2int16("levelObject.setPalette"):
		{
			return std::make_shared<ActLevelObjSetPalette>(
				getStringKey(elem, "level"),
				getStringKey(elem, "palette"),
				getColorKey(elem, "color", sf::Color::White));
		}
		case str2int16("load"):
		{
			return std::make_shared<ActLoad>(getStringVectorKey(elem, "file"));
		}
		case str2int16("loadRandom"):
		{
			return std::make_shared<ActLoadRandom>(
				getStringVectorKey(elem, "file"),
				getStringKey(elem, "endsWith", ".json"));
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
			if (elem.HasMember("json") == true)
			{
				if (elem["json"].IsString())
				{
					json = getStringVal(elem["json"]);
				}
				else
				{
					json = JsonUtils::jsonToString(elem["json"]);
				}
			}
			else if (isValidString(elem, "file") == true)
			{
				json = FileUtils::readText(elem["file"].GetString());
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
				getStringKey(elem, "id"),
				getUIntKey(elem, "index"),
				getBoolKey(elem, "playSound", true));
		}
		case str2int16("menu.clickVisible"):
		{
			return std::make_shared<ActMenuClickVisible>(
				getStringKey(elem, "id"),
				getUIntKey(elem, "index"),
				getBoolKey(elem, "playSound", true));
		}
		case str2int16("menu.moveScrollbar"):
		{
			return std::make_shared<ActMenuMoveScrollbar>(
				getStringKey(elem, "idMenu"),
				getStringKey(elem, "idScrollbar"),
				getStringKey(elem, "idAnchorTo"),
				getUIntKey(elem, "range"),
				getBoolKey(elem, "focus"));
		}
		case str2int16("menu.setColor"):
		{
			return std::make_shared<ActMenuSetColor>(
				getStringKey(elem, "id"),
				getUIntKey(elem, "index"),
				getColorKey(elem, "color", sf::Color::White));
		}
		case str2int16("menu.setFont"):
		{
			return std::make_shared<ActMenuSetFont>(
				getStringKey(elem, "id"),
				getUIntKey(elem, "index"),
				getStringKey(elem, "font"));
		}
		case str2int16("menu.setIndex"):
		{
			return std::make_shared<ActMenuSetIndex>(
				getStringKey(elem, "id"),
				getVariableKey(elem, "index"));
		}
		case str2int16("menu.setText"):
		{
			return parseSetMenuTextHelper<ActMenuSetText>(game, elem);
		}
		case str2int16("mount"):
		{
			return std::make_shared<ActMount>(
				getStringKey(elem, "file"),
				getStringKey(elem, "mount"),
				getBoolKey(elem, "append", true),
				getBoolKey(elem, "useSaveDir"));
		}
		case str2int16("movie.pause"):
		{
			return std::make_shared<ActMoviePause>(getStringKey(elem, "id"));
		}
		case str2int16("movie.play"):
		{
			return std::make_shared<ActMoviePlay>(getStringKey(elem, "id"));
		}
		case str2int16("palette.replace"):
		{
			return std::make_shared<ActPaletteReplace>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idSource"),
				getUIntKey(elem, "srcStart", 0),
				getUIntKey(elem, "size", 256),
				getUIntKey(elem, "dstStart", 0),
				getBoolKey(elem, "stepReplace"));
		}
		case str2int16("palette.shiftLeft"):
		{
			return std::make_shared<ActPaletteShiftLeft>(
				getStringKey(elem, "id"),
				getUIntKey(elem, "shift", 1),
				getVector2uKey<std::pair<size_t, size_t>>(elem, "range", { 0, 256 }));
		}
		case str2int16("palette.shiftRight"):
		{
			return std::make_shared<ActPaletteShiftRight>(
				getStringKey(elem, "id"),
				getUIntKey(elem, "shift", 1),
				getVector2uKey<std::pair<size_t, size_t>>(elem, "range", { 0, 256 }));
		}
		case str2int16("player.addItemQuantity"):
		{
			return std::make_shared<ActPlayerAddItemQuantity>(
				getStringKey(elem, "player"),
				getStringKey(elem, "level"),
				getStringKey(elem, "itemClass"),
				getInventoryPositionKey(elem, "position"),
				getVariableKey(elem, "value"),
				getBoolKey(elem, "remove"));
		}
		case str2int16("player.addToProperty"):
		{
			return std::make_shared<ActPlayerAddToProperty>(
				getStringKey(elem, "player"),
				getStringKey(elem, "level"),
				getStringKey(elem, "property"),
				getVariableKey(elem, "value"),
				getBoolKey(elem, "remove"));
		}
		case str2int16("player.move"):
		{
			return std::make_shared<ActPlayerMove>(
				getStringKey(elem, "player"),
				getStringKey(elem, "level"),
				getVector2UnsignedNumberKey<PairFloat, float>(elem, "position"),
				getBoolKey(elem, "resetDirection"),
				getBoolKey(elem, "smooth"));
		}
		case str2int16("player.moveToClick"):
		{
			return std::make_shared<ActPlayerMoveToClick>(
				getStringKey(elem, "player"),
				getStringKey(elem, "level"));
		}
		case str2int16("player.removeItemQuantity"):
		{
			return std::make_shared<ActPlayerAddItemQuantity>(
				getStringKey(elem, "player"),
				getStringKey(elem, "level"),
				getStringKey(elem, "itemClass"),
				getInventoryPositionKey(elem, "position"),
				getVariableKey(elem, "value"),
				true);
		}
		case str2int16("player.selectSpell"):
		{
			return std::make_shared<ActPlayerSelectSpell>(
				getStringKey(elem, "player"),
				getStringKey(elem, "level"),
				getStringKey(elem, "spell"));
		}
		case str2int16("player.setDefaultSpeed"):
		{
			auto speed = getPlayerAnimationSpeedVal(elem);
			if (elem.HasMember("animation") == false)
			{
				speed.animation = sf::Time::Zero;
			}
			if (elem.HasMember("walk") == false)
			{
				speed.walk = sf::Time::Zero;
			}
			return std::make_shared<ActPlayerSetDefaultSpeed>(
				getStringKey(elem, "player"),
				getStringKey(elem, "level"),
				speed);
		}
		case str2int16("player.setDirection"):
		{
			return std::make_shared<ActPlayerSetDirection>(
				getStringKey(elem, "player"),
				getStringKey(elem, "level"),
				getPlayerDirectionKey(elem, "direction"));
		}
		case str2int16("player.setProperty"):
		{
			return std::make_shared<ActPlayerSetProperty>(
				getStringKey(elem, "player"),
				getStringKey(elem, "level"),
				getStringKey(elem, "property"),
				getVariableKey(elem, "value"));
		}
		case str2int16("player.setRestStatus"):
		{
			return std::make_shared<ActPlayerSetRestStatus>(
				getStringKey(elem, "player"),
				getStringKey(elem, "level"),
				(uint8_t)getUIntKey(elem, "status"));
		}
		case str2int16("player.setTextureIndex"):
		{
			return std::make_shared<ActPlayerSetTextureIndex>(
				getStringKey(elem, "player"),
				getStringKey(elem, "level"),
				getUIntKey(elem, "index"));
		}
		case str2int16("quest.add"):
		{
			if (elem.HasMember("quest") == true)
			{
				auto quest = parseQuestObj(game, elem["quest"]);
				if (isValidId(quest.Id()) == false)
				{
					return nullptr;
				}
				return std::make_shared<ActQuestAdd>(
					getStringKey(elem, "level"),
					std::move(quest));
			}
			return nullptr;
		}
		case str2int16("quest.delete"):
		{
			return std::make_shared<ActQuestDelete>(
				getStringKey(elem, "level"),
				getStringKey(elem, "quest"));
		}
		case str2int16("quest.setState"):
		{
			return std::make_shared<ActQuestSetState>(
				getStringKey(elem, "level"),
				getStringKey(elem, "quest"),
				getIntKey(elem, "state"));
		}
		case str2int16("randomList"):
		{
			auto actionList = std::make_shared<ActRandomList>();
			bool hasActions = false;
			if (elem.HasMember("actions") == true &&
				elem["actions"].IsArray() == true)
			{
				for (const auto& val : elem["actions"])
				{
					auto action = parseAction(game, val);
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
			auto id = getStringKey(elem, "id");
			if (isValidId(id) == false)
			{
				return nullptr;
			}
			auto action = std::make_shared<ActResourceAdd>(id);
			if (elem.HasMember("ignorePrevious") == true)
			{
				action->setIgnorePrevious(
					getIgnoreResourceVal(elem["ignorePrevious"]));
			}
			return action;
		}
		case str2int16("resource.ignore"):
		{
			return std::make_shared<ActResourceIgnore>(
				getStringKey(elem, "id"),
				getIgnoreResourceKey(elem, "ignore", IgnoreResource::Draw | IgnoreResource::Update));
		}
		case str2int16("resource.moveToTop"):
		{
			return std::make_shared<ActResourceMoveToTop>(getStringKey(elem, "id"));
		}
		case str2int16("resource.pop"):
		{
			return std::make_shared<ActResourcePop>(
				getStringKey(elem, "id"),
				getIgnoreResourceKey(elem, "ignorePrevious"));
		}
		case str2int16("resource.popAll"):
		{
			return std::make_shared<ActResourcePopAll>(
				getStringKey(elem, "id"),
				getBoolKey(elem, "popBase"),
				getIgnoreResourceKey(elem, "ignorePrevious"));
		}
		case str2int16("shader.load"):
		{
			return std::make_shared<ActShaderLoad>(
				getStringKey(elem, "id"),
				getStringKey(elem, "fragmentFile"),
				getStringKey(elem, "vertexFile"),
				getStringKey(elem, "geometryFile"));
		}
		case str2int16("shader.setBool"):
		{
			return std::make_shared<ActShaderSetUniform<bool>>(
				getStringKey(elem, "id"),
				getStringKey(elem, "key"),
				getBoolKey(elem, "value"));
		}
		case str2int16("shader.setColor"):
		{
			return std::make_shared<ActShaderSetUniform<sf::Glsl::Vec4>>(
				getStringKey(elem, "id"),
				getStringKey(elem, "key"),
				sf::Glsl::Vec4(getColorKey(elem, "value")));
		}
		case str2int16("shader.setFloat"):
		{
			return std::make_shared<ActShaderSetUniform<float>>(
				getStringKey(elem, "id"),
				getStringKey(elem, "key"),
				getFloatKey(elem, "value"));
		}
		case str2int16("shader.setVec2"):
		{
			return std::make_shared<ActShaderSetUniform<sf::Glsl::Vec2>>(
				getStringKey(elem, "id"),
				getStringKey(elem, "key"),
				getVector2fKey<sf::Glsl::Vec2>(elem, "value"));
		}
		case str2int16("shader.setVec3"):
		{
			return std::make_shared<ActShaderSetUniform<sf::Glsl::Vec3>>(
				getStringKey(elem, "id"),
				getStringKey(elem, "key"),
				getVector3fKey<sf::Glsl::Vec3>(elem, "value"));
		}
		case str2int16("shader.setVec4"):
		{
			return std::make_shared<ActShaderSetUniform<sf::Glsl::Vec4>>(
				getStringKey(elem, "id"),
				getStringKey(elem, "key"),
				getVector4fKey<sf::Glsl::Vec4>(elem, "value"));
		}
		case str2int16("sound.loadPlay"):
		{
			return std::make_shared<ActSoundLoadPlay>(
				getStringKey(elem, "file"),
				getVariableKey(elem, "volume"),
				getBoolKey(elem, "unique"));
		}
		case str2int16("sound.play"):
		{
			return std::make_shared<ActSoundPlay>(
				getStringKey(elem, "id"),
				getVariableKey(elem, "volume"),
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
				getStringKey(elem, "id"),
				getColorKey(elem, "color", sf::Color::White));
		}
		case str2int16("text.setFont"):
		{
			return std::make_shared<ActTextSetFont>(
				getStringKey(elem, "id"),
				getStringKey(elem, "font"));
		}
		case str2int16("text.setSpacing"):
		{
			auto action = std::make_shared<ActTextSetSpacing>(
				getStringKey(elem, "id"));
			if (elem.HasMember("horizontal") == true)
			{
				action->setHorizontalSpaceOffset(getIntVal(elem["horizontal"]));
			}
			if (elem.HasMember("vertical") == true)
			{
				action->setVerticalSpaceOffset(getIntVal(elem["vertical"]));
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
				getStringKey(elem, "file"),
				getBoolKey(elem, "useSaveDir"));
		}
		case str2int16("unmountAll"):
		{
			return std::make_shared<ActUnmountAll>();
		}
		case str2int16("variable.add"):
		{
			return std::make_shared<ActVariableAdd>(
				getStringKey(elem, "key"),
				getVariableKey(elem, "value"));
		}
		case str2int16("variable.clear"):
		{
			return std::make_shared<ActVariableClear>(getStringKey(elem, "key"));
		}
		case str2int16("variable.save"):
		{
			return std::make_shared<ActVariableSave>(
				getStringKey(elem, "file"),
				getStringVectorKey(elem, "vars"));
		}
		case str2int16("variable.set"):
		{
			if (elem.HasMember("values") == true &&
				elem["values"].IsObject() == true)
			{
				return std::make_shared<ActVariablesSet>(
					getVariables(elem["values"]),
					getBoolKey(elem, "resolveValue", true));
			}
			auto key = getStringKey(elem, "key");
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
			auto key = getStringKey(elem, "key");
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
			return nullptr;
		}
	}

	std::shared_ptr<Action> parseAction(Game& game, const Value& elem)
	{
		if (elem.IsNull() == true)
		{
			return nullptr;
		}
		else if (isValidString(elem) == true)
		{
			if (elem.GetString()[0] != '#')
			{
				std::string str(elem.GetString(), elem.GetStringLength());
				return game.Resources().getAction(str);
			}
			else
			{
				std::string_view strNoShebang{ elem.GetString() + 1, elem.GetStringLength() - 1 };
				return game.getQueryAction(strNoShebang);
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
				auto action = parseAction(game, val);
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
			return parseAction(game, elem[key]);
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
			auto action = game.Resources().getAction(elem.GetString());
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
				auto action = parseAction(game, val);
				if (action != nullptr)
				{
					action->execute(game);
				}
			}
		}
	}
}
