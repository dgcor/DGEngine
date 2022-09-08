#pragma once

#include "Game/ResourceManager.h"
#include "Game/Utils/VarUtils.h"
#include "Json/JsonParser.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	template <class T>
	bool parseResourceFromId(ResourceManager& resources, const rapidjson::Value& elem)
	{
		using namespace std::literals;

		if (isValidString(elem, "fromId") == true)
		{
			if (isValidString(elem, "id") == true)
			{
				auto fromId = elem["fromId"sv].GetStringView();
				auto id = elem["id"sv].GetStringView();
				if (fromId != id && isValidId(id) == true)
				{
					auto obj = resources.getResource<T>(fromId);

					if constexpr (VarUtils::is_variant<T>::value == true)
					{
						if (std::holds_alternative<std::nullptr_t>(obj) == false)
						{
							resources.addResource<T>(id, obj, getStringViewKey(elem, "resource"));
						}
					}
					else
					{
						if (obj != nullptr)
						{
							resources.addResource<T>(id, obj, getStringViewKey(elem, "resource"));
						}
					}
				}
			}
			return true;
		}
		return false;
	}
}
