#pragma once

class Level;
class Player;

namespace Save
{
	void serialize(void* serializeObj, const Level& level,
		const Player& player, bool skipDefaults);
}
