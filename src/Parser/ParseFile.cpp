#include "ParseFile.h"

#include <cstdarg>
#include "FileUtils.h"
#include "ParseAction.h"
#include "ParseAnimation.h"
#include "ParseAudio.h"
#include "ParseBitmapFont.h"
#include "ParseButton.h"
#include "ParseCelFile.h"
#include "ParseCelTexture.h"
#include "ParseCelTexturePack.h"
#include "ParseCircle.h"
#include "ParseCursor.h"
#include "ParseEvent.h"
#include "ParseFont.h"
#include "ParseIcon.h"
#include "ParseImage.h"
#include "ParseInputText.h"
#include "ParseKeyboard.h"
#include "ParseLoadingScreen.h"
#include "ParseMenu.h"
#include "ParseMountFile.h"
#include "ParseMovie.h"
#include "ParsePalette.h"
#include "ParseRectangle.h"
#include "ParseScrollableText.h"
#include "ParseSound.h"
#include "ParseVariable.h"
#include "ParseText.h"
#include "ParseTexture.h"
#include "Parser/Game/ParseItem.h"
#include "Parser/Game/ParseItemClass.h"
#include "Parser/Game/ParseLevel.h"
#include "Parser/Game/ParseLevelObject.h"
#include "Parser/Game/ParsePlayer.h"
#include "Parser/Game/ParsePlayerClass.h"
#include "Parser/Game/ParseQuest.h"
#include "PhysFSStream.h"
#include "Utils.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using Utils::str2int;

	void parseFile(Game& game, const std::string& fileName)
	{
		if (fileName == "null")
		{
			return;
		}

		parseJson(game, FileUtils::readText(fileName.c_str()));
	}

	void parseFile(Game& game, const std::vector<std::string>& params)
	{
		if (params.empty() == true)
		{
			return;
		}

		auto fileName = params[0];
		if (fileName == "null")
		{
			return;
		}

		auto json = FileUtils::readText(fileName.c_str());
		for (size_t i = 1; i < params.size(); i++)
		{
			auto param = game.getVariableString(params[i]);
			Utils::replaceStringInPlace(json, "{" + std::to_string(i) + "}", param);
		}
		parseJson(game, json);
	}

	void parseFile(Game& game, const Value& params)
	{
		if (params.IsArray() == false)
		{
			return;
		}
		if (params.Empty() == true)
		{
			return;
		}

		auto fileName = getStringIdx(params, 0);
		if (fileName == "null")
		{
			return;
		}

		auto json = FileUtils::readText(fileName.c_str());
		for (size_t i = 1; i < params.Size(); i++)
		{
			auto param = game.getVariableString(getStringVal(params[i]));
			Utils::replaceStringInPlace(json, "{" + std::to_string(i) + "}", param);
		}
		parseJson(game, json);
	}

	void parseJson(Game& game, const std::string& str)
	{
		if (str.empty() == true)
		{
			return;
		}

		rapidjson::Document doc;  // Default template parameter uses UTF8 and MemoryPoolAllocator.

		if (doc.Parse(str.c_str()).HasParseError() == true)
		{
			return;
		}

		parseDocument(game, doc);
	}

	void parseDocument(Game& game, const Document& doc)
	{
		for (auto it = doc.MemberBegin(); it != doc.MemberEnd(); ++it)
		{
			switch (str2int(it->name.GetString()))
			{
			case str2int("action"): {
				parseActionAndExecute(game, it->value);
				break;
			}
			case str2int("animation"): {
				if (it->value.IsArray() == false) {
					parseAnimation(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parseAnimation(game, val);
					}
				}
				break;
			}
			case str2int("audio"): {
				if (it->value.IsArray() == false) {
					parseAudio(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parseAudio(game, val);
					}
				}
				break;
			}
			case str2int("bitmapFont"): {
				if (it->value.IsArray() == false) {
					parseBitmapFont(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parseBitmapFont(game, val);
					}
				}
				break;
			}
			case str2int("button"): {
				if (it->value.IsArray() == false) {
					parseButton(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parseButton(game, val);
					}
				}
				break;
			}
			case str2int("celFile"): {
				if (it->value.IsArray() == false) {
					parseCelFile(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parseCelFile(game, val);
					}
				}
				break;
			}
			case str2int("celTexture"): {
				if (it->value.IsArray() == false) {
					parseCelTexture(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parseCelTexture(game, val);
					}
				}
				break;
			}
			case str2int("celTexturePack"): {
				if (it->value.IsArray() == false) {
					parseCelTexturePack(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parseCelTexturePack(game, val);
					}
				}
				break;
			}
			case str2int("circle"): {
				if (it->value.IsArray() == false) {
					parseCircle(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parseCircle(game, val);
					}
				}
				break;
			}
			case str2int("cursor"): {
				if (it->value.IsArray() == false) {
					parseCursor(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parseCursor(game, val);
					}
				}
				break;
			}
			case str2int("event"): {
				if (it->value.IsArray() == false) {
					parseEvent(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parseEvent(game, val);
					}
				}
				break;
			}
			case str2int("font"): {
				if (it->value.IsArray() == false) {
					parseFont(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parseFont(game, val);
					}
				}
				break;
			}
			case str2int("framerate"): {
				game.Framerate(getUIntVal(it->value));
				break;
			}
			case str2int("icon"): {
				parseIcon(game, it->value);
				break;
			}
			case str2int("image"): {
				if (it->value.IsArray() == false) {
					parseImage(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parseImage(game, val);
					}
				}
				break;
			}
			case str2int("item"): {
				if (it->value.IsArray() == false) {
					parseItem(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parseItem(game, val);
					}
				}
				break;
			}
			case str2int("itemClass"): {
				if (it->value.IsArray() == false) {
					parseItemClass(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parseItemClass(game, val);
					}
				}
				break;
			}
			case str2int("init"): {
				game.init();
				break;
			}
			case str2int("inputText"): {
				if (it->value.IsArray() == false) {
					parseInputText(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parseInputText(game, val);
					}
				}
				break;
			}
			case str2int("keepAR"): {
				game.KeepAR(getBoolVal(it->value, true));
				break;
			}
			case str2int("keyboard"): {
				if (it->value.IsArray() == false) {
					parseKeyboard(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parseKeyboard(game, val);
					}
				}
				break;
			}
			case str2int("level"): {
				if (it->value.IsArray() == false) {
					parseLevel(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parseLevel(game, val);
					}
				}
				break;
			}
			case str2int("levelObject"): {
				if (it->value.IsArray() == false) {
					parseLevelObject(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parseLevelObject(game, val);
					}
				}
				break;
			}
			case str2int("load"): {
				if (it->value.IsString()) {
					parseFile(game, it->value.GetString());
				}
				else {
					parseFile(game, it->value);
				}
				break;
			}
			case str2int("loadingScreen"): {
				parseLoadingScreen(game, it->value);
				break;
			}
			case str2int("menu"): {
				if (it->value.IsArray() == false) {
					parseMenu(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parseMenu(game, val);
					}
				}
				break;
			}
			case str2int("minWindowSize"): {
				sf::Vector2u minSize(640, 480);
				game.MinSize(getVector2uVal<sf::Vector2u>(it->value, minSize));
				break;
			}
			case str2int("mountFile"): {
				if (it->value.IsArray() == false) {
					parseMountFile(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parseMountFile(game, val);
					}
				}
				break;
			}
			case str2int("movie"): {
				if (it->value.IsArray() == false) {
					parseMovie(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parseMovie(game, val);
					}
				}
				break;
			}
			case str2int("palette"): {
				if (it->value.IsArray() == false) {
					parsePalette(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parsePalette(game, val);
					}
				}
				break;
			}
			case str2int("player"): {
				if (it->value.IsArray() == false) {
					parsePlayer(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parsePlayer(game, val);
					}
				}
				break;
			}
			case str2int("playerClass"): {
				if (it->value.IsArray() == false) {
					parsePlayerClass(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parsePlayerClass(game, val);
					}
				}
				break;
			}
			case str2int("quest"): {
				if (it->value.IsArray() == false) {
					parseQuest(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parseQuest(game, val);
					}
				}
				break;
			}
			case str2int("rectangle"): {
				if (it->value.IsArray() == false) {
					parseRectangle(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parseRectangle(game, val);
					}
				}
				break;
			}
			case str2int("refWindowSize"): {
				sf::Vector2u refSize(640, 480);
				game.RefSize(getVector2uVal<sf::Vector2u>(it->value, refSize));
				break;
			}
			case str2int("saveDir"): {
				auto saveDir = getStringVal(it->value);
				if (saveDir.size() > 0 && FileUtils::setSaveDir(saveDir.c_str()) == true)
				{
					PHYSFS_mount(PHYSFS_getWriteDir(), NULL, 0);
				}
				break;
			}
			case str2int("scrollableText"): {
				if (it->value.IsArray() == false) {
					parseScrollableText(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parseScrollableText(game, val);
					}
				}
				break;
			}
			case str2int("smoothScreen"): {
				game.SmoothScreen(getBoolVal(it->value));
				break;
			}
			case str2int("sound"): {
				if (it->value.IsArray() == false) {
					parseSound(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parseSound(game, val);
					}
				}
				break;
			}
			case str2int("stretchToFit"): {
				game.StretchToFit(getBoolVal(it->value));
				break;
			}
			case str2int("text"): {
				if (it->value.IsArray() == false) {
					parseText(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parseText(game, val);
					}
				}
				break;
			}
			case str2int("texture"): {
				if (it->value.IsArray() == false) {
					parseTexture(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parseTexture(game, val);
					}
				}
				break;
			}
			case str2int("title"): {
				game.setTitle(getStringVal(it->value, game.getTitle()));
				break;
			}
			case str2int("variable"): {
				if (it->value.IsArray() == false) {
					parseVariable(game, it->value);
				}
				else {
					for (const auto& val : it->value) {
						parseVariable(game, val);
					}
				}
				break;
			}
			case str2int("version"): {
				game.setVersion(getStringVal(it->value, game.getVersion()));
				break;
			}
			case str2int("windowSize"): {
				sf::Vector2u minSize(640, 480);
				game.WindowSize(getVector2uVal<sf::Vector2u>(it->value, minSize));
				break;
			}
			}
		}
	}
}
