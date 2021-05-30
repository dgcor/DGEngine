#include "ParseFile2.h"
#include "Json/JsonUtils.h"
#include "Parser/ParseClassifier.h"
#include "Parser/ParseFile.h"
#include "Parser/ParseItem.h"
#include "Parser/ParseItemClass.h"
#include "Parser/ParseLevel.h"
#include "Parser/ParseLevelObject.h"
#include "Parser/ParseLevelObjectClass.h"
#include "Parser/ParsePanel2.h"
#include "Parser/ParsePlayer.h"
#include "Parser/ParsePlayerClass.h"
#include "Parser/ParseQuest.h"
#include "Parser/ParseSpell.h"
#include "Parser/ParseTexturePack2.h"
#include "Parser/Utils/ParseUtils.h"
#include "Utils/Utils.h"

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
			if (elem.IsArray() == false)
			{
				parseClassifier(game, elem);
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
		case str2int16("item"):
		{
			if (elem.IsArray() == false)
			{
				parseItem(game, elem);
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
		case str2int16("itemClass"):
		{
			if (elem.IsArray() == false)
			{
				parseItemClass(game, elem);
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
		case str2int16("level"):
		{
			if (elem.IsArray() == false)
			{
				parseLevel(game, elem);
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
		case str2int16("levelObject"):
		{
			if (elem.IsArray() == false)
			{
				parseLevelObject(game, elem);
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
		case str2int16("levelObjectClass"):
		{
			if (elem.IsArray() == false)
			{
				parseLevelObjectClass(game, elem);
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
				Parser2::parsePanel(game, elem);
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
		case str2int16("player"):
		{
			if (elem.IsArray() == false)
			{
				parsePlayer(game, elem);
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
		case str2int16("playerClass"):
		{
			if (elem.IsArray() == false)
			{
				parsePlayerClass(game, elem);
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
		case str2int16("quest"):
		{
			if (elem.IsArray() == false)
			{
				parseQuest(game, elem);
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
		case str2int16("spell"):
		{
			if (elem.IsArray() == false)
			{
				parseSpell(game, elem);
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
				Parser2::parseTexturePack(game, elem);
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
		default:
			return false;
		}
		return true;
	}
}
