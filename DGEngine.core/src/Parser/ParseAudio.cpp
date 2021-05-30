#include "ParseAudio.h"
#include <cassert>
#include "Game.h"
#include "ParseAudioCommon.h"
#include "SFML/MusicLoops.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	bool openMusicFromBuffer(Game& game, std::shared_ptr<sf::Music2>& music,
		sf::SoundBuffer& buffer, const std::string& id, const std::string_view resource)
	{
		if (music->openFromSoundBuffer(buffer) == true)
		{
			if (game.Resources().addSong(id, music, resource) == true)
			{
				return true;
			}
		}
		return false;
	}

	bool openMusicFromFile(Game& game, std::shared_ptr<sf::Music2> music,
		sf::InputStream& stream, const AudioSource audioSource,
		const std::string& id, const std::string_view resource)
	{
		if (music->openFromStream(stream) == true &&
			game.Resources().hasSong(id, true) == false)
		{
			if (game.Resources().addAudioSource(id, audioSource, resource) == true)
			{
				game.Resources().addSong(id, music, resource);
				return true;
			}
		}
		return false;
	}

	sf::Music2* parseAudioObjFromFile(Game& game, const Value& elem,
		const std::string& id, const std::string& file)
	{
		bool hasLoopNames = elem.HasMember("loopNames"sv);

		if (hasLoopNames == false &&
			elem.HasMember("loopPoints"sv) == false)
		{
			auto stream = std::make_shared<sf::PhysFSStream>(file);
			if (stream->hasError() == true)
			{
				return nullptr;
			}

			auto music = std::make_shared<sf::Music2>();
			auto resource = getStringViewKey(elem, "resource");

			if (openMusicFromFile(game, music, *stream, stream, id, resource) == true)
			{
				return music.get();
			}
			return nullptr;
		}
		auto sndFile = std::make_shared<SoundFileLoops>(file);
		if (sndFile->file.hasError() == true)
		{
			return nullptr;
		}

		auto music = std::make_shared<sf::MusicLoops>();
		auto music2 = std::dynamic_pointer_cast<sf::Music2>(music);
		auto resource = getStringViewKey(elem, "resource");

		if (openMusicFromFile(game, music2,
			sndFile->file, sndFile, id, resource) == true)
		{
			if (hasLoopNames == true)
			{
				parseAudioLoopNamesVal(elem, "loopNames", sndFile->loops);
			}
			if (elem.HasMember("loopPoints"sv) == true)
			{
				parseAudioLoopPointsVal(
					elem,
					"loopPoints",
					*music);
			}
			else if (isValidString(elem, "playText") == true)
			{
				updateAudioLoopString(
					getStringViewVal(elem["playText"sv]),
					sndFile->loops,
					*music);
			}
			return music.get();
		}
		return nullptr;
	}

	sf::Music2* parseAudioObjFromSource(Game& game,
		const Value& elem, const std::string& id)
	{
		auto source = game.Resources().getAudioSource(elem["sourceId"sv].GetStringView());

		if (std::holds_alternative<std::shared_ptr<sf::SoundBuffer>>(source) == true)
		{
			auto sndBuffer = std::get<std::shared_ptr<sf::SoundBuffer>>(source).get();

			if (sndBuffer == nullptr)
			{
				return nullptr;
			}

			std::shared_ptr<sf::Music2> music;

			if (elem.HasMember("loopPoints"sv) == true)
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
			auto resource = getStringViewKey(elem, "resource");
			if (openMusicFromBuffer(game, music, *sndBuffer, id, resource) == true)
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
			auto resource = getStringViewKey(elem, "resource");

			if (openMusicFromBuffer(game, music2,
				sndBuffer->soundBuffer, id, resource) == true)
			{
				if (elem.HasMember("loopPoints"sv) == true)
				{
					parseAudioLoopPointsVal(
						elem,
						"loopPoints",
						*music);
				}
				else if (isValidString(elem, "playText") == true)
				{
					updateAudioLoopString(
						getStringViewVal(elem["playText"sv]),
						sndBuffer->loops,
						*music);
				}
				return music.get();
			}
		}
		return nullptr;
	}

	sf::Music2* parseAudioObj(Game& game, const Value& elem)
	{
		std::string id;

		if (isValidString(elem, "id") == true)
		{
			id = elem["id"sv].GetStringView();
		}
		bool validId = isValidId(id);

		if (isValidString(elem, "sourceId") == true &&
			validId == true)
		{
			return parseAudioObjFromSource(game, elem, id);
		}
		else if (isValidString(elem, "file") == true)
		{
			std::string file(elem["file"sv].GetStringView());

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
			return parseAudioObjFromFile(game, elem, id, file);
		}
		return nullptr;
	}

	bool parseAudioFromId(Game& game, const Value& elem)
	{
		if (isValidString(elem, "fromId") == true)
		{
			if (isValidString(elem, "id") == true)
			{
				auto fromId = elem["fromId"sv].GetStringView();
				auto id = elem["id"sv].GetStringView();
				if (fromId != id && isValidId(id) == true)
				{
					auto obj = game.Resources().getSong(fromId);
					if (obj != nullptr)
					{
						game.Resources().addSong(id, obj, getStringViewKey(elem, "resource"));
					}
				}
			}
			return true;
		}
		return false;
	}

	void parseAudio(Game& game, const Value& elem,
		const parseAudioObjFuncPtr parseAudioObjFunc)
	{
		assert(parseAudioObjFunc != nullptr);

		if (parseAudioFromId(game, elem) == true)
		{
			return;
		}

		auto music = parseAudioObjFunc(game, elem);
		if (music == nullptr)
		{
			return;
		}

		music->setLoop(getBoolKey(elem, "loop"));

		auto musicLoop = dynamic_cast<sf::MusicLoops*>(music);
		if (musicLoop != nullptr)
		{
			musicLoop->setLoop(music->getLoop());
			music->setLoop(true);
		}
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

	void parseAudio(Game& game, const Value& elem)
	{
		parseAudio(game, elem, parseAudioObj);
	}
}
