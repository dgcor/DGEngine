#include "ParseFileGame.h"
#include "Json/JsonUtils.h"
#include "ParseFile.h"
#include "Parser/Game/ParseClassifier.h"
#include "Parser/Game/ParseItem.h"
#include "Parser/Game/ParseItemClass.h"
#include "Parser/Game/ParseLevel.h"
#include "Parser/Game/ParseLevelObject.h"
#include "Parser/Game/ParseLevelObjectClass.h"
#include "Parser/Game/ParsePlayer.h"
#include "Parser/Game/ParsePlayerClass.h"
#include "Parser/Game/ParseQuest.h"
#include "Parser/Game/ParseSpell.h"
#include "Utils/ParseUtils.h"
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseDocumentGameElem(Game& game, uint16_t nameHash16, const Value& elem,
		ReplaceVars& replaceVars, MemoryPoolAllocator<CrtAllocator>& allocator)
	{
		switch (nameHash16)
		{
		case str2int16("classifier"): {
			if (elem.IsArray() == false) {
				parseClassifier(game, elem);
			}
			else {
				for (const auto& val : elem) {
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("item"): {
			if (elem.IsArray() == false) {
				parseItem(game, elem);
			}
			else {
				for (const auto& val : elem) {
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("itemClass"): {
			if (elem.IsArray() == false) {
				parseItemClass(game, elem);
			}
			else {
				for (const auto& val : elem) {
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("level"): {
			if (elem.IsArray() == false) {
				parseLevel(game, elem);
			}
			else {
				for (const auto& val : elem) {
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("levelObject"): {
			if (elem.IsArray() == false) {
				parseLevelObject(game, elem);
			}
			else {
				for (const auto& val : elem) {
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("levelObjectClass"): {
			if (elem.IsArray() == false) {
				parseLevelObjectClass(game, elem);
			}
			else {
				for (const auto& val : elem) {
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("player"): {
			if (elem.IsArray() == false) {
				parsePlayer(game, elem);
			}
			else {
				for (const auto& val : elem) {
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("playerClass"): {
			if (elem.IsArray() == false) {
				parsePlayerClass(game, elem);
			}
			else {
				for (const auto& val : elem) {
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("quest"): {
			if (elem.IsArray() == false) {
				parseQuest(game, elem);
			}
			else {
				for (const auto& val : elem) {
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		case str2int16("spell"): {
			if (elem.IsArray() == false) {
				parseSpell(game, elem);
			}
			else {
				for (const auto& val : elem) {
					parseDocumentElemHelper(game, nameHash16, val, replaceVars, allocator);
				}
			}
			break;
		}
		}
	}
}
