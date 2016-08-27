#include "ParseAudio.h"
#include "ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseAudio(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false || isValidString(elem, "file") == false)
		{
			return;
		}

		auto music = std::make_shared<Music2>(elem["file"].GetString());
		if (music->load() == false)
		{
			return;
		}

		music->setLoop(getBoolKey(elem, "loop"));

		auto volume = getVariableKey(elem, "volume");
		auto vol = game.getVarOrProp<int64_t, unsigned>(volume, game.MusicVolume());
		if (vol > 100)
		{
			vol = 100;
		}
		music->setVolume((float)vol);

		if (getBoolKey(elem, "play") == true)
		{
			music->play();
		}

		game.Resources().addSong(elem["id"].GetString(), music);
	}
}
