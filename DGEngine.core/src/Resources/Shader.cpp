#include "Shader.h"
#include "Utils/StringHash.h"

void GameShaders::set(const std::string_view id, GameShader* shader) noexcept
{
	if (id.size() <= 1)
	{
		return;
	}
	switch (str2int16(id))
	{
	case str2int16("game"):
		Game = shader;
		break;
	case str2int16("level"):
		Level = shader;
		break;
	case str2int16("sprite"):
		Sprite = shader;
		break;
	default:
		break;
	}
}
