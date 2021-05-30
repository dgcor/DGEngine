#pragma once

#include "Action.h"
#include "Game.h"

class ActGameAddToProperty : public Action
{
private:
	std::string prop;
	Variable value;

public:
	ActGameAddToProperty(const std::string_view prop_, const Variable& value_)
		: prop(prop_), value(value_) {}

	bool execute(Game& game) override
	{
		Variable propVal;
		if (game.getGameProperty(prop, propVal) == true)
		{
			if (std::holds_alternative<int64_t>(propVal) == true)
			{
				if (std::holds_alternative<int64_t>(value) == true)
				{
					auto val = std::get<int64_t>(propVal) + std::get<int64_t>(value);
					propVal.emplace<int64_t>(val);
					game.setGameProperty(prop, propVal);
				}
				else if (std::holds_alternative<double>(value) == true)
				{
					auto val = (double)std::get<int64_t>(propVal) + std::get<double>(value);
					propVal.emplace<int64_t>((int64_t)std::round(val));
					game.setGameProperty(prop, propVal);
				}
			}
			else if (std::holds_alternative<double>(propVal) == true)
			{
				if (std::holds_alternative<int64_t>(value) == true)
				{
					auto val = std::get<double>(propVal) + (double)std::get<int64_t>(value);
					propVal.emplace<double>(val);
					game.setGameProperty(prop, propVal);
				}
				else if (std::holds_alternative<double>(value) == true)
				{
					auto val = std::get<double>(propVal) + std::get<double>(value);
					propVal.emplace<double>(val);
					game.setGameProperty(prop, propVal);
				}
			}
		}
		return true;
	}
};

class ActGameClearPlayingSounds : public Action
{
public:
	bool execute(Game& game) noexcept override
	{
		game.Resources().clearPlayingSounds();
		return true;
	}
};

class ActGameClose : public Action
{
public:
	bool execute(Game& game) override
	{
		game.close();
		return true;
	}
};

class ActGameDraw : public Action
{
public:
	bool execute(Game& game) override
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

	bool execute(Game& game) noexcept override
	{
		game.EnableInput(enable);
		return true;
	}
};

class ActGameLoad : public Action
{
private:
	std::string file;
	std::string mainFile;

public:
	ActGameLoad(const std::string_view file_, const std::string_view mainFile_)
		: file(file_), mainFile(mainFile_) {}

	bool execute(Game& game) override
	{
		game.load(game.getPath() + file, mainFile);
		return true;
	}
};

class ActGamePauseOnFocusLoss : public Action
{
private:
	bool pause;

public:
	ActGamePauseOnFocusLoss(bool pause_) noexcept : pause(pause_) {}

	bool execute(Game& game) noexcept override
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

	bool execute(Game& game) override
	{
		auto val = game.getVarOrProp<int64_t, unsigned>(gamma, 0u);
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

	bool execute(Game& game) override
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
	ActGameSetProperty(const std::string_view prop_, const Variable& value_)
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

	bool execute(Game& game) override
	{
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
		game.setGameProperty(prop, value2);
		return true;
	}
};

class ActGameSetSoundVolume : public Action
{
private:
	Variable volume;

public:
	ActGameSetSoundVolume(const Variable& volume_) noexcept : volume(volume_) {}

	bool execute(Game& game) override
	{
		auto val = game.getVarOrProp<int64_t, unsigned>(volume, 100u);
		game.SoundVolume(val);
		return true;
	}
};
