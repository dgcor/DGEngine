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
				getString(elem, "id"),
				parseAction(game, elem["action"]));
		}
		case str2int("audio.pause"):
		{
			return std::make_shared<ActAudioPause>(getString(elem, "id"));
		}
		case str2int("audio.pauseAll"):
		{
			return std::make_shared<ActAudioPauseAll>();
		}
		case str2int("audio.play"):
		{
			return std::make_shared<ActAudioPlay>(
				getString(elem, "id"),
				getBool(elem, "clear"),
				getBool(elem, "loop"));
		}
		case str2int("audio.resumeAll"):
		{
			return std::make_shared<ActAudioResumeAll>(getBool(elem, "restart"));
		}
		case str2int("audio.seek"):
		{
			return std::make_shared<ActAudioSeek>(
				getString(elem, "id"),
				sf::seconds((float)getUInt(elem, "seconds")));
		}
		case str2int("audio.setVolume"):
		{
			return std::make_shared<ActAudioSetVolume>(
				getString(elem, "id"),
				getVariable(elem, "volume"));
		}
		case str2int("audio.stop"):
		{
			return std::make_shared<ActAudioStop>(getString(elem, "id"));
		}
		case str2int("audio.stopAll"):
		{
			return std::make_shared<ActAudioStopAll>();
		}
		case str2int("button.click"):
		{
			return std::make_shared<ActButtonClick>(
				getString(elem, "id"),
				getBool(elem, "playSound", true));
		}
		case str2int("button.enable"):
		{
			return std::make_shared<ActButtonEnable>(
				getString(elem, "id"),
				getBool(elem, "enable", true));
		}
		case str2int("button.setBitmapFont"):
		{
			return std::make_shared<ActButtonSetBitmapFont>(
				getString(elem, "id"),
				getString(elem, "idFont"));
		}
		case str2int("button.setClickAction"):
		{
			std::shared_ptr<Action> action;
			if (elem.HasMember("action"))
			{
				action = parseAction(game, elem["action"]);
			}
			return std::make_shared<ActButtonSetClickAction>(
				getString(elem, "id"),
				action);
		}
		case str2int("button.setColor"):
		{
			return std::make_shared<ActButtonSetColor>(
				getString(elem, "id"),
				getColor(elem, "color", sf::Color::White));
		}
		case str2int("button.setFont"):
		{
			return std::make_shared<ActButtonSetFont>(
				getString(elem, "id"),
				getString(elem, "idFont"));
		}
		case str2int("button.setText"):
		{
			return std::make_shared<ActButtonSetText>(
				getString(elem, "id"),
				getString(elem, "text"));
		}
		case str2int("button.setTexture"):
		{
			return std::make_shared<ActSetTexture<BitmapButton>>(
				getString(elem, "id"),
				getString(elem, "texture"),
				getBool(elem, "resetRect"));
		}
		case str2int("button.setTextureRect"):
		{
			return std::make_shared<ActSetTextureRect<BitmapButton>>(
				getString(elem, "id"),
				getIntRect(elem, "rect"));
		}
		case str2int("button.setToggle"):
		{
			return std::make_shared<ActButtonSetToggle>(
				getString(elem, "id"),
				getBool(elem, "toggle"));
		}
		case str2int("condition"):
		{
			return getIfCondition(
				str2int(getString(elem, "condition").c_str()),
				game, elem);
		}
		case str2int("cursor.pop"):
		{
			return std::make_shared<ActCursorPop>(getBool(elem, "popAll"));
		}
		case str2int("cursor.show"):
		{
			return std::make_shared<ActCursorShow>(getBool(elem, "show"));
		}
		case str2int("dir.create"):
		{
			return std::make_shared<ActDirCreate>(getString(elem, "file"));
		}
		case str2int("drawable.addPositionOffset"):
		{
			return std::make_shared<ActDrawableAddPositionOffset>(
				getString(elem, "id"),
				getVector2f<sf::Vector2f>(elem, "offset"));
		}
		case str2int("drawable.addSizeOffset"):
		{
			return std::make_shared<ActDrawableAddSizeOffset>(
				getString(elem, "id"),
				getVector2f<sf::Vector2f>(elem, "offset"));
		}
		case str2int("drawable.anchor"):
		{
			return std::make_shared<ActDrawableAnchor>(
				getString(elem, "id"),
				getString(elem, "idAnchor"),
				getAnchor(elem, "anchor"),
				getVector2f<sf::Vector2f>(elem, "offset"),
				getBool(elem, "addSize"));
		}
		case str2int("drawable.anchorSizeX"):
		{
			return std::make_shared<ActDrawableAnchorSizeX>(
				getString(elem, "id"),
				getString(elem, "idAnchorTo"),
				(float)getInt(elem, "offset"));
		}
		case str2int("drawable.anchorSizeY"):
		{
			return std::make_shared<ActDrawableAnchorSizeY>(
				getString(elem, "id"),
				getString(elem, "idAnchorTo"),
				(float)getInt(elem, "offset"));
		}
		case str2int("drawable.anchorToFocused"):
		{
			return std::make_shared<ActDrawableAnchorToFocused>(
				getString(elem, "id"),
				getAnchor(elem, "anchor"),
				getVector2f<sf::Vector2f>(elem, "offset"),
				getBool(elem, "addSize"));
		}
		case str2int("drawable.centerOnMouseX"):
		{
			return std::make_shared<ActDrawableCenterOnMouseX>(
				getString(elem, "id"),
				getString(elem, "idAnchorTo"),
				getUInt(elem, "range"),
				getVariable(elem, "steps"));
		}
		case str2int("drawable.centerOnMouseY"):
		{
			return std::make_shared<ActDrawableCenterOnMouseY>(
				getString(elem, "id"),
				getString(elem, "idAnchorTo"),
				getUInt(elem, "range"),
				getVariable(elem, "steps"));
		}
		case str2int("drawable.delete"):
		{
			return std::make_shared<ActDrawableDelete>(getString(elem, "id"));
		}
		case str2int("drawable.horizontalAnchorToFocused"):
		{
			return std::make_shared<ActDrawableHorizontalAnchorToFocused>(
				getString(elem, "id"),
				(float)getDouble(elem, "offset"));
		}
		case str2int("drawable.moveX"):
		{
			return std::make_shared<ActDrawableMoveX>(
				getString(elem, "id"),
				getString(elem, "idAnchorTo"),
				getUInt(elem, "range"),
				getVariable(elem, "position"),
				getUInt(elem, "max"),
				getVariable(elem, "steps"));
		}
		case str2int("drawable.moveY"):
		{
			return std::make_shared<ActDrawableMoveY>(
				getString(elem, "id"),
				getString(elem, "idAnchorTo"),
				getUInt(elem, "range"),
				getVariable(elem, "position"),
				getUInt(elem, "max"),
				getVariable(elem, "steps"));
		}
		case str2int("drawable.resizeOnMouseX"):
		{
			return std::make_shared<ActDrawableResizeOnMouseX>(
				getString(elem, "id"),
				getVector2f<sf::Vector2f>(elem, "range"));
		}
		case str2int("drawable.resizeOnMouseY"):
		{
			return std::make_shared<ActDrawableResizeOnMouseY>(
				getString(elem, "id"),
				getVector2f<sf::Vector2f>(elem, "range"));
		}
		case str2int("drawable.resizeToPositionX"):
		{
			return std::make_shared<ActDrawableResizeToPositionX>(
				getString(elem, "id"),
				getString(elem, "idToPosition"),
				(float)getInt(elem, "offset"));
		}
		case str2int("drawable.resizeToPositionY"):
		{
			return std::make_shared<ActDrawableResizeToPositionY>(
				getString(elem, "id"),
				getString(elem, "idToPosition"),
				(float)getInt(elem, "offset"));
		}
		case str2int("drawable.setPosition"):
		{
			return std::make_shared<ActDrawableSetPosition>(
				getString(elem, "id"),
				getVector2f<sf::Vector2f>(elem, "position"),
				getVector2f<sf::Vector2f>(elem, "offset"));
		}
		case str2int("drawable.setPositionX"):
		{
			return std::make_shared<ActDrawableSetPositionX>(
				getString(elem, "id"),
				(float)getDouble(elem, "position"));
		}
		case str2int("drawable.setPositionY"):
		{
			return std::make_shared<ActDrawableSetPositionY>(
				getString(elem, "id"),
				(float)getDouble(elem, "position"));
		}
		case str2int("drawable.setSize"):
		{
			return std::make_shared<ActDrawableSetSize>(
				getString(elem, "id"),
				getVector2f<sf::Vector2f>(elem, "size"));
		}
		case str2int("drawable.setSizeX"):
		{
			return std::make_shared<ActDrawableSetSizeX>(
				getString(elem, "id"),
				(float)getDouble(elem, "size"));
		}
		case str2int("drawable.setSizeY"):
		{
			return std::make_shared<ActDrawableSetSizeY>(
				getString(elem, "id"),
				(float)getDouble(elem, "size"));
		}
		case str2int("drawable.toggleVisible"):
		{
			return std::make_shared<ActToggleVisible>(getString(elem, "id"));
		}
		case str2int("drawable.verticalAnchorToFocused"):
		{
			return std::make_shared<ActDrawableVerticalAnchorToFocused>(
				getString(elem, "id"),
				(float)getDouble(elem, "offset"));
		}
		case str2int("drawable.visible"):
		{
			return std::make_shared<ActSetVisible>(
				getString(elem, "id"),
				getBool(elem, "visible", true));
		}
		case str2int("event.delete"):
		{
			return std::make_shared<ActEventDelete>(getString(elem, "id"));
		}
		case str2int("file.copy"):
		{
			return std::make_shared<ActFileCopy>(
				getString(elem, "dir"),
				getStringVector(elem, "file"),
				getString(elem, "writeFile"));
		}
		case str2int("focus.click"):
		{
			return std::make_shared<ActFocusClick>(getBool(elem, "playSound", true));
		}
		case str2int("focus.moveDown"):
		{
			return std::make_shared<ActFocusMoveDown>();
		}
		case str2int("focus.moveUp"):
		{
			return std::make_shared<ActFocusMoveUp>();
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
			return std::make_shared<ActGameEnableInput>(getBool(elem, "enable", true));
		}
		case str2int("game.fadeIn"):
		{
			auto action = std::make_shared<ActGameFadeIn>(
				getColor(elem, "color"),
				getUInt(elem, "refresh", 15),
				getUInt(elem, "fade", 25));

			if (elem.HasMember("action"))
			{
				action->setAction(parseAction(game, elem["action"]));
			}
			return action;
		}
		case str2int("game.fadeOut"):
		{
			auto action = std::make_shared<ActGameFadeOut>(
				getColor(elem, "color", sf::Color::Transparent),
				getUInt(elem, "refresh", 15),
				getUInt(elem, "fade", 25));

			if (elem.HasMember("action"))
			{
				action->setAction(parseAction(game, elem["action"]));
			}
			return action;
		}
		case str2int("game.pauseOnFocusLoss"):
		{
			return std::make_shared<ActGamePauseOnFocusLoss>(getBool(elem, "pause", true));
		}
		case str2int("game.setMusicVolume"):
		{
			return std::make_shared<ActGameSetMusicVolume>(getVariable(elem, "volume"));
		}
		case str2int("game.setProperty"):
		{
			auto action = std::make_shared<ActGameSetProperty>(
				getString(elem, "property"),
				getVariable(elem, "value"));

			if (elem.HasMember("propRange") == true)
			{
				action->setPropRange(getVector2i<sf::Vector2i>(elem, "propRange"));
			}
			if (elem.HasMember("valueRange") == true)
			{
				action->setValueRange(getVector2i<sf::Vector2i>(elem, "valueRange"));
			}
			return action;
		}
		case str2int("game.setSoundVolume"):
		{
			return std::make_shared<ActGameSetSoundVolume>(getVariable(elem, "volume"));
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
						getString(elem, "id"),
						getIntRect(elem, "rect"));
				}

				std::vector<sf::IntRect> vecRect;
				for (const auto& val : arr)
				{
					vecRect.push_back(getIntRect(val));
				}

				return std::make_shared<ActSetTextureRectVec<Image>>(
					getString(elem, "id"),
					vecRect);
			}
		}
		case str2int("inputText.click"):
		{
			return std::make_shared<ActInputTextClick>(getString(elem, "id"));
		}
		case str2int("io.delete"):
		{
			return std::make_shared<ActIODelete>(getString(elem, "file"));
		}
		case str2int("io.deleteAll"):
		{
			return std::make_shared<ActIODeleteAll>(getString(elem, "file"));
		}
		case str2int("level.move"):
		{
			return std::make_shared<ActLevelMove>(
				getString(elem, "id"),
				getVector2i<sf::Vector2i>(elem, "position"));
		}
		case str2int("level.moveToClick"):
		{
			return std::make_shared<ActLevelMoveToClick>(getString(elem, "id"));
		}
		case str2int("level.moveToPlayer"):
		{
			return std::make_shared<ActLevelMoveToPlayer>(
				getString(elem, "id"),
				getString(elem, "idPlayer"));
		}
		case str2int("load"):
		{
			return std::make_shared<ActLoad>(getStringVector(elem, "file"));
		}
		case str2int("loadRandom"):
		{
			return std::make_shared<ActLoadRandom>(
				getStringVector(elem, "file"),
				getString(elem, "endsWith", ".json"));
		}
		case str2int("loadingScreen.clear"):
		{
			return std::make_shared<ActLoadingScreenClear>();
		}
		case str2int("loadingScreen.setProgress"):
		{
			return std::make_shared<ActLoadingScreenSetProgress>(getInt(elem, "progress"));
		}
		case str2int("loadJson"):
		{
			std::string json;
			if (elem["json"].IsString())
			{
				json = getString(elem, "json");
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
				getString(elem, "id"),
				getUInt(elem, "index"));
		}
		case str2int("menu.moveScrollbar"):
		{
			return std::make_shared<ActMenuMoveScrollbar>(
				getString(elem, "idMenu"),
				getString(elem, "idScrollbar"),
				getString(elem, "idAnchorTo"),
				getUInt(elem, "range"),
				getBool(elem, "focus"));
		}
		case str2int("menu.setBitmapFont"):
		{
			return std::make_shared<ActMenuSetBitmapFont>(
				getString(elem, "id"),
				getUInt(elem, "index"),
				getString(elem, "idFont"));
		}
		case str2int("menu.setColor"):
		{
			return std::make_shared<ActMenuSetColor>(
				getString(elem, "id"),
				getUInt(elem, "index"),
				getColor(elem, "color", sf::Color::White));
		}
		case str2int("menu.setFont"):
		{
			return std::make_shared<ActMenuSetFont>(
				getString(elem, "id"),
				getUInt(elem, "index"),
				getString(elem, "idFont"));
		}
		case str2int("menu.setText"):
		{
			return std::make_shared<ActMenuSetText>(
				getString(elem, "id"),
				getUInt(elem, "index"),
				getString(elem, "text"));
		}
		case str2int("movie.pause"):
		{
			return std::make_shared<ActMoviePause>(getString(elem, "id"));
		}
		case str2int("movie.play"):
		{
			return std::make_shared<ActMoviePlay>(getString(elem, "id"));
		}
		case str2int("player.setCelTexture"):
		{
			return std::make_shared<ActPlayerSetCelTexture>(
				getString(elem, "id"),
				getString(elem, "idLevel"),
				getString(elem, "idCelTexture"));
		}
		case str2int("quest.add"):
		{
			return std::make_shared<ActQuestAdd>(
				getString(elem, "idLevel"),
				parseQuestObj(game, elem));
		}
		case str2int("quest.delete"):
		{
			return std::make_shared<ActQuestDelete>(
				getString(elem, "idLevel"),
				getString(elem, "idQuest"));
		}
		case str2int("quest.setState"):
		{
			return std::make_shared<ActQuestSetState>(
				getString(elem, "idLevel"),
				getString(elem, "idQuest"),
				getInt(elem, "state"));
		}
		case str2int("resource.add"):
		{
			return std::make_shared<ActResourceAdd>(
				getString(elem, "id"),
				getIgnoreResource(elem, "ignorePrevious"));
		}
		case str2int("resource.ignore"):
		{
			return std::make_shared<ActResourceIgnore>(
				getString(elem, "id"),
				getIgnoreResource(elem, "ignore"));
		}
		case str2int("resource.pop"):
		{
			return std::make_shared<ActResourcePop>(
				getString(elem, "id"),
				getIgnoreResource(elem, "ignorePrevious"));
		}
		case str2int("resource.popAll"):
		{
			return std::make_shared<ActResourcePopAll>(
				getString(elem, "id"),
				getBool(elem, "popBase"),
				getIgnoreResource(elem, "ignorePrevious"));
		}
		case str2int("sound.play"):
		{
			return std::make_shared<ActSoundPlay>(
				getString(elem, "id"),
				getVariable(elem, "volume"));
		}
		case str2int("switch"):
		{
			return getSwitchCondition(game, elem);
		}
		case str2int("text.setText"):
		{
			return std::make_shared<ActTextSetText>(
				getString(elem, "id"),
				getString(elem, "text"),
				getUInt(elem, "lineSpacing"));
		}
		case str2int("variable.clear"):
		{
			return std::make_shared<ActVariableClear>(
				getString(elem, "key"));
		}
		case str2int("variable.save"):
		{
			return std::make_shared<ActVariableSave>(
				getString(elem, "file"),
				getStringVector(elem, "vars"));
		}
		case str2int("variable.set"):
		{
			return std::make_shared<ActVariableSet>(
				getString(elem, "key"),
				getVariable(elem, "val"));
		}
		case str2int("variable.setId"):
		{
			return std::make_shared<ActVariableSetId>(
				getString(elem, "id"),
				getString(elem, "key"),
				getString(elem, "property"));
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
