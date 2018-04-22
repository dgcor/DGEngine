#pragma once

class Level;
class Item;

namespace Save
{
	void serialize(void* serializeObj, const Level& level,
		const Item& item, bool skipDefaults);
}
