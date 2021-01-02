#pragma once

#include "Shader.h"
#include "Utils/UnorderedStringMap.h"

class ShaderManager
{
private:
	static const std::string gameText;
	static const std::string levelText;
	static const std::string spriteText;

	mutable UnorderedStringMap<GameShader> shaders;

public:
	static std::unique_ptr<sf::Shader> makeShader(const std::string& fragmentShaderText);
	static std::unique_ptr<sf::Shader> makeShader(const std::string& fragmentShaderText,
		const std::string& vertexShaderText);
	static std::unique_ptr<sf::Shader> makeShader(const std::string& fragmentShaderText,
		const std::string& vertexShaderText, const std::string& geometryShaderText);

	void add(const std::string_view id, GameShader&& shader);

	GameShader* get(const std::string_view id) const;
	bool has(const std::string_view id) const;

	void init();
	void init(GameShaders& gameShaders) const;
};
