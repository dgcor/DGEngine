#include "MusicLoops.h"
#include <SFML/System/Lock.hpp>
#include <SFML/System/Err.hpp>
#include <variant>

namespace sf
{
	void MusicLoops::setMultiLoopPoints(const std::vector<Music::TimeSpan>& timePoints_)
	{
		if (std::holds_alternative<std::nullptr_t>(m_data) == true ||
			timePoints_.empty() == true)
		{
			return;
		}
		currLoopIdx = 0;

		for (auto& timePoint : timePoints_)
		{
			Music::Span<Uint64> samplePoint(timeToSamples(timePoint.offset), timeToSamples(timePoint.length));

			// Check our state. This averts a divide-by-zero. GetChannelCount() is cheap enough to use often
			Uint64 sampleCount = 0;
			if (std::holds_alternative<MusicFile>(m_data) == true)
			{
				auto& m_file = std::get<MusicFile>(m_data);
				sampleCount = m_file.file.getSampleCount();
			}
			else
			{
				auto& m_buffer = std::get<MusicBuffer>(m_data);
				sampleCount = m_buffer.sampleCount;
			}

			if (getChannelCount() == 0 || sampleCount == 0)
			{
				sf::err() << "Music is not in a valid state to assign Loop Points." << std::endl;
				return;
			}

			// Round up to the next even sample if needed
			samplePoint.offset += (getChannelCount() - 1);
			samplePoint.offset -= (samplePoint.offset % getChannelCount());
			samplePoint.length += (getChannelCount() - 1);
			samplePoint.length -= (samplePoint.length % getChannelCount());

			// Validate
			if (samplePoint.offset >= sampleCount ||
				samplePoint.length == 0)
			{
				continue;
			}

			// Clamp End Point
			samplePoint.length = std::min(samplePoint.length, sampleCount - samplePoint.offset);

			samplePoints.push_back(samplePoint);
		}

		// When we apply this change, we need to "reset" this instance and its buffer

		// Get old playing status and position
		Status oldStatus = getStatus();

		// Unload
		stop();

		// Set
		m_loopSpan = samplePoints.front();
		setPlayingOffset(timePoints_.front().offset);

		// Resume
		if (oldStatus == Playing)
		{
			play();
		}
	}

	bool MusicLoops::updateLoopIndex()
	{
		if (samplePoints.empty() == true)
		{
			return false;
		}
		auto loopSizeM1 = samplePoints.size() - 1;

		if (currLoopIdx == loopSizeM1)
		{
			currLoopIdx = 0;
			if (loop == false)
			{
				Music2::setLoop(false);
				return false;
			}
		}
		else if (currLoopIdx < loopSizeM1)
		{
			currLoopIdx++;
		}
		m_loopSpan = samplePoints[currLoopIdx];
		return true;
	}

	Int64 MusicLoops::onLoopFile()
	{
		auto& m_file = std::get<MusicFile>(m_data);

		Uint64 currentOffset = m_file.file.getSampleOffset();
		if ((m_loopSpan.length != 0) &&
			(currentOffset == m_loopSpan.offset + m_loopSpan.length))
		{
			// Looping is enabled, and either we're at the loop end, or we're at the EOF
			// when it's equivalent to the loop end (loop end takes priority). Send us to loop begin
			if (updateLoopIndex() == true)
			{
				m_file.file.seek(m_loopSpan.offset);
				return m_file.file.getSampleOffset();
			}
			m_file.file.seek(m_file.file.getSampleCount());
			return NoLoop;
		}
		else if (currentOffset >= m_file.file.getSampleCount())
		{
			// If we're at the EOF, reset to 0
			m_file.file.seek(0);
			return 0;
		}
		return NoLoop;
	}

	Int64 MusicLoops::onLoopBuffer()
	{
		auto& m_buffer = std::get<MusicBuffer>(m_data);

		Uint64 currentOffset = m_buffer.sampleBufferOffset;
		if ((m_loopSpan.length != 0) &&
			(currentOffset == m_loopSpan.offset + m_loopSpan.length))
		{
			// Looping is enabled, and either we're at the loop end, or we're at the EOF
			// when it's equivalent to the loop end (loop end takes priority). Send us to loop begin
			if (updateLoopIndex() == true)
			{
				m_buffer.sampleBufferOffset = m_loopSpan.offset;
				return m_buffer.sampleBufferOffset;
			}
			m_buffer.sampleBufferOffset = m_buffer.sampleCount;
			return m_buffer.sampleBufferOffset;
		}
		else if (currentOffset >= m_buffer.sampleCount)
		{
			// If we're at the EOF, reset to 0
			m_buffer.sampleBufferOffset = 0;
			return 0;
		}
		return NoLoop;
	}

	Int64 MusicLoops::onLoop()
	{
		// Called by underlying SoundStream so we can determine where to loop.
		Lock lock(m_mutex);

		if (std::holds_alternative<MusicFile>(m_data) == true)
		{
			return onLoopFile();
		}
		else if (std::holds_alternative<MusicBuffer>(m_data) == true)
		{
			return onLoopBuffer();
		}
		return NoLoop;
	}
}
