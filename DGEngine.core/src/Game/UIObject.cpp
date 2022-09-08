#include "UIObject.h"
#include "Game/Utils/UIObjectUtils.h"
#include "Utils/StringHash.h"

bool UIObject::getUIObjProp(const uint16_t propHash, const std::string_view prop, Variable& var) const
{
	switch (propHash)
	{
	case str2int16("position"):
	{
		var = UIObjectUtils::getTuple2iProp(Position(), prop);
	}
	break;
	case str2int16("size"):
	{
		var = UIObjectUtils::getTuple2iProp(Size(), prop);
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
