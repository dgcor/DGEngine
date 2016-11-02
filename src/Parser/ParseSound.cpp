#include "ParseSound.h"
#include "Utils/ParseUtils.h"

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

	bool parseSoundFromId(Game& game, const Value& elem)
	{
		if (isValidString(elem, "fromId") == true)
		{
			if (isValidString(elem, "id") == true)
			{
				std::string fromId(elem["fromId"].GetString());
				std::string id(elem["id"].GetString());
				if (fromId != id && isValidId(id) == true)
				{
					auto obj = game.Resources().getSound(fromId);
					if (obj != nullptr)
					{
						game.Resources().addSound(id, obj);
					}
				}
			}
			return true;
		}
		return false;
	}

	void parseSound(Game& game, const Value& elem)
	{
		if (parseSoundFromId(game, elem) == true)
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

		parseSoundObj(game, id, file);
	}
}
