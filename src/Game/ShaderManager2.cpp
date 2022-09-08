#include "ShaderManager2.h"
#include "Utils/StringHash.h"

const std::string ShaderManager2::levelShaderCode{ R"(
#version 110
uniform sampler2D texture;
uniform float defaultLight;

void main()
{
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	float light = min(pixel.a, defaultLight);
	pixel.r = max(pixel.r - light, 0.0);
	pixel.g = max(pixel.g - light, 0.0);
	pixel.b = max(pixel.b - light, 0.0);
	pixel.a = 1.0;
	gl_FragColor = pixel;
}
)" };

void ShaderManager2::init(ShaderManager& shaderManager)
{
	GameShader gameShader;
	gameShader.shader = ShaderManager::makeShader(ShaderManager::gameShaderCode);
	gameShader.uniforms.push_back(str2int16("fade"));
	gameShader.uniforms.push_back(str2int16("gamma"));
	shaderManager.add("game", std::move(gameShader));

	GameShader levelShader;
	levelShader.shader = ShaderManager::makeShader(levelShaderCode);
	levelShader.uniforms.push_back(str2int16("defaultLight"));
	shaderManager.add("level", std::move(levelShader));

	GameShader spriteShader;
	spriteShader.shader = ShaderManager::makeShader(ShaderManager::spriteShaderCode);
	spriteShader.uniforms.push_back(str2int16("palette"));
	spriteShader.uniforms.push_back(str2int16("pixelSize"));
	spriteShader.uniforms.push_back(str2int16("outline"));
	shaderManager.add("sprite", std::move(spriteShader));
}
