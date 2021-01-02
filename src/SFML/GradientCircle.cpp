#include "GradientCircle.h"
#include <cmath>
#include <SFML/Graphics/RenderTarget.hpp>

GradientCircle::GradientCircle(float radius_, size_t pointCount)
	: radius(radius_), pointCount(pointCount)
{
	update();
}

sf::FloatRect GradientCircle::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}

void GradientCircle::setInnerColor(const sf::Color& color)
{
	innerColor = color;
	updateColors();
}

void GradientCircle::setOuterColor(const sf::Color& color)
{
	outerColor = color;
	updateColors();
}

void GradientCircle::setRadius(float radius_)
{
	radius = radius_;
	update();
}

void GradientCircle::setPointCount(size_t count)
{
	pointCount = count;
	update();
}

sf::Vector2f GradientCircle::getPoint(size_t index) const
{
	static const float pi = 3.141592654f;

	float angle = index * 2 * pi / pointCount - pi / 2;
	float x = std::cos(angle) * radius;
	float y = std::sin(angle) * radius;

	return sf::Vector2f(radius + x, radius + y);
}

void GradientCircle::update()
{
	// Get the total number of points of the shape
	std::size_t count = getPointCount();
	if (count < 3)
	{
		vertices.resize(0);
		return;
	}

	vertices.resize(count + 2); // + 2 for center and repeated first point

	// Position
	for (std::size_t i = 0; i < count; ++i)
	{
		vertices[i + 1].position = getPoint(i);
	}
	vertices[count + 1].position = vertices[1].position;

	// Update the bounding rectangle
	vertices[0] = vertices[1]; // so that the result of getBounds() is correct
	bounds = vertices.getBounds();

	// Compute the center and make it the first vertex
	vertices[0].position.x = bounds.left + bounds.width / 2;
	vertices[0].position.y = bounds.top + bounds.height / 2;

	// Color
	updateColors();
}

void GradientCircle::updateColors()
{
	if (vertices.getVertexCount() == 0)
	{
		return;
	}
	vertices[0].color = innerColor;
	for (size_t i = 1; i < vertices.getVertexCount(); i++)
	{
		vertices[i].color = outerColor;
	}
}

void GradientCircle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(vertices, states);
}
