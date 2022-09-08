#include "ParseClassifier.h"
#include "Game/Classifier.h"
#include "Game/Game.h"
#include "Game/Level/Level.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	ClassifierValue parseClassifierValue(const Value& elem)
	{
		ClassifierValue classifierVal;
		if (isValidString(elem, "text") == true)
		{
			classifierVal.compare = getStringVal(elem["text"sv]);
		}
		else
		{
			classifierVal.compare = std::make_pair(
				getMinMaxIntKey<LevelObjValue>(elem, "min",
					std::numeric_limits<LevelObjValue>::min()),
				getMinMaxIntKey<LevelObjValue>(elem, "max",
					std::numeric_limits<LevelObjValue>::max())
			);
		}
		classifierVal.value = getVariableKey(elem, "value");
		return classifierVal;
	}

	bool parseClassifierValueInterval(const Value& elem, ClassifierValueInterval& valueInterval)
	{
		if (isValidString(elem, "property") == false)
		{
			if (elem.HasMember("value"sv) == true)
			{
				const auto& elemValues = elem["value"sv];
				if (elemValues.IsObject() == true)
				{
					ClassifierValue classifierVal;
					classifierVal.compare = std::make_pair(
							std::numeric_limits<LevelObjValue>::min(),
							std::numeric_limits<LevelObjValue>::max()
					);
					classifierVal.value = getVariableKey(elemValues, "value");
					valueInterval.values.push_back(classifierVal);
					return true;
				}
			}
			return false;
		}
		if (elem.HasMember("value"sv) == false)
		{
			return false;
		}
		const auto& elemValues = elem["value"sv];
		if (elemValues.IsObject() == true)
		{
			ClassifierValue classifierVal = parseClassifierValue(elemValues);
			valueInterval.values.push_back(classifierVal);
		}
		else if (elemValues.IsArray() == true)
		{
			for (const auto& val : elemValues)
			{
				ClassifierValue classifierVal = parseClassifierValue(val);
				valueInterval.values.push_back(classifierVal);
			}
		}
		if (valueInterval.values.empty() == true)
		{
			return false;
		}
		valueInterval.property = elem["property"sv].GetStringView();
		return true;
	}

	std::vector<ClassifierValueInterval> parseClassifierValueIntervals(const Value& elem)
	{
		std::vector<ClassifierValueInterval> valueIntervals;
		if (elem.IsObject() == true)
		{
			ClassifierValueInterval value;
			if (parseClassifierValueInterval(elem, value) == true)
			{
				valueIntervals.push_back(value);
			}
		}
		else if (elem.IsArray() == true)
		{
			for (const auto& val : elem)
			{
				ClassifierValueInterval value;
				if (parseClassifierValueInterval(val, value) == true)
				{
					valueIntervals.push_back(value);
				}
			}
		}
		else if (elem.IsString() == true)
		{
			ClassifierValueInterval valueInterval;
			ClassifierValue classifierVal;
			classifierVal.compare = std::make_pair(
				std::numeric_limits<LevelObjValue>::min(),
				std::numeric_limits<LevelObjValue>::max()
			);
			classifierVal.value = elem.GetStringStr();
			valueInterval.values.push_back(classifierVal);
			valueIntervals.push_back(valueInterval);
		}
		return valueIntervals;
	}

	void parseClassifier(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false)
		{
			return;
		}
		auto id = elem["id"sv].GetStringView();
		if (isValidId(id) == false)
		{
			return;
		}
		auto level = game.Resources().getLevel<Level>(getStringViewKey(elem, "level"));
		if (level == nullptr)
		{
			return;
		}
		if (elem.HasMember("values"sv) == false)
		{
			return;
		}
		auto classifier = parseClassifierValueIntervals(elem["values"sv]);
		if (classifier.empty() == true)
		{
			return;
		}
		level->LevelObjects().addClassifier(id, std::make_unique<Classifier>(std::move(classifier)));
	}
}
