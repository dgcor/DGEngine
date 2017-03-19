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
#include "Actions/ActImage.h"
#include "Actions/ActInputText.h"
#include "Actions/ActIO.h"
#include "Actions/ActiontList.h"
#include "Actions/ActItem.h"
#include "Actions/ActLevel.h"
#include "Actions/ActLoad.h"
#include "Actions/ActLoadingScreen.h"
#include "Actions/ActMenu.h"
#include "Actions/ActMovie.h"
#include "Actions/ActPlayer.h"
#include "Actions/ActQuest.h"
#include "Actions/ActRandom.h"
#include "Actions/ActResource.h"
#include "Actions/ActSound.h"
#include "Actions/ActText.h"
#include "Actions/ActTexture.h"
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

	std::shared_ptr<Action> parseActionElem(Game& game, const Value& elem)
	{
		if (elem.HasMember("name") == false ||
			elem["name"].IsString() == false)
		{
			return nullptr;
		}
		switch (str2int32(elem["name"].GetString()))
		{
		case str2int32("=="):
		{
			return getIfCondition(str2int32("=="), game, elem);
		}
		case str2int32("!="):
		{
			return getIfCondition(str2int32("!="), game, elem);
		}
		case str2int32(">"):
		{
			return getIfCondition(str2int32(">"), game, elem);
		}
		case str2int32(">="):
		{
			return getIfCondition(str2int32(">="), game, elem);
		}
		case str2int32("<"):
		{
			return getIfCondition(str2int32("<"), game, elem);
		}
		case str2int32("<="):
		{
			return getIfCondition(str2int32("<="), game, elem);
		}
		case str2int32("action.set"):
		{
			return std::make_shared<ActActionSet>(
				getStringKey(elem, "id"),
				getActionKey(game, elem, "action"));
		}
		case str2int32("audio.pause"):
		{
			return std::make_shared<ActAudioPause>(getStringKey(elem, "id"));
		}
		case str2int32("audio.pauseAll"):
		{
			return std::make_shared<ActAudioPauseAll>();
		}
		case str2int32("audio.play"):
		{
			auto action = std::make_shared<ActAudioPlay>(
				getStringKey(elem, "id"),
				getBoolKey(elem, "clear"));

			if (elem.HasMember("loop") == true)
			{
				action->setLoop(getBoolVal(elem["loop"]));
			}
			return action;
		}
		case str2int32("audio.resumeAll"):
		{
			return std::make_shared<ActAudioResumeAll>(getBoolKey(elem, "restart"));
		}
		case str2int32("audio.seek"):
		{
			return std::make_shared<ActAudioSeek>(
				getStringKey(elem, "id"),
				sf::seconds((float)getUIntKey(elem, "seconds")));
		}
		case str2int32("audio.setVolume"):
		{
			return std::make_shared<ActAudioSetVolume>(
				getStringKey(elem, "id"),
				getVariableKey(elem, "volume"));
		}
		case str2int32("audio.stop"):
		{
			return std::make_shared<ActAudioStop>(getStringKey(elem, "id"));
		}
		case str2int32("audio.stopAll"):
		{
			return std::make_shared<ActAudioStopAll>();
		}
		case str2int32("button.click"):
		{
			return std::make_shared<ActButtonClick>(
				getStringKey(elem, "id"),
				getBoolKey(elem, "playSound", true));
		}
		case str2int32("button.enable"):
		{
			return std::make_shared<ActButtonEnable>(
				getStringKey(elem, "id"),
				getBoolKey(elem, "enable", true));
		}
		case str2int32("button.setColor"):
		{
			return std::make_shared<ActButtonSetColor>(
				getStringKey(elem, "id"),
				getColorKey(elem, "color", sf::Color::White));
		}
		case str2int32("button.setFont"):
		{
			return std::make_shared<ActButtonSetFont>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idFont"));
		}
		case str2int32("button.setText"):
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
		case str2int32("button.setTexture"):
		{
			return std::make_shared<ActSetTexture<BitmapButton>>(
				getStringKey(elem, "id"),
				getStringKey(elem, "texture"),
				getBoolKey(elem, "resetRect"));
		}
		case str2int32("button.setTextureFromItem"):
		{
			return std::make_shared<ActSetTextureFromItem<BitmapButton>>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idLevel"),
				getItemLocationVal(elem),
				getBoolKey(elem, "equipable", true),
				getBoolKey(elem, "resetRect"));
		}
		case str2int32("button.setTextureRect"):
		{
			return std::make_shared<ActSetTextureRect<BitmapButton>>(
				getStringKey(elem, "id"),
				getIntRectKey(elem, "rect"));
		}
		case str2int32("button.setToggle"):
		{
			return std::make_shared<ActButtonSetToggle>(
				getStringKey(elem, "id"),
				getBoolKey(elem, "toggle"));
		}
		case str2int32("condition"):
		{
			return getIfCondition(
				str2int32(getStringKey(elem, "condition").c_str()),
				game, elem);
		}
		case str2int32("cursor.pop"):
		{
			return std::make_shared<ActCursorPop>(getBoolKey(elem, "popAll"));
		}
		case str2int32("cursor.show"):
		{
			return std::make_shared<ActCursorShow>(getBoolKey(elem, "show"));
		}
		case str2int32("dir.create"):
		{
			return std::make_shared<ActDirCreate>(getStringKey(elem, "file"));
		}
		case str2int32("drawable.addPositionOffset"):
		{
			return std::make_shared<ActDrawableAddPositionOffset>(
				getStringKey(elem, "id"),
				getVector2fKey<sf::Vector2f>(elem, "offset"));
		}
		case str2int32("drawable.addSizeOffset"):
		{
			return std::make_shared<ActDrawableAddSizeOffset>(
				getStringKey(elem, "id"),
				getVector2fKey<sf::Vector2f>(elem, "offset"));
		}
		case str2int32("drawable.anchor"):
		{
			return std::make_shared<ActDrawableAnchor>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idAnchor"),
				getAnchorKey(elem, "anchor"),
				getVector2fKey<sf::Vector2f>(elem, "offset"),
				getBoolKey(elem, "addSize"));
		}
		case str2int32("drawable.anchorSizeX"):
		{
			return std::make_shared<ActDrawableAnchorSizeX>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idAnchorTo"),
				(float)getIntKey(elem, "offset"));
		}
		case str2int32("drawable.anchorSizeY"):
		{
			return std::make_shared<ActDrawableAnchorSizeY>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idAnchorTo"),
				(float)getIntKey(elem, "offset"));
		}
		case str2int32("drawable.anchorToFocused"):
		{
			return std::make_shared<ActDrawableAnchorToFocused>(
				getStringKey(elem, "id"),
				getAnchorKey(elem, "anchor"),
				getVector2fKey<sf::Vector2f>(elem, "offset"),
				getBoolKey(elem, "addSize"));
		}
		case str2int32("drawable.centerOnMouseX"):
		{
			return std::make_shared<ActDrawableCenterOnMouseX>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idAnchorTo"),
				getUIntKey(elem, "range"),
				getVariableKey(elem, "steps"));
		}
		case str2int32("drawable.centerOnMouseY"):
		{
			return std::make_shared<ActDrawableCenterOnMouseY>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idAnchorTo"),
				getUIntKey(elem, "range"),
				getVariableKey(elem, "steps"));
		}
		case str2int32("drawable.delete"):
		{
			return std::make_shared<ActDrawableDelete>(getStringKey(elem, "id"));
		}
		case str2int32("drawable.horizontalAnchorToFocused"):
		{
			return std::make_shared<ActDrawableHorizontalAnchorToFocused>(
				getStringKey(elem, "id"),
				(float)getIntKey(elem, "offset"));
		}
		case str2int32("drawable.moveX"):
		{
			return std::make_shared<ActDrawableMoveX>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idAnchorTo"),
				getUIntKey(elem, "range"),
				getVariableKey(elem, "position"),
				getUIntKey(elem, "max"),
				getVariableKey(elem, "steps"));
		}
		case str2int32("drawable.moveY"):
		{
			return std::make_shared<ActDrawableMoveY>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idAnchorTo"),
				getUIntKey(elem, "range"),
				getVariableKey(elem, "position"),
				getUIntKey(elem, "max"),
				getVariableKey(elem, "steps"));
		}
		case str2int32("drawable.moveStepX"):
		{
			return std::make_shared<ActDrawableMoveStepX>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idAnchorTo"),
				getUIntKey(elem, "range"),
				getVariableKey(elem, "steps"),
				getIntKey(elem, "stepOffset"));
		}
		case str2int32("drawable.moveStepY"):
		{
			return std::make_shared<ActDrawableMoveStepY>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idAnchorTo"),
				getUIntKey(elem, "range"),
				getVariableKey(elem, "steps"),
				getIntKey(elem, "stepOffset"));
		}
		case str2int32("drawable.resizeOnMouseX"):
		{
			return std::make_shared<ActDrawableResizeOnMouseX>(
				getStringKey(elem, "id"),
				getVector2fKey<sf::Vector2f>(elem, "range"));
		}
		case str2int32("drawable.resizeOnMouseY"):
		{
			return std::make_shared<ActDrawableResizeOnMouseY>(
				getStringKey(elem, "id"),
				getVector2fKey<sf::Vector2f>(elem, "range"));
		}
		case str2int32("drawable.resizeToPositionX"):
		{
			return std::make_shared<ActDrawableResizeToPositionX>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idToPosition"),
				(float)getIntKey(elem, "offset"));
		}
		case str2int32("drawable.resizeToPositionY"):
		{
			return std::make_shared<ActDrawableResizeToPositionY>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idToPosition"),
				(float)getIntKey(elem, "offset"));
		}
		case str2int32("drawable.setAction"):
		{
			return std::make_shared<ActDrawableSetAction>(
				getStringKey(elem, "id"),
				getStringCharKey(elem, "event"),
				getActionKey(game, elem, "action"));
		}
		case str2int32("drawable.setPosition"):
		{
			return std::make_shared<ActDrawableSetPosition>(
				getStringKey(elem, "id"),
				getVector2fKey<sf::Vector2f>(elem, "position"),
				getVector2fKey<sf::Vector2f>(elem, "offset"));
		}
		case str2int32("drawable.setPositionX"):
		{
			return std::make_shared<ActDrawableSetPositionX>(
				getStringKey(elem, "id"),
				(float)getIntKey(elem, "position"));
		}
		case str2int32("drawable.setPositionY"):
		{
			return std::make_shared<ActDrawableSetPositionY>(
				getStringKey(elem, "id"),
				(float)getIntKey(elem, "position"));
		}
		case str2int32("drawable.setSize"):
		{
			return std::make_shared<ActDrawableSetSize>(
				getStringKey(elem, "id"),
				getVector2fKey<sf::Vector2f>(elem, "size"));
		}
		case str2int32("drawable.setSizeX"):
		{
			return std::make_shared<ActDrawableSetSizeX>(
				getStringKey(elem, "id"),
				(float)getIntKey(elem, "size"));
		}
		case str2int32("drawable.setSizeY"):
		{
			return std::make_shared<ActDrawableSetSizeY>(
				getStringKey(elem, "id"),
				(float)getIntKey(elem, "size"));
		}
		case str2int32("drawable.toggleVisible"):
		{
			return std::make_shared<ActToggleVisible>(getStringKey(elem, "id"));
		}
		case str2int32("drawable.verticalAnchorToFocused"):
		{
			return std::make_shared<ActDrawableVerticalAnchorToFocused>(
				getStringKey(elem, "id"),
				(float)getIntKey(elem, "offset"));
		}
		case str2int32("drawable.visible"):
		{
			return std::make_shared<ActSetVisible>(
				getStringKey(elem, "id"),
				getBoolKey(elem, "visible", true));
		}
		case str2int32("event.add"):
		{
			return std::make_shared<ActEventAdd>(
				getStringKey(elem, "id"),
				getActionKey(game, elem, "action"),
				sf::seconds((float)getUIntKey(elem, "seconds")),
				getBoolKey(elem, "addToFront"));
		}
		case str2int32("event.delete"):
		{
			return std::make_shared<ActEventDelete>(getStringKey(elem, "id"));
		}
		case str2int32("event.resetTime"):
		{
			return std::make_shared<ActEventResetTime>(getStringKey(elem, "id"));
		}
		case str2int32("file.copy"):
		{
			return std::make_shared<ActFileCopy>(
				getStringKey(elem, "dir"),
				getStringVectorKey(elem, "file"),
				getStringKey(elem, "writeFile"),
				getStringKey(elem, "nullText"));
		}
		case str2int32("focus.add"):
		{
			return std::make_shared<ActFocusAdd>(
				getStringKey(elem, "id"),
				getBoolKey(elem, "focus", true));
		}
		case str2int32("focus.click"):
		{
			return std::make_shared<ActFocusClick>(getBoolKey(elem, "playSound", true));
		}
		case str2int32("focus.moveDown"):
		{
			return std::make_shared<ActFocusMoveDown>();
		}
		case str2int32("focus.moveUp"):
		{
			return std::make_shared<ActFocusMoveUp>();
		}
		case str2int32("focus.set"):
		{
			return std::make_shared<ActFocusSet>(
				getStringKey(elem, "id"),
				getBoolKey(elem, "focus", true));
		}
		case str2int32("game.clearPlayingSounds"):
		{
			return std::make_shared<ActGameClearPlayingSounds>();
		}
		case str2int32("game.close"):
		{
			return std::make_shared<ActGameClose>();
		}
		case str2int32("game.enableInput"):
		{
			return std::make_shared<ActGameEnableInput>(getBoolKey(elem, "enable", true));
		}
		case str2int32("game.fadeIn"):
		{
			auto action = std::make_shared<ActGameFade>(
				getColorKey(elem, "color"),
				false,
				getBoolKey(elem, "enableInput", true),
				getUIntKey(elem, "fade", 25),
				sf::milliseconds(getUIntKey(elem, "refresh", 15)));

			if (elem.HasMember("action"))
			{
				action->setAction(parseAction(game, elem["action"]));
			}
			return action;
		}
		case str2int32("game.fadeOut"):
		{
			auto action = std::make_shared<ActGameFade>(
				getColorKey(elem, "color", sf::Color::Transparent),
				true,
				getBoolKey(elem, "enableInput"),
				getUIntKey(elem, "fade", 25),
				sf::milliseconds(getUIntKey(elem, "refresh", 15)));

			if (elem.HasMember("action"))
			{
				action->setAction(parseAction(game, elem["action"]));
			}
			return action;
		}
		case str2int32("game.pauseOnFocusLoss"):
		{
			return std::make_shared<ActGamePauseOnFocusLoss>(getBoolKey(elem, "pause", true));
		}
		case str2int32("game.setMusicVolume"):
		{
			return std::make_shared<ActGameSetMusicVolume>(getVariableKey(elem, "volume"));
		}
		case str2int32("game.setProperty"):
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
		case str2int32("game.setSoundVolume"):
		{
			return std::make_shared<ActGameSetSoundVolume>(getVariableKey(elem, "volume"));
		}
		case str2int32("if.equal"):
		{
			return getIfCondition(str2int32("=="), game, elem);
		}
		case str2int32("if.fileExists"):
		{
			return getIfCondition(str2int32("fileExists"), game, elem);
		}
		case str2int32("if.greater"):
		{
			return getIfCondition(str2int32(">"), game, elem);
		}
		case str2int32("if.inList"):
		{
			return getInListCondition(game, elem);
		}
		case str2int32("if.lower"):
		{
			return getIfCondition(str2int32("<"), game, elem);
		}
		case str2int32("if.notEqual"):
		{
			return getIfCondition(str2int32("!="), game, elem);
		}
		case str2int32("if.resourceExists"):
		{
			return getIfCondition(str2int32("resourceExists"), game, elem);
		}
		case str2int32("image.centerTexture"):
		{
			return std::make_shared<ActImageCenterTexture>(getStringKey(elem, "id"));
		}
		case str2int32("image.setTexture"):
		{
			return std::make_shared<ActSetTexture<Image>>(
				getStringKey(elem, "id"),
				getStringKey(elem, "texture"),
				getBoolKey(elem, "resetRect"));
		}
		case str2int32("image.setTextureFromItem"):
		{
			return std::make_shared<ActSetTextureFromItem<Image>>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idLevel"),
				getItemLocationVal(elem),
				getBoolKey(elem, "equipable", true),
				getBoolKey(elem, "resetRect"));
		}
		case str2int32("image.setTextureRect"):
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
		case str2int32("inputText.click"):
		{
			return std::make_shared<ActInputTextClick>(getStringKey(elem, "id"));
		}
		case str2int32("io.delete"):
		{
			return std::make_shared<ActIODelete>(getStringKey(elem, "file"));
		}
		case str2int32("io.deleteAll"):
		{
			return std::make_shared<ActIODeleteAll>(getStringKey(elem, "file"));
		}
		case str2int32("item.drop"):
		{
			return std::make_shared<ActItemDrop>(
				getStringKey(elem, "idLevel"),
				getItemCoordInventoryVal(elem));
		}
		case str2int32("item.executeDropAction"):
		{
			return std::make_shared<ActItemExecuteDropAction>(
				getStringKey(elem, "idLevel"),
				getStringKey(elem, "idPlayer"));
		}
		case str2int32("item.loadFromLevel"):
		{
			auto action = std::make_shared<ActItemLoadFromLevel>(
				getStringKey(elem, "idLevel"),
				getItemCoordInventoryVal(elem));

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
		case str2int32("item.update"):
		{
			return std::make_shared<ActItemUpdate>(
				getStringKey(elem, "idLevel"),
				getItemCoordInventoryVal(elem));
		}
		case str2int32("level.clearObjects"):
		{
			return std::make_shared<ActLevelClearObjects>(getStringKey(elem, "id"));
		}
		case str2int32("level.clearPlayerClasses"):
		{
			return std::make_shared<ActLevelClearPlayerClasses>(
				getStringKey(elem, "id"),
				(size_t)getUIntKey(elem, "index"));
		}
		case str2int32("level.clearPlayers"):
		{
			return std::make_shared<ActLevelClearPlayers>(
				getStringKey(elem, "id"),
				(size_t)getUIntKey(elem, "index"));
		}
		case str2int32("level.move"):
		{
			return std::make_shared<ActLevelMove>(
				getStringKey(elem, "id"),
				getVector2uKey<MapCoord>(elem, "position"));
		}
		case str2int32("level.moveToClick"):
		{
			return std::make_shared<ActLevelMoveToClick>(getStringKey(elem, "id"));
		}
		case str2int32("level.moveToPlayer"):
		{
			return std::make_shared<ActLevelMoveToPlayer>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idPlayer"));
		}
		case str2int32("level.pause"):
		{
			return std::make_shared<ActLevelPause>(
				getStringKey(elem, "id"),
				getBoolKey(elem, "pause", true));
		}
		case str2int32("level.zoom"):
		{
			return std::make_shared<ActLevelZoom>(
				getStringKey(elem, "id"),
				getIntKey(elem, "zoom", 100),
				getBoolKey(elem, "relative"),
				getBoolKey(elem, "smooth"));
		}
		case str2int32("load"):
		{
			return std::make_shared<ActLoad>(getStringVectorKey(elem, "file"));
		}
		case str2int32("loadRandom"):
		{
			return std::make_shared<ActLoadRandom>(
				getStringVectorKey(elem, "file"),
				getStringKey(elem, "endsWith", ".json"));
		}
		case str2int32("loadingScreen.clear"):
		{
			return std::make_shared<ActLoadingScreenClear>();
		}
		case str2int32("loadingScreen.setProgress"):
		{
			return std::make_shared<ActLoadingScreenSetProgress>(getIntKey(elem, "progress"));
		}
		case str2int32("loadJson"):
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
		case str2int32("menu.click"):
		{
			return std::make_shared<ActMenuClick>(
				getStringKey(elem, "id"),
				getUIntKey(elem, "index"));
		}
		case str2int32("menu.moveScrollbar"):
		{
			return std::make_shared<ActMenuMoveScrollbar>(
				getStringKey(elem, "idMenu"),
				getStringKey(elem, "idScrollbar"),
				getStringKey(elem, "idAnchorTo"),
				getUIntKey(elem, "range"),
				getBoolKey(elem, "focus"));
		}
		case str2int32("menu.setColor"):
		{
			return std::make_shared<ActMenuSetColor>(
				getStringKey(elem, "id"),
				getUIntKey(elem, "index"),
				getColorKey(elem, "color", sf::Color::White));
		}
		case str2int32("menu.setFont"):
		{
			return std::make_shared<ActMenuSetFont>(
				getStringKey(elem, "id"),
				getUIntKey(elem, "index"),
				getStringKey(elem, "idFont"));
		}
		case str2int32("menu.setText"):
		{
			return std::make_shared<ActMenuSetText>(
				getStringKey(elem, "id"),
				getUIntKey(elem, "index"),
				getStringKey(elem, "text"));
		}
		case str2int32("movie.pause"):
		{
			return std::make_shared<ActMoviePause>(getStringKey(elem, "id"));
		}
		case str2int32("movie.play"):
		{
			return std::make_shared<ActMoviePlay>(getStringKey(elem, "id"));
		}
		case str2int32("player.move"):
		{
			return std::make_shared<ActPlayerMove>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idLevel"),
				getVector2uKey<MapCoord>(elem, "position"),
				getBoolKey(elem, "resetDirection"));
		}
		case str2int32("player.moveToClick"):
		{
			return std::make_shared<ActPlayerMoveToClick>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idLevel"));
		}
		case str2int32("player.setPalette"):
		{
			return std::make_shared<ActPlayerSetPalette>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idLevel"),
				getUIntKey(elem, "palette"));
		}
		case str2int32("player.setProperty"):
		{
			return std::make_shared<ActPlayerSetProperty>(
				getStringKey(elem, "id"),
				getStringKey(elem, "idLevel"),
				getStringKey(elem, "property"),
				getVariableKey(elem, "value"));
		}
		case str2int32("quest.add"):
		{
			auto quest = parseQuestObj(game, elem);
			if (isValidId(quest.Id()) == false)
			{
				return nullptr;
			}
			return std::make_shared<ActQuestAdd>(
				getStringKey(elem, "idLevel"),
				quest);
		}
		case str2int32("quest.delete"):
		{
			return std::make_shared<ActQuestDelete>(
				getStringKey(elem, "idLevel"),
				getStringKey(elem, "idQuest"));
		}
		case str2int32("quest.setState"):
		{
			return std::make_shared<ActQuestSetState>(
				getStringKey(elem, "idLevel"),
				getStringKey(elem, "idQuest"),
				getIntKey(elem, "state"));
		}
		case str2int32("randomList"):
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
		case str2int32("random"):
		{
			return std::make_shared<ActRandom>(
				getFloatKey(elem, "percentage", 0.5),
				getActionKey(game, elem, "action1"),
				getActionKey(game, elem, "action2"));
		}
		case str2int32("resource.add"):
		{
			auto id = getStringKey(elem, "id");
			if (isValidId(id) == false)
			{
				return nullptr;
			}
			return std::make_shared<ActResourceAdd>(id,
				getIgnoreResourceKey(elem, "ignorePrevious"));
		}
		case str2int32("resource.ignore"):
		{
			return std::make_shared<ActResourceIgnore>(
				getStringKey(elem, "id"),
				getIgnoreResourceKey(elem, "ignore"));
		}
		case str2int32("resource.pop"):
		{
			return std::make_shared<ActResourcePop>(
				getStringKey(elem, "id"),
				getIgnoreResourceKey(elem, "ignorePrevious"));
		}
		case str2int32("resource.popAll"):
		{
			return std::make_shared<ActResourcePopAll>(
				getStringKey(elem, "id"),
				getBoolKey(elem, "popBase"),
				getIgnoreResourceKey(elem, "ignorePrevious"));
		}
		case str2int32("sound.loadPlay"):
		{
			return std::make_shared<ActSoundLoadPlay>(
				getStringKey(elem, "file"),
				getVariableKey(elem, "volume"));
		}
		case str2int32("sound.play"):
		{
			return std::make_shared<ActSoundPlay>(
				getStringKey(elem, "id"),
				getVariableKey(elem, "volume"));
		}
		case str2int32("switch"):
		{
			return getSwitchCondition(game, elem);
		}
		case str2int32("text.setColor"):
		{
			return std::make_shared<ActTextSetColor>(
				getStringKey(elem, "id"),
				getColorKey(elem, "color", sf::Color::White));
		}
		case str2int32("text.setSpacing"):
		{
			auto action = std::make_shared<ActTextSetSpacing>(getStringKey(elem, "id"));
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
		case str2int32("text.setText"):
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
		case str2int32("text.setTextFromQuery"):
		{
			auto action = std::make_shared<ActTextSetTextFromQuery>(
				getStringKey(elem, "id"),
				getStringKey(elem, "query"),
				getStringKey(elem, "text"),
				getBoolKey(elem, "trim"),
				getBoolKey(elem, "removeEmptyLines"));
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
		case str2int32("variable.clear"):
		{
			return std::make_shared<ActVariableClear>(
				getStringKey(elem, "key"));
		}
		case str2int32("variable.save"):
		{
			return std::make_shared<ActVariableSave>(
				getStringKey(elem, "file"),
				getStringVectorKey(elem, "vars"));
		}
		case str2int32("variable.set"):
		{
			return std::make_shared<ActVariableSet>(
				getStringKey(elem, "key"),
				getVariableKey(elem, "val"));
		}
		case str2int32("variable.setId"):
		{
			return std::make_shared<ActVariableSetId>(
				getStringKey(elem, "id"),
				getStringKey(elem, "key"),
				getStringKey(elem, "property"));
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
