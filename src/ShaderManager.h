#pragma once

#include <memory>
#include <SFML/Graphics/Shader.hpp>
#include <string>
#include <unordered_map>

struct GameShaders
{
	sf::Shader* Game{ nullptr };
	sf::Shader* Sprite{ nullptr };

	bool hasGameShader() const noexcept { return Game != nullptr; }
	bool hasSpriteShader() const noexcept { return Sprite != nullptr; }
};

class ShaderManager
{
private:
	static const std::string gameText;
	static const std::string spriteText;

	std::unordered_map<std::string, std::shared_ptr<sf::Shader>> shaders;

public:
	void add(const std::string& id, const std::string& fragmentShaderText);
	void add(const std::string& id, const std::string& fragmentShaderText,
		const std::string& vertexShaderText);
	void add(const std::string& id, const std::string& fragmentShaderText,
		const std::string& vertexShaderText, const std::string& geometryShaderText);

	sf::Shader* get(const std::string& id) const;
	bool has(const std::string& id) const;

	void init();
	void init(GameShaders& gameShaders) const;
};
