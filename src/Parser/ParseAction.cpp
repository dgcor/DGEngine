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
#include "Actions/ActGame.h"
#include "Actions/ActInputText.h"
#include "Actions/ActIO.h"
#include "Actions/ActiontList.h"
#include "Actions/ActLevel.h"
#include "Actions/ActLoad.h"
#include "Actions/ActLoadingScreen.h"
#include "Actions/ActMenu.h"
#include "Actions/ActMovie.h"
#include "Actions/ActPlayer.h"
#include "Actions/ActQuest.h"
#include "Actions/ActResource.h"
#include "Actions/ActSound.h"
#include "Actions/ActText.h"
#include "Actions/ActTexture.h"
#include "Actions/ActVariable.h"
#include "Actions/ActVisibility.h"
#include "GameUtils.h"
#include "Json/JsonUtils.h"
#include "ParseCondition.h"
#include "ParseQuest.h"
#include "ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using Utils::str2int;

	std::shared_ptr<Action> parseActionElem(Game& game, const Value& elem)
	{
		if (elem.HasMember("name") == false ||
			elem["name"].IsString() == false)
		{
			return nullptr;
		}
		switch (str2int(elem["name"].GetString()))
		{
		case str2int("=="):
		{
			return getIfCondition(str2int("=="), game, elem);
		}
		case str2int("!="):
		{
			return getIfCondition(str2int("!="), game, elem);
		}
		case str2int(">"):
		{
			return getIfCondition(str2int(">"), game, elem);
		}
		case str2int(">="):
		{
			return getIfCondition(str2int(">="), game, elem);
		}
		case str2int("<"):
		{
			return getIfCondition(str2int("<"), game, elem);
		}
		case str2int("<="):
		{
			return getIfCondition(str2int("<="), game, elem);
		}
		case str2int("action.set"):
		{
			return std::make_shared<ActActionSet>(
				getStringKey(elem, "id"),
				parseAction(game, elem["action"]));
		}
		case str2int("audio.pause"):
		{
			return std::make_shared<ActAudioPause>(getStringKey(elem, "id"));
		}
		case str2int("audio.pauseAll"):
		{
			return std::make_shared<ActAudioPauseAll>();
		}
		case str2int("audio.play"):
		{
			return std::make_shared<ActAudioPlay>(
				getStringKey(elem, "id"),
				getBoolKey(elem, "clear"),
				getBoolKey(elem, "loop"));
		}
		case str2int("audio.resumeAll"):
		{
			return std::make_shared<ActAudioResumeAll>(getBoolKey(elem, "restart"));
		}
		case str2int("audio.seek"):
		{
			return std::make_shared<ActAudioSeek>(
				getStringKey(elem, "id"),
				sf::seconds((float)getUIntKey(elem, "seconds")));
		}
		case str2int("audio.setVolume"):
		{
			return std::make_shared<ActAudioSetVolume>(
				getStringKey(elem, "id"),
				getVariableKey(elem, "volume"));
		}
		case str2int("audio.stop"):
		{
			return std::make_shared<ActAudioStop>(getStringKey(elem, "id"));
		}
		case str2int("audio.stopAll"):
		{
			return std::make_shared<ActAudioStopAll>();
		}
		case str2int("button.click"):
		{
			return std::make_shared<ActButtonClick>(
				getStringKey(elem, "id"),
				getBoolKey(elem, "playSound", true));
		}
		case str2int("button.enable"):
		{
			return std::make_shared<ActButtonEnable>(
				getStringKey(elem, "id"),
				getBoolKey(elem, "enable", true));
		}
		case str2int("button.setBitmapFont"):
		{
			return std::make_shared<ActButtonSetBitmapFont>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idFont"));
		}
		case str2int("button.setClickAction"):
		{
			std::shared_ptr<Action> action;
			if (elem.HasMember("action"))
			{
				action = parseAction(game, elem["action"]);
			}
			return std::make_shared<ActButtonSetClickAction>(
				getStringKey(elem, "id"),
				action);
		}
		case str2int("button.setColor"):
		{
			return std::make_shared<ActButtonSetColor>(
				getStringKey(elem, "id"),
				getColorKey(elem, "color", sf::Color::White));
		}
		case str2int("button.setFont"):
		{
			return std::make_shared<ActButtonSetFont>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idFont"));
		}
		case str2int("button.setText"):
		{
			if (elem.HasMember("binding") == false)
			{
				return std::make_shared<ActButtonSetText>(
					getStringKey(elem, "id"),
					getStringKey(elem, "text"));
			}
			else
			{
				return std::make_shared<ActButtonSetText>(
					getStringKey(elem, "id"),
					getStringKey(elem, "format"),
					getStringVectorKey(elem, "binding"));
			}
		}
		case str2int("button.setTexture"):
		{
			return std::make_shared<ActSetTexture<BitmapButton>>(
				getStringKey(elem, "id"),
				getStringKey(elem, "texture"),
				getBoolKey(elem, "resetRect"));
		}
		case str2int("button.setTextureRect"):
		{
			return std::make_shared<ActSetTextureRect<BitmapButton>>(
				getStringKey(elem, "id"),
				getIntRectKey(elem, "rect"));
		}
		case str2int("button.setToggle"):
		{
			return std::make_shared<ActButtonSetToggle>(
				getStringKey(elem, "id"),
				getBoolKey(elem, "toggle"));
		}
		case str2int("condition"):
		{
			return getIfCondition(
				str2int(getStringKey(elem, "condition").c_str()),
				game, elem);
		}
		case str2int("cursor.pop"):
		{
			return std::make_shared<ActCursorPop>(getBoolKey(elem, "popAll"));
		}
		case str2int("cursor.show"):
		{
			return std::make_shared<ActCursorShow>(getBoolKey(elem, "show"));
		}
		case str2int("dir.create"):
		{
			return std::make_shared<ActDirCreate>(getStringKey(elem, "file"));
		}
		case str2int("drawable.addPositionOffset"):
		{
			return std::make_shared<ActDrawableAddPositionOffset>(
				getStringKey(elem, "id"),
				getVector2fKey<sf::Vector2f>(elem, "offset"));
		}
		case str2int("drawable.addSizeOffset"):
		{
			return std::make_shared<ActDrawableAddSizeOffset>(
				getStringKey(elem, "id"),
				getVector2fKey<sf::Vector2f>(elem, "offset"));
		}
		case str2int("drawable.anchor"):
		{
			return std::make_shared<ActDrawableAnchor>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idAnchor"),
				getAnchorKey(elem, "anchor"),
				getVector2fKey<sf::Vector2f>(elem, "offset"),
				getBoolKey(elem, "addSize"));
		}
		case str2int("drawable.anchorSizeX"):
		{
			return std::make_shared<ActDrawableAnchorSizeX>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idAnchorTo"),
				(float)getIntKey(elem, "offset"));
		}
		case str2int("drawable.anchorSizeY"):
		{
			return std::make_shared<ActDrawableAnchorSizeY>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idAnchorTo"),
				(float)getIntKey(elem, "offset"));
		}
		case str2int("drawable.anchorToFocused"):
		{
			return std::make_shared<ActDrawableAnchorToFocused>(
				getStringKey(elem, "id"),
				getAnchorKey(elem, "anchor"),
				getVector2fKey<sf::Vector2f>(elem, "offset"),
				getBoolKey(elem, "addSize"));
		}
		case str2int("drawable.centerOnMouseX"):
		{
			return std::make_shared<ActDrawableCenterOnMouseX>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idAnchorTo"),
				getUIntKey(elem, "range"),
				getVariableKey(elem, "steps"));
		}
		case str2int("drawable.centerOnMouseY"):
		{
			return std::make_shared<ActDrawableCenterOnMouseY>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idAnchorTo"),
				getUIntKey(elem, "range"),
				getVariableKey(elem, "steps"));
		}
		case str2int("drawable.delete"):
		{
			return std::make_shared<ActDrawableDelete>(getStringKey(elem, "id"));
		}
		case str2int("drawable.horizontalAnchorToFocused"):
		{
			return std::make_shared<ActDrawableHorizontalAnchorToFocused>(
				getStringKey(elem, "id"),
				(float)getDoubleKey(elem, "offset"));
		}
		case str2int("drawable.moveX"):
		{
			return std::make_shared<ActDrawableMoveX>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idAnchorTo"),
				getUIntKey(elem, "range"),
				getVariableKey(elem, "position"),
				getUIntKey(elem, "max"),
				getVariableKey(elem, "steps"));
		}
		case str2int("drawable.moveY"):
		{
			return std::make_shared<ActDrawableMoveY>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idAnchorTo"),
				getUIntKey(elem, "range"),
				getVariableKey(elem, "position"),
				getUIntKey(elem, "max"),
				getVariableKey(elem, "steps"));
		}
		case str2int("drawable.moveStepX"):
		{
			return std::make_shared<ActDrawableMoveStepX>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idAnchorTo"),
				getUIntKey(elem, "range"),
				getVariableKey(elem, "steps"),
				getIntKey(elem, "stepOffset"));
		}
		case str2int("drawable.moveStepY"):
		{
			return std::make_shared<ActDrawableMoveStepY>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idAnchorTo"),
				getUIntKey(elem, "range"),
				getVariableKey(elem, "steps"),
				getIntKey(elem, "stepOffset"));
		}
		case str2int("drawable.resizeOnMouseX"):
		{
			return std::make_shared<ActDrawableResizeOnMouseX>(
				getStringKey(elem, "id"),
				getVector2fKey<sf::Vector2f>(elem, "range"));
		}
		case str2int("drawable.resizeOnMouseY"):
		{
			return std::make_shared<ActDrawableResizeOnMouseY>(
				getStringKey(elem, "id"),
				getVector2fKey<sf::Vector2f>(elem, "range"));
		}
		case str2int("drawable.resizeToPositionX"):
		{
			return std::make_shared<ActDrawableResizeToPositionX>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idToPosition"),
				(float)getIntKey(elem, "offset"));
		}
		case str2int("drawable.resizeToPositionY"):
		{
			return std::make_shared<ActDrawableResizeToPositionY>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idToPosition"),
				(float)getIntKey(elem, "offset"));
		}
		case str2int("drawable.setPosition"):
		{
			return std::make_shared<ActDrawableSetPosition>(
				getStringKey(elem, "id"),
				getVector2fKey<sf::Vector2f>(elem, "position"),
				getVector2fKey<sf::Vector2f>(elem, "offset"));
		}
		case str2int("drawable.setPositionX"):
		{
			return std::make_shared<ActDrawableSetPositionX>(
				getStringKey(elem, "id"),
				(float)getDoubleKey(elem, "position"));
		}
		case str2int("drawable.setPositionY"):
		{
			return std::make_shared<ActDrawableSetPositionY>(
				getStringKey(elem, "id"),
				(float)getDoubleKey(elem, "position"));
		}
		case str2int("drawable.setSize"):
		{
			return std::make_shared<ActDrawableSetSize>(
				getStringKey(elem, "id"),
				getVector2fKey<sf::Vector2f>(elem, "size"));
		}
		case str2int("drawable.setSizeX"):
		{
			return std::make_shared<ActDrawableSetSizeX>(
				getStringKey(elem, "id"),
				(float)getDoubleKey(elem, "size"));
		}
		case str2int("drawable.setSizeY"):
		{
			return std::make_shared<ActDrawableSetSizeY>(
				getStringKey(elem, "id"),
				(float)getDoubleKey(elem, "size"));
		}
		case str2int("drawable.toggleVisible"):
		{
			return std::make_shared<ActToggleVisible>(getStringKey(elem, "id"));
		}
		case str2int("drawable.verticalAnchorToFocused"):
		{
			return std::make_shared<ActDrawableVerticalAnchorToFocused>(
				getStringKey(elem, "id"),
				(float)getDoubleKey(elem, "offset"));
		}
		case str2int("drawable.visible"):
		{
			return std::make_shared<ActSetVisible>(
				getStringKey(elem, "id"),
				getBoolKey(elem, "visible", true));
		}
		case str2int("event.delete"):
		{
			return std::make_shared<ActEventDelete>(getStringKey(elem, "id"));
		}
		case str2int("file.copy"):
		{
			return std::make_shared<ActFileCopy>(
				getStringKey(elem, "dir"),
				getStringVectorKey(elem, "file"),
				getStringKey(elem, "writeFile"),
				getStringKey(elem, "nullText"));
		}
		case str2int("focus.click"):
		{
			return std::make_shared<ActFocusClick>(getBoolKey(elem, "playSound", true));
		}
		case str2int("focus.moveDown"):
		{
			return std::make_shared<ActFocusMoveDown>();
		}
		case str2int("focus.moveUp"):
		{
			return std::make_shared<ActFocusMoveUp>();
		}
		case str2int("focus.set"):
		{
			return std::make_shared<ActFocusSet>(
				getStringKey(elem, "id"),
				getBoolKey(elem, "focus", true));
		}
		case str2int("game.clearPlayingSounds"):
		{
			return std::make_shared<ActGameClearPlayingSounds>();
		}
		case str2int("game.close"):
		{
			return std::make_shared<ActGameClose>();
		}
		case str2int("game.enableInput"):
		{
			return std::make_shared<ActGameEnableInput>(getBoolKey(elem, "enable", true));
		}
		case str2int("game.fadeIn"):
		{
			auto action = std::make_shared<ActGameFadeIn>(
				getColorKey(elem, "color"),
				getUIntKey(elem, "refresh", 15),
				getUIntKey(elem, "fade", 25));

			if (elem.HasMember("action"))
			{
				action->setAction(parseAction(game, elem["action"]));
			}
			return action;
		}
		case str2int("game.fadeOut"):
		{
			auto action = std::make_shared<ActGameFadeOut>(
				getColorKey(elem, "color", sf::Color::Transparent),
				getUIntKey(elem, "refresh", 15),
				getUIntKey(elem, "fade", 25));

			if (elem.HasMember("action"))
			{
				action->setAction(parseAction(game, elem["action"]));
			}
			return action;
		}
		case str2int("game.pauseOnFocusLoss"):
		{
			return std::make_shared<ActGamePauseOnFocusLoss>(getBoolKey(elem, "pause", true));
		}
		case str2int("game.setMusicVolume"):
		{
			return std::make_shared<ActGameSetMusicVolume>(getVariableKey(elem, "volume"));
		}
		case str2int("game.setProperty"):
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
		case str2int("game.setSoundVolume"):
		{
			return std::make_shared<ActGameSetSoundVolume>(getVariableKey(elem, "volume"));
		}
		case str2int("if.equal"):
		{
			return getIfCondition(str2int("=="), game, elem);
		}
		case str2int("if.fileExists"):
		{
			return getIfCondition(str2int("fileExists"), game, elem);
		}
		case str2int("if.greater"):
		{
			return getIfCondition(str2int(">"), game, elem);
		}
		case str2int("if.lower"):
		{
			return getIfCondition(str2int("<"), game, elem);
		}
		case str2int("if.notEqual"):
		{
			return getIfCondition(str2int("!="), game, elem);
		}
		case str2int("image.setTextureRect"):
		{
			if (elem.HasMember("rect") && elem["rect"].IsArray())
			{
				const auto& arr = elem["rect"];
				if (arr[0].IsArray() == false)
				{
					return std::make_shared<ActSetTextureRect<Image>>(
						getStringKey(elem, "id"),
						getIntRectKey(elem, "rect"));
				}

				std::vector<sf::IntRect> vecRect;
				for (const auto& val : arr)
				{
					vecRect.push_back(getIntRectVal(val));
				}

				return std::make_shared<ActSetTextureRectVec<Image>>(
					getStringKey(elem, "id"),
					vecRect);
			}
		}
		case str2int("inputText.click"):
		{
			return std::make_shared<ActInputTextClick>(getStringKey(elem, "id"));
		}
		case str2int("io.delete"):
		{
			return std::make_shared<ActIODelete>(getStringKey(elem, "file"));
		}
		case str2int("io.deleteAll"):
		{
			return std::make_shared<ActIODeleteAll>(getStringKey(elem, "file"));
		}
		case str2int("level.move"):
		{
			return std::make_shared<ActLevelMove>(
				getStringKey(elem, "id"),
				getVector2iKey<sf::Vector2i>(elem, "position"));
		}
		case str2int("level.moveToClick"):
		{
			return std::make_shared<ActLevelMoveToClick>(getStringKey(elem, "id"));
		}
		case str2int("level.moveToPlayer"):
		{
			return std::make_shared<ActLevelMoveToPlayer>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idPlayer"));
		}
		case str2int("load"):
		{
			return std::make_shared<ActLoad>(getStringVectorKey(elem, "file"));
		}
		case str2int("loadRandom"):
		{
			return std::make_shared<ActLoadRandom>(
				getStringVectorKey(elem, "file"),
				getStringKey(elem, "endsWith", ".json"));
		}
		case str2int("loadingScreen.clear"):
		{
			return std::make_shared<ActLoadingScreenClear>();
		}
		case str2int("loadingScreen.setProgress"):
		{
			return std::make_shared<ActLoadingScreenSetProgress>(getIntKey(elem, "progress"));
		}
		case str2int("loadJson"):
		{
			std::string json;
			if (elem["json"].IsString())
			{
				json = getStringKey(elem, "json");
			}
			else
			{
				json = JsonUtils::toString(elem["json"]);
			}
			return std::make_shared<ActLoadJson>(json);
		}
		case str2int("menu.click"):
		{
			return std::make_shared<ActMenuClick>(
				getStringKey(elem, "id"),
				getUIntKey(elem, "index"));
		}
		case str2int("menu.moveScrollbar"):
		{
			return std::make_shared<ActMenuMoveScrollbar>(
				getStringKey(elem, "idMenu"),
				getStringKey(elem, "idScrollbar"),
				getStringKey(elem, "idAnchorTo"),
				getUIntKey(elem, "range"),
				getBoolKey(elem, "focus"));
		}
		case str2int("menu.setBitmapFont"):
		{
			return std::make_shared<ActMenuSetBitmapFont>(
				getStringKey(elem, "id"),
				getUIntKey(elem, "index"),
				getStringKey(elem, "idFont"));
		}
		case str2int("menu.setColor"):
		{
			return std::make_shared<ActMenuSetColor>(
				getStringKey(elem, "id"),
				getUIntKey(elem, "index"),
				getColorKey(elem, "color", sf::Color::White));
		}
		case str2int("menu.setFont"):
		{
			return std::make_shared<ActMenuSetFont>(
				getStringKey(elem, "id"),
				getUIntKey(elem, "index"),
				getStringKey(elem, "idFont"));
		}
		case str2int("menu.setText"):
		{
			return std::make_shared<ActMenuSetText>(
				getStringKey(elem, "id"),
				getUIntKey(elem, "index"),
				getStringKey(elem, "text"));
		}
		case str2int("movie.pause"):
		{
			return std::make_shared<ActMoviePause>(getStringKey(elem, "id"));
		}
		case str2int("movie.play"):
		{
			return std::make_shared<ActMoviePlay>(getStringKey(elem, "id"));
		}
		case str2int("player.moveToClick"):
		{
			return std::make_shared<ActPlayerMoveToClick>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idLevel"));
		}
		case str2int("player.setPalette"):
		{
			return std::make_shared<ActPlayerSetPalette>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idLevel"),
				getUIntKey(elem, "palette"));
		}
		case str2int("quest.add"):
		{
			return std::make_shared<ActQuestAdd>(
				getStringKey(elem, "idLevel"),
				parseQuestObj(game, elem));
		}
		case str2int("quest.delete"):
		{
			return std::make_shared<ActQuestDelete>(
				getStringKey(elem, "idLevel"),
				getStringKey(elem, "idQuest"));
		}
		case str2int("quest.setState"):
		{
			return std::make_shared<ActQuestSetState>(
				getStringKey(elem, "idLevel"),
				getStringKey(elem, "idQuest"),
				getIntKey(elem, "state"));
		}
		case str2int("resource.add"):
		{
			return std::make_shared<ActResourceAdd>(
				getStringKey(elem, "id"),
				getIgnoreResourceKey(elem, "ignorePrevious"));
		}
		case str2int("resource.ignore"):
		{
			return std::make_shared<ActResourceIgnore>(
				getStringKey(elem, "id"),
				getIgnoreResourceKey(elem, "ignore"));
		}
		case str2int("resource.pop"):
		{
			return std::make_shared<ActResourcePop>(
				getStringKey(elem, "id"),
				getIgnoreResourceKey(elem, "ignorePrevious"));
		}
		case str2int("resource.popAll"):
		{
			return std::make_shared<ActResourcePopAll>(
				getStringKey(elem, "id"),
				getBoolKey(elem, "popBase"),
				getIgnoreResourceKey(elem, "ignorePrevious"));
		}
		case str2int("sound.play"):
		{
			return std::make_shared<ActSoundPlay>(
				getStringKey(elem, "id"),
				getVariableKey(elem, "volume"));
		}
		case str2int("switch"):
		{
			return getSwitchCondition(game, elem);
		}
		case str2int("text.setText"):
		{
			std::shared_ptr<ActTextSetText> action;
			if (elem.HasMember("binding") == false)
			{
				action = std::make_shared<ActTextSetText>(
					getStringKey(elem, "id"),
					getStringKey(elem, "text"));
			}
			else
			{
				action = std::make_shared<ActTextSetText>(
					getStringKey(elem, "id"),
					getStringKey(elem, "format"),
					getStringVectorKey(elem, "binding"));
			}
			if (elem.HasMember("horizontalSpaceOffset") == true)
			{
				action->setHorizontalSpaceOffset(getIntVal(elem["horizontalSpaceOffset"]));
			}
			if (elem.HasMember("verticalSpaceOffset") == true)
			{
				action->setVerticalSpaceOffset(getIntVal(elem["verticalSpaceOffset"]));
			}
			return action;
		}
		case str2int("variable.clear"):
		{
			return std::make_shared<ActVariableClear>(
				getStringKey(elem, "key"));
		}
		case str2int("variable.save"):
		{
			return std::make_shared<ActVariableSave>(
				getStringKey(elem, "file"),
				getStringVectorKey(elem, "vars"));
		}
		case str2int("variable.set"):
		{
			return std::make_shared<ActVariableSet>(
				getStringKey(elem, "key"),
				getVariableKey(elem, "val"));
		}
		case str2int("variable.setId"):
		{
			return std::make_shared<ActVariableSetId>(
				getStringKey(elem, "id"),
				getStringKey(elem, "key"),
				getStringKey(elem, "property"));
		}
		default:
		{
			return nullptr;
		}
		}
	}

	std::shared_ptr<Action> parseAction(Game& game, const Value& elem)
	{
		if (elem.IsNull() == true)
		{
			return nullptr;
		}
		else if (elem.IsString() == true)
		{
			return game.Resources().getAction(elem.GetString());
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
}
