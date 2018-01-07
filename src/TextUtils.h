#pragma once

#include <string>
#include <vector>

class Game;

namespace TextUtils
{
	enum class TextOp : uint32_t
	{
		Set = 0,
		Replace = 1,
		ReplaceAll = 2,
		Query = 3,
		FormatString = 4,
		Trim = 8,
		RemoveEmptyLines = 16,
	};

	using T = std::underlying_type_t<TextOp>;

	inline TextOp operator~ (TextOp a) { return (TextOp)~static_cast<T>(a); }
	inline TextOp operator| (TextOp a, TextOp b) { return (TextOp)(static_cast<T>(a) | static_cast<T>(b)); }
	inline TextOp operator& (TextOp a, TextOp b) { return (TextOp)(static_cast<T>(a) & static_cast<T>(b)); }
	inline TextOp operator^ (TextOp a, TextOp b) { return (TextOp)(static_cast<T>(a) ^ static_cast<T>(b)); }
	inline TextOp& operator|= (TextOp& a, TextOp b) { a = (TextOp)(static_cast<T>(a) | static_cast<T>(b)); return a; }
	inline TextOp& operator&= (TextOp& a, TextOp b) { a = (TextOp)(static_cast<T>(a) & static_cast<T>(b)); return a; }
	inline TextOp& operator^= (TextOp& a, TextOp b) { a = (TextOp)(static_cast<T>(a) ^ static_cast<T>(b)); return a; }

	std::string getFormatString(const Game& game, const std::string& format,
		const std::vector<std::string>& bindings);

	std::string getTextQueryable(const Game& game, const std::string& format,
		const std::string& query);

	std::string getText(const Game& game, TextOp textOp, const std::string& textOrformat,
		const std::vector<std::string>& bindings);

	void appendText(const Game& game, TextOp textOp, std::string& str,
		const std::string& textOrformat, const std::vector<std::string>& bindings);
}
