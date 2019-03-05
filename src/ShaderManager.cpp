#include "ShaderManager.h"

const std::string ShaderManager::gameText{ R"(
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

const std::string ShaderManager::spriteText{ R"(
#version 110
uniform sampler2D palette;
uniform sampler2D texture;
uniform vec2 pixelSize;
uniform vec4 outline;
uniform vec4 ignore;
uniform vec4 light;
uniform bool hasPalette;

void main()
{
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	if (hasPalette == true && pixel.a == 1.0)
	{
		pixel = texture2D(palette, vec2(pixel.r, 0.0)) - light;
	}
	else
	{
		pixel = pixel - light;
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

void ShaderManager::add(const std::string& id, const std::string& fragmentShaderText)
{
	if (shaders.find(id) == shaders.end())
	{
		auto shader = std::make_shared<sf::Shader>();
		if (shader->isAvailable() == true &&
			shader->loadFromMemory(fragmentShaderText, sf::Shader::Fragment) == true)
		{
			shader->setUniform("texture", sf::Shader::CurrentTexture);
			shaders[id] = std::move(shader);
		}
	}
}

void ShaderManager::add(const std::string& id, const std::string& fragmentShaderText,
	const std::string& vertexShaderText)
{
	if (shaders.find(id) == shaders.end())
	{
		auto shader = std::make_shared<sf::Shader>();
		if (shader->isAvailable() == true &&
			shader->loadFromMemory(vertexShaderText, fragmentShaderText) == true)
		{
			shader->setUniform("texture", sf::Shader::CurrentTexture);
			shaders[id] = std::move(shader);
		}
	}
}

void ShaderManager::add(const std::string& id, const std::string& fragmentShaderText,
	const std::string& vertexShaderText, const std::string& geometryShaderText)
{
	if (shaders.find(id) == shaders.end())
	{
		auto shader = std::make_shared<sf::Shader>();
		if (shader->isAvailable() == true &&
			shader->loadFromMemory(vertexShaderText,
				geometryShaderText, fragmentShaderText) == true)
		{
			shader->setUniform("texture", sf::Shader::CurrentTexture);
			shaders[id] = std::move(shader);
		}
	}
}

sf::Shader* ShaderManager::get(const std::string& id) const
{
	auto it = shaders.find(id);
	if (it != shaders.end())
	{
		return it->second.get();
	}
	return nullptr;
}

bool ShaderManager::has(const std::string& id) const
{
	return shaders.find(id) != shaders.end();
}

void ShaderManager::init()
{
	add("game", gameText);
	add("sprite", spriteText);
}

void ShaderManager::init(GameShaders& gameShaders) const
{
	gameShaders.Game = get("game");
	gameShaders.Sprite = get("sprite");
}
