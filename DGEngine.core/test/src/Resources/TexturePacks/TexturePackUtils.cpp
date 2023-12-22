#include "catch2/catch_test_macros.hpp"

#include "Resources/TexturePacks/TexturePackUtils.h"

TEST_CASE("TexturePackUtils::calculateDirection", "TexturePackUtils")
{
	auto CHECK_calculateDirection = [](uint32_t frameIdx, uint32_t startIdx,
		uint32_t numFrames, uint32_t directions, uint32_t directionIdx, AnimationFlags flags)
	{
		AnimationFlags calculatedFlags;
		CHECK(TexturePackUtils::calculateDirection(frameIdx, startIdx, numFrames, directions, calculatedFlags) == directionIdx);
		CHECK(calculatedFlags == flags);
	};

	SECTION("[0, 0) + directions = 0")
	{
		CHECK_calculateDirection(0, 0, 0, 0, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(1, 0, 0, 0, 0, AnimationFlags::Overflow);
	}
	SECTION("[0, 0) + directions = 1")
	{
		CHECK_calculateDirection(0, 0, 0, 1, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(1, 0, 0, 1, 0, AnimationFlags::Overflow);
	}
	SECTION("[10, 0) + directions = 0")
	{
		CHECK_calculateDirection(0, 10, 0, 0, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(1, 10, 0, 0, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(10, 10, 0, 0, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(11, 10, 0, 0, 0, AnimationFlags::Overflow);
	}
	SECTION("[10, 0) + directions = 1")
	{
		CHECK_calculateDirection(0, 10, 0, 1, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(1, 10, 0, 1, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(10, 10, 0, 1, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(11, 10, 0, 1, 0, AnimationFlags::Overflow);
	}
	SECTION("[0, 0) + directions = 4")
	{
		CHECK_calculateDirection(0, 0, 0, 4, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(1, 0, 0, 4, 0, AnimationFlags::Overflow);
	}
	SECTION("[0, 1) + directions = 0")
	{
		CHECK_calculateDirection(0, 0, 1, 0, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(1, 0, 1, 0, 0, AnimationFlags::Overflow);
	}
	SECTION("[0, 1) + directions = 1")
	{
		CHECK_calculateDirection(0, 0, 1, 1, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(1, 0, 1, 1, 0, AnimationFlags::Overflow);
	}
	SECTION("[10, 1) + directions = 0")
	{
		CHECK_calculateDirection(0, 10, 1, 0, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(1, 10, 1, 0, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(10, 10, 1, 0, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(11, 10, 1, 0, 0, AnimationFlags::Overflow);
	}
	SECTION("[10, 1) + directions = 1")
	{
		CHECK_calculateDirection(0, 10, 1, 1, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(1, 10, 1, 1, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(10, 10, 1, 1, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(11, 10, 1, 1, 0, AnimationFlags::Overflow);
	}
	SECTION("[0, 1) + directions = 4")
	{
		CHECK_calculateDirection(0, 0, 1, 4, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(1, 0, 1, 4, 0, AnimationFlags::Overflow);
	}
	SECTION("[0, 2) + directions = 0")
	{
		CHECK_calculateDirection(0, 0, 2, 0, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(1, 0, 2, 0, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(2, 0, 2, 0, 0, AnimationFlags::Overflow);
	}
	SECTION("[0, 2) + directions = 1")
	{
		CHECK_calculateDirection(0, 0, 2, 1, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(1, 0, 2, 1, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(2, 0, 2, 1, 0, AnimationFlags::Overflow);
	}
	SECTION("[0, 4) + directions = 1")
	{
		CHECK_calculateDirection(0, 0, 4, 1, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(1, 0, 4, 1, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(2, 0, 4, 1, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(3, 0, 4, 1, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(4, 0, 4, 1, 0, AnimationFlags::Overflow);
	}
	SECTION("[0, 4) + directions = 2")
	{
		CHECK_calculateDirection(0, 0, 4, 2, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(1, 0, 4, 2, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(2, 0, 4, 2, 1, AnimationFlags::Valid);
		CHECK_calculateDirection(3, 0, 4, 2, 1, AnimationFlags::Valid);
		CHECK_calculateDirection(4, 0, 4, 2, 0, AnimationFlags::Overflow);
	}
	SECTION("[0, 4) + directions = 3")
	{
		CHECK_calculateDirection(0, 0, 4, 3, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(1, 0, 4, 3, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(2, 0, 4, 3, 1, AnimationFlags::Valid);
		CHECK_calculateDirection(3, 0, 4, 3, 2, AnimationFlags::Valid);
		CHECK_calculateDirection(4, 0, 4, 3, 0, AnimationFlags::Overflow);
	}
	SECTION("[0, 4) + directions = 4")
	{
		CHECK_calculateDirection(0, 0, 4, 4, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(1, 0, 4, 4, 1, AnimationFlags::Valid);
		CHECK_calculateDirection(2, 0, 4, 4, 2, AnimationFlags::Valid);
		CHECK_calculateDirection(3, 0, 4, 4, 3, AnimationFlags::Valid);
		CHECK_calculateDirection(4, 0, 4, 4, 0, AnimationFlags::Overflow);
	}
	SECTION("[0, 8) + directions = 4")
	{
		CHECK_calculateDirection(0, 0, 8, 4, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(1, 0, 8, 4, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(2, 0, 8, 4, 1, AnimationFlags::Valid);
		CHECK_calculateDirection(3, 0, 8, 4, 1, AnimationFlags::Valid);
		CHECK_calculateDirection(4, 0, 8, 4, 2, AnimationFlags::Valid);
		CHECK_calculateDirection(5, 0, 8, 4, 2, AnimationFlags::Valid);
		CHECK_calculateDirection(6, 0, 8, 4, 3, AnimationFlags::Valid);
		CHECK_calculateDirection(7, 0, 8, 4, 3, AnimationFlags::Valid);
		CHECK_calculateDirection(8, 0, 8, 4, 0, AnimationFlags::Overflow);
	}
	SECTION("[0, 8) + directions = 5")
	{
		CHECK_calculateDirection(0, 0, 8, 5, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(1, 0, 8, 5, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(2, 0, 8, 5, 1, AnimationFlags::Valid);
		CHECK_calculateDirection(3, 0, 8, 5, 1, AnimationFlags::Valid);
		CHECK_calculateDirection(4, 0, 8, 5, 2, AnimationFlags::Valid);
		CHECK_calculateDirection(5, 0, 8, 5, 3, AnimationFlags::Valid);
		CHECK_calculateDirection(6, 0, 8, 5, 3, AnimationFlags::Valid);
		CHECK_calculateDirection(7, 0, 8, 5, 4, AnimationFlags::Valid);
		CHECK_calculateDirection(8, 0, 8, 5, 0, AnimationFlags::Overflow);
	}
	SECTION("[0, 8) + directions = 6")
	{
		CHECK_calculateDirection(0, 0, 8, 6, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(1, 0, 8, 6, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(2, 0, 8, 6, 1, AnimationFlags::Valid);
		CHECK_calculateDirection(3, 0, 8, 6, 2, AnimationFlags::Valid);
		CHECK_calculateDirection(4, 0, 8, 6, 3, AnimationFlags::Valid);
		CHECK_calculateDirection(5, 0, 8, 6, 3, AnimationFlags::Valid);
		CHECK_calculateDirection(6, 0, 8, 6, 4, AnimationFlags::Valid);
		CHECK_calculateDirection(7, 0, 8, 6, 5, AnimationFlags::Valid);
		CHECK_calculateDirection(8, 0, 8, 6, 0, AnimationFlags::Overflow);
	}
	SECTION("[0, 8) + directions = 7")
	{
		CHECK_calculateDirection(0, 0, 8, 7, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(1, 0, 8, 7, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(2, 0, 8, 7, 1, AnimationFlags::Valid);
		CHECK_calculateDirection(3, 0, 8, 7, 2, AnimationFlags::Valid);
		CHECK_calculateDirection(4, 0, 8, 7, 3, AnimationFlags::Valid);
		CHECK_calculateDirection(5, 0, 8, 7, 4, AnimationFlags::Valid);
		CHECK_calculateDirection(6, 0, 8, 7, 5, AnimationFlags::Valid);
		CHECK_calculateDirection(7, 0, 8, 7, 6, AnimationFlags::Valid);
		CHECK_calculateDirection(8, 0, 8, 7, 0, AnimationFlags::Overflow);
	}
	SECTION("[0, 8) + directions = 8")
	{
		CHECK_calculateDirection(0, 0, 8, 8, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(1, 0, 8, 8, 1, AnimationFlags::Valid);
		CHECK_calculateDirection(2, 0, 8, 8, 2, AnimationFlags::Valid);
		CHECK_calculateDirection(3, 0, 8, 8, 3, AnimationFlags::Valid);
		CHECK_calculateDirection(4, 0, 8, 8, 4, AnimationFlags::Valid);
		CHECK_calculateDirection(5, 0, 8, 8, 5, AnimationFlags::Valid);
		CHECK_calculateDirection(6, 0, 8, 8, 6, AnimationFlags::Valid);
		CHECK_calculateDirection(7, 0, 8, 8, 7, AnimationFlags::Valid);
		CHECK_calculateDirection(8, 0, 8, 8, 0, AnimationFlags::Overflow);
	}
	SECTION("[0, 8) + directions = 9")
	{
		CHECK_calculateDirection(0, 0, 8, 9, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(1, 0, 8, 9, 1, AnimationFlags::Valid);
		CHECK_calculateDirection(2, 0, 8, 9, 2, AnimationFlags::Valid);
		CHECK_calculateDirection(3, 0, 8, 9, 3, AnimationFlags::Valid);
		CHECK_calculateDirection(4, 0, 8, 9, 4, AnimationFlags::Valid);
		CHECK_calculateDirection(5, 0, 8, 9, 5, AnimationFlags::Valid);
		CHECK_calculateDirection(6, 0, 8, 9, 6, AnimationFlags::Valid);
		CHECK_calculateDirection(7, 0, 8, 9, 7, AnimationFlags::Valid);
		CHECK_calculateDirection(8, 0, 8, 9, 0, AnimationFlags::Overflow);
	}
	SECTION("[0, 8) + directions = 16")
	{
		CHECK_calculateDirection(0, 0, 8, 16, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(1, 0, 8, 16, 1, AnimationFlags::Valid);
		CHECK_calculateDirection(2, 0, 8, 16, 2, AnimationFlags::Valid);
		CHECK_calculateDirection(3, 0, 8, 16, 3, AnimationFlags::Valid);
		CHECK_calculateDirection(4, 0, 8, 16, 4, AnimationFlags::Valid);
		CHECK_calculateDirection(5, 0, 8, 16, 5, AnimationFlags::Valid);
		CHECK_calculateDirection(6, 0, 8, 16, 6, AnimationFlags::Valid);
		CHECK_calculateDirection(7, 0, 8, 16, 7, AnimationFlags::Valid);
		CHECK_calculateDirection(8, 0, 8, 16, 0, AnimationFlags::Overflow);
	}
	SECTION("[4, 4) + directions = 0")
	{
		CHECK_calculateDirection(0, 4, 4, 0, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(1, 4, 4, 0, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(2, 4, 4, 0, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(3, 4, 4, 0, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(4, 4, 4, 0, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(5, 4, 4, 0, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(6, 4, 4, 0, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(7, 4, 4, 0, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(8, 4, 4, 0, 0, AnimationFlags::Overflow);
	}
	SECTION("[4, 4) + directions = 1")
	{
		CHECK_calculateDirection(0, 4, 4, 1, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(1, 4, 4, 1, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(2, 4, 4, 1, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(3, 4, 4, 1, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(4, 4, 4, 1, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(5, 4, 4, 1, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(6, 4, 4, 1, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(7, 4, 4, 1, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(8, 4, 4, 1, 0, AnimationFlags::Overflow);
	}
	SECTION("[4, 4) + directions = 2")
	{
		CHECK_calculateDirection(0, 4, 4, 2, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(1, 4, 4, 2, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(2, 4, 4, 2, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(3, 4, 4, 2, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(4, 4, 4, 2, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(5, 4, 4, 2, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(6, 4, 4, 2, 1, AnimationFlags::Valid);
		CHECK_calculateDirection(7, 4, 4, 2, 1, AnimationFlags::Valid);
		CHECK_calculateDirection(8, 4, 4, 2, 0, AnimationFlags::Overflow);
	}
	SECTION("[4, 4) + directions = 3")
	{
		CHECK_calculateDirection(0, 4, 4, 3, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(1, 4, 4, 3, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(2, 4, 4, 3, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(3, 4, 4, 3, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(4, 4, 4, 3, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(5, 4, 4, 3, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(6, 4, 4, 3, 1, AnimationFlags::Valid);
		CHECK_calculateDirection(7, 4, 4, 3, 2, AnimationFlags::Valid);
		CHECK_calculateDirection(8, 4, 4, 3, 0, AnimationFlags::Overflow);
	}
	SECTION("[4, 4) + directions = 4")
	{
		CHECK_calculateDirection(0, 4, 4, 4, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(1, 4, 4, 4, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(2, 4, 4, 4, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(3, 4, 4, 4, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(4, 4, 4, 4, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(5, 4, 4, 4, 1, AnimationFlags::Valid);
		CHECK_calculateDirection(6, 4, 4, 4, 2, AnimationFlags::Valid);
		CHECK_calculateDirection(7, 4, 4, 4, 3, AnimationFlags::Valid);
		CHECK_calculateDirection(8, 4, 4, 4, 0, AnimationFlags::Overflow);
	}
	SECTION("[4, 4) + directions = 8")
	{
		CHECK_calculateDirection(0, 4, 4, 8, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(1, 4, 4, 8, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(2, 4, 4, 8, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(3, 4, 4, 8, 0, AnimationFlags::Overflow);
		CHECK_calculateDirection(4, 4, 4, 8, 0, AnimationFlags::Valid);
		CHECK_calculateDirection(5, 4, 4, 8, 1, AnimationFlags::Valid);
		CHECK_calculateDirection(6, 4, 4, 8, 2, AnimationFlags::Valid);
		CHECK_calculateDirection(7, 4, 4, 8, 3, AnimationFlags::Valid);
		CHECK_calculateDirection(8, 4, 4, 8, 0, AnimationFlags::Overflow);
	}
}

TEST_CASE("TexturePackUtils::calculateRange", "TexturePackUtils")
{
	using Range = std::pair<uint32_t, uint32_t>;

	auto CHECK_calculateRange = [](uint32_t startIdx, uint32_t stopIdx,
		int32_t directionIdx, uint32_t directions, const Range& range, bool success)
	{
		AnimationInfo animInfo;
		CHECK(TexturePackUtils::calculateRange(startIdx, stopIdx, directionIdx, directions, animInfo) == success);
		CHECK(animInfo.indexRange == range);
		if (success == true)
		{
			CHECK(animInfo.Valid() == true);
			CHECK(animInfo.Overflow() == false);
		}
		else
		{
			CHECK(animInfo.Valid() == false);
			CHECK(animInfo.Overflow() == true);
		}
	};

	SECTION("[0, 0] + directions = 0")
	{
		CHECK_calculateRange(0, 0, -1, 0, Range(0, 0), false);
		CHECK_calculateRange(0, 0, 0, 0, Range(0, 0), false);
		CHECK_calculateRange(0, 0, 1, 0, Range(0, 0), false);
	}
	SECTION("[0, 1] + directions = 0")
	{
		CHECK_calculateRange(0, 1, -1, 0, Range(0, 1), false);
		CHECK_calculateRange(0, 1, 0, 0, Range(0, 1), false);
		CHECK_calculateRange(0, 1, 1, 0, Range(0, 1), false);
	}
	SECTION("[0, 5] + directions = 0")
	{
		CHECK_calculateRange(0, 5, -1, 0, Range(0, 5), false);
		CHECK_calculateRange(0, 5, 0, 0, Range(0, 5), false);
		CHECK_calculateRange(0, 5, 1, 0, Range(0, 5), false);
	}
	SECTION("[0, 5] + directions = 1")
	{
		CHECK_calculateRange(0, 5, -1, 1, Range(0, 5), false);
		CHECK_calculateRange(0, 5, 0, 1, Range(0, 5), true);
		CHECK_calculateRange(0, 5, 1, 1, Range(0, 5), false);
		CHECK_calculateRange(0, 5, 2, 1, Range(0, 5), false);
	}
	SECTION("[0, 5] + directions = 2")
	{
		CHECK_calculateRange(0, 5, 0, 2, Range(0, 2), true);
		CHECK_calculateRange(0, 5, 1, 2, Range(3, 5), true);
		CHECK_calculateRange(0, 5, 2, 2, Range(0, 5), false);
		CHECK_calculateRange(0, 5, 3, 2, Range(0, 5), false);
		CHECK_calculateRange(0, 5, 4, 2, Range(0, 5), false);
		CHECK_calculateRange(0, 5, 5, 2, Range(0, 5), false);
		CHECK_calculateRange(0, 5, 6, 2, Range(0, 5), false);
	}
	SECTION("[0, 5] + directions = 3")
	{
		CHECK_calculateRange(0, 5, 0, 3, Range(0, 1), true);
		CHECK_calculateRange(0, 5, 1, 3, Range(2, 3), true);
		CHECK_calculateRange(0, 5, 2, 3, Range(4, 5), true);
		CHECK_calculateRange(0, 5, 3, 3, Range(0, 5), false);
		CHECK_calculateRange(0, 5, 4, 3, Range(0, 5), false);
		CHECK_calculateRange(0, 5, 5, 3, Range(0, 5), false);
		CHECK_calculateRange(0, 5, 6, 3, Range(0, 5), false);
	}
	SECTION("[0, 5] + directions = 4")
	{
		CHECK_calculateRange(0, 5, 0, 4, Range(0, 1), true);
		CHECK_calculateRange(0, 5, 1, 4, Range(2, 2), true);
		CHECK_calculateRange(0, 5, 2, 4, Range(3, 4), true);
		CHECK_calculateRange(0, 5, 3, 4, Range(5, 5), true);
		CHECK_calculateRange(0, 5, 4, 4, Range(0, 5), false);
		CHECK_calculateRange(0, 5, 5, 4, Range(0, 5), false);
		CHECK_calculateRange(0, 5, 6, 4, Range(0, 5), false);
	}
	SECTION("[0, 5] + directions = 5")
	{
		CHECK_calculateRange(0, 5, 0, 5, Range(0, 1), true);
		CHECK_calculateRange(0, 5, 1, 5, Range(2, 2), true);
		CHECK_calculateRange(0, 5, 2, 5, Range(3, 3), true);
		CHECK_calculateRange(0, 5, 3, 5, Range(4, 4), true);
		CHECK_calculateRange(0, 5, 4, 5, Range(5, 5), true);
		CHECK_calculateRange(0, 5, 5, 5, Range(0, 5), false);
		CHECK_calculateRange(0, 5, 6, 5, Range(0, 5), false);
	}
	SECTION("[0, 5] + directions = 6")
	{
		CHECK_calculateRange(0, 5, 0, 6, Range(0, 0), true);
		CHECK_calculateRange(0, 5, 1, 6, Range(1, 1), true);
		CHECK_calculateRange(0, 5, 2, 6, Range(2, 2), true);
		CHECK_calculateRange(0, 5, 3, 6, Range(3, 3), true);
		CHECK_calculateRange(0, 5, 4, 6, Range(4, 4), true);
		CHECK_calculateRange(0, 5, 5, 6, Range(5, 5), true);
		CHECK_calculateRange(0, 5, 6, 6, Range(0, 5), false);
	}
	SECTION("[0, 5] + directions = 9")
	{
		CHECK_calculateRange(0, 5, 0, 9, Range(0, 0), true);
		CHECK_calculateRange(0, 5, 1, 9, Range(1, 1), true);
		CHECK_calculateRange(0, 5, 2, 9, Range(2, 2), true);
		CHECK_calculateRange(0, 5, 3, 9, Range(3, 3), true);
		CHECK_calculateRange(0, 5, 4, 9, Range(4, 4), true);
		CHECK_calculateRange(0, 5, 5, 9, Range(5, 5), true);
		CHECK_calculateRange(0, 5, 6, 9, Range(0, 5), false);
		CHECK_calculateRange(0, 5, 7, 9, Range(0, 5), false);
		CHECK_calculateRange(0, 5, 8, 9, Range(0, 5), false);
		CHECK_calculateRange(0, 5, 9, 9, Range(0, 5), false);
		CHECK_calculateRange(0, 5, 10, 9, Range(0, 5), false);
	}
	SECTION("[0, 7] + directions = 6")
	{
		CHECK_calculateRange(0, 7, 0, 6, Range(0, 1), true);
		CHECK_calculateRange(0, 7, 1, 6, Range(2, 2), true);
		CHECK_calculateRange(0, 7, 2, 6, Range(3, 3), true);
		CHECK_calculateRange(0, 7, 3, 6, Range(4, 5), true);
		CHECK_calculateRange(0, 7, 4, 6, Range(6, 6), true);
		CHECK_calculateRange(0, 7, 5, 6, Range(7, 7), true);
		CHECK_calculateRange(0, 7, 6, 6, Range(0, 7), false);
		CHECK_calculateRange(0, 7, 7, 6, Range(0, 7), false);
	}
	SECTION("[2, 1] + directions = 0")
	{
		CHECK_calculateRange(2, 1, -1, 0, Range(2, 1), false);
		CHECK_calculateRange(2, 1, 0, 0, Range(2, 1), false);
		CHECK_calculateRange(2, 1, 1, 0, Range(2, 1), false);
	}
	SECTION("[2, 1] + directions = 1")
	{
		CHECK_calculateRange(2, 1, -1, 1, Range(2, 1), false);
		CHECK_calculateRange(2, 1, 0, 1, Range(2, 1), false);
		CHECK_calculateRange(2, 1, 1, 1, Range(2, 1), false);
	}
	SECTION("[5, 0] + directions = 2")
	{
		CHECK_calculateRange(5, 0, -1, 2, Range(5, 0), false);
		CHECK_calculateRange(5, 0, 0, 2, Range(5, 0), false);
		CHECK_calculateRange(5, 0, 1, 2, Range(5, 0), false);
		CHECK_calculateRange(5, 0, 2, 2, Range(5, 0), false);
		CHECK_calculateRange(5, 0, 3, 2, Range(5, 0), false);
	}
	SECTION("[4, 4) + directions = 3")
	{
		CHECK_calculateRange(4, 7, 0, 3, Range(4, 5), true);
		CHECK_calculateRange(4, 7, 1, 3, Range(6, 6), true);
		CHECK_calculateRange(4, 7, 2, 3, Range(7, 7), true);
		CHECK_calculateRange(4, 7, 3, 3, Range(4, 7), false);
	}
}
