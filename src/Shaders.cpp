#include "Shaders.h"

const std::string Shaders::outlineText{ R"(
#version 110
uniform sampler2D tex;
uniform float texWidth;
uniform float texHeight;
uniform vec4 outline;
uniform vec4 ignore;

void main()
{
	vec2 v_texCoords = gl_TexCoord[0].xy;
	vec4 pixel = texture2D(tex, v_texCoords);

	if (pixel.a != 1.0)
	{
		vec4 vu = texture2D(tex, vec2(v_texCoords.x , v_texCoords.y - texHeight));
		vec4 vd = texture2D(tex, vec2(v_texCoords.x , v_texCoords.y + texHeight));
		vec4 vl = texture2D(tex, vec2(v_texCoords.x - texWidth, v_texCoords.y));
		vec4 vr = texture2D(tex, vec2(v_texCoords.x + texWidth, v_texCoords.y));

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

const std::string Shaders::paletteText{ R"(
#version 110
uniform sampler2D palette;
uniform sampler2D tex;

void main()
{
	vec4 pixel = texture2D(tex, gl_TexCoord[0].xy);
	if (pixel.a == 1.0)
	{
		pixel = texture2D(palette, vec2(pixel.r, 0.0));
	}
	gl_FragColor = pixel;
}
)" };

bool Shaders::hasOutline{ false };
bool Shaders::hasPalette{ false };

sf::Shader Shaders::Outline;
sf::Shader Shaders::Palette;

void Shaders::init()
{
	if (hasOutline == false)
	{
		if (Outline.isAvailable() == true &&
			Outline.loadFromMemory(outlineText, sf::Shader::Fragment) == true)
		{
			hasOutline = true;
		}
		Outline.setUniform("tex", sf::Shader::CurrentTexture);
	}

	if (hasPalette == false)
	{
		if (Palette.isAvailable() == true &&
			Palette.loadFromMemory(paletteText, sf::Shader::Fragment) == true)
		{
			hasPalette = true;
		}
		Palette.setUniform("tex", sf::Shader::CurrentTexture);
	}
}
