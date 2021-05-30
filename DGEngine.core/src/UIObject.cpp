#include "UIObject.h"
#include "Utils/Utils.h"

bool UIObject::getUIObjProp(const uint16_t propHash,
	const std::string_view prop, Variable& var) const
{
	switch (propHash)
	{
	case str2int16("position"):
	{
		if (prop == "x")
		{
			var = Variable((int64_t)Position().x);
		}
		else
		{
			var = Variable((int64_t)Position().y);
		}
	}
	break;
	case str2int16("size"):
	{
		if (prop == "x")
		{
			var = Variable((int64_t)Size().x);
		}
		else
		{
			var = Variable((int64_t)Size().y);
		}
	}
	break;
	case str2int16("visible"):
		var = Variable(Visible());
		break;
	default:
		return false;
	}
	return true;
}
