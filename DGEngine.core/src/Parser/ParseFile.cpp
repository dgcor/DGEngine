#include "ParseFile.h"
#include <cstdarg>
#include "Game/Game.h"
#include "Game/Utils/FileUtils.h"
#include "Game/Utils/GameUtils.h"
#include "Hooks.h"
#include "Json/JsonUtils.h"
#include "ParseAction.h"
#include "ParseCursor.h"
#include "ParseEvent.h"
#include "ParseGameInputEvent.h"
#include "ParseInputEvent.h"
#include "ParseMountFile.h"
#include "Parser/Drawables/ParseAnimation.h"
#include "Parser/Drawables/ParseButton.h"
#include "Parser/Drawables/ParseCircle.h"
#include "Parser/Drawables/ParseImage.h"
#include "Parser/Drawables/ParseInputText.h"
#include "Parser/Drawables/ParseLoadingScreen.h"
#include "Parser/Drawables/ParseMenu.h"
#include "Parser/Drawables/ParseMovie.h"
#include "Parser/Drawables/ParsePanel.h"
#include "Parser/Drawables/ParseRectangle.h"
#include "Parser/Drawables/ParseScrollable.h"
#include "Parser/Drawables/ParseShape.h"
#include "Parser/Drawables/ParseText.h"
#include "Parser/Game/ParseGame.h"
#include "Parser/Resources/ParseAudio.h"
#include "Parser/Resources/ParseFileBytes.h"
#include "Parser/Resources/ParseFont.h"
#include "Parser/Resources/ParseImageContainer.h"
#include "Parser/Resources/ParsePalette.h"
#include "Parser/Resources/ParseShader.h"
#include "Parser/Resources/ParseSound.h"
#include "Parser/Resources/ParseTexture.h"
#include "Parser/Resources/ParseTexturePack.h"
#include "ParseVariable.h"
#include "Utils/Log.h"
#include "Utils/ParseUtils.h"
#include "Utils/StringHash.h"
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseDocumentElem(Game& game, uint16_t nameHash16, const Value& elem,
		ReplaceVars& replaceVars, MemoryPoolAllocator<CrtAllocator>& allocator);

	void parseFile(Game& game, const std::string_view fileName)
	{
		auto fileName2 = GameUtils::replaceStringWithVarOrProp(fileName, game);
		if (fileName2 == "null")
		{
			SPDLOG_WARN("parseFile name null");
			return;
		}

		if (parseJson(game, FileUtils::readText(fileName2)) == true)
		{
			SPDLOG_INFO("parseFile: {}", fileName2);
		}
		else
		{
			SPDLOG_WARN("parseFile failed: {}", fileName2);
		}
	}

	void parseFile(Game& game, const std::vector<std::string>& params)
	{
		if (params.empty() == true)
		{
			SPDLOG_WARN("parseFile invalid params");
			return;
		}

		auto fileName = GameUtils::replaceStringWithVarOrProp(params[0], game);
		if (fileName == "null")
		{
			SPDLOG_WARN("parseFile name null");
			return;
		}

		auto json = FileUtils::readText(fileName);
		for (size_t i = 1; i < params.size(); i++)
		{
			auto param = GameUtils::replaceStringWithVarOrProp(params[i], game);
			Utils::replaceStringInPlace(json, "{" + Utils::toString(i) + "}", param);
		}
		if (parseJson(game, json) == true)
		{
			SPDLOG_INFO("parseFile: {}", fileName);
		}
		else
		{
			SPDLOG_WARN("parseFile failed: {}", fileName);
		}
	}

	void parseFile(Game& game, const Value& params)
	{
		if (params.IsArray() == false ||
			params.Empty() == true)
		{
			SPDLOG_WARN("parseFile invalid params");
			return;
		}

		auto fileName = GameUtils::replaceStringWithVarOrProp(
			getStringViewVal(params[0]), game
		);
		if (fileName == "null")
		{
			SPDLOG_WARN("parseFile name null");
			return;
		}

		auto json = FileUtils::readText(fileName);
		for (SizeType i = 1; i < params.Size(); i++)
		{
			auto param = GameUtils::replaceStringWithVarOrProp(getStringVal(params[i]), game);
			Utils::replaceStringInPlace(json, "{" + Utils::toString(i) + "}", param);
		}
		if (parseJson(game, json) == true)
		{
			SPDLOG_INFO("parseFile: {}", fileName);
		}
		else
		{
			SPDLOG_WARN("parseFile failed: {}", fileName);
		}
	}

	bool parseJson(Game& game, const std::string_view json,
		const std::vector<std::string>& params)
	{
		std::string json2(json);
		for (size_t i = 0; i < params.size(); i++)
		{
			auto param = GameUtils::replaceStringWithVarOrProp(params[i], game);
			Utils::replaceStringInPlace(json2, "{" + Utils::toString(i + 1) + "}", param);
		}
		return parseJson(game, json2);
	}

	bool parseJson(Game& game, const std::string_view json)
	{
		if (json.empty() == true)
		{
			return false;
		}

		Document doc; // Default template parameter uses UTF8 and MemoryPoolAllocator.

		if (JsonUtils::loadJson(json, doc) == false)
		{
			SPDLOG_WARN("parseJson failed");
			return false;
		}

		parseDocument(game, doc);
		return true;
	}

	void parseDocument(Game& game, Document& doc, ReplaceVars replaceVars_)
	{
		ReplaceVars replaceVars = replaceVars_;
		for (auto& it : std::ranges::subrange(doc.MemberBegin(), doc.MemberEnd()))
		{
			parseDocumentElemHelper(game, str2int16(it.name.GetStringView()),
				it.value, replaceVars, doc.GetAllocator());
		}
	}

	void parseLoad(Game& game, const Value& elem)
	{
		if (elem.IsString())
		{
			parseFile(game, elem.GetStringView());
		}
		else
		{
			parseFile(game, elem);
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
			JsonUtils::replaceValuesWithGameVar(elemCopy, allocator, game, changeValueType, '|');
			parseDocumentElem(game, nameHash16, elemCopy, replaceVars, allocator);
		}
		else
		{
			parseDocumentElem(game, nameHash16, elem, replaceVars, allocator);
		}
	}

	void parseDocumentElemArray(const parseDocumentElemFuncPtr parseFunc, Game& game,
		uint16_t nameHash16, const Value& elem, ReplaceVars& replaceVars,
		MemoryPoolAllocator<CrtAllocator>& allocator)
	{
		if (elem.IsArray() == false)
		{
			parseFunc(game, elem);
		}
		else
		{
			for (const auto& val : elem)
			{
				parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
			}
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
			parseDocumentElemArray(parseActionAndExecute, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("animation"):
		{
			parseDocumentElemArray(parseAnimation, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("audio"):
		{
			parseDocumentElemArray(parseAudio, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("button"):
		{
			parseDocumentElemArray(parseButton, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("circle"):
		{
			parseDocumentElemArray(parseCircle, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("cursor"):
		{
			parseDocumentElemArray(parseCursor, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("event"):
		{
			parseDocumentElemArray(parseEvent, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("file"):
		{
			parseDocumentElemArray(parseFileBytes, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("font"):
		{
			parseDocumentElemArray(parseFont, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("game"):
		{
			parseGame(game, elem);
			break;
		}
		case str2int16("gameInputEvent"):
		{
			parseDocumentElemArray(parseGameInputEvent, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("image"):
		{
			parseDocumentElemArray(parseImage, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("imageContainer"):
		{
			parseDocumentElemArray(parseImageContainer, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("inputEvent"):
		{
			parseDocumentElemArray(parseInputEvent, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("inputText"):
		{
			parseDocumentElemArray(parseInputText, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("load"):
		{
			parseLoad(game, elem);
			break;
		}
		case str2int16("loadingScreen"):
		{
			parseLoadingScreen(game, elem);
			break;
		}
		case str2int16("menu"):
		{
			parseDocumentElemArray(parseMenu, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("mountFile"):
		{
			parseDocumentElemArray(parseMountFile, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("movie"):
		{
			parseDocumentElemArray(parseMovie, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("palette"):
		{
			parseDocumentElemArray(parsePalette, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("panel"):
		{
			parseDocumentElemArray(parsePanel, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("rectangle"):
		{
			parseDocumentElemArray(parseRectangle, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("replaceVars"):
		{
			replaceVars = getReplaceVarsVal(elem);
			break;
		}
		case str2int16("scrollable"):
		{
			parseDocumentElemArray(parseScrollable, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("shader"):
		{
			parseDocumentElemArray(parseShader, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("shape"):
		{
			parseDocumentElemArray(parseShape, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("sound"):
		{
			parseDocumentElemArray(parseSound, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("text"):
		{
			parseDocumentElemArray(parseText, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("texture"):
		{
			parseDocumentElemArray(parseTexture, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("texturePack"):
		{
			parseDocumentElemArray(parseTexturePack, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("variable"):
		{
			parseDocumentElemArray(parseVariable, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		default:
			break;
		}
	}
}
