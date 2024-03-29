#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>

class GradientCircle : public sf::Drawable, public sf::Transformable
{
private:
	sf::VertexArray vertices{ sf::TriangleFan };
	sf::Color innerColor{ sf::Color::White };
	sf::Color outerColor{ sf::Color::White };
	sf::FloatRect bounds{};
	float radius{ 0 };
	size_t pointCount{ 0 };

	void updateColors();
	void update();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
	explicit GradientCircle(float radius_ = 0, size_t pointCount = 30);

	auto& getInnerColor() const { return innerColor; }
	auto& getOuterColor() const { return outerColor; }
	auto getRadius() const { return radius; }
	auto getPointCount() const { return pointCount; }
	sf::Vector2f getPoint(size_t index) const;

	auto getLocalBounds() const { return bounds; }
	sf::FloatRect getGlobalBounds() const;

	void setInnerColor(const sf::Color& color);
	void setOuterColor(const sf::Color& color);
	void setRadius(float radius_);
	void setPointCount(size_t count);
};
