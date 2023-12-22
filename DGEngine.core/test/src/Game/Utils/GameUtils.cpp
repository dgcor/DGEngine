#include "catch2/catch_test_macros.hpp"

#include "Game/Drawables/Circle.h"
#include "Game/Game.h"
#include "Game/Utils/GameUtils.h"
#include "Game/Utils/VarUtils.h"

TEST_CASE("GameUtils::FPSToTime", "GameUtils")
{
	CHECK(GameUtils::FPSToTime(-1) == sf::milliseconds(1000));
	CHECK(GameUtils::FPSToTime(0) == sf::milliseconds(1000));
	CHECK(GameUtils::FPSToTime(1) == sf::milliseconds(1000));
	CHECK(GameUtils::FPSToTime(1000) == sf::milliseconds(1));
	CHECK(GameUtils::FPSToTime(5000) == sf::milliseconds(1));
}

TEST_CASE("GameUtils::getAnimationType", "GameUtils")
{
	CHECK(GameUtils::getAnimationType("looped", AnimationType::PlayOnce) == AnimationType::Looped);
	CHECK(GameUtils::getAnimationType("playOnce", AnimationType::Looped) == AnimationType::PlayOnce);
	CHECK(GameUtils::getAnimationType("backAndForth", AnimationType::Looped) == AnimationType::BackAndForth);
	CHECK(GameUtils::getAnimationType("LOOPED", AnimationType::PlayOnce) == AnimationType::Looped);
	CHECK(GameUtils::getAnimationType("looped ", AnimationType::PlayOnce) == AnimationType::PlayOnce);
	CHECK(GameUtils::getAnimationType("invalid ", AnimationType::PlayOnce) == AnimationType::PlayOnce);
	CHECK(GameUtils::getAnimationType("", AnimationType::PlayOnce) == AnimationType::PlayOnce);
}

TEST_CASE("GameUtils::getAlignmentPosition", "GameUtils")
{
	sf::Vector2f size(10.f, 10.f);

	CHECK(GameUtils::getAlignmentPosition(sf::Vector2f(20.f, 20.f), size, HorizontalAlign::Left, VerticalAlign::Top) == sf::Vector2f(20.f, 10.f));
	CHECK(GameUtils::getAlignmentPosition(sf::Vector2f(20.f, 20.f), size, HorizontalAlign::Center, VerticalAlign::Center) == sf::Vector2f(15.f, 15.f));
	CHECK(GameUtils::getAlignmentPosition(sf::Vector2f(20.f, 20.f), size, HorizontalAlign::Right, VerticalAlign::Bottom) == sf::Vector2f(10.f, 20.f));

	size = sf::Vector2f(10.f, 10.f);

	CHECK(GameUtils::getAlignmentPosition(sf::Vector2f(3.f, 0.f), size, HorizontalAlign::Right, VerticalAlign::Center) == sf::Vector2f(-7.f, -5.f));
	CHECK(GameUtils::getAlignmentPosition(sf::Vector2f(3.f, 0.f), size, HorizontalAlign::Left, VerticalAlign::Top) == sf::Vector2f(3.f, -10.f));
	CHECK(GameUtils::getAlignmentPosition(sf::Vector2f(3.f, 0.f), size, HorizontalAlign::Center, VerticalAlign::Bottom) == sf::Vector2f(-2.f, 0.f));
}

TEST_CASE("GameUtils::getAnchorPosition", "GameUtils")
{
	sf::Vector2f position(20.f, 20.f);
	sf::Vector2f size(10.f, 10.f);
	sf::Vector2f anchorToSize(55.f, 55.f);

	SECTION("valid")
	{
		CHECK(GameUtils::getAnchorPosition(Anchor::Bottom | Anchor::Left,	size, position, anchorToSize) == sf::Vector2f(10.f, 75.f));
		CHECK(GameUtils::getAnchorPosition(Anchor::Bottom,					size, position, anchorToSize) == sf::Vector2f(43.f, 75.f));
		CHECK(GameUtils::getAnchorPosition(Anchor::Bottom | Anchor::Right,	size, position, anchorToSize) == sf::Vector2f(75.f, 75.f));
		CHECK(GameUtils::getAnchorPosition(Anchor::Left,					size, position, anchorToSize) == sf::Vector2f(10.f, 43.f));
		CHECK(GameUtils::getAnchorPosition(Anchor::None,					size, position, anchorToSize) == sf::Vector2f(43.f, 43.f));
		CHECK(GameUtils::getAnchorPosition(Anchor::Right,					size, position, anchorToSize) == sf::Vector2f(75.f, 43.f));
		CHECK(GameUtils::getAnchorPosition(Anchor::Top | Anchor::Left,		size, position, anchorToSize) == sf::Vector2f(10.f, 10.f));
		CHECK(GameUtils::getAnchorPosition(Anchor::Top,						size, position, anchorToSize) == sf::Vector2f(43.f, 10.f));
		CHECK(GameUtils::getAnchorPosition(Anchor::Top | Anchor::Right,		size, position, anchorToSize) == sf::Vector2f(75.f, 10.f));
	}
	SECTION("invalid")
	{
		CHECK(GameUtils::getAnchorPosition(Anchor::All,						size, position, anchorToSize) == position);
		CHECK(GameUtils::getAnchorPosition(Anchor::Left | Anchor::Right,	size, position, anchorToSize) == position);
		CHECK(GameUtils::getAnchorPosition(Anchor::Top | Anchor::Bottom,	size, position, anchorToSize) == position);
	}
}

TEST_CASE("GameUtils::setAnchorPosSize", "GameUtils")
{
	sf::Vector2u oldWSize(320, 240);
	sf::Vector2u newWSize(640, 480);

	SECTION("none")
	{
		Anchor anchor = Anchor::None;
		sf::Vector2f pos(20.f, 20.f);
		sf::Vector2f size(10.f, 10.f);

		GameUtils::setAnchorPosSize(anchor, pos, size, oldWSize, newWSize);
		CHECK(pos == sf::Vector2f(180.f, 140.f));
		CHECK(size == sf::Vector2f(10.f, 10.f));
	}
	SECTION("top")
	{
		Anchor anchor = Anchor::Top;
		sf::Vector2f pos(20.f, 20.f);
		sf::Vector2f size(10.f, 10.f);

		GameUtils::setAnchorPosSize(anchor, pos, size, oldWSize, newWSize);
		CHECK(pos == sf::Vector2f(180.f, 20.f));
		CHECK(size == sf::Vector2f(10.f, 10.f));
	}
	SECTION("bottom")
	{
		Anchor anchor = Anchor::Bottom;
		sf::Vector2f pos(20.f, 20.f);
		sf::Vector2f size(10.f, 10.f);

		GameUtils::setAnchorPosSize(anchor, pos, size, oldWSize, newWSize);
		CHECK(pos == sf::Vector2f(180.f, 260.f));
		CHECK(size == sf::Vector2f(10.f, 10.f));
	}
	SECTION("left")
	{
		Anchor anchor = Anchor::Left;
		sf::Vector2f pos(20.f, 20.f);
		sf::Vector2f size(10.f, 10.f);

		GameUtils::setAnchorPosSize(anchor, pos, size, oldWSize, newWSize);
		CHECK(pos == sf::Vector2f(20.f, 140.f));
		CHECK(size == sf::Vector2f(10.f, 10.f));
	}
	SECTION("right")
	{
		Anchor anchor = Anchor::Right;
		sf::Vector2f pos(20.f, 20.f);
		sf::Vector2f size(10.f, 10.f);

		GameUtils::setAnchorPosSize(anchor, pos, size, oldWSize, newWSize);
		CHECK(pos == sf::Vector2f(340.f, 140.f));
		CHECK(size == sf::Vector2f(10.f, 10.f));
	}
	SECTION("topLeft")
	{
		Anchor anchor = Anchor::Top | Anchor::Left;
		sf::Vector2f pos(20.f, 20.f);
		sf::Vector2f size(10.f, 10.f);

		GameUtils::setAnchorPosSize(anchor, pos, size, oldWSize, newWSize);
		CHECK(pos == sf::Vector2f(20.f, 20.f));
		CHECK(size == sf::Vector2f(10.f, 10.f));
	}
	SECTION("topRight")
	{
		Anchor anchor = Anchor::Top | Anchor::Right;
		sf::Vector2f pos(20.f, 20.f);
		sf::Vector2f size(10.f, 10.f);

		GameUtils::setAnchorPosSize(anchor, pos, size, oldWSize, newWSize);
		CHECK(pos == sf::Vector2f(340.f, 20.f));
		CHECK(size == sf::Vector2f(10.f, 10.f));
	}
	SECTION("topLeftRight")
	{
		Anchor anchor = Anchor::Top | Anchor::Left | Anchor::Right;
		sf::Vector2f pos(20.f, 20.f);
		sf::Vector2f size(10.f, 10.f);

		GameUtils::setAnchorPosSize(anchor, pos, size, oldWSize, newWSize);
		CHECK(pos == sf::Vector2f(20.f, 20.f));
		CHECK(size == sf::Vector2f(330.f, 10.f));
	}
	SECTION("bottomLeft")
	{
		Anchor anchor = Anchor::Bottom | Anchor::Left;
		sf::Vector2f pos(20.f, 20.f);
		sf::Vector2f size(10.f, 10.f);

		GameUtils::setAnchorPosSize(anchor, pos, size, oldWSize, newWSize);
		CHECK(pos == sf::Vector2f(20.f, 260.f));
		CHECK(size == sf::Vector2f(10.f, 10.f));
	}
	SECTION("bottomRight")
	{
		Anchor anchor = Anchor::Bottom | Anchor::Right;
		sf::Vector2f pos(20.f, 20.f);
		sf::Vector2f size(10.f, 10.f);

		GameUtils::setAnchorPosSize(anchor, pos, size, oldWSize, newWSize);
		CHECK(pos == sf::Vector2f(340.f, 260.f));
		CHECK(size == sf::Vector2f(10.f, 10.f));
	}
	SECTION("bottomLeftRight")
	{
		Anchor anchor = Anchor::Bottom | Anchor::Left | Anchor::Right;
		sf::Vector2f pos(20.f, 20.f);
		sf::Vector2f size(10.f, 10.f);

		GameUtils::setAnchorPosSize(anchor, pos, size, oldWSize, newWSize);
		CHECK(pos == sf::Vector2f(20.f, 260.f));
		CHECK(size == sf::Vector2f(330.f, 10.f));
	}
	SECTION("all")
	{
		Anchor anchor = Anchor::All;
		sf::Vector2f pos(20.f, 20.f);
		sf::Vector2f size(10.f, 10.f);

		GameUtils::setAnchorPosSize(anchor, pos, size, oldWSize, newWSize);
		CHECK(pos == sf::Vector2f(20.f, 20.f));
		CHECK(size == sf::Vector2f(330.f, 250.f));
	}
}

TEST_CASE("GameUtils::getAnchor", "GameUtils")
{
	CHECK(GameUtils::getAnchor("none", Anchor::All) == Anchor::None);
	CHECK(GameUtils::getAnchor("top", Anchor::All) == Anchor::Top);
	CHECK(GameUtils::getAnchor("topleft", Anchor::All) == (Anchor::Top | Anchor::Left));
	CHECK(GameUtils::getAnchor("topright", Anchor::All) == (Anchor::Top | Anchor::Right));
	CHECK(GameUtils::getAnchor("bottom", Anchor::All) == Anchor::Bottom);
	CHECK(GameUtils::getAnchor("bottomleft", Anchor::All) == (Anchor::Bottom | Anchor::Left));
	CHECK(GameUtils::getAnchor("bottomright", Anchor::All) == (Anchor::Bottom | Anchor::Right));
	CHECK(GameUtils::getAnchor("left", Anchor::All) == Anchor::Left);
	CHECK(GameUtils::getAnchor("right", Anchor::All) == Anchor::Right);
	CHECK(GameUtils::getAnchor("all", Anchor::None) == Anchor::All);
	CHECK(GameUtils::getAnchor("NONE", Anchor::All) == Anchor::None);
	CHECK(GameUtils::getAnchor("none ", Anchor::All) == Anchor::All);
	CHECK(GameUtils::getAnchor("invalid ", Anchor::All) == Anchor::All);
	CHECK(GameUtils::getAnchor("", Anchor::All) == Anchor::All);
}

TEST_CASE("GameUtils::getBindingFlags", "GameUtils")
{
	CHECK(GameUtils::getBindingFlags("onchange", BindingFlags::WhenHidden) == BindingFlags::OnChange);
	CHECK(GameUtils::getBindingFlags("once", BindingFlags::OnChange) == BindingFlags::Once);
	CHECK(GameUtils::getBindingFlags("always", BindingFlags::OnChange) == BindingFlags::Always);
	CHECK(GameUtils::getBindingFlags("whenhidden", BindingFlags::OnChange) == BindingFlags::WhenHidden);
	CHECK(GameUtils::getBindingFlags("ONCHANGE", BindingFlags::WhenHidden) == BindingFlags::OnChange);
	CHECK(GameUtils::getBindingFlags("onchange ", BindingFlags::WhenHidden) == BindingFlags::WhenHidden);
	CHECK(GameUtils::getBindingFlags("invalid ", BindingFlags::WhenHidden) == BindingFlags::WhenHidden);
	CHECK(GameUtils::getBindingFlags("", BindingFlags::WhenHidden) == BindingFlags::WhenHidden);
}

TEST_CASE("GameUtils::getBlendMode", "GameUtils")
{
	CHECK(GameUtils::getBlendMode("alpha", BlendMode::None) == BlendMode::Alpha);
	CHECK(GameUtils::getBlendMode("add", BlendMode::Alpha) == BlendMode::Add);
	CHECK(GameUtils::getBlendMode("multiply", BlendMode::Alpha) == BlendMode::Multiply);
	CHECK(GameUtils::getBlendMode("min", BlendMode::Alpha) == BlendMode::Min);
	CHECK(GameUtils::getBlendMode("max", BlendMode::Alpha) == BlendMode::Max);
	CHECK(GameUtils::getBlendMode("none", BlendMode::Alpha) == BlendMode::None);
	CHECK(GameUtils::getBlendMode("ALPHA", BlendMode::None) == BlendMode::Alpha);
	CHECK(GameUtils::getBlendMode("alpha ", BlendMode::None) == BlendMode::None);
	CHECK(GameUtils::getBlendMode("invalid ", BlendMode::None) == BlendMode::None);
	CHECK(GameUtils::getBlendMode("", BlendMode::None) == BlendMode::None);
}

TEST_CASE("GameUtils::getColorFormat", "GameUtils")
{
	CHECK(GameUtils::getColorFormat("rgb") == Palette::ColorFormat::RGB);
	CHECK(GameUtils::getColorFormat("bgr") == Palette::ColorFormat::BGR);
	CHECK(GameUtils::getColorFormat("rgba") == Palette::ColorFormat::RGBA);
	CHECK(GameUtils::getColorFormat("bgra") == Palette::ColorFormat::BGRA);
	CHECK(GameUtils::getColorFormat("argb") == Palette::ColorFormat::ARGB);
	CHECK(GameUtils::getColorFormat("abgr") == Palette::ColorFormat::ABGR);
	CHECK(GameUtils::getColorFormat("BGR") == Palette::ColorFormat::BGR);
	CHECK(GameUtils::getColorFormat("bgr ") == Palette::ColorFormat::RGB);
	CHECK(GameUtils::getColorFormat("invalid ") == Palette::ColorFormat::RGB);
	CHECK(GameUtils::getColorFormat("") == Palette::ColorFormat::RGB);
}

TEST_CASE("GameUtils::getDirection", "GameUtils")
{
	CHECK(GameUtils::getDirection("up", Direction::Right) == Direction::Up);
	CHECK(GameUtils::getDirection("down", Direction::Up) == Direction::Down);
	CHECK(GameUtils::getDirection("left", Direction::Up) == Direction::Left);
	CHECK(GameUtils::getDirection("right", Direction::Up) == Direction::Right);
	CHECK(GameUtils::getDirection("UP", Direction::Right) == Direction::Up);
	CHECK(GameUtils::getDirection("up ", Direction::Right) == Direction::Right);
	CHECK(GameUtils::getDirection("invalid ", Direction::Right) == Direction::Right);
	CHECK(GameUtils::getDirection("", Direction::Right) == Direction::Right);
}

TEST_CASE("GameUtils::getHorizontalAlignment", "GameUtils")
{
	CHECK(GameUtils::getHorizontalAlignment("left", HorizontalAlign::Right) == HorizontalAlign::Left);
	CHECK(GameUtils::getHorizontalAlignment("center", HorizontalAlign::Left) == HorizontalAlign::Center);
	CHECK(GameUtils::getHorizontalAlignment("right", HorizontalAlign::Left) == HorizontalAlign::Right);
	CHECK(GameUtils::getHorizontalAlignment("LEFT", HorizontalAlign::Right) == HorizontalAlign::Left);
	CHECK(GameUtils::getHorizontalAlignment("left ", HorizontalAlign::Right) == HorizontalAlign::Right);
	CHECK(GameUtils::getHorizontalAlignment("invalid ", HorizontalAlign::Right) == HorizontalAlign::Right);
	CHECK(GameUtils::getHorizontalAlignment("", HorizontalAlign::Right) == HorizontalAlign::Right);
}

TEST_CASE("GameUtils::getVerticalAlignment", "GameUtils")
{
	CHECK(GameUtils::getVerticalAlignment("top", VerticalAlign::Bottom) == VerticalAlign::Top);
	CHECK(GameUtils::getVerticalAlignment("center", VerticalAlign::Top) == VerticalAlign::Center);
	CHECK(GameUtils::getVerticalAlignment("bottom", VerticalAlign::Top) == VerticalAlign::Bottom);
	CHECK(GameUtils::getVerticalAlignment("TOP", VerticalAlign::Bottom) == VerticalAlign::Top);
	CHECK(GameUtils::getVerticalAlignment("top ", VerticalAlign::Bottom) == VerticalAlign::Bottom);
	CHECK(GameUtils::getVerticalAlignment("invalid ", VerticalAlign::Bottom) == VerticalAlign::Bottom);
	CHECK(GameUtils::getVerticalAlignment("", VerticalAlign::Bottom) == VerticalAlign::Bottom);
}

TEST_CASE("GameUtils::getIgnoreResource", "GameUtils")
{
	CHECK(GameUtils::getIgnoreResource("none", IgnoreResource::All) == IgnoreResource::None);
	CHECK(GameUtils::getIgnoreResource("alldraws", IgnoreResource::None) == (IgnoreResource::Draw | IgnoreResource::All));
	CHECK(GameUtils::getIgnoreResource("alldrawsandupdates", IgnoreResource::None) == (IgnoreResource::Draw | IgnoreResource::Update | IgnoreResource::All));
	CHECK(GameUtils::getIgnoreResource("allupdates", IgnoreResource::None) == (IgnoreResource::Update | IgnoreResource::All));
	CHECK(GameUtils::getIgnoreResource("draw", IgnoreResource::None) == IgnoreResource::Draw);
	CHECK(GameUtils::getIgnoreResource("drawandupdate", IgnoreResource::None) == (IgnoreResource::Draw | IgnoreResource::Update));
	CHECK(GameUtils::getIgnoreResource("update", IgnoreResource::None) == IgnoreResource::Update);
	CHECK(GameUtils::getIgnoreResource("NONE", IgnoreResource::All) == IgnoreResource::None);
	CHECK(GameUtils::getIgnoreResource("none ", IgnoreResource::All) == IgnoreResource::All);
	CHECK(GameUtils::getIgnoreResource("invalid ", IgnoreResource::All) == IgnoreResource::All);
	CHECK(GameUtils::getIgnoreResource("", IgnoreResource::All) == IgnoreResource::All);
}

TEST_CASE("GameUtils::getInputEventType", "GameUtils")
{
	CHECK(GameUtils::getInputEventType("none", InputEventType::All) == InputEventType::None);
	CHECK(GameUtils::getInputEventType("leftclick", InputEventType::None) == InputEventType::LeftClick);
	CHECK(GameUtils::getInputEventType("middleclick", InputEventType::None) == InputEventType::MiddleClick);

	CHECK(GameUtils::getInputEventType("rightclick", InputEventType::None) == InputEventType::RightClick);
	CHECK(GameUtils::getInputEventType("mousepress", InputEventType::None) == InputEventType::MousePress);
	CHECK(GameUtils::getInputEventType("mousemove", InputEventType::None) == InputEventType::MouseMove);
	CHECK(GameUtils::getInputEventType("mouserelease", InputEventType::None) == InputEventType::MouseRelease);
	CHECK(GameUtils::getInputEventType("mousescroll", InputEventType::None) == InputEventType::MouseScroll);
	CHECK(GameUtils::getInputEventType("keypress", InputEventType::None) == InputEventType::KeyPress);
	CHECK(GameUtils::getInputEventType("textenter", InputEventType::None) == InputEventType::TextEnter);
	CHECK(GameUtils::getInputEventType("touchbegin", InputEventType::None) == InputEventType::TouchBegin);
	CHECK(GameUtils::getInputEventType("touchmove", InputEventType::None) == InputEventType::TouchMove);
	CHECK(GameUtils::getInputEventType("touchend", InputEventType::None) == InputEventType::TouchEnd);

	CHECK(GameUtils::getInputEventType("allleftclick", InputEventType::None) == (InputEventType::All ^ (InputEventType::MiddleClick | InputEventType::RightClick)));
	CHECK(GameUtils::getInputEventType("allmiddleclick", InputEventType::None) == (InputEventType::All ^ (InputEventType::LeftClick | InputEventType::RightClick)));
	CHECK(GameUtils::getInputEventType("allrightclick", InputEventType::None) == (InputEventType::All ^ (InputEventType::LeftClick | InputEventType::MiddleClick)));
	CHECK(GameUtils::getInputEventType("all", InputEventType::None) == InputEventType::All);

	CHECK(GameUtils::getInputEventType("NONE", InputEventType::All) == InputEventType::None);
	CHECK(GameUtils::getInputEventType("none ", InputEventType::All) == InputEventType::All);
	CHECK(GameUtils::getInputEventType("invalid ", InputEventType::All) == InputEventType::All);
	CHECK(GameUtils::getInputEventType("", InputEventType::All) == InputEventType::All);
}

TEST_CASE("GameUtils::getPrimitiveType", "GameUtils")
{
	CHECK(GameUtils::getPrimitiveType("points", sf::PrimitiveType::Lines) == sf::PrimitiveType::Points);
	CHECK(GameUtils::getPrimitiveType("lines", sf::PrimitiveType::Points) == sf::PrimitiveType::Lines);
	CHECK(GameUtils::getPrimitiveType("linestrip", sf::PrimitiveType::Points) == sf::PrimitiveType::LineStrip);
	CHECK(GameUtils::getPrimitiveType("triangles", sf::PrimitiveType::Points) == sf::PrimitiveType::Triangles);
	CHECK(GameUtils::getPrimitiveType("trianglestrip", sf::PrimitiveType::Points) == sf::PrimitiveType::TriangleStrip);
	CHECK(GameUtils::getPrimitiveType("trianglefan", sf::PrimitiveType::Points) == sf::PrimitiveType::TriangleFan);
	CHECK(GameUtils::getPrimitiveType("POINTS", sf::PrimitiveType::Lines) == sf::PrimitiveType::Points);
	CHECK(GameUtils::getPrimitiveType("points ", sf::PrimitiveType::Lines) == sf::PrimitiveType::Lines);
	CHECK(GameUtils::getPrimitiveType("invalid ", sf::PrimitiveType::Lines) == sf::PrimitiveType::Lines);
	CHECK(GameUtils::getPrimitiveType("", sf::PrimitiveType::Lines) == sf::PrimitiveType::Lines);
}

TEST_CASE("GameUtils::getKeyCode", "GameUtils")
{
	SECTION("int")
	{
		CHECK(GameUtils::getKeyCode(0) == sf::Keyboard::Key::Num0);
		CHECK(GameUtils::getKeyCode(1) == sf::Keyboard::Key::Num1);
		CHECK(GameUtils::getKeyCode(2) == sf::Keyboard::Key::Num2);
		CHECK(GameUtils::getKeyCode(3) == sf::Keyboard::Key::Num3);
		CHECK(GameUtils::getKeyCode(4) == sf::Keyboard::Key::Num4);
		CHECK(GameUtils::getKeyCode(5) == sf::Keyboard::Key::Num5);
		CHECK(GameUtils::getKeyCode(6) == sf::Keyboard::Key::Num6);
		CHECK(GameUtils::getKeyCode(7) == sf::Keyboard::Key::Num7);
		CHECK(GameUtils::getKeyCode(8) == sf::Keyboard::Key::Num8);
		CHECK(GameUtils::getKeyCode(9) == sf::Keyboard::Key::Num9);
		CHECK(GameUtils::getKeyCode(10, sf::Keyboard::Key::A) == sf::Keyboard::Key::A);
	}
	SECTION("string")
	{
		CHECK(GameUtils::getKeyCode("A") == sf::Keyboard::Key::A);
		CHECK(GameUtils::getKeyCode("B") == sf::Keyboard::Key::B);
		CHECK(GameUtils::getKeyCode("C") == sf::Keyboard::Key::C);
		CHECK(GameUtils::getKeyCode("D") == sf::Keyboard::Key::D);
		CHECK(GameUtils::getKeyCode("E") == sf::Keyboard::Key::E);
		CHECK(GameUtils::getKeyCode("F") == sf::Keyboard::Key::F);
		CHECK(GameUtils::getKeyCode("G") == sf::Keyboard::Key::G);
		CHECK(GameUtils::getKeyCode("H") == sf::Keyboard::Key::H);
		CHECK(GameUtils::getKeyCode("I") == sf::Keyboard::Key::I);
		CHECK(GameUtils::getKeyCode("J") == sf::Keyboard::Key::J);
		CHECK(GameUtils::getKeyCode("K") == sf::Keyboard::Key::K);
		CHECK(GameUtils::getKeyCode("L") == sf::Keyboard::Key::L);
		CHECK(GameUtils::getKeyCode("M") == sf::Keyboard::Key::M);
		CHECK(GameUtils::getKeyCode("N") == sf::Keyboard::Key::N);
		CHECK(GameUtils::getKeyCode("O") == sf::Keyboard::Key::O);
		CHECK(GameUtils::getKeyCode("P") == sf::Keyboard::Key::P);
		CHECK(GameUtils::getKeyCode("Q") == sf::Keyboard::Key::Q);
		CHECK(GameUtils::getKeyCode("R") == sf::Keyboard::Key::R);
		CHECK(GameUtils::getKeyCode("S") == sf::Keyboard::Key::S);
		CHECK(GameUtils::getKeyCode("T") == sf::Keyboard::Key::T);
		CHECK(GameUtils::getKeyCode("U") == sf::Keyboard::Key::U);
		CHECK(GameUtils::getKeyCode("V") == sf::Keyboard::Key::V);
		CHECK(GameUtils::getKeyCode("W") == sf::Keyboard::Key::W);
		CHECK(GameUtils::getKeyCode("X") == sf::Keyboard::Key::X);
		CHECK(GameUtils::getKeyCode("Y") == sf::Keyboard::Key::Y);
		CHECK(GameUtils::getKeyCode("Z") == sf::Keyboard::Key::Z);

		CHECK(GameUtils::getKeyCode("a") == sf::Keyboard::Key::A);
		CHECK(GameUtils::getKeyCode("b") == sf::Keyboard::Key::B);
		CHECK(GameUtils::getKeyCode("c") == sf::Keyboard::Key::C);
		CHECK(GameUtils::getKeyCode("d") == sf::Keyboard::Key::D);
		CHECK(GameUtils::getKeyCode("e") == sf::Keyboard::Key::E);
		CHECK(GameUtils::getKeyCode("f") == sf::Keyboard::Key::F);
		CHECK(GameUtils::getKeyCode("g") == sf::Keyboard::Key::G);
		CHECK(GameUtils::getKeyCode("h") == sf::Keyboard::Key::H);
		CHECK(GameUtils::getKeyCode("i") == sf::Keyboard::Key::I);
		CHECK(GameUtils::getKeyCode("j") == sf::Keyboard::Key::J);
		CHECK(GameUtils::getKeyCode("k") == sf::Keyboard::Key::K);
		CHECK(GameUtils::getKeyCode("l") == sf::Keyboard::Key::L);
		CHECK(GameUtils::getKeyCode("m") == sf::Keyboard::Key::M);
		CHECK(GameUtils::getKeyCode("n") == sf::Keyboard::Key::N);
		CHECK(GameUtils::getKeyCode("o") == sf::Keyboard::Key::O);
		CHECK(GameUtils::getKeyCode("p") == sf::Keyboard::Key::P);
		CHECK(GameUtils::getKeyCode("q") == sf::Keyboard::Key::Q);
		CHECK(GameUtils::getKeyCode("r") == sf::Keyboard::Key::R);
		CHECK(GameUtils::getKeyCode("s") == sf::Keyboard::Key::S);
		CHECK(GameUtils::getKeyCode("t") == sf::Keyboard::Key::T);
		CHECK(GameUtils::getKeyCode("u") == sf::Keyboard::Key::U);
		CHECK(GameUtils::getKeyCode("v") == sf::Keyboard::Key::V);
		CHECK(GameUtils::getKeyCode("w") == sf::Keyboard::Key::W);
		CHECK(GameUtils::getKeyCode("x") == sf::Keyboard::Key::X);
		CHECK(GameUtils::getKeyCode("y") == sf::Keyboard::Key::Y);
		CHECK(GameUtils::getKeyCode("z") == sf::Keyboard::Key::Z);

		CHECK(GameUtils::getKeyCode("0") == sf::Keyboard::Key::Num0);
		CHECK(GameUtils::getKeyCode("1") == sf::Keyboard::Key::Num1);
		CHECK(GameUtils::getKeyCode("2") == sf::Keyboard::Key::Num2);
		CHECK(GameUtils::getKeyCode("3") == sf::Keyboard::Key::Num3);
		CHECK(GameUtils::getKeyCode("4") == sf::Keyboard::Key::Num4);
		CHECK(GameUtils::getKeyCode("5") == sf::Keyboard::Key::Num5);
		CHECK(GameUtils::getKeyCode("6") == sf::Keyboard::Key::Num6);
		CHECK(GameUtils::getKeyCode("7") == sf::Keyboard::Key::Num7);
		CHECK(GameUtils::getKeyCode("8") == sf::Keyboard::Key::Num8);
		CHECK(GameUtils::getKeyCode("9") == sf::Keyboard::Key::Num9);

		CHECK(GameUtils::getKeyCode("[") == sf::Keyboard::Key::LBracket);
		CHECK(GameUtils::getKeyCode("]") == sf::Keyboard::Key::RBracket);
		CHECK(GameUtils::getKeyCode(";") == sf::Keyboard::Key::Semicolon);
		CHECK(GameUtils::getKeyCode(",") == sf::Keyboard::Key::Comma);
		CHECK(GameUtils::getKeyCode(".") == sf::Keyboard::Key::Period);
		CHECK(GameUtils::getKeyCode("\'") == sf::Keyboard::Key::Apostrophe);
		CHECK(GameUtils::getKeyCode("\\") == sf::Keyboard::Key::Backslash);
		CHECK(GameUtils::getKeyCode("~") == sf::Keyboard::Key::Tilde);
		CHECK(GameUtils::getKeyCode("=") == sf::Keyboard::Key::Equal);
		CHECK(GameUtils::getKeyCode(" ") == sf::Keyboard::Key::Space);
		CHECK(GameUtils::getKeyCode("+") == sf::Keyboard::Key::Add);
		CHECK(GameUtils::getKeyCode("-") == sf::Keyboard::Key::Subtract);
		CHECK(GameUtils::getKeyCode("*") == sf::Keyboard::Key::Multiply);
		CHECK(GameUtils::getKeyCode("/") == sf::Keyboard::Key::Divide);

		CHECK(GameUtils::getKeyCode("all") == sf::Keyboard::Key::KeyCount);
		CHECK(GameUtils::getKeyCode("any") == sf::Keyboard::Key::KeyCount);
		CHECK(GameUtils::getKeyCode("esc") == sf::Keyboard::Key::Escape);
		CHECK(GameUtils::getKeyCode("escape") == sf::Keyboard::Key::Escape);
		CHECK(GameUtils::getKeyCode("lcontrol") == sf::Keyboard::Key::LControl);
		CHECK(GameUtils::getKeyCode("lshift") == sf::Keyboard::Key::LShift);
		CHECK(GameUtils::getKeyCode("lalt") == sf::Keyboard::Key::LAlt);
		CHECK(GameUtils::getKeyCode("lsystem") == sf::Keyboard::Key::LSystem);
		CHECK(GameUtils::getKeyCode("rcontrol") == sf::Keyboard::Key::RControl);
		CHECK(GameUtils::getKeyCode("rshift") == sf::Keyboard::Key::RShift);
		CHECK(GameUtils::getKeyCode("ralt") == sf::Keyboard::Key::RAlt);
		CHECK(GameUtils::getKeyCode("rsystem") == sf::Keyboard::Key::RSystem);
		CHECK(GameUtils::getKeyCode("menu") == sf::Keyboard::Key::Menu);
		CHECK(GameUtils::getKeyCode("leftbracket") == sf::Keyboard::Key::LBracket);
		CHECK(GameUtils::getKeyCode("rightbracket") == sf::Keyboard::Key::RBracket);
		CHECK(GameUtils::getKeyCode("semicolon") == sf::Keyboard::Key::Semicolon);
		CHECK(GameUtils::getKeyCode("comma") == sf::Keyboard::Key::Comma);
		CHECK(GameUtils::getKeyCode("period") == sf::Keyboard::Key::Period);
		CHECK(GameUtils::getKeyCode("apostrophe") == sf::Keyboard::Key::Apostrophe);
		CHECK(GameUtils::getKeyCode("quote") == sf::Keyboard::Key::Apostrophe);
		CHECK(GameUtils::getKeyCode("slash") == sf::Keyboard::Key::Slash);
		CHECK(GameUtils::getKeyCode("backslash") == sf::Keyboard::Key::Backslash);
		CHECK(GameUtils::getKeyCode("tilde") == sf::Keyboard::Key::Tilde);
		CHECK(GameUtils::getKeyCode("equal") == sf::Keyboard::Key::Equal);
		CHECK(GameUtils::getKeyCode("hyphen") == sf::Keyboard::Key::Hyphen);
		CHECK(GameUtils::getKeyCode("space") == sf::Keyboard::Key::Space);
		CHECK(GameUtils::getKeyCode("enter") == sf::Keyboard::Key::Return);
		CHECK(GameUtils::getKeyCode("return") == sf::Keyboard::Key::Return);
		CHECK(GameUtils::getKeyCode("backspace") == sf::Keyboard::Key::BackSpace);
		CHECK(GameUtils::getKeyCode("tab") == sf::Keyboard::Key::Tab);
		CHECK(GameUtils::getKeyCode("pageup") == sf::Keyboard::Key::PageUp);
		CHECK(GameUtils::getKeyCode("pagedown") == sf::Keyboard::Key::PageDown);
		CHECK(GameUtils::getKeyCode("end") == sf::Keyboard::Key::End);
		CHECK(GameUtils::getKeyCode("home") == sf::Keyboard::Key::Home);
		CHECK(GameUtils::getKeyCode("insert") == sf::Keyboard::Key::Insert);
		CHECK(GameUtils::getKeyCode("delete") == sf::Keyboard::Key::Delete);
		CHECK(GameUtils::getKeyCode("add") == sf::Keyboard::Key::Add);
		CHECK(GameUtils::getKeyCode("subtract") == sf::Keyboard::Key::Subtract);
		CHECK(GameUtils::getKeyCode("multiply") == sf::Keyboard::Key::Multiply);
		CHECK(GameUtils::getKeyCode("divide") == sf::Keyboard::Key::Divide);
		CHECK(GameUtils::getKeyCode("left") == sf::Keyboard::Key::Left);
		CHECK(GameUtils::getKeyCode("right") == sf::Keyboard::Key::Right);
		CHECK(GameUtils::getKeyCode("up") == sf::Keyboard::Key::Up);
		CHECK(GameUtils::getKeyCode("down") == sf::Keyboard::Key::Down);
		CHECK(GameUtils::getKeyCode("numpad0") == sf::Keyboard::Key::Numpad0);
		CHECK(GameUtils::getKeyCode("numpad1") == sf::Keyboard::Key::Numpad1);
		CHECK(GameUtils::getKeyCode("numpad2") == sf::Keyboard::Key::Numpad2);
		CHECK(GameUtils::getKeyCode("numpad3") == sf::Keyboard::Key::Numpad3);
		CHECK(GameUtils::getKeyCode("numpad4") == sf::Keyboard::Key::Numpad4);
		CHECK(GameUtils::getKeyCode("numpad5") == sf::Keyboard::Key::Numpad5);
		CHECK(GameUtils::getKeyCode("numpad6") == sf::Keyboard::Key::Numpad6);
		CHECK(GameUtils::getKeyCode("numpad7") == sf::Keyboard::Key::Numpad7);
		CHECK(GameUtils::getKeyCode("numpad8") == sf::Keyboard::Key::Numpad8);
		CHECK(GameUtils::getKeyCode("numpad9") == sf::Keyboard::Key::Numpad9);
		CHECK(GameUtils::getKeyCode("f1") == sf::Keyboard::Key::F1);
		CHECK(GameUtils::getKeyCode("f2") == sf::Keyboard::Key::F2);
		CHECK(GameUtils::getKeyCode("f3") == sf::Keyboard::Key::F3);
		CHECK(GameUtils::getKeyCode("f4") == sf::Keyboard::Key::F4);
		CHECK(GameUtils::getKeyCode("f5") == sf::Keyboard::Key::F5);
		CHECK(GameUtils::getKeyCode("f6") == sf::Keyboard::Key::F6);
		CHECK(GameUtils::getKeyCode("f7") == sf::Keyboard::Key::F7);
		CHECK(GameUtils::getKeyCode("f8") == sf::Keyboard::Key::F8);
		CHECK(GameUtils::getKeyCode("f9") == sf::Keyboard::Key::F9);
		CHECK(GameUtils::getKeyCode("f10") == sf::Keyboard::Key::F10);
		CHECK(GameUtils::getKeyCode("f11") == sf::Keyboard::Key::F11);
		CHECK(GameUtils::getKeyCode("f12") == sf::Keyboard::Key::F12);
		CHECK(GameUtils::getKeyCode("f13") == sf::Keyboard::Key::F13);
		CHECK(GameUtils::getKeyCode("f14") == sf::Keyboard::Key::F14);
		CHECK(GameUtils::getKeyCode("f15") == sf::Keyboard::Key::F15);
		CHECK(GameUtils::getKeyCode("pause") == sf::Keyboard::Key::Pause);
	}
	SECTION("invalid")
	{
		CHECK(GameUtils::getKeyCode("invalid") == sf::Keyboard::Key::Unknown);
		CHECK(GameUtils::getKeyCode("invalid", sf::Keyboard::Key::A) == sf::Keyboard::Key::A);
	}
}

TEST_CASE("GameUtils::getScanCode", "GameUtils")
{
	SECTION("int")
	{
		CHECK(GameUtils::getScanCode(0) == sf::Keyboard::Scancode::Num0);
		CHECK(GameUtils::getScanCode(1) == sf::Keyboard::Scancode::Num1);
		CHECK(GameUtils::getScanCode(2) == sf::Keyboard::Scancode::Num2);
		CHECK(GameUtils::getScanCode(3) == sf::Keyboard::Scancode::Num3);
		CHECK(GameUtils::getScanCode(4) == sf::Keyboard::Scancode::Num4);
		CHECK(GameUtils::getScanCode(5) == sf::Keyboard::Scancode::Num5);
		CHECK(GameUtils::getScanCode(6) == sf::Keyboard::Scancode::Num6);
		CHECK(GameUtils::getScanCode(7) == sf::Keyboard::Scancode::Num7);
		CHECK(GameUtils::getScanCode(8) == sf::Keyboard::Scancode::Num8);
		CHECK(GameUtils::getScanCode(9) == sf::Keyboard::Scancode::Num9);
		CHECK(GameUtils::getScanCode(10, sf::Keyboard::Scancode::A) == sf::Keyboard::Scancode::A);
	}
	SECTION("string")
	{
		CHECK(GameUtils::getScanCode("A") == sf::Keyboard::Scancode::A);
		CHECK(GameUtils::getScanCode("B") == sf::Keyboard::Scancode::B);
		CHECK(GameUtils::getScanCode("C") == sf::Keyboard::Scancode::C);
		CHECK(GameUtils::getScanCode("D") == sf::Keyboard::Scancode::D);
		CHECK(GameUtils::getScanCode("E") == sf::Keyboard::Scancode::E);
		CHECK(GameUtils::getScanCode("F") == sf::Keyboard::Scancode::F);
		CHECK(GameUtils::getScanCode("G") == sf::Keyboard::Scancode::G);
		CHECK(GameUtils::getScanCode("H") == sf::Keyboard::Scancode::H);
		CHECK(GameUtils::getScanCode("I") == sf::Keyboard::Scancode::I);
		CHECK(GameUtils::getScanCode("J") == sf::Keyboard::Scancode::J);
		CHECK(GameUtils::getScanCode("K") == sf::Keyboard::Scancode::K);
		CHECK(GameUtils::getScanCode("L") == sf::Keyboard::Scancode::L);
		CHECK(GameUtils::getScanCode("M") == sf::Keyboard::Scancode::M);
		CHECK(GameUtils::getScanCode("N") == sf::Keyboard::Scancode::N);
		CHECK(GameUtils::getScanCode("O") == sf::Keyboard::Scancode::O);
		CHECK(GameUtils::getScanCode("P") == sf::Keyboard::Scancode::P);
		CHECK(GameUtils::getScanCode("Q") == sf::Keyboard::Scancode::Q);
		CHECK(GameUtils::getScanCode("R") == sf::Keyboard::Scancode::R);
		CHECK(GameUtils::getScanCode("S") == sf::Keyboard::Scancode::S);
		CHECK(GameUtils::getScanCode("T") == sf::Keyboard::Scancode::T);
		CHECK(GameUtils::getScanCode("U") == sf::Keyboard::Scancode::U);
		CHECK(GameUtils::getScanCode("V") == sf::Keyboard::Scancode::V);
		CHECK(GameUtils::getScanCode("W") == sf::Keyboard::Scancode::W);
		CHECK(GameUtils::getScanCode("X") == sf::Keyboard::Scancode::X);
		CHECK(GameUtils::getScanCode("Y") == sf::Keyboard::Scancode::Y);
		CHECK(GameUtils::getScanCode("Z") == sf::Keyboard::Scancode::Z);

		CHECK(GameUtils::getScanCode("a") == sf::Keyboard::Scancode::A);
		CHECK(GameUtils::getScanCode("b") == sf::Keyboard::Scancode::B);
		CHECK(GameUtils::getScanCode("c") == sf::Keyboard::Scancode::C);
		CHECK(GameUtils::getScanCode("d") == sf::Keyboard::Scancode::D);
		CHECK(GameUtils::getScanCode("e") == sf::Keyboard::Scancode::E);
		CHECK(GameUtils::getScanCode("f") == sf::Keyboard::Scancode::F);
		CHECK(GameUtils::getScanCode("g") == sf::Keyboard::Scancode::G);
		CHECK(GameUtils::getScanCode("h") == sf::Keyboard::Scancode::H);
		CHECK(GameUtils::getScanCode("i") == sf::Keyboard::Scancode::I);
		CHECK(GameUtils::getScanCode("j") == sf::Keyboard::Scancode::J);
		CHECK(GameUtils::getScanCode("k") == sf::Keyboard::Scancode::K);
		CHECK(GameUtils::getScanCode("l") == sf::Keyboard::Scancode::L);
		CHECK(GameUtils::getScanCode("m") == sf::Keyboard::Scancode::M);
		CHECK(GameUtils::getScanCode("n") == sf::Keyboard::Scancode::N);
		CHECK(GameUtils::getScanCode("o") == sf::Keyboard::Scancode::O);
		CHECK(GameUtils::getScanCode("p") == sf::Keyboard::Scancode::P);
		CHECK(GameUtils::getScanCode("q") == sf::Keyboard::Scancode::Q);
		CHECK(GameUtils::getScanCode("r") == sf::Keyboard::Scancode::R);
		CHECK(GameUtils::getScanCode("s") == sf::Keyboard::Scancode::S);
		CHECK(GameUtils::getScanCode("t") == sf::Keyboard::Scancode::T);
		CHECK(GameUtils::getScanCode("u") == sf::Keyboard::Scancode::U);
		CHECK(GameUtils::getScanCode("v") == sf::Keyboard::Scancode::V);
		CHECK(GameUtils::getScanCode("w") == sf::Keyboard::Scancode::W);
		CHECK(GameUtils::getScanCode("x") == sf::Keyboard::Scancode::X);
		CHECK(GameUtils::getScanCode("y") == sf::Keyboard::Scancode::Y);
		CHECK(GameUtils::getScanCode("z") == sf::Keyboard::Scancode::Z);

		CHECK(GameUtils::getScanCode("0") == sf::Keyboard::Scancode::Num0);
		CHECK(GameUtils::getScanCode("1") == sf::Keyboard::Scancode::Num1);
		CHECK(GameUtils::getScanCode("2") == sf::Keyboard::Scancode::Num2);
		CHECK(GameUtils::getScanCode("3") == sf::Keyboard::Scancode::Num3);
		CHECK(GameUtils::getScanCode("4") == sf::Keyboard::Scancode::Num4);
		CHECK(GameUtils::getScanCode("5") == sf::Keyboard::Scancode::Num5);
		CHECK(GameUtils::getScanCode("6") == sf::Keyboard::Scancode::Num6);
		CHECK(GameUtils::getScanCode("7") == sf::Keyboard::Scancode::Num7);
		CHECK(GameUtils::getScanCode("8") == sf::Keyboard::Scancode::Num8);
		CHECK(GameUtils::getScanCode("9") == sf::Keyboard::Scancode::Num9);

		CHECK(GameUtils::getScanCode("[") == sf::Keyboard::Scancode::LBracket);
		CHECK(GameUtils::getScanCode("]") == sf::Keyboard::Scancode::RBracket);
		CHECK(GameUtils::getScanCode(";") == sf::Keyboard::Scancode::Semicolon);
		CHECK(GameUtils::getScanCode(",") == sf::Keyboard::Scancode::Comma);
		CHECK(GameUtils::getScanCode(".") == sf::Keyboard::Scancode::Period);
		CHECK(GameUtils::getScanCode("\'") == sf::Keyboard::Scancode::Apostrophe);
		CHECK(GameUtils::getScanCode("\\") == sf::Keyboard::Scancode::Backslash);
		CHECK(GameUtils::getScanCode("`") == sf::Keyboard::Scancode::Grave);
		CHECK(GameUtils::getScanCode("~") == sf::Keyboard::Scancode::Grave);
		CHECK(GameUtils::getScanCode("=") == sf::Keyboard::Scancode::Equal);
		CHECK(GameUtils::getScanCode(" ") == sf::Keyboard::Scancode::Space);
		CHECK(GameUtils::getScanCode("+") == sf::Keyboard::Scancode::Equal);
		CHECK(GameUtils::getScanCode("-") == sf::Keyboard::Scancode::Hyphen);
		CHECK(GameUtils::getScanCode("/") == sf::Keyboard::Scancode::Slash);

		CHECK(GameUtils::getScanCode("all") == sf::Keyboard::Scancode::ScancodeCount);
		CHECK(GameUtils::getScanCode("any") == sf::Keyboard::Scancode::ScancodeCount);
		CHECK(GameUtils::getScanCode("esc") == sf::Keyboard::Scancode::Escape);
		CHECK(GameUtils::getScanCode("escape") == sf::Keyboard::Scancode::Escape);
		CHECK(GameUtils::getScanCode("lcontrol") == sf::Keyboard::Scancode::LControl);
		CHECK(GameUtils::getScanCode("lshift") == sf::Keyboard::Scancode::LShift);
		CHECK(GameUtils::getScanCode("lalt") == sf::Keyboard::Scancode::LAlt);
		CHECK(GameUtils::getScanCode("lsystem") == sf::Keyboard::Scancode::LSystem);
		CHECK(GameUtils::getScanCode("rcontrol") == sf::Keyboard::Scancode::RControl);
		CHECK(GameUtils::getScanCode("rshift") == sf::Keyboard::Scancode::RShift);
		CHECK(GameUtils::getScanCode("ralt") == sf::Keyboard::Scancode::RAlt);
		CHECK(GameUtils::getScanCode("rsystem") == sf::Keyboard::Scancode::RSystem);
		CHECK(GameUtils::getScanCode("menu") == sf::Keyboard::Scancode::Menu);
		CHECK(GameUtils::getScanCode("leftbracket") == sf::Keyboard::Scancode::LBracket);
		CHECK(GameUtils::getScanCode("rightbracket") == sf::Keyboard::Scancode::RBracket);
		CHECK(GameUtils::getScanCode("semicolon") == sf::Keyboard::Scancode::Semicolon);
		CHECK(GameUtils::getScanCode("comma") == sf::Keyboard::Scancode::Comma);
		CHECK(GameUtils::getScanCode("period") == sf::Keyboard::Scancode::Period);
		CHECK(GameUtils::getScanCode("apostrophe") == sf::Keyboard::Scancode::Apostrophe);
		CHECK(GameUtils::getScanCode("quote") == sf::Keyboard::Scancode::Apostrophe);
		CHECK(GameUtils::getScanCode("slash") == sf::Keyboard::Scancode::Slash);
		CHECK(GameUtils::getScanCode("backslash") == sf::Keyboard::Scancode::Backslash);
		CHECK(GameUtils::getScanCode("tilde") == sf::Keyboard::Scancode::Grave);
		CHECK(GameUtils::getScanCode("equal") == sf::Keyboard::Scancode::Equal);
		CHECK(GameUtils::getScanCode("hyphen") == sf::Keyboard::Scancode::Hyphen);
		CHECK(GameUtils::getScanCode("space") == sf::Keyboard::Scancode::Space);
		CHECK(GameUtils::getScanCode("enter") == sf::Keyboard::Scancode::Enter);
		CHECK(GameUtils::getScanCode("return") == sf::Keyboard::Scancode::Enter);
		CHECK(GameUtils::getScanCode("backspace") == sf::Keyboard::Scancode::Backspace);
		CHECK(GameUtils::getScanCode("tab") == sf::Keyboard::Scancode::Tab);
		CHECK(GameUtils::getScanCode("pageup") == sf::Keyboard::Scancode::PageUp);
		CHECK(GameUtils::getScanCode("pagedown") == sf::Keyboard::Scancode::PageDown);
		CHECK(GameUtils::getScanCode("end") == sf::Keyboard::Scancode::End);
		CHECK(GameUtils::getScanCode("home") == sf::Keyboard::Scancode::Home);
		CHECK(GameUtils::getScanCode("insert") == sf::Keyboard::Scancode::Insert);
		CHECK(GameUtils::getScanCode("delete") == sf::Keyboard::Scancode::Delete);
		CHECK(GameUtils::getScanCode("add") == sf::Keyboard::Scancode::Equal);
		CHECK(GameUtils::getScanCode("subtract") == sf::Keyboard::Scancode::Hyphen);
		CHECK(GameUtils::getScanCode("divide") == sf::Keyboard::Scancode::Slash);
		CHECK(GameUtils::getScanCode("left") == sf::Keyboard::Scancode::Left);
		CHECK(GameUtils::getScanCode("right") == sf::Keyboard::Scancode::Right);
		CHECK(GameUtils::getScanCode("up") == sf::Keyboard::Scancode::Up);
		CHECK(GameUtils::getScanCode("down") == sf::Keyboard::Scancode::Down);
		CHECK(GameUtils::getScanCode("numpad0") == sf::Keyboard::Scancode::Numpad0);
		CHECK(GameUtils::getScanCode("numpad1") == sf::Keyboard::Scancode::Numpad1);
		CHECK(GameUtils::getScanCode("numpad2") == sf::Keyboard::Scancode::Numpad2);
		CHECK(GameUtils::getScanCode("numpad3") == sf::Keyboard::Scancode::Numpad3);
		CHECK(GameUtils::getScanCode("numpad4") == sf::Keyboard::Scancode::Numpad4);
		CHECK(GameUtils::getScanCode("numpad5") == sf::Keyboard::Scancode::Numpad5);
		CHECK(GameUtils::getScanCode("numpad6") == sf::Keyboard::Scancode::Numpad6);
		CHECK(GameUtils::getScanCode("numpad7") == sf::Keyboard::Scancode::Numpad7);
		CHECK(GameUtils::getScanCode("numpad8") == sf::Keyboard::Scancode::Numpad8);
		CHECK(GameUtils::getScanCode("numpad9") == sf::Keyboard::Scancode::Numpad9);
		CHECK(GameUtils::getScanCode("f1") == sf::Keyboard::Scancode::F1);
		CHECK(GameUtils::getScanCode("f2") == sf::Keyboard::Scancode::F2);
		CHECK(GameUtils::getScanCode("f3") == sf::Keyboard::Scancode::F3);
		CHECK(GameUtils::getScanCode("f4") == sf::Keyboard::Scancode::F4);
		CHECK(GameUtils::getScanCode("f5") == sf::Keyboard::Scancode::F5);
		CHECK(GameUtils::getScanCode("f6") == sf::Keyboard::Scancode::F6);
		CHECK(GameUtils::getScanCode("f7") == sf::Keyboard::Scancode::F7);
		CHECK(GameUtils::getScanCode("f8") == sf::Keyboard::Scancode::F8);
		CHECK(GameUtils::getScanCode("f9") == sf::Keyboard::Scancode::F9);
		CHECK(GameUtils::getScanCode("f10") == sf::Keyboard::Scancode::F10);
		CHECK(GameUtils::getScanCode("f11") == sf::Keyboard::Scancode::F11);
		CHECK(GameUtils::getScanCode("f12") == sf::Keyboard::Scancode::F12);
		CHECK(GameUtils::getScanCode("f13") == sf::Keyboard::Scancode::F13);
		CHECK(GameUtils::getScanCode("f14") == sf::Keyboard::Scancode::F14);
		CHECK(GameUtils::getScanCode("f15") == sf::Keyboard::Scancode::F15);
		CHECK(GameUtils::getScanCode("pause") == sf::Keyboard::Scancode::Pause);
	}
	SECTION("invalid")
	{
		CHECK(GameUtils::getScanCode("invalid") == sf::Keyboard::Scancode::Unknown);
		CHECK(GameUtils::getScanCode("invalid", sf::Keyboard::Scancode::A) == sf::Keyboard::Scancode::A);
	}
}

TEST_CASE("GameUtils::getTime", "GameUtils")
{
	SECTION("time ms")
	{
		constexpr auto fmt = "ms";
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(0.1f), fmt)) == 100);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(1.f), fmt)) == 1000);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(1.1f), fmt)) == 1100);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(1.5f), fmt)) == 1500);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(1.9f), fmt)) == 1900);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(1.1f), fmt, true)) == 1100);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(1.5f), fmt, true)) == 1500);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(1.9f), fmt, true)) == 1900);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(10.f), fmt)) == 10000);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(10000.f), fmt)) == 10000000);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(-1.1f), fmt)) == -1100);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(-1.1f), fmt, true)) == -1100);
	}
	SECTION("time")
	{
		constexpr auto fmt = "";
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(1.f), fmt)) == 1);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(1.1f), fmt)) == 1);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(1.5f), fmt)) == 1);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(1.9f), fmt)) == 1);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(1.1f), fmt, true)) == 2);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(1.5f), fmt, true)) == 2);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(1.9f), fmt, true)) == 2);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(10.f), fmt)) == 10);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(10000.f), fmt)) == 10000);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(-1.1f), fmt)) == -2);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(-1.1f), fmt, true)) == -1);
	}
	SECTION("time s")
	{
		constexpr auto fmt = "s";
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(1.f), fmt)) == 1);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(1.1f), fmt)) == 1);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(1.5f), fmt)) == 1);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(1.9f), fmt)) == 1);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(1.1f), fmt, true)) == 2);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(1.5f), fmt, true)) == 2);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(1.9f), fmt, true)) == 2);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(10.f), fmt)) == 10);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(10000.f), fmt)) == 10000);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(-1.1f), fmt)) == -2);
		CHECK(VarUtils::toInt64(GameUtils::getTime(sf::seconds(-1.1f), fmt, true)) == -1);
	}
	SECTION("time S")
	{
		constexpr auto fmt = "S";
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.f), fmt)) == "1");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt)) == "1");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt, true)) == "2");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(59.f), fmt)) == "59");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(10000.f), fmt)) == "10000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(-1.1f), fmt)) == "-2");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(-1.1f), fmt, true)) == "-1");
	}
	SECTION("time Sf")
	{
		constexpr auto fmt = "Sf";
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.f), fmt)) == "1.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt)) == "1.1");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt, true)) == "1.1");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1235f), fmt)) == "1.1");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.505f), fmt)) == "1.5");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(59.f), fmt)) == "59.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(10000.f), fmt)) == "10000.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(-1.1f), fmt)) == "-1.1");
	}
	SECTION("time Sff")
	{
		constexpr auto fmt = "Sff";
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.f), fmt)) == "1.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt)) == "1.10");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt, true)) == "1.10");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1235f), fmt)) == "1.12");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.505f), fmt)) == "1.51");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(59.f), fmt)) == "59.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(10000.f), fmt)) == "10000.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(-1.1f), fmt)) == "-1.10");
	}
	SECTION("time Sfff")
	{
		constexpr auto fmt = "Sfff";
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.f), fmt)) == "1.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt)) == "1.100");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt, true)) == "1.100");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1235f), fmt)) == "1.123");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.505f), fmt)) == "1.505");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(59.f), fmt)) == "59.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(10000.f), fmt)) == "10000.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(-1.1f), fmt)) == "-1.100");
	}
	SECTION("time SS")
	{
		constexpr auto fmt = "SS";
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.f), fmt)) == "01");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt)) == "01");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt, true)) == "02");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(59.f), fmt)) == "59");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(10000.f), fmt)) == "10000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(-1.1f), fmt)) == "-02");
	}
	SECTION("time SSf")
	{
		constexpr auto fmt = "SSf";
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.f), fmt)) == "01.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt)) == "01.1");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt, true)) == "01.1");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1235f), fmt)) == "01.1");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.505f), fmt)) == "01.5");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(59.f), fmt)) == "59.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(10000.f), fmt)) == "10000.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(-1.1f), fmt)) == "-01.1");
	}
	SECTION("time SSff")
	{
		constexpr auto fmt = "SSff";
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.f), fmt)) == "01.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt)) == "01.10");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt, true)) == "01.10");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1235f), fmt)) == "01.12");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.505f), fmt)) == "01.51");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(59.f), fmt)) == "59.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(10000.f), fmt)) == "10000.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(-1.1f), fmt)) == "-01.10");
	}
	SECTION("time SSfff")
	{
		constexpr auto fmt = "SSfff";
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.f), fmt)) == "01.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt)) == "01.100");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt, true)) == "01.100");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1235f), fmt)) == "01.123");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.505f), fmt)) == "01.505");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(59.f), fmt)) == "59.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(10000.f), fmt)) == "10000.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(-1.1f), fmt)) == "-01.100");
	}
	SECTION("time MSS")
	{
		constexpr auto fmt = "MSS";
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.f), fmt)) == "0:01");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt)) == "0:01");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt, true)) == "0:02");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(59.f), fmt)) == "0:59");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(60.f), fmt)) == "1:00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(61.f), fmt)) == "1:01");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3599.f), fmt)) == "59:59");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3600.f), fmt)) == "60:00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3601.f), fmt)) == "60:01");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(10000.f), fmt)) == "166:40");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(-1.1f), fmt)) == "-0:02");
	}
	SECTION("time MSSf")
	{
		constexpr auto fmt = "MSSf";
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.f), fmt)) == "0:01.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt)) == "0:01.1");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt, true)) == "0:01.1");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1235f), fmt)) == "0:01.1");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.505f), fmt)) == "0:01.5");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(59.f), fmt)) == "0:59.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(60.f), fmt)) == "1:00.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(61.f), fmt)) == "1:01.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3599.f), fmt)) == "59:59.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3600.f), fmt)) == "60:00.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3601.f), fmt)) == "60:01.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(10000.f), fmt)) == "166:40.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(-1.1f), fmt)) == "-0:01.1");
	}
	SECTION("time MSSff")
	{
		constexpr auto fmt = "MSSff";
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.f), fmt)) == "0:01.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt)) == "0:01.10");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt, true)) == "0:01.10");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1235f), fmt)) == "0:01.12");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.505f), fmt)) == "0:01.51");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(59.f), fmt)) == "0:59.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(60.f), fmt)) == "1:00.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(61.f), fmt)) == "1:01.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3599.f), fmt)) == "59:59.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3600.f), fmt)) == "60:00.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3601.f), fmt)) == "60:01.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(10000.f), fmt)) == "166:40.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(-1.1f), fmt)) == "-0:01.10");
	}
	SECTION("time MSSfff")
	{
		constexpr auto fmt = "MSSfff";
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.f), fmt)) == "0:01.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt)) == "0:01.100");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt, true)) == "0:01.100");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1235f), fmt)) == "0:01.123");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.505f), fmt)) == "0:01.505");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(59.f), fmt)) == "0:59.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(60.f), fmt)) == "1:00.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(61.f), fmt)) == "1:01.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3599.f), fmt)) == "59:59.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3600.f), fmt)) == "60:00.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3601.f), fmt)) == "60:01.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(10000.f), fmt)) == "166:40.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(-1.1f), fmt)) == "-0:01.100");
	}
	SECTION("time MMSS")
	{
		constexpr auto fmt = "MMSS";
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.f), fmt)) == "00:01");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt)) == "00:01");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt, true)) == "00:02");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(59.f), fmt)) == "00:59");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(60.f), fmt)) == "01:00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(61.f), fmt)) == "01:01");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3599.f), fmt)) == "59:59");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3600.f), fmt)) == "60:00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3601.f), fmt)) == "60:01");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(10000.f), fmt)) == "166:40");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(-1.1f), fmt)) == "-00:02");
	}
	SECTION("time MMSSf")
	{
		constexpr auto fmt = "MMSSf";
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.f), fmt)) == "00:01.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt)) == "00:01.1");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt, true)) == "00:01.1");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1235f), fmt)) == "00:01.1");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.505f), fmt)) == "00:01.5");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(59.f), fmt)) == "00:59.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(60.f), fmt)) == "01:00.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(61.f), fmt)) == "01:01.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3599.f), fmt)) == "59:59.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3600.f), fmt)) == "60:00.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3601.f), fmt)) == "60:01.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(10000.f), fmt)) == "166:40.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(-1.1f), fmt)) == "-00:01.1");
	}
	SECTION("time MMSSff")
	{
		constexpr auto fmt = "MMSSff";
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.f), fmt)) == "00:01.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt)) == "00:01.10");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt, true)) == "00:01.10");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1235f), fmt)) == "00:01.12");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.505f), fmt)) == "00:01.51");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(59.f), fmt)) == "00:59.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(60.f), fmt)) == "01:00.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(61.f), fmt)) == "01:01.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3599.f), fmt)) == "59:59.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3600.f), fmt)) == "60:00.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3601.f), fmt)) == "60:01.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(10000.f), fmt)) == "166:40.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(-1.1f), fmt)) == "-00:01.10");
	}
	SECTION("time MMSSfff")
	{
		constexpr auto fmt = "MMSSfff";
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.f), fmt)) == "00:01.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt)) == "00:01.100");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt, true)) == "00:01.100");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1235f), fmt)) == "00:01.123");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.505f), fmt)) == "00:01.505");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(59.f), fmt)) == "00:59.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(60.f), fmt)) == "01:00.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(61.f), fmt)) == "01:01.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3599.f), fmt)) == "59:59.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3600.f), fmt)) == "60:00.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3601.f), fmt)) == "60:01.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(10000.f), fmt)) == "166:40.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(-1.1f), fmt)) == "-00:01.100");
	}
	SECTION("time HMMSS")
	{
		constexpr auto fmt = "HMMSS";
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.f), fmt)) == "0:00:01");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt)) == "0:00:01");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt, true)) == "0:00:02");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(59.f), fmt)) == "0:00:59");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(60.f), fmt)) == "0:01:00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(61.f), fmt)) == "0:01:01");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3599.f), fmt)) == "0:59:59");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3600.f), fmt)) == "1:00:00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3601.f), fmt)) == "1:00:01");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(10000.f), fmt)) == "2:46:40");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(215999.f), fmt)) == "59:59:59");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(216000.f), fmt)) == "60:00:00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(216001.f), fmt)) == "60:00:01");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(-1.1f), fmt)) == "-0:00:02");
	}
	SECTION("time HMMSSf")
	{
		constexpr auto fmt = "HMMSSf";
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.f), fmt)) == "0:00:01.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt)) == "0:00:01.1");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt, true)) == "0:00:01.1");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1235f), fmt)) == "0:00:01.1");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.505f), fmt)) == "0:00:01.5");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(59.f), fmt)) == "0:00:59.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(60.f), fmt)) == "0:01:00.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(61.f), fmt)) == "0:01:01.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3599.f), fmt)) == "0:59:59.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3600.f), fmt)) == "1:00:00.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3601.f), fmt)) == "1:00:01.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(10000.f), fmt)) == "2:46:40.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(215999.f), fmt)) == "59:59:59.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(216000.f), fmt)) == "60:00:00.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(216001.f), fmt)) == "60:00:01.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(-1.1f), fmt)) == "-0:00:01.1");
	}
	SECTION("time HMMSSff")
	{
		constexpr auto fmt = "HMMSSff";
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.f), fmt)) == "0:00:01.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt)) == "0:00:01.10");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt, true)) == "0:00:01.10");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1235f), fmt)) == "0:00:01.12");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.505f), fmt)) == "0:00:01.51");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(59.f), fmt)) == "0:00:59.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(60.f), fmt)) == "0:01:00.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(61.f), fmt)) == "0:01:01.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3599.f), fmt)) == "0:59:59.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3600.f), fmt)) == "1:00:00.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3601.f), fmt)) == "1:00:01.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(10000.f), fmt)) == "2:46:40.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(215999.f), fmt)) == "59:59:59.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(216000.f), fmt)) == "60:00:00.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(216001.f), fmt)) == "60:00:01.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(-1.1f), fmt)) == "-0:00:01.10");
	}
	SECTION("time HMMSSfff")
	{
		constexpr auto fmt = "HMMSSfff";
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.f), fmt)) == "0:00:01.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt)) == "0:00:01.100");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt, true)) == "0:00:01.100");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1235f), fmt)) == "0:00:01.123");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.505f), fmt)) == "0:00:01.505");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(59.f), fmt)) == "0:00:59.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(60.f), fmt)) == "0:01:00.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(61.f), fmt)) == "0:01:01.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3599.f), fmt)) == "0:59:59.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3600.f), fmt)) == "1:00:00.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3601.f), fmt)) == "1:00:01.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(10000.f), fmt)) == "2:46:40.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(215999.f), fmt)) == "59:59:59.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(216000.f), fmt)) == "60:00:00.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(216001.f), fmt)) == "60:00:01.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(-1.1f), fmt)) == "-0:00:01.100");
	}
	SECTION("time HHMMSS")
	{
		constexpr auto fmt = "HHMMSS";
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.f), fmt)) == "00:00:01");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt)) == "00:00:01");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt, true)) == "00:00:02");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(59.f), fmt)) == "00:00:59");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(60.f), fmt)) == "00:01:00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(61.f), fmt)) == "00:01:01");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3599.f), fmt)) == "00:59:59");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3600.f), fmt)) == "01:00:00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3601.f), fmt)) == "01:00:01");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(10000.f), fmt)) == "02:46:40");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(215999.f), fmt)) == "59:59:59");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(216000.f), fmt)) == "60:00:00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(216001.f), fmt)) == "60:00:01");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1000000.f), fmt)) == "277:46:40");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(-1.1f), fmt)) == "-00:00:02");
	}
	SECTION("time HHMMSSf")
	{
		constexpr auto fmt = "HHMMSSf";
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.f), fmt)) == "00:00:01.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt)) == "00:00:01.1");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt, true)) == "00:00:01.1");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1235f), fmt)) == "00:00:01.1");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.505f), fmt)) == "00:00:01.5");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(59.f), fmt)) == "00:00:59.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(60.f), fmt)) == "00:01:00.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(61.f), fmt)) == "00:01:01.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3599.f), fmt)) == "00:59:59.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3600.f), fmt)) == "01:00:00.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3601.f), fmt)) == "01:00:01.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(10000.f), fmt)) == "02:46:40.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(215999.f), fmt)) == "59:59:59.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(216000.f), fmt)) == "60:00:00.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(216001.f), fmt)) == "60:00:01.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1000000.f), fmt)) == "277:46:40.0");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(-1.1f), fmt)) == "-00:00:01.1");
	}
	SECTION("time HHMMSSff")
	{
		constexpr auto fmt = "HHMMSSff";
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.f), fmt)) == "00:00:01.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt)) == "00:00:01.10");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt, true)) == "00:00:01.10");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1235f), fmt)) == "00:00:01.12");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.505f), fmt)) == "00:00:01.51");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(59.f), fmt)) == "00:00:59.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(60.f), fmt)) == "00:01:00.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(61.f), fmt)) == "00:01:01.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3599.f), fmt)) == "00:59:59.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3600.f), fmt)) == "01:00:00.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3601.f), fmt)) == "01:00:01.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(10000.f), fmt)) == "02:46:40.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(215999.f), fmt)) == "59:59:59.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(216000.f), fmt)) == "60:00:00.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(216001.f), fmt)) == "60:00:01.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1000000.f), fmt)) == "277:46:40.00");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(-1.1f), fmt)) == "-00:00:01.10");
	}
	SECTION("time HHMMSSfff")
	{
		constexpr auto fmt = "HHMMSSfff";
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.f), fmt)) == "00:00:01.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt)) == "00:00:01.100");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1f), fmt, true)) == "00:00:01.100");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.1235f), fmt)) == "00:00:01.123");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1.505f), fmt)) == "00:00:01.505");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(59.f), fmt)) == "00:00:59.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(60.f), fmt)) == "00:01:00.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(61.f), fmt)) == "00:01:01.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3599.f), fmt)) == "00:59:59.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3600.f), fmt)) == "01:00:00.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(3601.f), fmt)) == "01:00:01.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(10000.f), fmt)) == "02:46:40.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(215999.f), fmt)) == "59:59:59.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(216000.f), fmt)) == "60:00:00.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(216001.f), fmt)) == "60:00:01.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(1000000.f), fmt)) == "277:46:40.000");
		CHECK(VarUtils::toString(GameUtils::getTime(sf::seconds(-1.1f), fmt)) == "-00:00:01.100");
	}
}

TEST_CASE("GameUtils::replaceStringWithQueryable", "GameUtils")
{
	Circle obj(2.f, 30);

	CHECK(GameUtils::replaceStringWithQueryable("", obj) == "");
	CHECK(GameUtils::replaceStringWithQueryable("text", obj) == "text");
	CHECK(GameUtils::replaceStringWithQueryable("%text%", obj) == "%text%");
	CHECK(GameUtils::replaceStringWithQueryable("%radius%", obj) == "2");
	CHECK(GameUtils::replaceStringWithQueryable("%points%", obj) == "30");
	CHECK(GameUtils::replaceStringWithQueryable("%size.x%", obj) == "4");
	CHECK(GameUtils::replaceStringWithQueryable("%size.y%", obj) == "4");
	CHECK(GameUtils::replaceStringWithQueryable("!size.y!", obj, '!') == "4");
	CHECK(GameUtils::replaceStringWithQueryable("The size is: [%size.x%, %size.y%]", obj) == "The size is: [4, 4]");
	CHECK(GameUtils::replaceStringWithQueryable("The size is: [%%size.x%, %size.y%]", obj) == "The size is: [%4, 4]");
	CHECK(GameUtils::replaceStringWithQueryable("The size is: [%%size.x%, %size.y%%]", obj) == "The size is: [%4, 4%]");
	CHECK(GameUtils::replaceStringWithQueryable("The size is: [%%size.x%%, %size.y%%]", obj) == "The size is: [%4%, 4%]");
	CHECK(GameUtils::replaceStringWithQueryable("The size is: [%%size.x%,% %size.y%%]", obj) == "The size is: [%4,% 4%]");
}

TEST_CASE("GameUtils::replaceStringWithVarOrProp", "GameUtils")
{
	Game game(true);
	auto obj = std::make_shared<Circle>(2.f, 30);
	game.Resources().addDrawable("obj", obj, true);

	CHECK(GameUtils::replaceStringWithVarOrProp("", game) == "");
	CHECK(GameUtils::replaceStringWithVarOrProp("text", game) == "text");
	CHECK(GameUtils::replaceStringWithVarOrProp("%text%", game) == "%text%");
	CHECK(GameUtils::replaceStringWithVarOrProp("%game.gamma%", game) == "0");
	CHECK(GameUtils::replaceStringWithVarOrProp("%game.keepAR%", game) == "true");
	CHECK(GameUtils::replaceStringWithVarOrProp("%game.size.x%", game) == "640");
	CHECK(GameUtils::replaceStringWithVarOrProp("%game.size.y%", game) == "480");
	CHECK(GameUtils::replaceStringWithVarOrProp("!game.size.y!", game, '!') == "480");
	CHECK(GameUtils::replaceStringWithVarOrProp("The size is: [%game.size.x%, %game.size.y%]", game) == "The size is: [640, 480]");
	CHECK(GameUtils::replaceStringWithVarOrProp("The size is: [%%game.size.x%, %game.size.y%]", game) == "The size is: [%640, 480]");
	CHECK(GameUtils::replaceStringWithVarOrProp("The size is: [%%game.size.x%, %game.size.y%%]", game) == "The size is: [%640, 480%]");
	CHECK(GameUtils::replaceStringWithVarOrProp("The size is: [%%game.size.x%%, %game.size.y%%]", game) == "The size is: [%640%, 480%]");
	CHECK(GameUtils::replaceStringWithVarOrProp("The size is: [%%game.size.x%,% %game.size.y%%]", game) == "The size is: [%640,% 480%]");
	CHECK(GameUtils::replaceStringWithVarOrProp("keepAR: %game.keepAR%, radius: %obj.radius.y%", game) == "keepAR: true, radius: 2");
}
