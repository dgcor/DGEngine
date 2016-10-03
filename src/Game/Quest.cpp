#include "Quest.h"
#include "Utils.h"

using Utils::str2int;

bool Quest::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.empty() == true)
	{
		return false;
	}
	switch (str2int(prop.c_str()))
	{
	case str2int("id"):
		var = Variable(id);
		break;
	case str2int("name"):
		var = Variable(name);
		break;
	case str2int("state"):
		var = Variable((int64_t)state);
		break;
	default:
		return false;
	}
	return true;
}
