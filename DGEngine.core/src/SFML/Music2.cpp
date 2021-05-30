#include "Music2.h"
#include <SFML/System/Lock.hpp>
#include <SFML/System/Err.hpp>
#include <fstream>

namespace sf
{
	Music2::~Music2()
	{
		stop();
		if (m_type == 1)
		{
			m_file.~MusicFile();
		}
	}

	bool Music2::openFromFile(const std::string& filename)
	{
		stop();

		if (m_type == 2)
		{
			return false;
		}
		if (m_file.file.openFromFile(filename) == false)
		{
			return false;
		}
		m_type = 1;

		initializeFile();

		return true;
	}

	bool Music2::openFromMemory(const void* data, std::size_t sizeInBytes)
	{
		stop();

		if (m_type == 2)
		{
			return false;
		}
		if (m_file.file.openFromMemory(data, sizeInBytes) == false)
		{
			return false;
		}
		m_type = 1;

		initializeFile();

		return true;
	}

	bool Music2::openFromStream(InputStream& stream)
	{
		stop();

		if (m_type == 2)
		{
			return false;
		}
		if (m_file.file.openFromStream(stream) == false)
		{
			return false;
		}
		m_type = 1;

		initializeFile();

		return true;
	}

	bool Music2::openFromSoundBuffer(const sf::SoundBuffer& buffer)
	{
		stop();

		if (m_type == 1)
		{
			return false;
		}
		m_type = 2;

		initializeBuffer(buffer);

		return true;
	}

	Time Music2::getDuration() const
	{
		if (m_type == 2)
		{
			return m_buffer.duration;
		}
		return m_file.file.getDuration();
	}

	Music::TimeSpan Music2::getLoopPoints() const
	{
		return Music::TimeSpan(samplesToTime(m_loopSpan.offset), samplesToTime(m_loopSpan.length));
	}

	void Music2::setLoopPoints(Music::TimeSpan timePoints)
	{
		if (m_type == 0)
		{
			return;
		}

		Music::Span<Uint64> samplePoints(timeToSamples(timePoints.offset), timeToSamples(timePoints.length));

		// Check our state. This averts a divide-by-zero. GetChannelCount() is cheap enough to use often
		Uint64 sampleCount = 0;
		if (m_type == 2)
		{
			sampleCount = m_buffer.sampleCount;
		}
		else
		{
			sampleCount = m_file.file.getSampleCount();
		}

		if (getChannelCount() == 0 || sampleCount == 0)
		{
			sf::err() << "Music is not in a valid state to assign Loop Points." << std::endl;
			return;
		}

		// Round up to the next even sample if needed
		samplePoints.offset += (getChannelCount() - 1);
		samplePoints.offset -= (samplePoints.offset % getChannelCount());
		samplePoints.length += (getChannelCount() - 1);
		samplePoints.length -= (samplePoints.length % getChannelCount());

		// Validate
		if (samplePoints.offset >= sampleCount)
		{
			sf::err() << "LoopPoints offset val must be in range [0, Duration)." << std::endl;
			return;
		}
		if (samplePoints.length == 0)
		{
			sf::err() << "LoopPoints length val must be nonzero." << std::endl;
			return;
		}

		// Clamp End Point
		samplePoints.length = std::min(samplePoints.length, sampleCount - samplePoints.offset);

		// If this change has no effect, we can return without touching anything
		if (samplePoints.offset == m_loopSpan.offset &&
			samplePoints.length == m_loopSpan.length)
		{
			return;
		}

		// When we apply this change, we need to "reset" this instance and its buffer

		// Get old playing status and position
		Status oldStatus = getStatus();
		Time oldPos = getPlayingOffset();

		// Unload
		stop();

		// Set
		m_loopSpan = samplePoints;

		// Restore
		if (oldPos != Time::Zero)
		{
			setPlayingOffset(oldPos);
		}

		// Resume
		if (oldStatus == Playing)
		{
			play();
		}
	}

	bool Music2::onGetData(SoundStream::Chunk& data)
	{
		Lock lock(m_mutex);
		if (m_type == 2)
		{
			return onGetDataBuffer(data);
		}
		return onGetDataFile(data);
	}

	bool Music2::onGetDataFile(SoundStream::Chunk& data)
	{
		Uint64 toFill = m_file.samples.size();
		Uint64 currentOffset = m_file.file.getSampleOffset();
		Uint64 loopEnd = m_loopSpan.offset + m_loopSpan.length;

		// If the loop end is enabled and imminent, request less data.
		// This will trip an "onLoop()" call from the underlying SoundStream,
		// and we can then take action.
		if (getLoop() &&
			(m_loopSpan.length != 0) &&
			(currentOffset <= loopEnd) &&
			(currentOffset + toFill > loopEnd))
		{
			toFill = loopEnd - currentOffset;
		}

		// Fill the chunk parameters
		data.samples = &m_file.samples[0];
		data.sampleCount = static_cast<std::size_t>(
			m_file.file.read(&m_file.samples[0], toFill));
		currentOffset += data.sampleCount;

		// Check if we have stopped obtaining samples or reached either the EOF or the loop end point
		return (data.sampleCount != 0) &&
			(currentOffset < m_file.file.getSampleCount()) &&
			!(currentOffset == loopEnd && m_loopSpan.length != 0);
	}

	bool Music2::onGetDataBuffer(SoundStream::Chunk& data)
	{
		Uint64 toFill = m_buffer.sampleBufferSize;
		Uint64 currentOffset = m_buffer.sampleBufferOffset;
		Uint64 loopEnd = m_loopSpan.offset + m_loopSpan.length;

		// If the loop end is enabled and imminent, request less data.
		// This will trip an "onLoop()" call from the underlying SoundStream,
		// and we can then take action.
		if (getLoop() &&
			(m_loopSpan.length != 0)
			&& (currentOffset <= loopEnd)
			&& (currentOffset + toFill > loopEnd))
		{
			toFill = loopEnd - currentOffset;
		}

		if ((currentOffset + toFill) > m_buffer.sampleCount)
		{
			toFill = m_buffer.sampleCount - currentOffset;
		}

		// Fill the chunk parameters
		data.samples = m_buffer.samples + currentOffset;
		data.sampleCount = static_cast<std::size_t>(toFill);
		currentOffset += data.sampleCount;
		m_buffer.sampleBufferOffset = currentOffset;

		// Check if we have stopped obtaining samples or reached either the EOF or the loop end point
		return (data.sampleCount != 0)
			&& (currentOffset < m_buffer.sampleCount)
			&& !(currentOffset == loopEnd && m_loopSpan.length != 0);
	}

	void Music2::onSeek(Time timeOffset)
	{
		Lock lock(m_mutex);
		switch (m_type)
		{
		default:
		case 0:
			return;
		case 1:
		{
			m_file.file.seek(timeOffset);
			break;
		}
		case 2:
		{
			m_buffer.sampleBufferOffset = std::min(timeToSamples(timeOffset), m_buffer.sampleCount);
			break;
		}
		}
	}

	Int64 Music2::onLoop()
	{
		// Called by underlying SoundStream so we can determine where to loop.
		Lock lock(m_mutex);
		switch (m_type)
		{
		default:
		case 0:
			return 0;
		case 1:
			return onLoopFile();
		case 2:
			return onLoopBuffer();
		}
	}

	Int64 Music2::onLoopFile()
	{
		Uint64 currentOffset = m_file.file.getSampleOffset();
		if (getLoop() &&
			(m_loopSpan.length != 0) &&
			(currentOffset == m_loopSpan.offset + m_loopSpan.length))
		{
			// Looping is enabled, and either we're at the loop end, or we're at the EOF
			// when it's equivalent to the loop end (loop end takes priority). Send us to loop begin
			m_file.file.seek(m_loopSpan.offset);
			return m_file.file.getSampleOffset();
		}
		else if (getLoop() &&
			(currentOffset >= m_file.file.getSampleCount()))
		{
			// If we're at the EOF, reset to 0
			m_file.file.seek(0);
			return 0;
		}
		return NoLoop;
	}

	Int64 Music2::onLoopBuffer()
	{
		Uint64 currentOffset = m_buffer.sampleBufferOffset;
		if (getLoop() &&
			(m_loopSpan.length != 0) &&
			(currentOffset == m_loopSpan.offset + m_loopSpan.length))
		{
			// Looping is enabled, and either we're at the loop end, or we're at the EOF
			// when it's equivalent to the loop end (loop end takes priority). Send us to loop begin
			m_buffer.sampleBufferOffset = std::min(m_loopSpan.offset, m_buffer.sampleCount);
			return m_buffer.sampleBufferOffset;
		}
		else if (getLoop() &&
			(currentOffset >= m_buffer.sampleCount))
		{
			// If we're at the EOF, reset to 0
			m_buffer.sampleBufferOffset = 0;
			return 0;
		}
		return NoLoop;
	}

	void Music2::initializeFile()
	{
		// Compute the music positions
		m_loopSpan.offset = 0;
		m_loopSpan.length = m_file.file.getSampleCount();

		// Resize the internal buffer so that it can contain 1 second of audio samples
		m_file.samples.resize(m_file.file.getSampleRate() * m_file.file.getChannelCount());

		// Initialize the stream
		SoundStream::initialize(m_file.file.getChannelCount(), m_file.file.getSampleRate());
	}

	void Music2::initializeBuffer(const sf::SoundBuffer& buffer)
	{
		m_buffer.samples = buffer.getSamples();
		m_buffer.channelCount = buffer.getChannelCount();
		m_buffer.sampleCount = buffer.getSampleCount();
		m_buffer.sampleRate = buffer.getSampleRate();
		m_buffer.sampleBufferSize = (size_t)(m_buffer.sampleRate * m_buffer.channelCount);
		m_buffer.duration = buffer.getDuration();

		// Compute the music positions
		m_loopSpan.offset = 0;
		m_loopSpan.length = m_buffer.sampleCount;

		// Initialize the stream
		SoundStream::initialize((unsigned)m_buffer.channelCount, (unsigned)m_buffer.sampleRate);
	}

	Uint64 Music2::timeToSamples(Time position,
		unsigned sampleRate, unsigned channelCount)
	{
		// Always ROUND, no unchecked truncation, hence the addition in the numerator.
		// This avoids most precision errors arising from "samples => Time => samples" conversions
		// Original rounding calculation is ((Micros * Freq * Channels) / 1000000) + 0.5
		// We refactor it to keep Int64 as the data type throughout the whole operation.
		return ((position.asMicroseconds() * sampleRate * channelCount) + 500000) / 1000000;
	}

	Time Music2::samplesToTime(Uint64 samples,
		unsigned sampleRate, unsigned channelCount)
	{
		Time position = Time::Zero;

		// Make sure we don't divide by 0
		if (sampleRate != 0 && channelCount != 0)
		{
			position = microseconds((samples * 1000000) / ((Uint64)channelCount * sampleRate));
		}

		return position;
	}
}
