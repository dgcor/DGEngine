#include "catch2/catch_test_macros.hpp"

#include "Game/Drawables/Circle.h"
#include "Game/Game.h"
#include "Game/Utils/TextUtils.h"

TEST_CASE("TextUtils::getFormatString", "TextUtils")
{
	Game game(true);
	std::string str;
	std::vector<std::string> bindings;

	SECTION("returns bool")
	{
		CHECK(TextUtils::getFormatString(game, "", bindings, str) == false);
		CHECK(TextUtils::getFormatString(game, "text", bindings, str) == false);
		CHECK(str == "");

		bindings = { "%%" };

		CHECK(TextUtils::getFormatString(game, "", bindings, str) == true);
		CHECK(TextUtils::getFormatString(game, "text", bindings, str) == false);
		CHECK(str == "text");

		CHECK(TextUtils::getFormatString(game, "Values: [1]", bindings, str) == false);
		CHECK(str == "Values: %%");

		bindings = { "game.keepAR" };

		CHECK(TextUtils::getFormatString(game, "Values: (1)", bindings, str) == false);
		CHECK(str == "Values: (1)");

		CHECK(TextUtils::getFormatString(game, "Values: [1]", bindings, str) == false);
		CHECK(str == "Values: game.keepAR");

		bindings = { "%game.keepAR%" };

		CHECK(TextUtils::getFormatString(game, "Values: [1]", bindings, str) == true);
		CHECK(str == "Values: true");

		CHECK(TextUtils::getFormatString(game, "Values: [1], [1]", bindings, str) == true);
		CHECK(str == "Values: true, true");

		CHECK(TextUtils::getFormatString(game, "Values: [1], [2]", bindings, str) == true);
		CHECK(str == "Values: true, [2]");

		bindings = { "%game.keepAR%", "%game.musicVolume%" };

		CHECK(TextUtils::getFormatString(game, "Values: [1]", bindings, str) == true);
		CHECK(str == "Values: true");

		CHECK(TextUtils::getFormatString(game, "Values: [1], [2]", bindings, str) == true);
		CHECK(str == "Values: true, 100");

		CHECK(TextUtils::getFormatString(game, "Values: [2], [1], [2]", bindings, str) == true);
		CHECK(str == "Values: 100, true, 100");

		bindings = { "%game.invalid%", "%game.musicVolume%" };

		CHECK(TextUtils::getFormatString(game, "Values: [1], [2]", bindings, str) == false);
		CHECK(str == "Values: %game.invalid%, 100");

		CHECK(TextUtils::getFormatString(game, "Values: [2]", bindings, str) == false);
		CHECK(str == "Values: 100");
	}
	SECTION("returns string")
	{
		bindings.clear();

		CHECK(TextUtils::getFormatString(game, "", bindings) == "");
		CHECK(TextUtils::getFormatString(game, "text", bindings) == "");

		bindings = { "%%" };

		CHECK(TextUtils::getFormatString(game, "", bindings) == "");
		CHECK(TextUtils::getFormatString(game, "text", bindings) == "text");
		CHECK(TextUtils::getFormatString(game, "Values: [1]", bindings) == "Values: %%");

		bindings = { "game.keepAR" };

		CHECK(TextUtils::getFormatString(game, "Values: (1)", bindings) == "Values: (1)");
		CHECK(TextUtils::getFormatString(game, "Values: [1]", bindings) == "Values: game.keepAR");

		bindings = { "%game.keepAR%" };

		CHECK(TextUtils::getFormatString(game, "Values: [1]", bindings) == "Values: true");
		CHECK(TextUtils::getFormatString(game, "Values: [1], [1]", bindings) == "Values: true, true");
		CHECK(TextUtils::getFormatString(game, "Values: [1], [2]", bindings) == "Values: true, [2]");

		bindings = { "%game.keepAR%", "%game.musicVolume%" };

		CHECK(TextUtils::getFormatString(game, "Values: [1]", bindings) == "Values: true");
		CHECK(TextUtils::getFormatString(game, "Values: [1], [2]", bindings) == "Values: true, 100");
		CHECK(TextUtils::getFormatString(game, "Values: [2], [1], [2]", bindings) == "Values: 100, true, 100");

		bindings = { "%game.invalid%", "%game.musicVolume%" };

		CHECK(TextUtils::getFormatString(game, "Values: [1], [2]", bindings) == "Values: %game.invalid%, 100");
		CHECK(TextUtils::getFormatString(game, "Values: [2]", bindings) == "Values: 100");
	}
}

TEST_CASE("TextUtils::getTextQueryable", "TextUtils")
{
	Game game;
	auto obj = std::make_shared<Circle>(2.f, 30);
	game.Resources().addDrawable("obj", obj, true);

	CHECK(TextUtils::getTextQueryable(game, "", "") == "");
	CHECK(TextUtils::getTextQueryable(game, "", "obj") == "");
	CHECK(TextUtils::getTextQueryable(game, "text", "") == "text");
	CHECK(TextUtils::getTextQueryable(game, "text", "obj") == "text");
	CHECK(TextUtils::getTextQueryable(game, "%text%", "obj") == "%text%");
	CHECK(TextUtils::getTextQueryable(game, "%radius%", "obj") == "2");
	CHECK(TextUtils::getTextQueryable(game, "%size.x%", "obj") == "4");
	CHECK(TextUtils::getTextQueryable(game, "%size.y%", "obj") == "4");
	CHECK(TextUtils::getTextQueryable(game, "The size is: [%size.x%, %size.y%]", "obj") == "The size is: [4, 4]");
	CHECK(TextUtils::getTextQueryable(game, "The size is: [%%size.x%, %size.y%]", "obj") == "The size is: [%4, 4]");
	CHECK(TextUtils::getTextQueryable(game, "The size is: [%%size.x%, %size.y%%]", "obj") == "The size is: [%4, 4%]");
	CHECK(TextUtils::getTextQueryable(game, "The size is: [%%size.x%%, %size.y%%]", "obj") == "The size is: [%4%, 4%]");
	CHECK(TextUtils::getTextQueryable(game, "The size is: [%%size.x%,% %size.y%%]", "obj") == "The size is: [%4,% 4%]");
}

TEST_CASE("TextUtils::getText", "TextUtils")
{
	using namespace TextUtils;

	Game game(true);
	auto obj = std::make_shared<Circle>(2.f, 30);
	game.Resources().addDrawable("obj", obj, true);
	std::vector<std::string> bindings;

	SECTION("set")
	{
		auto textOp = TextOp::Set;

		CHECK(getText(game, textOp, "", bindings) == "");
		CHECK(getText(game, textOp, "text", bindings) == "text");

		// bindings ignored
		bindings = { "game" };

		CHECK(getText(game, textOp, "Values: [1]", bindings) == "Values: [1]");
		CHECK(getText(game, textOp, "Values: %keepAR%", bindings) == "Values: %keepAR%");
	}
	SECTION("replace")
	{
		auto textOp = TextOp::Replace;

		bindings.clear();

		CHECK(getText(game, textOp, "", bindings) == "");
		CHECK(getText(game, textOp, "text", bindings) == "text");
		CHECK(getText(game, textOp, "Values: %game.keepAR%", bindings) == "Values: %game.keepAR%");
		CHECK(getText(game, textOp, "game.keepAR", bindings) == "game.keepAR");
		CHECK(getText(game, textOp, "%game.keepAR%", bindings) == "true");

		// bindings ignored
		bindings = { "game" };

		CHECK(getText(game, textOp, "Values: [1]", bindings) == "Values: [1]");
		CHECK(getText(game, textOp, "Values: %keepAR%", bindings) == "Values: %keepAR%");
		CHECK(getText(game, textOp, "Values: %game.keepAR%", bindings) == "Values: %game.keepAR%");
		CHECK(getText(game, textOp, "game.keepAR", bindings) == "game.keepAR");
		CHECK(getText(game, textOp, "%game.keepAR%", bindings) == "true");
	}
	SECTION("query")
	{
		auto textOp = TextOp::Query;

		bindings.clear();

		CHECK(getText(game, textOp, "", bindings) == "");

		bindings = { "obj" };

		CHECK(getText(game, textOp, "", bindings) == "");
		CHECK(getText(game, textOp, "", bindings) == "");
		CHECK(getText(game, textOp, "text", bindings) == "text");
		CHECK(getText(game, textOp, "text", bindings) == "text");
		CHECK(getText(game, textOp, "%text%", bindings) == "%text%");
		CHECK(getText(game, textOp, "%radius%", bindings) == "2");
		CHECK(getText(game, textOp, "%size.x%", bindings) == "4");
		CHECK(getText(game, textOp, "%size.y%", bindings) == "4");
		CHECK(getText(game, textOp, "The size is: [%size.x%, %size.y%]", bindings) == "The size is: [4, 4]");
		CHECK(getText(game, textOp, "The size is: [%%size.x%, %size.y%]", bindings) == "The size is: [%4, 4]");
		CHECK(getText(game, textOp, "The size is: [%%size.x%, %size.y%%]", bindings) == "The size is: [%4, 4%]");
		CHECK(getText(game, textOp, "The size is: [%%size.x%%, %size.y%%]", bindings) == "The size is: [%4%, 4%]");
		CHECK(getText(game, textOp, "The size is: [%%size.x%,% %size.y%%]", bindings) == "The size is: [%4,% 4%]");
	}
	SECTION("formatString")
	{
		auto textOp = TextOp::FormatString;

		bindings.clear();

		CHECK(getText(game, textOp, "", bindings) == "");
		CHECK(getText(game, textOp, "text", bindings) == "");

		bindings = { "%%" };

		CHECK(getText(game, textOp, "", bindings) == "");
		CHECK(getText(game, textOp, "text", bindings) == "text");
		CHECK(getText(game, textOp, "Values: [1]", bindings) == "Values: %%");

		bindings = { "game.keepAR" };

		CHECK(getText(game, textOp, "Values: (1)", bindings) == "Values: (1)");
		CHECK(getText(game, textOp, "Values: [1]", bindings) == "Values: game.keepAR");

		bindings = { "%game.keepAR%" };

		CHECK(getText(game, textOp, "Values: [1]", bindings) == "Values: true");
		CHECK(getText(game, textOp, "Values: [1], [1]", bindings) == "Values: true, true");
		CHECK(getText(game, textOp, "Values: [1], [2]", bindings) == "Values: true, [2]");

		bindings = { "%game.keepAR%", "%game.musicVolume%" };

		CHECK(getText(game, textOp, "Values: [1]", bindings) == "Values: true");
		CHECK(getText(game, textOp, "Values: [1], [2]", bindings) == "Values: true, 100");
		CHECK(getText(game, textOp, "Values: [2], [1], [2]", bindings) == "Values: 100, true, 100");

		bindings = { "%game.invalid%", "%game.musicVolume%" };

		CHECK(getText(game, textOp, "Values: [1], [2]", bindings) == "Values: %game.invalid%, 100");
		CHECK(getText(game, textOp, "Values: [2]", bindings) == "Values: 100");
	}
	SECTION("replaceAll")
	{
		auto textOp = TextOp::ReplaceAll;

		// bindings ignored
		bindings.clear();

		CHECK(getText(game, textOp, "", bindings) == "");
		CHECK(getText(game, textOp, "text", bindings) == "text");
		CHECK(getText(game, textOp, "%text%", bindings) == "%text%");
		CHECK(getText(game, textOp, "%game.gamma%", bindings) == "0");
		CHECK(getText(game, textOp, "%game.keepAR%", bindings) == "true");
		CHECK(getText(game, textOp, "%game.size.x%", bindings) == "640");
		CHECK(getText(game, textOp, "%game.size.y%", bindings) == "480");
		CHECK(getText(game, textOp, "The size is: [%game.size.x%, %game.size.y%]", bindings) == "The size is: [640, 480]");
		CHECK(getText(game, textOp, "The size is: [%%game.size.x%, %game.size.y%]", bindings) == "The size is: [%640, 480]");
		CHECK(getText(game, textOp, "The size is: [%%game.size.x%, %game.size.y%%]", bindings) == "The size is: [%640, 480%]");
		CHECK(getText(game, textOp, "The size is: [%%game.size.x%%, %game.size.y%%]", bindings) == "The size is: [%640%, 480%]");
		CHECK(getText(game, textOp, "The size is: [%%game.size.x%,% %game.size.y%%]", bindings) == "The size is: [%640,% 480%]");
		CHECK(getText(game, textOp, "keepAR: %game.keepAR%, radius: %obj.radius.y%", bindings) == "keepAR: true, radius: 2");
	}
	SECTION("trim")
	{
		auto textOp = TextOp::Set | TextOp::Trim;

		bindings.clear();

		CHECK(getText(game, textOp, "", bindings) == "");
		CHECK(getText(game, textOp, " ", bindings) == "");
		CHECK(getText(game, textOp, "   ", bindings) == "");
		CHECK(getText(game, textOp, "\t", bindings) == "");
		CHECK(getText(game, textOp, "\t\n", bindings) == "");
		CHECK(getText(game, textOp, " \t\r\n", bindings) == "");
		CHECK(getText(game, textOp, " \ttext\n   ", bindings) == "text");
		CHECK(getText(game, textOp, "text", bindings) == "text");

		textOp = TextOp::Replace | TextOp::Trim;

		CHECK(getText(game, textOp, "%game.keepAR%", bindings) == "true");
		CHECK(getText(game, textOp, " %game.keepAR% ", bindings) == "%game.keepAR%");

		textOp = TextOp::Query | TextOp::Trim;
		bindings = { "obj" };

		CHECK(getText(game, textOp, "%radius%", bindings) == "2");
		CHECK(getText(game, textOp, " %radius% ", bindings) == "2");
		CHECK(getText(game, textOp, " \r\n\t%radius% \t\n\r\n\n", bindings) == "2");

		textOp = TextOp::FormatString | TextOp::Trim;
		bindings = { "%game.keepAR%" };

		CHECK(getText(game, textOp, "Values: [1]", bindings) == "Values: true");
		CHECK(getText(game, textOp, " Values: [1] ", bindings) == "Values: true");
		CHECK(getText(game, textOp, " \r\n\tValues: [1] \t\n\r\n\n", bindings) == "Values: true");

		textOp = TextOp::ReplaceAll | TextOp::Trim;
		bindings.clear();

		CHECK(getText(game, textOp, "%game.gamma%", bindings) == "0");
		CHECK(getText(game, textOp, " %game.gamma% ", bindings) == "0");
		CHECK(getText(game, textOp, " \r\n\t%game.gamma% \t\n\r\n\n", bindings) == "0");
	}
	SECTION("removeEmptyLines")
	{
		auto textOp = TextOp::Set | TextOp::RemoveEmptyLines;

		bindings.clear();

		CHECK(getText(game, textOp, "", bindings) == "");
		CHECK(getText(game, textOp, "\n", bindings) == "");
		CHECK(getText(game, textOp, "\t", bindings) == "\t");
		CHECK(getText(game, textOp, "\n\n", bindings) == "");
		CHECK(getText(game, textOp, "\nt\n", bindings) == "t");
		CHECK(getText(game, textOp, "\nt\nest", bindings) == "t\nest");
		CHECK(getText(game, textOp, " \ntext\n   ", bindings) == " \ntext\n   ");
		CHECK(getText(game, textOp, "\nthis\nis\n\na\nstring\n", bindings) == "this\nis\na\nstring");

		textOp = TextOp::Replace | TextOp::RemoveEmptyLines;

		CHECK(getText(game, textOp, "%game.keepAR%", bindings) == "true");
		CHECK(getText(game, textOp, "\n%game.keepAR%\n", bindings) == "%game.keepAR%");

		textOp = TextOp::Query | TextOp::RemoveEmptyLines;
		bindings = { "obj" };

		CHECK(getText(game, textOp, "%radius%", bindings) == "2");
		CHECK(getText(game, textOp, "\n%radius%\n", bindings) == "2");

		textOp = TextOp::FormatString | TextOp::RemoveEmptyLines;
		bindings = { "%game.keepAR%" };

		CHECK(getText(game, textOp, "Values: [1]", bindings) == "Values: true");
		CHECK(getText(game, textOp, "\nValues: [1]\n", bindings) == "Values: true");

		textOp = TextOp::ReplaceAll | TextOp::RemoveEmptyLines;
		bindings.clear();

		CHECK(getText(game, textOp, "%game.gamma%", bindings) == "0");
		CHECK(getText(game, textOp, "\n%game.gamma%\n", bindings) == "0");
	}
	SECTION("trim + removeEmptyLines")
	{
		auto textOp = TextOp::Set | TextOp::Trim | TextOp::RemoveEmptyLines;

		bindings.clear();

		CHECK(getText(game, textOp, " \r\n\tthis\nis\n\na\nstring \r\n\t", bindings) == "this\nis\na\nstring");

		textOp = TextOp::Replace | TextOp::Trim | TextOp::RemoveEmptyLines;

		CHECK(getText(game, textOp, " \r\n\t%game.keepAR% \r\n\t", bindings) == "%game.keepAR%");

		textOp = TextOp::Query | TextOp::Trim | TextOp::RemoveEmptyLines;
		bindings = { "obj" };

		CHECK(getText(game, textOp, " \r\n\t%radius% \r\n\t", bindings) == "2");

		textOp = TextOp::FormatString | TextOp::Trim | TextOp::RemoveEmptyLines;
		bindings = { "%game.keepAR%" };

		CHECK(getText(game, textOp, " \r\n\tValues: \n\n[1] \r\n\t", bindings) == "Values: \ntrue");

		textOp = TextOp::ReplaceAll | TextOp::Trim | TextOp::RemoveEmptyLines;
		bindings.clear();

		CHECK(getText(game, textOp, " \r\n\t%game.gamma% \r\n\t", bindings) == "0");
	}
	SECTION("all")
	{
		// uses FormatString
		auto textOp = TextOp::Set | TextOp::Replace | TextOp::Query | TextOp::FormatString |
			TextOp::ReplaceAll | TextOp::Trim | TextOp::RemoveEmptyLines;

		bindings.clear();

		CHECK(getText(game, textOp, " \r\n\tthis\nis\n\na\nstring \r\n\t", bindings) == "this\nis\na\nstring");
		CHECK(getText(game, textOp, " \r\n\t%game.keepAR% \r\n\t", bindings) == "true");

		bindings = { "obj" };

		CHECK(getText(game, textOp, " \r\n\t%radius% \r\n\t", bindings) == "%radius%");

		bindings = { "%game.keepAR%" };

		CHECK(getText(game, textOp, " \r\n\tValues: \n\n[1] \r\n\t", bindings) == "Values: \n[1]");

		bindings.clear();

		CHECK(getText(game, textOp, " \r\n\t%game.gamma% \r\n\t", bindings) == "0");
	}
}

TEST_CASE("TextUtils::appendText", "TextUtils")
{
	// same as getText
	using namespace TextUtils;

	Game game;
	std::vector<std::string> bindings;
	std::string str;

	auto textOp = TextOp::Set;

	appendText(game, textOp, str, "", bindings);
	CHECK(str == "");
	appendText(game, textOp, str, " \r\n\tgamma:", bindings);
	CHECK(str == " \r\n\tgamma:");

	textOp = TextOp::ReplaceAll | TextOp::Trim;

	appendText(game, textOp, str, " \t%game.gamma%\t", bindings);
	CHECK(str == "gamma: \t0");

	textOp = TextOp::Replace;
	bindings = { "game" };

	appendText(game, textOp, str, "%game.keepAR%", bindings);
	CHECK(str == "gamma: \t0false");
}
