#include "ParseSound.h"
#include "ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseSound(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false || isValidString(elem, "file") == false)
		{
			return;
		}

		sf::PhysFSStream stream(elem["file"].GetString());
		if (stream.hasError() == true)
		{
			return;
		}

		auto sound = std::make_shared<sf::SoundBuffer>();
		if (sound->loadFromStream(stream) == false)
		{
			return;
		}

		game.Resources().addSound(elem["id"].GetString(), sound);
	}
}
