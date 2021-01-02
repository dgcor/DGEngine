#include "TextUtils.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils/Utils.h"

namespace TextUtils
{
	bool getFormatString(const Game& game, const std::string_view format,
		const std::vector<std::string>& bindings, std::string& outStr)
	{
		if (bindings.size() > 0)
		{
			if (format == "[1]")
			{
				return game.getVarOrPropStringS(bindings[0], outStr);
			}
			else
			{
				bool hasBinding = true;
				outStr = format;
				if (format.size() > 2)
				{
					for (size_t i = 0; i < bindings.size(); i++)
					{
						std::string str;
						hasBinding &= game.getVarOrPropStringS(bindings[i], str);
						Utils::replaceStringInPlace(
							outStr,
							"[" + Utils::toString(i + 1) + "]",
							str);
					}
				}
				return hasBinding;
			}
		}
		return false;
	}

	std::string getFormatString(const Game& game, const std::string_view format,
		const std::vector<std::string>& bindings)
	{
		std::string str;
		getFormatString(game, format, bindings, str);
		return str;
	}

	std::string getTextQueryable(const Game& game, const std::string_view format,
		const std::string_view query)
	{
		auto queryable = game.getQueryable(query);
		if (queryable != nullptr)
		{
			return GameUtils::replaceStringWithQueryable(format, *queryable);
		}
		return std::string(format);
	}

	std::string getText(const Game& game, TextOp textOp, const std::string_view textOrformat,
		const std::vector<std::string>& bindings)
	{
		std::string str;
		appendText(game, textOp, str, textOrformat, bindings);
		return str;
	}

	void appendText(const Game& game, TextOp textOp, std::string& str,
		const std::string_view textOrformat, const std::vector<std::string>& bindings)
	{
		switch (TextOp(((uint32_t)textOp) & 0x3u))
		{
		default:
		case TextOp::Set:
			str += textOrformat;
			break;
		case TextOp::Replace:
			str += game.getVarOrPropStringS(textOrformat);
			break;
		case TextOp::Query:
		{
			if (bindings.empty() == false)
			{
				str += getTextQueryable(game, textOrformat, bindings.front());
			}
			else
			{
				str += textOrformat;
			}
			break;
		}
		case TextOp::FormatString:
			str += getFormatString(game, textOrformat, bindings);
			break;
		}
		if ((uint32_t)textOp & (uint32_t)TextOp::ReplaceAll)
		{
			str = GameUtils::replaceStringWithVarOrProp(str, game);
		}
		if ((uint32_t)textOp & (uint32_t)TextOp::Trim)
		{
			str = Utils::trim(str, " \t\r\n");
		}
		if ((uint32_t)textOp & (uint32_t)TextOp::RemoveEmptyLines)
		{
			str = Utils::removeEmptyLines(str);
		}
	}
}
