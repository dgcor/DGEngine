#pragma once

#include <string>

class Level;
class LevelObject;

namespace Save
{
	void save(const std::string& filePath, const Level& level,
		bool skipDefaults, bool skipCurrentPlayer);
	void save(const std::string& filePath, const Level& level,
		const LevelObject& obj, bool skipDefaults);
	void serialize(void* serializeObj, const Level& level,
		bool skipDefaults, bool skipCurrentPlayer);
}
