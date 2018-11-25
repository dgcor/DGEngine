#include "Classifier.h"

Variable Classifier::get(const Queryable& obj, uint16_t skipFirst) const
{
	Variable returnVar;
	bool hasResult = false;
	for (const auto& classVal : classifierValues)
	{
		bool hasValue = false;
		Variable value;
		if (classVal.property.empty() == true)
		{
			hasValue = true;
		}
		else if (obj.getProperty(classVal.property, value) == true)
		{
			hasValue = true;
		}
		if (hasValue == true)
		{
			for (const auto& itemElem : classVal.values)
			{
				bool compareResult = false;

				if (std::holds_alternative<ClassifierValue::ValuePair>(itemElem.compare) == true)
				{
					const auto& minMax = std::get<ClassifierValue::ValuePair>(itemElem.compare);
					LevelObjValue intVal = 0;
					if (std::holds_alternative<int64_t>(value) == true)
					{
						intVal = (LevelObjValue)std::get<int64_t>(value);
					}
					else if (std::holds_alternative<bool>(value) == true)
					{
						intVal = (LevelObjValue)std::get<bool>(value);
					}
					if (intVal >= minMax.first && intVal <= minMax.second)
					{
						compareResult = true;
					}
				}
				else
				{
					const auto& str = std::get<std::string>(value);
					if (std::holds_alternative<std::string>(itemElem.compare) == true &&
						std::get<std::string>(itemElem.compare) == str)
					{
						compareResult = true;
					}
				}
				if (compareResult == true)
				{
					if (skipFirst > 0)
					{
						skipFirst--;
					}
					else
					{
						returnVar = itemElem.value;
						hasResult = true;
					}
					break;
				}
			}
			if (hasResult == true)
			{
				break;
			}
		}
	}
	return returnVar;
}
