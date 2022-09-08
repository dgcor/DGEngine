#include "ParseShader.h"
#include "Game/Game.h"
#include "Game/Utils/FileUtils.h"
#include "Parser/ParseCommon.h"
#include "Parser/Utils/ParseUtils.h"
#include "Utils/StringHash.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	void parseShader(Game& game, const Value& elem)
	{
		auto id = parseValidId(elem);
		if (id.empty() == true)
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
			game.Shaders().set(elem["setShader"sv].GetStringView(), shader);
		}
	}
}
