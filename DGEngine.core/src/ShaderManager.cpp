#include "ShaderManager.h"
#include "Utils/Utils.h"

const std::string ShaderManager::gameShaderCode{ R"(
#version 110
uniform sampler2D texture;
uniform vec4 fade;
uniform float gamma;

vec3 Gamma(vec3 value, float param)
{
	return vec3(pow(abs(value.r), param), pow(abs(value.g), param), pow(abs(value.b), param));
}

void main()
{
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy) - fade;
	float gamma2 = 1.0 - (0.007 * gamma);
	gl_FragColor = vec4(Gamma(pixel.rgb, gamma2), pixel.a);
}
)" };

const std::string ShaderManager::levelShaderCode{ R"(
#version 110
uniform sampler2D texture;

void main()
{
	gl_FragColor = texture2D(texture, gl_TexCoord[0].xy);
}
)" };

const std::string ShaderManager::spriteShaderCode{ R"(
#version 110
uniform sampler2D palette;
uniform sampler2D texture;
uniform vec2 pixelSize;
uniform vec4 outline;
uniform vec4 ignore;
uniform bool hasPalette;

void main()
{
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	if (hasPalette == true && pixel.a == 1.0)
	{
		pixel = texture2D(palette, vec2(pixel.r, 0.0));
	}

	if (outline.a > 0.0 && pixel.a == 0.0)
	{
		vec4 vu = texture2D(texture, vec2(gl_TexCoord[0].x , gl_TexCoord[0].y - pixelSize.y));
		vec4 vd = texture2D(texture, vec2(gl_TexCoord[0].x , gl_TexCoord[0].y + pixelSize.y));
		vec4 vl = texture2D(texture, vec2(gl_TexCoord[0].x - pixelSize.x, gl_TexCoord[0].y));
		vec4 vr = texture2D(texture, vec2(gl_TexCoord[0].x + pixelSize.x, gl_TexCoord[0].y));

		if (vu.a == 1.0 || vd.a == 1.0 || vl.a == 1.0 || vr.a == 1.0)
		{
			if (ignore.a == 0.0)
			{
				pixel = outline;
			}
			else
			{
				if (vu != ignore &&
					vd != ignore &&
					vl != ignore &&
					vr != ignore)
				{
					pixel = outline;
				}
			}
		}
	}
	gl_FragColor = pixel;
}
)" };

std::unique_ptr<sf::Shader> ShaderManager::makeShader(const std::string& fragmentShaderText)
{
	auto shader = std::make_unique<sf::Shader>();
	if (shader->isAvailable() == true &&
		shader->loadFromMemory(fragmentShaderText, sf::Shader::Fragment) == true)
	{
		shader->setUniform("texture", sf::Shader::CurrentTexture);
		return shader;
	}
	return {};
}

std::unique_ptr<sf::Shader> ShaderManager::makeShader(const std::string& fragmentShaderText,
	const std::string& vertexShaderText)
{
	auto shader = std::make_unique<sf::Shader>();
	if (shader->isAvailable() == true &&
		shader->loadFromMemory(vertexShaderText, fragmentShaderText) == true)
	{
		shader->setUniform("texture", sf::Shader::CurrentTexture);
		return shader;
	}
	return {};
}

std::unique_ptr<sf::Shader> ShaderManager::makeShader(const std::string& fragmentShaderText,
	const std::string& vertexShaderText, const std::string& geometryShaderText)
{
	auto shader = std::make_unique<sf::Shader>();
	if (shader->isAvailable() == true &&
		shader->loadFromMemory(vertexShaderText,
			geometryShaderText, fragmentShaderText) == true)
	{
		shader->setUniform("texture", sf::Shader::CurrentTexture);
		return shader;
	}
	return {};
}

void ShaderManager::init(ShaderManager& shaderManager)
{
	GameShader gameShader;
	gameShader.shader = makeShader(gameShaderCode);
	gameShader.uniforms.push_back(str2int16("fade"));
	gameShader.uniforms.push_back(str2int16("gamma"));
	shaderManager.add("game", std::move(gameShader));

	GameShader levelShader;
	levelShader.shader = makeShader(levelShaderCode);
	shaderManager.add("level", std::move(levelShader));

	GameShader spriteShader;
	spriteShader.shader = makeShader(spriteShaderCode);
	spriteShader.uniforms.push_back(str2int16("palette"));
	spriteShader.uniforms.push_back(str2int16("pixelSize"));
	spriteShader.uniforms.push_back(str2int16("outline"));
	shaderManager.add("sprite", std::move(spriteShader));
}

void ShaderManager::init(GameShaders& gameShaders) const
{
	gameShaders.Game = get("game");
	gameShaders.Level = get("level");
	gameShaders.Sprite = get("sprite");
}

void ShaderManager::add(const std::string_view id, GameShader&& shader)
{
	if (shaders.find(sv2str(id)) == shaders.end())
	{
		auto it = shaders.insert(std::make_pair(id, std::move(shader)));
		if (it.second == false)
		{
			it.first->second = std::move(shader);
		}
	}
}

GameShader* ShaderManager::get(const std::string_view id) const
{
	auto it = shaders.find(sv2str(id));
	if (it != shaders.end())
	{
		return &it->second;
	}
	return nullptr;
}

bool ShaderManager::has(const std::string_view id) const
{
	return shaders.find(sv2str(id)) != shaders.end();
}
