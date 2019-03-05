#pragma once

#include "Action.h"
#include "FileUtils.h"
#include "Game.h"

class ActShaderLoad : public Action
{
private:
	std::string id;
	std::string fragmentShaderFile;
	std::string vertexShaderFile;
	std::string geometryShaderFile;

public:
	ActShaderLoad(const std::string& id_, const std::string& fragmentShaderFile_,
		const std::string& vertexShaderFile_, const std::string& geometryShaderFile_)
		: id(id_), fragmentShaderFile(fragmentShaderFile_),
		vertexShaderFile(vertexShaderFile_), geometryShaderFile(geometryShaderFile_) {}

	virtual bool execute(Game& game)
	{
		if (fragmentShaderFile.empty() == false)
		{
			auto fragmentShaderText = FileUtils::readText(fragmentShaderFile.c_str());
			if (vertexShaderFile.empty() == false)
			{
				auto vertexShaderText = FileUtils::readText(vertexShaderFile.c_str());
				if (geometryShaderFile.empty() == false)
				{
					auto geometryShaderText = FileUtils::readText(geometryShaderFile.c_str());
					game.Resources().Shaders().add(id,
						fragmentShaderText, vertexShaderText, geometryShaderText);
				}
				else
				{
					game.Resources().Shaders().add(id, fragmentShaderText, vertexShaderText);
				}
			}
			else
			{
				game.Resources().Shaders().add(id, fragmentShaderText);
			}
		}
		return true;
	}
};

class ActShaderSetGameShader : public Action
{
private:
	std::string id;
	std::string gameShaderId;

public:
	ActShaderSetGameShader(const std::string& id_, const std::string& gameShaderId_)
		: id(id_), gameShaderId(gameShaderId_) {}

	virtual bool execute(Game& game)
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
	ActShaderSetUniform(const std::string& id_, const std::string& key_,
		const T& value_) : id(id_), key(key_), value(value_) {}

	virtual bool execute(Game& game)
	{
		auto shader = game.Resources().Shaders().get(id);
		if (shader != nullptr)
		{
			shader->setUniform(key, value);
		}
		return true;
	}
};
