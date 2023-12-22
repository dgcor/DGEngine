#pragma once

#include "Game/LevelObject/LevelObjectClassDefaults.h"
#include "Game/Level/Level.h"
#include "Json/JsonParser.h"
#include "Parser/Utils/ParseUtils.h"
#include <type_traits>

namespace Parser
{
	void parseLevelObjectClassActions(LevelObjectClass& levelObjClass, Game& game, const rapidjson::Value& elem);

	template <class T>
	void parseLevelObjectClassDefaults(LevelObjectClassDefaults<T>& levelObjClass, Level& level, const rapidjson::Value& elem)
	{
		using namespace std::literals;

		if (elem.HasMember("defaults"sv) == false)
		{
			return;
		}
		const auto& defaults = elem["defaults"sv];
		if (defaults.IsObject() == false)
		{
			return;
		}
		for (auto it = defaults.MemberBegin(); it != defaults.MemberEnd(); ++it)
		{
			if (it->name.GetStringLength() > 0)
			{
				auto name = it->name.GetStringView();
				auto nameHash = str2int16(name);
				level.setPropertyName(nameHash, name);

				if constexpr (std::is_integral_v<T> == true)
				{
					levelObjClass.setDefaultByHash(nameHash, getMinMaxIntVal<T>(it->value));
				}
				else
				{
					levelObjClass.setDefaultByHash(nameHash, getMinMaxNumber32Val(it->value));
				}
			}
		}
	}

	template <class T>
	void parseLevelObjectClassDescription(T& levelObjClass, const Level& level, const rapidjson::Value& elem)
	{
		levelObjClass.setDescription(
			getUIntKey(elem, "index"),
			level.LevelObjects().getClassifier(getStringViewKey(elem, "name")),
			(uint16_t)getUIntKey(elem, "skip"));
	}

	template <class T>
	void parseLevelObjectClassDescriptions(T& levelObjClass, const Level& level, const rapidjson::Value& elem)
	{
		using namespace std::literals;

		if (elem.HasMember("descriptions"sv) == false)
		{
			return;
		}
		const auto& descriptions = elem["descriptions"sv];
		if (descriptions.IsObject() == true)
		{
			parseLevelObjectClassDescription(levelObjClass, level, descriptions);
		}
		else if (descriptions.IsArray() == true)
		{
			for (const auto& val : descriptions)
			{
				parseLevelObjectClassDescription(levelObjClass, level, val);
			}
		}
	}

	template <class T>
	void parseLevelObjectClassFormulas(T& levelObjClass, const rapidjson::Value& elem)
	{
		using namespace std::literals;

		if (elem.HasMember("formulas"sv) == false)
		{
			return;
		}
		const auto& formulas = elem["formulas"sv];
		if (formulas.IsObject() == false)
		{
			return;
		}
		for (auto it = formulas.MemberBegin(); it != formulas.MemberEnd(); ++it)
		{
			auto nameHash = str2int16(it->name.GetStringView());
			if (nameHash != str2int16(""))
			{
				if (it->value.IsNull() == true)
				{
					levelObjClass.deleteFormula(nameHash);
				}
				else
				{
					levelObjClass.setFormula(nameHash, getStringVal(it->value));
				}
			}
		}
	}
}
