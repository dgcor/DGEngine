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

const std::string ShaderManager::levelText{ R"(
#version 110
uniform sampler2D texture;
uniform vec4 visibleRect;
uniform int numberOfLights;
uniform float lights[512];
uniform float defaultLight;
uniform float lightRadius;
uniform float elapsedTime;

void main()
{
	float light = 1.0 - defaultLight;

	if (numberOfLights > 0 && light > 0.0)
	{
		for(int i = 0; i < numberOfLights; i += 4)
		{
			vec2 coord = vec2(gl_TexCoord[0].x, 1.0 - gl_TexCoord[0].y);
			vec2 pixelPos = visibleRect.xy + (visibleRect.zw * coord);
			float dist = distance(pixelPos, vec2(lights[i], lights[i+1]));
			dist = clamp(dist / lightRadius, 0.0, lights[i+3]) / lights[i+3];
			light = clamp(dist, 0.0, light);
			if (light == 0.0)
			{
				break;
			}
		}
	}

	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	pixel.r = pixel.r - light;
	pixel.g = pixel.g - light;
	pixel.b = pixel.b - light;
	gl_FragColor = pixel;
}
)" };

const std::string ShaderManager::spriteText{ R"(
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
	add("level", levelText);
	add("sprite", spriteText);
}

void ShaderManager::init(GameShaders& gameShaders) const
{
	gameShaders.Game = get("game");
	gameShaders.Level = get("level");
	gameShaders.Sprite = get("sprite");
}
