#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

class Game;

namespace TextUtils
{
	enum class TextOp : uint32_t
	{
		Set = 0,
		Replace = 1,
		Query = 2,
		FormatString = 4,
		ReplaceAll = 8,
		Trim = 16,
		RemoveEmptyLines = 32,
	};

	using T = std::underlying_type_t<TextOp>;

	constexpr TextOp operator~ (TextOp a) noexcept { return (TextOp)~static_cast<T>(a); }
	constexpr TextOp operator| (TextOp a, TextOp b) noexcept { return (TextOp)(static_cast<T>(a) | static_cast<T>(b)); }
	constexpr TextOp operator& (TextOp a, TextOp b) noexcept { return (TextOp)(static_cast<T>(a) & static_cast<T>(b)); }
	constexpr TextOp operator^ (TextOp a, TextOp b) noexcept { return (TextOp)(static_cast<T>(a) ^ static_cast<T>(b)); }
	constexpr TextOp& operator|= (TextOp& a, TextOp b) noexcept { a = (TextOp)(static_cast<T>(a) | static_cast<T>(b)); return a; }
	constexpr TextOp& operator&= (TextOp& a, TextOp b) noexcept { a = (TextOp)(static_cast<T>(a) & static_cast<T>(b)); return a; }
	constexpr TextOp& operator^= (TextOp& a, TextOp b) noexcept { a = (TextOp)(static_cast<T>(a) ^ static_cast<T>(b)); return a; }

	// formats string using bindings
	// bindings: %game.size.x%, %game.size.y%
	// output: "size: [1], [2]" -> "Size: 640, 480"
	bool getFormatString(const Game& game, const std::string_view format, const std::vector<std::string>& bindings, std::string& outStr);

	// formats string using bindings
	// bindings: %game.size.x%, %game.size.y%
	// output: "size: [1], [2]" -> "Size: 640, 480"
	std::string getFormatString(const Game& game, const std::string_view format, const std::vector<std::string>& bindings);

	// formats string using query
	// query: game
	// output: "size: %size.x%, %size.y%" -> "Size: 640, 480"
	std::string getTextQueryable(const Game& game, const std::string_view format, const std::string_view query);

	std::string getText(const Game& game, TextOp textOp, const std::string_view textOrformat, const std::vector<std::string>& bindings);

	void appendText(const Game& game, TextOp textOp, std::string& str,
		const std::string_view textOrformat, const std::vector<std::string>& bindings);
}
