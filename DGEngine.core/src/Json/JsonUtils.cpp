#include "JsonUtils.h"
#include "Game.h"
#include "Utils/Utils.h"

namespace JsonUtils
{
	using namespace rapidjson;

	const Value& query(const Value& elem, const Value& query)
	{
		if (query.IsString() == true)
		{
			auto elemPtr = &elem;
			auto queryStr = query.GetStringView();
			while (true)
			{
				auto str1 = Utils::splitStringIn2(queryStr, '{');
				if (str1.second.empty() == false)
				{
					// get first part to see if it's array or object
					auto querySize = str1.first.size();
					if (querySize > 1 && str1.first[querySize - 1] == '/')
					{
						querySize--;
					}
					const Value* obj = nullptr;
					Pointer pointer(str1.first.data(), querySize);
					if (pointer.IsValid() == true)
					{
						obj = pointer.Get(*elemPtr);
					}
					if (obj == nullptr || obj->IsArray() == false)
					{
						break;
					}

					// get the key/val to try and match
					auto str2 = Utils::splitStringIn2(str1.second, '}');
					auto strKeyVal = Utils::splitStringIn2(str2.first, '=');

					if (strKeyVal.first.empty() == true || strKeyVal.second.empty() == true)
					{
						break;
					}

					bool foundMatch = false;
					std::string key(strKeyVal.first);
					for (const auto& elem : *obj)
					{
						if (elem.HasMember(key) == false ||
							elem[key].IsArray() == true ||
							elem[key].IsObject() == true)
						{
							continue;
						}
						if ((elem[key].IsString() == true &&
							elem[key].GetString() == strKeyVal.second) ||
							toString(elem[key]) == strKeyVal.second)
						{
							queryStr = str2.second;
							elemPtr = &elem;
							foundMatch = true;
							break;
						}
					}
					if (foundMatch == true)
					{
						continue;
					}
					break;
				}

				Pointer pointer(str1.first.data(), str1.first.size());
				if (pointer.IsValid() == false)
				{
					break;
				}
				auto obj2 = pointer.Get(*elemPtr);
				if (obj2 != nullptr)
				{
					return *obj2;
				}
				break;
			}
		}
		return query;
	}

	void replaceValueWithVariable(Value& elem,
		Value::AllocatorType& allocator, const Variable& var)
	{
		if (std::holds_alternative<std::string>(var) == true)
		{
			const auto& str = std::get<std::string>(var);
			elem.SetString(str, allocator);
		}
		else if (std::holds_alternative<int64_t>(var) == true)
		{
			elem.SetInt64(std::get<int64_t>(var));
		}
		else if (std::holds_alternative<double>(var) == true)
		{
			elem.SetDouble(std::get<double>(var));
		}
		else if (std::holds_alternative<bool>(var) == true)
		{
			elem.SetBool(std::get<bool>(var));
		}
	}

	void replaceStringWithVariable(Value& elem,
		Value::AllocatorType& allocator, const std::string_view str,
		const Variable& var, bool changeValueType)
	{
		if (elem.IsString() == false ||
			elem.GetStringLength() == 0)
		{
			return;
		}
		std::string str1(elem.GetStringView());

		if (changeValueType == true &&
			str1 == str)
		{
			replaceValueWithVariable(elem, allocator, var);
			return;
		}
		Utils::replaceStringInPlace(str1, str, VarUtils::toString(var));

		if (elem.GetString() != str1)
		{
			elem.SetString(str1, allocator);
		}
	}

	void replaceStringWithQueryable(Value& elem,
		Value::AllocatorType& allocator,
		const Queryable& obj, bool changeValueType)
	{
		if (elem.IsString() == false ||
			elem.GetStringLength() <= 2)
		{
			return;
		}

		auto str = elem.GetStringView();
		std::string str2;
		size_t firstTokenStart = 0;
		bool hadMatch = false;
		while (true)
		{
			firstTokenStart = str.find('%', firstTokenStart);
			if (firstTokenStart == std::string_view::npos)
			{
				break;
			}
			size_t firstTokenStop = firstTokenStart + 1;
			size_t secondTokenStart = str.find_first_of('%', firstTokenStop);
			if (secondTokenStart == std::string_view::npos)
			{
				break;
			}
			size_t secondTokenStop = secondTokenStart + 1;
			if (hadMatch == false)
			{
				hadMatch = true;
				str2 = str;
			}

			std::string_view strProp(str.data() + firstTokenStart, secondTokenStop - firstTokenStart);
			Variable var;
			if (obj.getProperty(strProp.substr(1, strProp.size() - 2), var) == true)
			{
				if (changeValueType == true &&
					strProp == str2)
				{
					replaceValueWithVariable(elem, allocator, var);
					return;
				}
				Utils::replaceStringInPlace(str2, strProp, VarUtils::toString(var));
			}
			firstTokenStart = secondTokenStop;
		}
		if (hadMatch == true &&
			str != str2)
		{
			elem.SetString(str2, allocator);
		}
	}

	void replaceValueWithString(Value& elem, Value::AllocatorType& allocator,
		const std::string_view oldStr, const std::string_view newStr)
	{
		if (elem.IsString() == true)
		{
			auto str1 = elem.GetStringView();
			std::string str2(str1);
			Utils::replaceStringInPlace(str2, oldStr, newStr);
			if (str1 != str2)
			{
				elem.SetString(str2, allocator);
			}
		}
		else if (elem.IsObject() == true)
		{
			for (auto it = elem.MemberBegin(); it != elem.MemberEnd(); ++it)
			{
				replaceValueWithString(it->value, allocator, oldStr, newStr);
			}
		}
		else if (elem.IsArray() == true)
		{
			for (auto it = elem.Begin(); it != elem.End(); ++it)
			{
				replaceValueWithString(*it, allocator, oldStr, newStr);
			}
		}
	}

	void replaceValueWithQueryable(Value& elem,
		Value::AllocatorType& allocator, const Queryable& obj,
		const std::function<void(Value&)> valFunc)
	{
		if (elem.IsString() == true)
		{
			replaceStringWithQueryable(elem, allocator, obj, true);
			if (valFunc)
			{
				valFunc(elem);
			}
		}
		else if (elem.IsObject() == true)
		{
			for (auto it = elem.MemberBegin(); it != elem.MemberEnd(); ++it)
			{
				replaceValueWithQueryable(it->value, allocator, obj, valFunc);
			}
		}
		else if (elem.IsArray() == true)
		{
			for (auto it = elem.Begin(); it != elem.End(); ++it)
			{
				replaceValueWithQueryable(*it, allocator, obj, valFunc);
			}
		}
	}

	void replaceValueWithGameVar(Value& elem,
		Value::AllocatorType& allocator,
		const Game& game, bool changeValueType, char token)
	{
		if (elem.IsString() == true)
		{
			if (elem.IsString() == false ||
				elem.GetStringLength() <= 2)
			{
				return;
			}

			auto str = elem.GetStringView();
			std::string str2;
			size_t firstTokenStart = 0;
			bool hadMatch = false;
			while (true)
			{
				firstTokenStart = str.find(token, firstTokenStart);
				if (firstTokenStart == std::string_view::npos)
				{
					break;
				}
				size_t firstTokenStop = firstTokenStart + 1;
				size_t secondTokenStart = str.find_first_of(token, firstTokenStop);
				if (secondTokenStart == std::string_view::npos)
				{
					break;
				}
				size_t secondTokenStop = secondTokenStart + 1;
				if (hadMatch == false)
				{
					hadMatch = true;
					str2 = str;
				}
				std::string_view strProp(str.data() + firstTokenStop, secondTokenStart - firstTokenStop);
				Variable var;
				if (game.getVarOrPropNoToken(strProp, var) == true)
				{
					std::string_view strProp2(str.data() + firstTokenStart, secondTokenStop - firstTokenStart);
					if (changeValueType == true &&
						strProp2 == str2)
					{
						replaceValueWithVariable(elem, allocator, var);
						return;
					}
					Utils::replaceStringInPlace(str2, strProp2, VarUtils::toString(var));
				}
				firstTokenStart = secondTokenStop;
			}
			if (hadMatch == true &&
				str != str2)
			{
				elem.SetString(str2, allocator);
			}
		}
		else if (elem.IsObject() == true)
		{
			for (auto it = elem.MemberBegin(); it != elem.MemberEnd(); ++it)
			{
				replaceValueWithGameVar(it->value, allocator, game, changeValueType, token);
			}
		}
		else if (elem.IsArray() == true)
		{
			for (auto it = elem.Begin(); it != elem.End(); ++it)
			{
				replaceValueWithGameVar(*it, allocator, game, changeValueType, token);
			}
		}
	}

	std::string toString(const Value& elem)
	{
		if (elem.IsString() == true)
		{
			return { elem.GetString(), elem.GetStringLength() };
		}
		else if (elem.IsInt64() == true)
		{
			return Utils::toString(elem.GetInt64());
		}
		else if (elem.IsDouble() == true)
		{
			return Utils::toString(elem.GetDouble());
		}
		else if (elem.IsBool() == true)
		{
			if (elem.GetBool() == true)
			{
				return { "true" };
			}
			else
			{
				return { "false" };
			}
		}
		else if (elem.IsNull() == true)
		{
			return { "null" };
		}
		else if (elem.IsUint64() == true)
		{
			return Utils::toString(elem.GetUint64());
		}
		return jsonToString(elem);
	}

	std::string jsonToString(const Value& elem)
	{
		StringBuffer buffer;
		Writer<StringBuffer> writer(buffer);
		elem.Accept(writer);
		return std::string(buffer.GetString(), buffer.GetSize());
	}

	std::string jsonToPrettyString(const Value& elem)
	{
		StringBuffer buffer;
		PrettyWriter<StringBuffer> writer(buffer);
		writer.SetIndent(' ', 2);
		writer.SetFormatOptions(PrettyFormatOptions::kFormatSingleLineArray);
		elem.Accept(writer);
		return std::string(buffer.GetString(), buffer.GetSize());
	}

	bool loadFile(const std::string_view file, Document& doc)
	{
		if (file.empty() == true)
		{
			return false;
		}
		return loadJson(FileUtils::readText(file.data()), doc);
	}

	bool loadJson(const std::string_view json, Document& doc)
	{
		if (json.empty() == true)
		{
			return false;
		}
		// Default template parameter uses UTF8 and MemoryPoolAllocator.
		return (doc.Parse(json.data(), json.size()).HasParseError() == false);
	}

	bool loadJsonAndReplaceValues(const std::string_view json, Document& doc,
		const Game& obj, bool changeValueType, char token)
	{
		if (loadJson(json, doc) == true)
		{
			MemoryPoolAllocator<CrtAllocator> allocator;
			replaceValueWithGameVar(doc, allocator, obj, changeValueType, token);
			return true;
		}
		return false;
	}

	void saveToFile(const std::string_view file, const Value& elem)
	{
		FileUtils::saveText(file, jsonToString(elem));
	}
}
