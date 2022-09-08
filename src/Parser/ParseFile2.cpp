#include "ParseFile2.h"
#include "Json/JsonUtils.h"
#include "Parser/Classifier/ParseClassifier.h"
#include "Parser/Drawables/ParsePanel2.h"
#include "Parser/Item/ParseItem.h"
#include "Parser/Item/ParseItemClass.h"
#include "Parser/Level/ParseLevel.h"
#include "Parser/LevelObject/ParseLevelObject.h"
#include "Parser/LevelObject/ParseLevelObjectClass.h"
#include "Parser/ParseFile.h"
#include "Parser/Player/ParsePlayer.h"
#include "Parser/Player/ParsePlayerClass.h"
#include "Parser/Quest/ParseQuest.h"
#include "Parser/Resources/ParseTexturePack2.h"
#include "Parser/Spell/ParseSpell.h"
#include "Parser/Utils/ParseUtils.h"
#include "Utils/StringHash.h"

namespace Parser2
{
	using namespace Parser;
	using namespace rapidjson;

	bool parseDocumentElem(Game& game, uint16_t nameHash16, const Value& elem,
		ReplaceVars& replaceVars, MemoryPoolAllocator<CrtAllocator>& allocator)
	{
		switch (nameHash16)
		{
		case str2int16("classifier"):
		{
			parseDocumentElemArray(parseClassifier, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("item"):
		{
			parseDocumentElemArray(parseItem, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("itemClass"):
		{
			parseDocumentElemArray(parseItemClass, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("level"):
		{
			parseDocumentElemArray(parseLevel, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("levelObject"):
		{
			parseDocumentElemArray(parseLevelObject, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("levelObjectClass"):
		{
			parseDocumentElemArray(parseLevelObjectClass, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("panel"):
		{
			parseDocumentElemArray(Parser2::parsePanel, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("player"):
		{
			parseDocumentElemArray(parsePlayer, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("playerClass"):
		{
			parseDocumentElemArray(parsePlayerClass, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("quest"):
		{
			parseDocumentElemArray(parseQuest, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("spell"):
		{
			parseDocumentElemArray(parseSpell, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		case str2int16("texturePack"):
		{
			parseDocumentElemArray(Parser2::parseTexturePack, game, nameHash16, elem, replaceVars, allocator);
			break;
		}
		default:
			return false;
		}
		return true;
	}
}
