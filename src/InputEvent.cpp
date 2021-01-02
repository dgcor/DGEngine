#include "InputEvent.h"
#include "Utils/Utils.h"

size_t CompareEvent::operator()(const sf::Event& obj) const noexcept
{
	std::size_t hash = 0;
	Utils::hashCombine(hash, obj.type);
	switch (obj.type)
	{
	case sf::Event::Closed:
	case sf::Event::Resized:
	case sf::Event::LostFocus:
	case sf::Event::GainedFocus:
	case sf::Event::TextEntered:
		break;
	case sf::Event::KeyPressed:
	case sf::Event::KeyReleased:
	{
		Utils::hashCombine(hash, obj.key.code);
		Utils::hashCombine(hash, obj.key.alt);
		Utils::hashCombine(hash, obj.key.control);
		Utils::hashCombine(hash, obj.key.shift);
		Utils::hashCombine(hash, obj.key.system);
		break;
	}
	case sf::Event::MouseWheelScrolled:
	case sf::Event::MouseButtonPressed:
	case sf::Event::MouseButtonReleased:
	case sf::Event::MouseMoved:
	case sf::Event::TouchBegan:
	case sf::Event::TouchMoved:
	case sf::Event::TouchEnded:
		break;
	default:
		break;
	}
	return hash;
}

bool CompareEvent::compare(const sf::Event& obj1, const sf::Event& obj2) noexcept
{
	if (obj1.type != obj2.type)
	{
		return false;
	}
	switch (obj1.type)
	{
	case sf::Event::Closed:
	case sf::Event::Resized:
	case sf::Event::LostFocus:
	case sf::Event::GainedFocus:
	case sf::Event::TextEntered:
		return obj1.type == obj2.type;
	case sf::Event::KeyPressed:
	case sf::Event::KeyReleased:
	{
		return (obj1.key.code == obj2.key.code &&
			obj1.key.alt == obj2.key.alt &&
			obj1.key.control == obj2.key.control &&
			obj1.key.shift == obj2.key.shift &&
			obj1.key.system == obj2.key.system);
	}
	case sf::Event::MouseWheelScrolled:
	case sf::Event::MouseButtonPressed:
	case sf::Event::MouseButtonReleased:
	case sf::Event::MouseMoved:
	case sf::Event::TouchBegan:
	case sf::Event::TouchMoved:
	case sf::Event::TouchEnded:
		return obj1.type == obj2.type;
	default:
		return obj1.type == obj2.type;
	}
}

bool CompareEvent::operator()(const sf::Event& obj1, const sf::Event& obj2) const noexcept
{
	return compare(obj1, obj2);
}

bool InputEvent::isActive() const
{
	switch (type)
	{
	case InputType::Mouse:
		return sf::Mouse::isButtonPressed((sf::Mouse::Button)(value));
	case InputType::Keyboard:
		return sf::Keyboard::isKeyPressed((sf::Keyboard::Key)(value));
	case InputType::Joystick:
		return sf::Joystick::isButtonPressed(0, (unsigned int)(value));
	default:
		return false;
	}
}
