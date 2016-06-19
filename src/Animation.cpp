////////////////////////////////////////////////////////////
//
// Copyright (C) 2014 Maximilian Wagenbach (aka. Foaly) (foaly.f@web.de)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
// you must not claim that you wrote the original software.
// If you use this software in a product, an acknowledgment
// in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
// and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#include "Animation.h"
#include "Game.h"

void Animation::setFrames(int16_t framesX_, int16_t framesY_)
{
	framesX = framesX_ - 1;
	framesY = framesY_ - 1;
	auto size = Size();
	frameWidth = (int16_t)size.x / framesX_;
	frameHeight = (int16_t)size.y / framesY_;
	framePosX = 0;
	framePosY = 0;

	setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
}

void Animation::update(Game& game)
{
	if (pause == true ||
		(framesX == 0 && framesY == 0) ||
		Visible() == false)
	{
		return;
	}

	// add delta time
	currentTime += game.getElapsedTime();

	// if current time is bigger then the frame time advance one frame
	if (currentTime >= frameTime)
	{
		// reset time, but keep the remainder
		currentTime = sf::microseconds(currentTime.asMicroseconds() % frameTime.asMicroseconds());

		sf::IntRect rect(
			frameWidth * framePosX,
			frameHeight * framePosY,
			frameWidth,
			frameHeight);

		setTextureRect(rect);

		if (framePosY < framesY)
		{
			framePosY++;
		}
		else
		{
			framePosY = 0;

			if (framePosX < framesX)
			{
				framePosX++;
			}
			else
			{
				framePosX = 0;

				if (loop == false)
				{
					pause = true;
				}
			}
		}
	}
}
