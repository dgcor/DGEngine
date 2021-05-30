#include "ParseFile.h"
#include <cstdarg>
#include "FileUtils.h"
#include "Game.h"
#include "GameUtils.h"
#include "Hooks.h"
#include "Json/JsonUtils.h"
#include "ParseAction.h"
#include "ParseAnimation.h"
#include "ParseAudio.h"
#include "ParseButton.h"
#include "ParseCircle.h"
#include "ParseCursor.h"
#include "ParseEvent.h"
#include "ParseFileBytes.h"
#include "ParseFont.h"
#include "ParseIcon.h"
#include "ParseImage.h"
#include "ParseImageContainer.h"
#include "ParseInputEvent.h"
#include "ParseInputText.h"
#include "ParseLoadingScreen.h"
#include "ParseMenu.h"
#include "ParseMountFile.h"
#include "ParseMovie.h"
#include "ParsePalette.h"
#include "ParsePanel.h"
#include "ParseRectangle.h"
#include "ParseScrollable.h"
#include "ParseShader.h"
#include "ParseShape.h"
#include "ParseSound.h"
#include "ParseText.h"
#include "ParseTexture.h"
#include "ParseTexturePack.h"
#include "ParseVariable.h"
#include "Utils/ParseUtils.h"
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseFile(Game& game, const rapidjson::Value& params);
	void parseDocumentElem(Game& game, uint16_t nameHash16, const Value& elem,
		ReplaceVars& replaceVars, MemoryPoolAllocator<CrtAllocator>& allocator);

	void parseFile(Game& game, const std::string_view fileName)
	{
		auto fileName2 = GameUtils::replaceStringWithVarOrProp(fileName, game);
		if (fileName2 == "null")
		{
			return;
		}

		parseJson(game, FileUtils::readText(fileName2));
	}

	void parseFile(Game& game, const std::vector<std::string>& params)
	{
		if (params.empty() == true)
		{
			return;
		}

		auto fileName = GameUtils::replaceStringWithVarOrProp(params[0], game);
		if (fileName == "null")
		{
			return;
		}

		auto json = FileUtils::readText(fileName);
		for (size_t i = 1; i < params.size(); i++)
		{
			auto param = GameUtils::replaceStringWithVarOrProp(params[i], game);
			Utils::replaceStringInPlace(json, "{" + Utils::toString(i) + "}", param);
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

		auto fileName = GameUtils::replaceStringWithVarOrProp(
			getStringViewIdx(params, 0), game
		);
		if (fileName == "null")
		{
			return;
		}

		auto json = FileUtils::readText(fileName);
		for (SizeType i = 1; i < params.Size(); i++)
		{
			auto param = GameUtils::replaceStringWithVarOrProp(getStringVal(params[i]), game);
			Utils::replaceStringInPlace(json, "{" + Utils::toString(i) + "}", param);
		}
		parseJson(game, json);
	}

	void parseJson(Game& game, const std::string_view json,
		const std::vector<std::string>& params)
	{
		std::string json2(json);
		for (size_t i = 0; i < params.size(); i++)
		{
			auto param = GameUtils::replaceStringWithVarOrProp(params[i], game);
			Utils::replaceStringInPlace(json2, "{" + Utils::toString(i + 1) + "}", param);
		}
		parseJson(game, json2);
	}

	void parseJson(Game& game, const std::string_view json)
	{
		Document doc;  // Default template parameter uses UTF8 and MemoryPoolAllocator.

		if (JsonUtils::loadJson(json, doc) == false)
		{
			return;
		}

		parseDocument(game, doc);
	}

	void parseDocument(Game& game, const Document& doc, ReplaceVars replaceVars_)
	{
		ReplaceVars replaceVars = replaceVars_;
		MemoryPoolAllocator<CrtAllocator> allocator;
		for (auto it = doc.MemberBegin(); it != doc.MemberEnd(); ++it)
		{
			parseDocumentElemHelper(game, str2int16(getStringViewVal(it->name)),
				it->value, replaceVars, allocator);
		}
	}

	void parseDocumentElemHelper(Game& game, uint16_t nameHash16, const Value& elem,
		ReplaceVars& replaceVars, MemoryPoolAllocator<CrtAllocator>& allocator)
	{
		auto replaceVarsInElem = replaceVars;
		bool changeValueType = replaceVars == ReplaceVars::Value;
		if (elem.IsObject() == true)
		{
			replaceVarsInElem = getReplaceVarsKey(elem, "replaceVars", replaceVarsInElem);
			changeValueType = replaceVarsInElem == ReplaceVars::Value;
		}
		if (replaceVarsInElem != ReplaceVars::None)
		{
			Value elemCopy(elem, allocator);
			// if replaceVars is enabled, replace strings between | instead of %
			JsonUtils::replaceValueWithGameVar(
				elemCopy, allocator, game, changeValueType, '|');
			parseDocumentElem(game, nameHash16, elemCopy, replaceVars, allocator);
		}
		else
		{
			parseDocumentElem(game, nameHash16, elem, replaceVars, allocator);
		}
	}

	void parseDocumentElem(Game& game, uint16_t nameHash16, const Value& elem,
		ReplaceVars& replaceVars, MemoryPoolAllocator<CrtAllocator>& allocator)
	{
		if (Hooks::ParseDocumentElem != nullptr &&
			Hooks::ParseDocumentElem(game, nameHash16, elem, replaceVars, allocator) == true)
		{
			return;
		}
		switch (nameHash16)
		{
		case str2int16("action"):
		{
			if (elem.IsArray() == false)
			{
				parseActionAndExecute(game, elem);
			}
			else
			{
				for (const auto& val : elem)
				{
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("animation"):
		{
			if (elem.IsArray() == false)
			{
				parseAnimation(game, elem);
			}
			else
			{
				for (const auto& val : elem)
				{
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("audio"):
		{
			if (elem.IsArray() == false)
			{
				parseAudio(game, elem);
			}
			else
			{
				for (const auto& val : elem)
				{
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("button"):
		{
			if (elem.IsArray() == false)
			{
				parseButton(game, elem);
			}
			else
			{
				for (const auto& val : elem)
				{
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("circle"):
		{
			if (elem.IsArray() == false)
			{
				parseCircle(game, elem);
			}
			else
			{
				for (const auto& val : elem)
				{
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("cursor"):
		{
			if (elem.IsArray() == false)
			{
				parseCursor(game, elem);
			}
			else
			{
				for (const auto& val : elem)
				{
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("event"):
		{
			if (elem.IsArray() == false)
			{
				parseEvent(game, elem);
			}
			else
			{
				for (const auto& val : elem)
				{
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("file"):
		{
			if (elem.IsArray() == false)
			{
				parseFileBytes(game, elem);
			}
			else
			{
				for (const auto& val : elem)
				{
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("font"):
		{
			if (elem.IsArray() == false)
			{
				parseFont(game, elem);
			}
			else
			{
				for (const auto& val : elem)
				{
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("framerate"):
		{
			game.Framerate(getUIntVal(elem));
			break;
		}
		case str2int16("icon"):
		{
			parseIcon(game, elem);
			break;
		}
		case str2int16("image"):
		{
			if (elem.IsArray() == false)
			{
				parseImage(game, elem);
			}
			else
			{
				for (const auto& val : elem)
				{
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("imageContainer"):
		{
			if (elem.IsArray() == false)
			{
				parseImageContainer(game, elem);
			}
			else
			{
				for (const auto& val : elem)
				{
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("init"):
		{
			game.init();
			break;
		}
		case str2int16("inputEvent"):
		{
			if (elem.IsArray() == false)
			{
				parseInputEvent(game, elem);
			}
			else
			{
				for (const auto& val : elem)
				{
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("inputText"):
		{
			if (elem.IsArray() == false)
			{
				parseInputText(game, elem);
			}
			else
			{
				for (const auto& val : elem)
				{
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("keepAR"):
		{
			game.KeepAR(getBoolVal(elem, true));
			break;
		}
		case str2int16("load"):
		{
			if (elem.IsString())
			{
				parseFile(game, elem.GetStringView());
			}
			else
			{
				parseFile(game, elem);
			}
			break;
		}
		case str2int16("loadingScreen"):
		{
			parseLoadingScreen(game, elem);
			break;
		}
		case str2int16("maxWindowHeight"):
		{
			game.MaxHeight(getUIntVal(elem));
			break;
		}
		case str2int16("menu"):
		{
			if (elem.IsArray() == false)
			{
				parseMenu(game, elem);
			}
			else
			{
				for (const auto& val : elem)
				{
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("minWindowSize"):
		{
			sf::Vector2u minSize(Game::MinSizeX, Game::MinSizeY);
			game.MinSize(getVector2uVal<sf::Vector2u>(elem, minSize));
			break;
		}
		case str2int16("mountFile"):
		{
			if (elem.IsArray() == false)
			{
				parseMountFile(game, elem);
			}
			else
			{
				for (const auto& val : elem)
				{
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("movie"):
		{
			if (elem.IsArray() == false)
			{
				parseMovie(game, elem);
			}
			else
			{
				for (const auto& val : elem)
				{
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("palette"):
		{
			if (elem.IsArray() == false)
			{
				parsePalette(game, elem);
			}
			else
			{
				for (const auto& val : elem)
				{
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("panel"):
		{
			if (elem.IsArray() == false)
			{
				parsePanel(game, elem);
			}
			else
			{
				for (const auto& val : elem)
				{
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("rectangle"):
		{
			if (elem.IsArray() == false)
			{
				parseRectangle(game, elem);
			}
			else
			{
				for (const auto& val : elem)
				{
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("refWindowSize"):
		{
			sf::Vector2u refSize(Game::RefSizeX, Game::RefSizeY);
			game.RefSize(getVector2uVal<sf::Vector2u>(elem, refSize));
			break;
		}
		case str2int16("replaceVars"):
		{
			replaceVars = getReplaceVarsVal(elem);
			break;
		}
		case str2int16("saveDir"):
		{
			auto saveDir = getStringViewVal(elem);
			if (saveDir.size() > 0 && FileUtils::setSaveDir(saveDir.data()) == true)
			{
				PHYSFS_mount(PHYSFS_getWriteDir(), nullptr, 0);
			}
			break;
		}
		case str2int16("scrollable"):
		{
			if (elem.IsArray() == false)
			{
				parseScrollable(game, elem);
			}
			else
			{
				for (const auto& val : elem)
				{
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("shader"):
		{
			if (elem.IsArray() == false)
			{
				parseShader(game, elem);
			}
			else
			{
				for (const auto& val : elem)
				{
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("shape"):
		{
			if (elem.IsArray() == false)
			{
				parseShape(game, elem);
			}
			else
			{
				for (const auto& val : elem)
				{
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("smoothScreen"):
		{
			game.SmoothScreen(getBoolVal(elem));
			break;
		}
		case str2int16("sound"):
		{
			if (elem.IsArray() == false)
			{
				parseSound(game, elem);
			}
			else
			{
				for (const auto& val : elem)
				{
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("stretchToFit"):
		{
			game.StretchToFit(getBoolVal(elem));
			break;
		}
		case str2int16("text"):
		{
			if (elem.IsArray() == false)
			{
				parseText(game, elem);
			}
			else
			{
				for (const auto& val : elem)
				{
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("texture"):
		{
			if (elem.IsArray() == false)
			{
				parseTexture(game, elem);
			}
			else
			{
				for (const auto& val : elem)
				{
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("texturePack"):
		{
			if (elem.IsArray() == false)
			{
				parseTexturePack(game, elem);
			}
			else
			{
				for (const auto& val : elem)
				{
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("title"):
		{
			game.setTitle(getStringVal(elem, game.getTitle()));
			break;
		}
		case str2int16("variable"):
		{
			if (elem.IsArray() == false)
			{
				parseVariable(game, elem);
			}
			else
			{
				for (const auto& val : elem)
				{
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("version"):
		{
			game.setVersion(getStringVal(elem, game.getVersion()));
			break;
		}
		case str2int16("windowSize"):
		{
			sf::Vector2u defSize(Game::DefaultSizeX, Game::DefaultSizeY);
			game.WindowSize(getVector2uVal<sf::Vector2u>(elem, defSize));
			break;
		}
		default:
			break;
		}
	}
}
