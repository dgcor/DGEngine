#include "Sol.h"

bool Sol::passable(size_t index) const
{
	if (index >= data.size())
	{
		return 0;
	}
	else
	{
		return !(data[index] & 0x01);
	}
}
