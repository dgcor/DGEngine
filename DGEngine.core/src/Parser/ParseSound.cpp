#include "ParseSound.h"
#include <cassert>
#include "Game.h"
#include "ParseAudioCommon.h"
#include "SFML/PhysFSStream.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	sf::SoundBuffer* parseSoundObj(Game& game, const std::string& id,
		const std::string& file, const std::string_view resource)
	{
		sf::PhysFSStream stream(file);
		if (stream.hasError() == true)
		{
			return nullptr;
		}

		auto sndBuffer = std::make_shared<sf::SoundBuffer>();

		if (sndBuffer->loadFromStream(stream) == false)
		{
			return nullptr;
		}
		if (game.Resources().addAudioSource(id, sndBuffer, resource) == true)
		{
			return sndBuffer.get();
		}
		return nullptr;
	}

	sf::SoundBuffer* parseSoundLoopsObj(Game& game, const Value& elem,
		const std::string& id, const std::string& file)
	{
		sf::PhysFSStream stream(file);
		if (stream.hasError() == true)
		{
			return nullptr;
		}

		auto sndBuffer = std::make_shared<SoundBufferLoops>();

		if (sndBuffer->soundBuffer.loadFromStream(stream) == false)
		{
			return nullptr;
		}
		auto resource = getStringViewKey(elem, "resource");
		if (game.Resources().addAudioSource(id, sndBuffer, resource) == true)
		{
			parseAudioLoopNamesVal(elem, "loopNames", sndBuffer->loops);

			return &sndBuffer->soundBuffer;
		}
		return nullptr;
	}

	sf::SoundBuffer* parseSingleSoundObj(Game& game, const Value& elem)
	{
		auto file = elem["file"sv].GetStringStr();
		std::string id;

		if (isValidString(elem, "id") == true)
		{
			id = elem["id"sv].GetStringView();
		}
		else if (getIdFromFile(file, id) == false)
		{
			return nullptr;
		}
		if (isValidId(id) == false)
		{
			return nullptr;
		}
		if (elem.HasMember("loopNames"sv) == true)
		{
			return parseSoundLoopsObj(game, elem, id, file);
		}
		return parseSoundObj(game, id, file, getStringViewKey(elem, "resource"));
	}

	sf::SoundBuffer* parseMultiSoundObj(Game& game, const std::string& id,
		const std::string_view resource, const sf::Int16* samples,
		sf::Uint64 sampleCount, unsigned channelCount, unsigned sampleRate)
	{
		auto sndBuffer = std::make_shared<sf::SoundBuffer>();

		if (sndBuffer->loadFromSamples(samples,
			sampleCount, channelCount, sampleRate) == false)
		{
			return nullptr;
		}
		if (game.Resources().addAudioSource(id, sndBuffer, resource) == true)
		{
			return sndBuffer.get();
		}
		return nullptr;
	}

	sf::SoundBuffer* parseMultiSoundLoopsObj(Game& game, const Value& elem,
		const std::string& id, const sf::Int16* samples,
		sf::Uint64 sampleCount, unsigned channelCount, unsigned sampleRate)
	{
		auto sndBuffer = std::make_shared<SoundBufferLoops>();

		if (sndBuffer->soundBuffer.loadFromSamples(samples,
			sampleCount, channelCount, sampleRate) == false)
		{
			return nullptr;
		}
		auto resource = getStringViewKey(elem, "resource");
		if (game.Resources().addAudioSource(id, sndBuffer, resource) == true)
		{
			parseAudioLoopNamesVal(elem, "loopNames", sndBuffer->loops);

			return &sndBuffer->soundBuffer;
		}
		return nullptr;
	}

	sf::SoundBuffer* parseMultiSoundObj(Game& game, const Value& elem)
	{
		std::string id;
		std::vector<sf::Int16> samplesBuffer;
		unsigned channelCount = 0;
		unsigned sampleRate = 0;

		if (isValidString(elem, "id") == true)
		{
			id = elem["id"sv].GetStringView();
		}

		const auto& fileElem = elem["file"sv];

		bool hasFile = true;
		SizeType fileIdx = 0;
		while (hasFile == true)
		{
			const Value* fileVal = nullptr;
			hasFile = false;

			if (fileElem.IsObject() == true)
			{
				fileVal = &fileElem;
			}
			else if (fileElem.IsArray() == true &&
				fileIdx < fileElem.Size())
			{
				fileVal = &fileElem[fileIdx];
				fileIdx++;
				if (fileIdx < fileElem.Size())
				{
					hasFile = true;
				}
			}
			else
			{
				break;
			}

			const char* fileName = nullptr;

			if (fileVal->IsString() == true)
			{
				fileName = fileVal->GetString();
			}
			else if (fileVal->IsObject() == true &&
				isValidString(*fileVal, "name") == true)
			{
				fileName = (*fileVal)["name"sv].GetString();
			}
			else
			{
				continue;
			}

			sf::PhysFSStream file(fileName);
			if (file.hasError() == true)
			{
				continue;
			}
			if (isValidId(id) == false)
			{
				getIdFromFile(fileName, id);
			}

			sf::SoundBuffer sndBuffer;
			if (sndBuffer.loadFromStream(file) == false)
			{
				continue;
			}

			if (channelCount == 0)
			{
				channelCount = sndBuffer.getChannelCount();
				sampleRate = sndBuffer.getSampleRate();
			}
			else if (channelCount != sndBuffer.getChannelCount() ||
				sampleRate != sndBuffer.getSampleRate())
			{
				continue;
			}

			bool hasChunk = true;
			SizeType chunkIdx = 0;
			while (hasChunk == true)
			{
				sf::Music::TimeSpan chunk;
				hasChunk = false;

				if (fileVal->IsObject() == true &&
					fileVal->HasMember("chunks") == true)
				{
					const auto& chunksElem = (*fileVal)["chunks"sv];
					if (chunksElem.IsObject() == true)
					{
						chunk = parseAudioTimeSpan(chunksElem);
					}
					else if (chunksElem.IsArray() == true &&
						chunkIdx < chunksElem.Size())
					{
						chunk = parseAudioTimeSpan(chunksElem[chunkIdx]);
						chunkIdx++;
						if (chunkIdx < chunksElem.Size())
						{
							hasChunk = true;
						}
					}
				}

				sf::Uint64 startPos = 0;
				sf::Uint64 newSize = sndBuffer.getSampleCount();

				if (chunk.offset != sf::Time::Zero)
				{
					startPos = std::min(
						sf::Music2::timeToSamples(chunk.offset, sampleRate, channelCount),
						newSize);
				}
				if (chunk.length != sf::Time::Zero)
				{
					auto length = sf::Music2::timeToSamples(chunk.length, sampleRate, channelCount);
					if (startPos + length > newSize)
					{
						newSize = newSize - startPos;
					}
					else
					{
						newSize = length;
					}
				}
				if (newSize == 0)
				{
					break;
				}

				auto oldSize = samplesBuffer.size();

				samplesBuffer.resize(oldSize + (size_t)newSize);

				auto samplesPtr = samplesBuffer.data() + oldSize;
				auto sndBufferPtr = sndBuffer.getSamples() + (size_t)startPos;
				memcpy(samplesPtr, sndBufferPtr, (size_t)newSize * sizeof(sf::Int16));
			}
		}
		if (isValidId(id) == false ||
			samplesBuffer.empty() == true)
		{
			return nullptr;
		}

		if (elem.HasMember("loopNames"sv) == false)
		{
			return parseMultiSoundObj(game, id, getStringViewKey(elem, "resource"),
				samplesBuffer.data(), samplesBuffer.size(), channelCount, sampleRate);
		}
		return parseMultiSoundLoopsObj(game, elem, id, samplesBuffer.data(),
			samplesBuffer.size(), channelCount, sampleRate);
	}

	bool parseSoundFromId(Game& game, const Value& elem)
	{
		if (isValidString(elem, "fromId") == true)
		{
			if (isValidString(elem, "id") == true)
			{
				auto fromId = elem["fromId"sv].GetStringView();
				auto id = elem["id"sv].GetStringView();
				if (fromId != id && isValidId(id) == true)
				{
					auto obj = game.Resources().getAudioSource(fromId);
					if (std::holds_alternative<std::shared_ptr<sf::SoundBuffer>>(obj) == true)
					{
						game.Resources().addAudioSource(id, obj, getStringViewKey(elem, "resource"));
					}
				}
			}
			return true;
		}
		return false;
	}

	sf::SoundBuffer* getSoundObj(Game& game, const Value& elem)
	{
		if (isValidString(elem, "file") == true)
		{
			return parseSingleSoundObj(game, elem);
		}
		else if (isValidArray(elem, "file") == true)
		{
			return parseMultiSoundObj(game, elem);
		}
		return nullptr;
	}

	void parseSound(Game& game, const Value& elem,
		const getSoundObjFuncPtr getSoundObjFunc)
	{
		assert(getSoundObjFunc != nullptr);

		if (parseSoundFromId(game, elem) == true)
		{
			return;
		}

		auto sndBuffer = getSoundObjFunc(game, elem);
		if (sndBuffer == nullptr)
		{
			return;
		}

		if (getBoolKey(elem, "play") == true)
		{
			sf::Sound sound(*sndBuffer);

			auto volume = getVariableKey(elem, "volume");
			auto vol = game.getVarOrProp<int64_t, unsigned>(volume, game.SoundVolume());
			if (vol > 100)
			{
				vol = 100;
			}
			sound.setVolume((float)vol);

			game.Resources().addPlayingSound(sound);
		}
	}

	void parseSound(Game& game, const Value& elem)
	{
		parseSound(game, elem, getSoundObj);
	}
}
