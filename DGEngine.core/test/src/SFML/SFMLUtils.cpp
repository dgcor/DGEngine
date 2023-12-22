#include "catch2/catch_test_macros.hpp"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "SFML/SFMLUtils.h"

TEST_CASE("SFMLUtils::getBlendMode", "SFMLUtils")
{
	CHECK(SFMLUtils::getBlendMode(BlendMode::Alpha) == sf::BlendAlpha);
	CHECK(SFMLUtils::getBlendMode(BlendMode::Add) == sf::BlendAdd);
	CHECK(SFMLUtils::getBlendMode(BlendMode::Multiply) == sf::BlendMultiply);
	CHECK(SFMLUtils::getBlendMode(BlendMode::Min) == sf::BlendMin);
	CHECK(SFMLUtils::getBlendMode(BlendMode::Max) == sf::BlendMax);
	CHECK(SFMLUtils::getBlendMode(BlendMode::None) == sf::BlendNone);
}

TEST_CASE("SFMLUtils::rgbToColor", "SFMLUtils")
{
	CHECK(SFMLUtils::rgbToColor(0xAABBCC) == sf::Color(0xAA, 0xBB, 0xCC, 0xFF));
}

TEST_CASE("SFMLUtils::rgbaToColor", "SFMLUtils")
{
	CHECK(SFMLUtils::rgbaToColor(0xAABBCCDD) == sf::Color(0xAA, 0xBB, 0xCC, 0xDD));
}

TEST_CASE("SFMLUtils::stringToColor", "SFMLUtils")
{
	CHECK(SFMLUtils::stringToColor("0xAABBCC").value() == sf::Color(0xAA, 0xBB, 0xCC, 0xFF));
	CHECK(SFMLUtils::stringToColor("0xaabbcc").value() == sf::Color(0xAA, 0xBB, 0xCC, 0xFF));
	CHECK(SFMLUtils::stringToColor("0XAABBCC").value() == sf::Color(0xAA, 0xBB, 0xCC, 0xFF));
	CHECK(SFMLUtils::stringToColor("0xAABBCCDD").value() == sf::Color(0xAA, 0xBB, 0xCC, 0xDD));
	CHECK(SFMLUtils::stringToColor("0xaabbccdd").value() == sf::Color(0xAA, 0xBB, 0xCC, 0xDD));
	CHECK(SFMLUtils::stringToColor("0XAABBCCDD").value() == sf::Color(0xAA, 0xBB, 0xCC, 0xDD));
	CHECK(SFMLUtils::stringToColor("0XAABBCCDDD").has_value() == false);
	CHECK(SFMLUtils::stringToColor("invalid").has_value() == false);
}

TEST_CASE("SFMLUtils::spriteCenterTexture", "SFMLUtils")
{
	sf::Image img;
	img.create(64, 64);

	sf::Texture texture;
	texture.loadFromImage(img);

	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(10, 10, 32, 32));

	auto rect = sprite.getTextureRect();
	auto orig = sprite.getOrigin();

	CHECK(rect.left == 10);
	CHECK(rect.top == 10);
	CHECK(rect.width == 32);
	CHECK(rect.height == 32);
	CHECK(orig.x == 0.f);
	CHECK(orig.y == 0.f);

	SFMLUtils::spriteCenterTexture(sprite);
	rect = sprite.getTextureRect();
	orig = sprite.getOrigin();

	CHECK(rect.left == 16);
	CHECK(rect.top == 16);
	CHECK(rect.width == 32);
	CHECK(rect.height == 32);
	CHECK(orig.x == 0.f);
	CHECK(orig.y == 0.f);

	sprite.setTextureRect(sf::IntRect(0, 0, 80, 80));
	SFMLUtils::spriteCenterTexture(sprite);
	rect = sprite.getTextureRect();
	orig = sprite.getOrigin();

	CHECK(rect.left == 0);
	CHECK(rect.top == 0);
	CHECK(rect.width == 80);
	CHECK(rect.height == 80);
	CHECK(orig.x == -8.f);
	CHECK(orig.y == -8.f);
}
