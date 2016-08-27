#include "Quest.h"
#include "Utils.h"

using Utils::str2int;

std::string Quest::getProperty(const std::string& prop) const
{
	if (prop.size() > 1)
	{
		switch (str2int(prop.c_str()))
		{
		case str2int("id"):
			return id;
		case str2int("name"):
			return name;
		case str2int("state"):
			return std::to_string(state);
		default:
			break;
		}
	}
	return "";
}
