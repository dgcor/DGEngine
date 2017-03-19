#include "Quest.h"
#include "Utils.h"

bool Quest::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.empty() == true)
	{
		return false;
	}
	switch (str2int32(prop.c_str()))
	{
	case str2int32("id"):
		var = Variable(id);
		break;
	case str2int32("name"):
		var = Variable(name);
		break;
	case str2int32("state"):
		var = Variable((int64_t)state);
		break;
	default:
		return false;
	}
	return true;
}
