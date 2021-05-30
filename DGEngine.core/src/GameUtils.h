#pragma once

#include "Alignment.h"
#include "Anchor.h"
#include "AnimationType.h"
#include "BindingFlags.h"
#include "BlendMode.h"
#include "IgnoreResource.h"
#include "InputEvent.h"
#include "Palette.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <string>
#include <string_view>
#include "UIObject.h"
#include <vector>

class Game;

namespace GameUtils
{
	const int DoubleClickDelay = 500;

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

	HorizontalAlign getHorizontalAlignment(const std::string_view str,
		HorizontalAlign val = HorizontalAlign::Left);

	VerticalAlign getVerticalAlignment(const std::string_view str,
		VerticalAlign val = VerticalAlign::Top);

	sf::Keyboard::Key getKeyCode(int num, sf::Keyboard::Key val) noexcept;

	// also gets extra keys (cast as sf::Keyboard::Key)
	sf::Keyboard::Key getKeyCode(const std::string_view str, sf::Keyboard::Key val);

	IgnoreResource getIgnoreResource(const std::string_view str, IgnoreResource val);

	InputEventType getInputEventType(const std::string_view str, InputEventType val);

	sf::PrimitiveType getPrimitiveType(const std::string_view str,
		sf::PrimitiveType val = sf::PrimitiveType::TriangleFan);

	sf::Time getTime(int fps);

	Variable getTime(sf::Time time, std::string_view format, bool roundUp = false);

	// replaces "%str%" with obj.getProperty("str")
	std::string replaceStringWithQueryable(const std::string_view str,
		const Queryable& obj, char token = '%');

	// replaces "%str%" with game.getVarOrProp("str")
	std::string replaceStringWithVarOrProp(const std::string_view str,
		const Game& obj, char token = '%');
}
