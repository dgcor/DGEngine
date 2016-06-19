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

#pragma once

#include <cinttypes>
#include "Image.h"
#include <SFML/System/Time.hpp>

class Animation : public Image
{
private:
	int16_t framesX;
	int16_t framesY;
	int16_t frameWidth;
	int16_t frameHeight;
	int16_t framePosX;
	int16_t framePosY;

	sf::Time frameTime;
	sf::Time currentTime;
	bool pause;
	bool loop;

public:
	Animation(const sf::Texture& tex, sf::Time frameTime = sf::milliseconds(50),
		bool pause_ = false, bool loop_ = true) : Image(tex),
		framePosX(0), framePosY(0), frameTime(frameTime), pause(pause_), loop(loop_) {}

	sf::Time getFrameTime() const { return frameTime; }
	void setFrameTime(sf::Time time) { frameTime = time; }

	bool Pause() const { return pause; }
	void Pause(bool pause_) { pause = pause_; }
	bool Loop() const { return loop; }
	void Loop(bool loop_) { loop = loop_; }
	void stop()
	{
		pause = true;
		framePosX = 0;
		framePosY = 0;
	}

	void setFrames(int16_t framesX_, int16_t framesY_);

	virtual void update(Game& game);
};
