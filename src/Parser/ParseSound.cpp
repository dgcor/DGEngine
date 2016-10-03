#include "ParseSound.h"
#include "ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	std::shared_ptr<sf::SoundBuffer> parseSoundObj(Game& game,
		const std::string& id, const std::string& file)
	{
		sf::PhysFSStream stream(file);
		if (stream.hasError() == true)
		{
			return nullptr;
		}

		auto sound = std::make_shared<sf::SoundBuffer>();
		if (sound->loadFromStream(stream) == false)
		{
			return nullptr;
		}

		game.Resources().addSound(id, sound);
		return sound;
	}

	void parseSound(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false || isValidString(elem, "file") == false)
		{
			return;
		}

		parseSoundObj(game, elem["id"].GetString(), elem["file"].GetString());
	}
}
