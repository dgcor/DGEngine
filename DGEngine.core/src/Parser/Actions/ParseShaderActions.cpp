#include "ParseShaderActions.h"
#include "Game/Actions/ActShader.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser::Actions
{
	using namespace rapidjson;

	std::shared_ptr<Action> parseShaderSetGameShader(const Value& elem)
	{
		return std::make_shared<ActShaderSetGameShader>(
			getStringViewKey(elem, "shader"),
			getStringViewKey(elem, "gameShader"));
	}

	std::shared_ptr<Action> parseShaderSetBool(const Value& elem)
	{
		return std::make_shared<ActShaderSetUniform<bool>>(
			getStringViewKey(elem, "id"),
			getStringViewKey(elem, "key"),
			getBoolKey(elem, "value"));
	}

	std::shared_ptr<Action> parseShaderSetColor(const Value& elem)
	{
		return std::make_shared<ActShaderSetUniform<sf::Glsl::Vec4>>(
			getStringViewKey(elem, "id"),
			getStringViewKey(elem, "key"),
			sf::Glsl::Vec4(getColorKey(elem, "value")));
	}

	std::shared_ptr<Action> parseShaderSetFloat(const Value& elem)
	{
		return std::make_shared<ActShaderSetUniform<float>>(
			getStringViewKey(elem, "id"),
			getStringViewKey(elem, "key"),
			getFloatKey(elem, "value"));
	}

	std::shared_ptr<Action> parseShaderSetVec2(const Value& elem)
	{
		return std::make_shared<ActShaderSetUniform<sf::Glsl::Vec2>>(
			getStringViewKey(elem, "id"),
			getStringViewKey(elem, "key"),
			getVector2fKey<sf::Glsl::Vec2>(elem, "value"));
	}

	std::shared_ptr<Action> parseShaderSetVec3(const Value& elem)
	{
		return std::make_shared<ActShaderSetUniform<sf::Glsl::Vec3>>(
			getStringViewKey(elem, "id"),
			getStringViewKey(elem, "key"),
			getVector3fKey<sf::Glsl::Vec3>(elem, "value"));
	}

	std::shared_ptr<Action> parseShadersetVec4(const Value& elem)
	{
		return std::make_shared<ActShaderSetUniform<sf::Glsl::Vec4>>(
			getStringViewKey(elem, "id"),
			getStringViewKey(elem, "key"),
			getVector4fKey<sf::Glsl::Vec4>(elem, "value"));
	}
}
