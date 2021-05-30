#include "Wave2.h"
#define DR_WAV_NO_STDIO
#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"
#include <SFML/System/InputStream.hpp>

static size_t readWave2(void* pUserData, void* pBufferOut, size_t bytesToRead)
{
	sf::InputStream* stream = (sf::InputStream*)pUserData;
	if (stream == nullptr)
	{
		return -1;
	}
	return (size_t)stream->read(pBufferOut, bytesToRead);
}

static drwav_bool32 seekWave2(void* pUserData, int offset, drwav_seek_origin origin)
{
	sf::InputStream* stream = (sf::InputStream*)pUserData;
	if (stream == nullptr)
	{
		return DRWAV_FALSE;
	}
	if (origin == drwav_seek_origin_current)
	{
		offset += (int)stream->tell();
	}
	auto ret = stream->seek(offset);
	if (ret >= 0)
	{
		return DRWAV_TRUE;
	}
	return DRWAV_FALSE;
}

Wave2FileReader::~Wave2FileReader()
{
	if (isInitialized == true)
	{
		drwav_uninit(&wave);
		isInitialized = false;
	}
}

bool Wave2FileReader::check(sf::InputStream& stream_)
{
	drwav wave;
	if (!drwav_init(&wave, readWave2, seekWave2, &stream_, nullptr))
	{
		return false;
	}
	drwav_uninit(&wave);
	return true;
}

bool Wave2FileReader::open(sf::InputStream& stream_, Info& info)
{
	if (!drwav_init(&wave, readWave2, seekWave2, &stream_, nullptr))
	{
		return false;
	}
	isInitialized = true;
	info.channelCount = wave.channels;
	info.sampleCount = wave.totalPCMFrameCount * wave.channels;
	info.sampleRate = wave.sampleRate;
	return true;
}

void Wave2FileReader::seek(sf::Uint64 sampleOffset)
{
	if (isInitialized == true)
	{
		drwav_seek_to_pcm_frame(&wave, sampleOffset / wave.channels);
	}
}

sf::Uint64 Wave2FileReader::read(sf::Int16* samples, sf::Uint64 maxCount)
{
	if (isInitialized == true)
	{
		return drwav_read_pcm_frames_s16le(&wave, maxCount / wave.channels, samples) * wave.channels;
	}
	return 0;
}
