#pragma once

#include "Action.h"
#include "Game.h"

class ActGameAddToProperty : public Action
{
private:
	std::string prop;
	Variable value;

public:
	ActGameAddToProperty(const std::string& prop_, const Variable& value_)
		: prop(prop_), value(value_) {}

	virtual bool execute(Game& game)
	{
		auto prop2 = game.getVarOrPropStringS(prop);
		Variable value2;
		if (game.getProperty(prop, value2) == true)
		{
			if (std::holds_alternative<int64_t>(value2) == true &&
				std::holds_alternative<int64_t>(value) == true)
			{
				auto val = std::get<int64_t>(value2) + std::get<int64_t>(value);
				value2.emplace<int64_t>(val);
				game.setProperty(prop2, value2);
			}
			else if (std::holds_alternative<double>(value2) == true &&
				std::holds_alternative<double>(value) == true)
			{
				auto val = std::get<double>(value2) + std::get<double>(value);
				value2.emplace<double>(val);
				game.setProperty(prop2, value2);
			}
		}
		return true;
	}
};

class ActGameClearPlayingSounds : public Action
{
public:
	virtual bool execute(Game& game) noexcept
	{
		game.Resources().clearPlayingSounds();
		return true;
	}
};

class ActGameClose : public Action
{
public:
	virtual bool execute(Game& game)
	{
		game.Window().close();
		return true;
	}
};

class ActGameDraw : public Action
{
public:
	virtual bool execute(Game& game)
	{
		game.draw();
		return true;
	}
};

class ActGameEnableInput : public Action
{
private:
	bool enable;

public:
	ActGameEnableInput(bool enable_) noexcept : enable(enable_) {}

	virtual bool execute(Game& game) noexcept
	{
		game.EnableInput(enable);
		return true;
	}
};

class ActGamePauseOnFocusLoss : public Action
{
private:
	bool pause;

public:
	ActGamePauseOnFocusLoss(bool pause_) noexcept : pause(pause_) {}

	virtual bool execute(Game& game) noexcept
	{
		game.PauseOnFocusLoss(pause);
		return true;
	}
};

class ActGameSetGamma : public Action
{
private:
	Variable gamma;

public:
	ActGameSetGamma(const Variable& gamma_) noexcept : gamma(gamma_) {}

	virtual bool execute(Game& game)
	{
		auto val = game.getVarOrProp<int64_t, unsigned>(gamma, 30u);
		game.Gamma(val);
		return true;
	}
};

class ActGameSetMusicVolume : public Action
{
private:
	Variable volume;

public:
	ActGameSetMusicVolume(const Variable& volume_) noexcept : volume(volume_) {}

	virtual bool execute(Game& game)
	{
		auto val = game.getVarOrProp<int64_t, unsigned>(volume, 100u);
		game.MusicVolume(val);
		return true;
	}
};

class ActGameSetProperty : public Action
{
private:
	std::string prop;
	Variable value;
	sf::Vector2i propRange;
	sf::Vector2i valueRange;
	bool hasPropRange{ false };
	bool hasValueRange{ false };

public:
	ActGameSetProperty(const std::string& prop_, const Variable& value_)
		: prop(prop_), value(value_) {}

	void setPropRange(const sf::Vector2i& propRange_) noexcept
	{
		propRange = propRange_;
		hasPropRange = true;
	}
	void setValueRange(const sf::Vector2i& valueRange_) noexcept
	{
		valueRange = valueRange_;
		hasValueRange = true;
	}

	virtual bool execute(Game& game)
	{
		auto prop2 = game.getVarOrPropStringS(prop);
		auto value2 = game.getVarOrProp(value);
		if (std::holds_alternative<int64_t>(value2) == true)
		{
			if (hasPropRange == true &&
				hasValueRange == true)
			{
				auto val = std::get<int64_t>(value2);
				val = (int64_t)Utils::normalizeNumber<sf::Vector2i>((long)val, valueRange, propRange);
				value2.emplace<int64_t>(val);
			}
		}
		game.setProperty(prop2, value2);
		return true;
	}
};

class ActGameSetSoundVolume : public Action
{
private:
	Variable volume;

public:
	ActGameSetSoundVolume(const Variable& volume_) noexcept : volume(volume_) {}

	virtual bool execute(Game& game)
	{
		auto val = game.getVarOrProp<int64_t, unsigned>(volume, 100u);
		game.SoundVolume(val);
		return true;
	}
};
