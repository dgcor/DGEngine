#include "ParseSpell.h"
#include "Game/Spell.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	std::unique_ptr<Spell> parseSpellObj(Game& game,
		Level& level, const Value& elem)
	{
		if (isValidString(elem, "class") == false)
		{
			return nullptr;
		}

		auto class_ = level.getClass<SpellClass>(elem["class"].GetString());
		if (class_ == nullptr)
		{
			return nullptr;
		}

		auto spell = std::make_unique<Spell>(class_);

		if (elem.HasMember("properties") == true)
		{
			const auto& props = elem["properties"];
			if (props.IsObject() == true)
			{
				for (auto it = props.MemberBegin(); it != props.MemberEnd(); ++it)
				{
					if (it->name.GetStringLength() > 0)
					{
						auto name = getStringViewVal(it->name);
						auto nameHash = str2int16(name);
						level.setPropertyName(nameHash, name);
						spell->setIntByHash(nameHash,
							getMinMaxIntVal<LevelObjValue>(it->value));
					}
				}
			}
		}

		return spell;
	}
}
