#include "ParseAudio.h"
#include "ParseAudioCommon.h"
#include "MusicLoops.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

#if (SFML_VERSION_MAJOR > 2 || (SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR >= 5))
	bool openMusicFromBuffer(Game& game, std::shared_ptr<sf::Music2>& music,
		sf::SoundBuffer& buffer, const std::string& id)
	{
		if (music->openFromSoundBuffer(buffer) == true)
		{
			if (game.Resources().addSong(id, music) == true)
			{
				return true;
			}
		}
		return false;
	}
#endif

	bool openMusicFromFile(Game& game, std::shared_ptr<sf::Music2>& music,
		sf::PhysFSStream& stream, const AudioSource audioSource, const std::string& id)
	{
		if (music->openFromStream(stream) == true &&
			game.Resources().hasSong(id, true) == false)
		{
			if (game.Resources().addAudioSource(id, audioSource) == true)
			{
				game.Resources().addSong(id, music);
				return true;
			}
		}
		return false;
	}

	sf::Music2* parseAudioFileObjHelper(Game& game, const Value& elem,
		const std::string& id, const std::string& file)
	{
		bool hasLoopNames = elem.HasMember("loopNames");

		if (hasLoopNames == false &&
			elem.HasMember("loopPoints") == false)
		{
			auto stream = std::make_shared<sf::PhysFSStream>(file);
			if (stream->hasError() == true)
			{
				return nullptr;
			}

			auto music = std::make_shared<sf::Music2>();

			if (openMusicFromFile(game, music, *stream, stream, id) == true)
			{
				return music.get();
			}
			return nullptr;
		}
#if (SFML_VERSION_MAJOR > 2 || (SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR >= 5))
		auto sndFile = std::make_shared<SoundFileLoops>(file);
		if (sndFile->file.hasError() == true)
		{
			return nullptr;
		}

		auto music = std::make_shared<sf::MusicLoops>();
		auto music2 = std::dynamic_pointer_cast<sf::Music2>(music);

		if (openMusicFromFile(game, music2,
			sndFile->file, sndFile, id) == true)
		{
			if (hasLoopNames == true)
			{
				parseAudioLoopNamesVal(elem, "loopNames", sndFile->loops);
			}
			if (isValidString(elem, "loopPoints") == true)
			{
				parseAudioLoopPointsVal(
					elem,
					"loopPoints",
					*music);
			}
			else if (isValidString(elem, "playText") == true)
			{
				updateAudioLoopString(
					elem["playText"].GetString(),
					sndFile->loops,
					*music);
			}
			return music.get();
		}
#endif
		return nullptr;
	}

	sf::Music2* parseAudioObj(Game& game, const Value& elem)
	{
		std::string id;

		if (isValidString(elem, "id") == true)
		{
			id = elem["id"].GetString();
		}
		bool validId = isValidId(id);

#if (SFML_VERSION_MAJOR > 2 || (SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR >= 5))
		if (isValidString(elem, "sourceId") == true &&
			validId == true)
		{
			auto source = game.Resources().getAudioSource(elem["sourceId"].GetString());

			if (std::holds_alternative<std::shared_ptr<sf::SoundBuffer>>(source) == true)
			{
				auto sndBuffer = std::get<std::shared_ptr<sf::SoundBuffer>>(source).get();

				if (sndBuffer == nullptr)
				{
					return nullptr;
				}

				std::shared_ptr<sf::Music2> music;

				if (isValidString(elem, "loopPoints") == true)
				{
					music = std::make_shared<sf::MusicLoops>();

					parseAudioLoopPointsVal(
						elem,
						"loopPoints",
						(*(sf::MusicLoops*)music.get()));
				}
				else
				{
					music = std::make_shared<sf::Music2>();
				}
				if (openMusicFromBuffer(game, music, *sndBuffer, id) == true)
				{
					return music.get();
				}
			}
			else if (std::holds_alternative<std::shared_ptr<SoundBufferLoops>>(source) == true)
			{
				auto sndBuffer = std::get<std::shared_ptr<SoundBufferLoops>>(source).get();

				if (sndBuffer == nullptr)
				{
					return nullptr;
				}

				auto music = std::make_shared<sf::MusicLoops>();
				auto music2 = std::dynamic_pointer_cast<sf::Music2>(music);

				if (openMusicFromBuffer(game, music2,
					sndBuffer->soundBuffer, id) == true)
				{
					if (isValidString(elem, "loopPoints") == true)
					{
						parseAudioLoopPointsVal(
							elem,
							"loopPoints",
							*music);
					}
					else if (isValidString(elem, "playText") == true)
					{
						updateAudioLoopString(
							elem["playText"].GetString(),
							sndBuffer->loops,
							*music);
					}
					return music.get();
				}
			}
		}
		else if (isValidString(elem, "file") == true)
#else
		if (isValidString(elem, "file") == true)
#endif
		{
			std::string file(elem["file"].GetString());

			if (validId == false)
			{
				if (getIdFromFile(file, id) == false)
				{
					return nullptr;
				}
				if (isValidId(id) == false)
				{
					return nullptr;
				}
			}
			return parseAudioFileObjHelper(game, elem, id, file);
		}
		return nullptr;
	}

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

		auto music = parseAudioObj(game, elem);
		if (music == nullptr)
		{
			return;
		}

		music->setLoop(getBoolKey(elem, "loop"));

#if (SFML_VERSION_MAJOR > 2 || (SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR >= 5))
		auto musicLoop = dynamic_cast<sf::MusicLoops*>(music);
		if (musicLoop != nullptr)
		{
			musicLoop->setLoop(music->getLoop());
			music->setLoop(true);
		}
#endif
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
	}
}
