#include "TextUtils.h"
#include "Game.h"
#include "GameUtils.h"

namespace TextUtils
{
	std::string getFormatString(const Game& game, const std::string_view format,
		const std::vector<std::string>& bindings)
	{
		if (bindings.size() > 0)
		{
			if (format == "[1]")
			{
				return game.getVarOrPropStringS(bindings[0]);
			}
			else
			{
				std::string displayText(format);
				if (format.size() > 2)
				{
					for (size_t i = 0; i < bindings.size(); i++)
					{
						auto str = game.getVarOrPropStringS(bindings[i]);
						Utils::replaceStringInPlace(
							displayText,
							"[" + Utils::toString(i + 1) + "]",
							str);
					}
				}
				return displayText;
			}
		}
		return "";
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
