#include "ParseAudio.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	bool parseAudioFromId(Game& game, const Value& elem)
	{
		if (isValidString(elem, "fromId") == true)
		{
			if (isValidString(elem, "id") == true)
			{
				std::string fromId(elem["fromId"].GetString());
				std::string id(elem["id"].GetString());
				if (fromId != id && isValidId(id) == true)
				{
					auto obj = game.Resources().getSong(fromId);
					if (obj != nullptr)
					{
						game.Resources().addSong(id, obj);
					}
				}
			}
			return true;
		}
		return false;
	}

	void parseAudio(Game& game, const Value& elem)
	{
		if (parseAudioFromId(game, elem) == true)
		{
			return;
		}

		if (isValidString(elem, "file") == false)
		{
			return;
		}

		std::string file(elem["file"].GetString());
		std::string id;

		if (isValidString(elem, "id") == true)
		{
			id = elem["id"].GetString();
		}
		else if (getIdFromFile(file, id) == false)
		{
			return;
		}
		if (isValidId(id) == false)
		{
			return;
		}

		auto music = std::make_shared<Music2>(file.c_str());
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

		game.Resources().addSong(id, music);
	}
}
