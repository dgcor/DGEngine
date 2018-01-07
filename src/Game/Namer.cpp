#include "Namer.h"

std::string Namer::getName(const Queryable& obj, uint16_t skipFirst) const
{
	std::string strName;
	bool hasName = false;
	for (const auto& nameVal : nameValues)
	{
		bool hasValue = false;
		LevelObjValue value = 0;
		if (nameVal.property.empty() == true)
		{
			hasValue = true;
		}
		else
		{
			Variable var;
			if (obj.getProperty(nameVal.property, var) == true)
			{
				if (std::holds_alternative<int64_t>(var) == true)
				{
					value = (LevelObjValue)std::get<int64_t>(var);
					hasValue = true;
				}
				else if (std::holds_alternative<bool>(var) == true)
				{
					value = (LevelObjValue)std::get<bool>(var);
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
					if (skipFirst > 0)
					{
						skipFirst--;
					}
					else
					{
						strName = itemElem.text;
						hasName = true;
					}
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
