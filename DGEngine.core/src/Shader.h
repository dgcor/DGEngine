#pragma once

#include <memory>
#include <SFML/Graphics/Shader.hpp>
#include "Utils/FixedArray.h"

struct GameShader
{
	std::unique_ptr<sf::Shader> shader;
	FixedArray<uint16_t, 6> uniforms;
};

struct GameShaders
{
	GameShader* Game{ nullptr };
	GameShader* Level{ nullptr };
	GameShader* Sprite{ nullptr };

	bool hasGameShader() const noexcept { return Game != nullptr; }
	bool hasLevelShader() const noexcept { return Level != nullptr; }
	bool hasSpriteShader() const noexcept { return Sprite != nullptr; }
};
