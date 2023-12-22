#pragma once

#include <functional>
#include "Game/Alignment.h"
#include "Game/Anchor.h"
#include "Game/AnimationType.h"
#include "Game/BindingFlags.h"
#include "Game/BlendMode.h"
#include "Game/Direction.h"
#include "Game/IgnoreResource.h"
#include "Game/InputEvent.h"
#include "Game/UIObject.h"
#include "Resources/Palette.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <string>
#include <string_view>
#include <vector>

class Game;

namespace GameUtils
{
	const int DoubleClickDelay = 500;

	// converts fps (1-1000) to time
	sf::Time FPSToTime(int fps);

	AnimationType getAnimationType(const std::string_view str,
		AnimationType val = AnimationType::Looped);

	sf::Vector2f getAlignmentPosition(const sf::Vector2f& position, const sf::Vector2f& size,
		HorizontalAlign horizAlign, VerticalAlign vertAlign) noexcept;

	sf::Vector2f getAnchorPosition(Anchor srcAnchor, const sf::Vector2f& srcSize,
		const sf::Vector2f& srcPos, const sf::Vector2f& anchorToSize) noexcept;

	void setAnchorPosSize(Anchor anchor, sf::Vector2f& pos, sf::Vector2f& size,
		const sf::Vector2u& oldWindowSize, const sf::Vector2u& newWindowSize) noexcept;

	Anchor getAnchor(const std::string_view str, Anchor val);

	BindingFlags getBindingFlags(const std::string_view str, BindingFlags val);

	BlendMode getBlendMode(const std::string_view str, BlendMode val);

	Palette::ColorFormat getColorFormat(const std::string_view str);

	Direction getDirection(const std::string_view str, Direction val);

	HorizontalAlign getHorizontalAlignment(const std::string_view str,
		HorizontalAlign val = HorizontalAlign::Left);

	VerticalAlign getVerticalAlignment(const std::string_view str,
		VerticalAlign val = VerticalAlign::Top);

	IgnoreResource getIgnoreResource(const std::string_view str, IgnoreResource val);

	InputEventType getInputEventType(const std::string_view str, InputEventType val);

	sf::PrimitiveType getPrimitiveType(const std::string_view str,
		sf::PrimitiveType val = sf::PrimitiveType::TriangleFan);

	sf::Keyboard::Key getKeyCode(int num, sf::Keyboard::Key val = sf::Keyboard::Unknown) noexcept;

	sf::Keyboard::Key getKeyCode(const std::string_view str,
		sf::Keyboard::Key val = sf::Keyboard::Unknown);

	sf::Keyboard::Scancode getScanCode(int num, sf::Keyboard::Scancode val = sf::Keyboard::Scancode::Unknown) noexcept;

	sf::Keyboard::Scancode getScanCode(const std::string_view str,
		sf::Keyboard::Scancode val = sf::Keyboard::Scancode::Unknown);

	std::string getScanCodeDescription(const std::string_view str);

	// converts time to seconds or string
	// roundUp doesn't apply when milliseconds are used
	// (empty) - 0 (int) - seconds
	// ms - 0 (int) - milliseconds
	// s - 0 (int) - seconds
	// S - 0
	// Sf - 0.0
	// Sff - 0.00
	// Sfff - 0.000
	// SS - 00
	// SSf - 00.0
	// SSff - 00.00
	// SSfff - 00.000
	// MSS - 0:00
	// MSSf - 0:00.0
	// MSSff - 0:00.00
	// MSSfff - 0:00.000
	// MMSS - 00:00
	// MMSSf - 00:00.0
	// MMSSff - 00:00.00
	// MMSSfff - 00:00.000
	// HMMSS - 0:00:00
	// HMMSSf - 0:00:00.0
	// HMMSSff - 0:00:00.00
	// HMMSSfff - 0:00:00.000
	// HHMMSS - 00:00:00
	// HHMMSSf - 00:00:00.0
	// HHMMSSff - 00:00:00.00
	// HHMMSSfff - 00:00:00.000
	Variable getTime(sf::Time time, std::string_view format, bool roundUp = false);

	// replaces "%str%" with with function
	std::string replaceStringWithFunction(const std::string_view str, char token,
		const std::function<void(const std::string_view&, std::string&)> stringReplaceFunc);

	// replaces "%str%" with obj.getProperty("str")
	std::string replaceStringWithQueryable(const std::string_view str,
		const Queryable& obj, char token = '%');

	// replaces "%str%" with game.getVarOrProp("str")
	std::string replaceStringWithVarOrProp(const std::string_view str,
		const Game& obj, char token = '%');
}
