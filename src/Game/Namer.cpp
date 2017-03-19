#include "Namer.h"

std::string Namer::getName(const Queryable& obj) const
{
	std::string strName;
	bool hasName = false;
	for (const auto& nameVal : nameValues)
	{
		bool hasValue = false;
		int32_t value = 0;
		if (nameVal.property.empty() == true)
		{
			hasValue = true;
		}
		else
		{
			Variable var;
			if (obj.getProperty(nameVal.property, var) == true)
			{
				if (var.is<int64_t>() == true)
				{
					value = (int32_t)var.get<int64_t>();
					hasValue = true;
				}
				else if (var.is<bool>() == true)
				{
					value = (int32_t)var.get<bool>();
					hasValue = true;
				}
			}
		}
		if (hasValue == true)
		{
			for (const auto& itemElem : nameVal.values)
			{
				if (value >= itemElem.min && value <= itemElem.max)
				{
					strName = itemElem.text;
					hasName = true;
					break;
				}
			}
			if (hasName == true)
			{
				break;
			}
		}
	}
	return strName;
}
