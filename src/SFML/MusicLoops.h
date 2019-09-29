#pragma once

#include "Music2.h"

namespace sf
{
	class MusicLoops : public Music2
	{
	private:
		std::vector<Music::Span<Uint64>> samplePoints;
		size_t currLoopIdx{ 0 };
		bool loop{ false };

		bool updateLoopIndex();

		Int64 onLoopFile();
		Int64 onLoopBuffer();

	public:
		MusicLoops() { Music2::setLoop(true); }

		void setMultiLoopPoints(const std::vector<Music::TimeSpan>& timePoints_);

		void setLoop(bool loop_) { loop = loop_; }
		bool getLoop() const { return loop; }

		Int64 onLoop() override;
	};
}
