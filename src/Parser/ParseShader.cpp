#include "ParseShader.h"
#include "FileUtils.h"
#include "Game.h"
#include "Utils/ParseUtils.h"
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	void parseShader(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false)
		{
			return;
		}
		auto id = elem["id"sv].GetStringView();
		if (isValidId(id) == false)
		{
			return;
		}
		if (game.Resources().Shaders().has(id) == false)
		{
			GameShader shader;
			if (isValidString(elem, "fragmentFile") == true)
			{
				auto fragmentShaderText = FileUtils::readText(elem["fragmentFile"sv].GetStringView());
				if (isValidString(elem, "vertexFile") == true)
				{
					auto vertexShaderText = FileUtils::readText(elem["vertexFile"sv].GetStringView());
					if (isValidString(elem, "geometryFile") == true)
					{
						auto geometryShaderText = FileUtils::readText(elem["geometryFile"sv].GetStringView());
						shader.shader = ShaderManager::makeShader(
							fragmentShaderText, vertexShaderText, geometryShaderText
						);
					}
					else
					{
						shader.shader = ShaderManager::makeShader(
							fragmentShaderText, vertexShaderText
						);
					}
				}
				else
				{
					shader.shader = ShaderManager::makeShader(fragmentShaderText);
				}
			}

			if (shader.shader == nullptr)
			{
				return;
			}

			if (isValidArray(elem, "uniforms") == true)
			{
				for (const auto& val : elem["uniforms"sv])
				{
					if (isValidString(val) == true)
					{
						shader.uniforms.push_back(str2int16(val.GetStringView()));
					}
				}
			}
			else if (isValidString(elem, "uniforms") == true)
			{
				shader.uniforms.push_back(str2int16(elem["uniforms"sv].GetStringView()));
			}

			game.Resources().Shaders().add(id, std::move(shader));
		}

		if (isValidString(elem, "setShader") == true)
		{
			auto shader = game.Resources().Shaders().get(id);
			game.setShader(elem["setShader"sv].GetStringView(), shader);
		}
	}
}
