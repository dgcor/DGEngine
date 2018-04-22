#pragma once

class Level;
class SimpleLevelObject;

namespace Save
{
	void serialize(void* serializeObj, const Level& level,
		const SimpleLevelObject& obj, bool skipDefaults);
}
