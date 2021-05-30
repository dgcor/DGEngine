#pragma once

#include "Shader.h"
#include "Utils/UnorderedStringMap.h"

class ShaderManager
{
private:
	mutable UnorderedStringMap<GameShader> shaders;

public:
	static const std::string gameShaderCode;
	static const std::string levelShaderCode;
	static const std::string spriteShaderCode;

	static std::unique_ptr<sf::Shader> makeShader(const std::string& fragmentShaderCode);
	static std::unique_ptr<sf::Shader> makeShader(const std::string& fragmentShaderCode,
		const std::string& vertexShaderCode);
	static std::unique_ptr<sf::Shader> makeShader(const std::string& fragmentShaderCode,
		const std::string& vertexShaderCode, const std::string& geometryShaderCode);

	static void init(ShaderManager& shaderManager);

	void init(GameShaders& gameShaders) const;

	void add(const std::string_view id, GameShader&& shader);

	GameShader* get(const std::string_view id) const;
	bool has(const std::string_view id) const;
};
