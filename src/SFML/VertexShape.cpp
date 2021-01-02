#include "VertexShape.h"
#include <cmath>
#include <SFML/Graphics/RenderTarget.hpp>

void VertexShape::update()
{
	bounds = vertices.getBounds();
}

void VertexShape::addVertex(const sf::Vertex& v)
{
	vertices.append(v);
	update();
}

sf::FloatRect VertexShape::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}

void VertexShape::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(vertices, states);
}
