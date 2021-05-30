#pragma once

#include "Action.h"
#include "Game.h"

class ActShaderSetGameShader : public Action
{
private:
	std::string id;
	std::string gameShaderId;

public:
	ActShaderSetGameShader(const std::string_view id_, const std::string_view gameShaderId_)
		: id(id_), gameShaderId(gameShaderId_) {}

	bool execute(Game& game) override
	{
		auto shader = game.Resources().Shaders().get(id);
		game.setShader(gameShaderId, shader);
		return true;
	}
};

template <class T>
class ActShaderSetUniform : public Action
{
private:
	std::string id;
	std::string key;
	T value;

public:
	ActShaderSetUniform(const std::string_view id_, const std::string_view key_,
		const T& value_) : id(id_), key(key_), value(value_) {}

	bool execute(Game& game) override
	{
		auto gameShader = game.Resources().Shaders().get(id);
		if (gameShader != nullptr)
		{
			gameShader->shader->setUniform(key, value);
		}
		return true;
	}
};
