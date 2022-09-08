#include "Quest.h"
#include "Utils/StringHash.h"

bool Quest::getProperty(const std::string_view prop, Variable& var) const noexcept
{
	if (prop.empty() == true)
	{
		return false;
	}
	switch (str2int16(prop))
	{
	case str2int16("id"):
		var = Variable(id);
		break;
	case str2int16("name"):
		var = Variable(name);
		break;
	case str2int16("state"):
		var = Variable((int64_t)state);
		break;
	default:
		return false;
	}
	return true;
}
