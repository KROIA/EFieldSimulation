#include "drawable.h"



Drawable::Drawable()
	: m_pos(0, 0)
{

}
Drawable::Drawable(const Drawable& other)
	: m_pos(other.m_pos)
{

}
Drawable::~Drawable()
{

}

const Drawable& Drawable::operator=(const Drawable& other)
{
	m_pos = other.m_pos;

	return *this;
}

void Drawable::setPos(const sf::Vector2f& pos)
{
	m_pos = pos;
}
void Drawable::setPos(float x, float y)
{
	m_pos.x = x;
	m_pos.y = y;
}
void Drawable::move(const sf::Vector2f& deltaPos)
{
	m_pos += deltaPos;
}
void Drawable::move(float dx, float dy)
{
	m_pos.x += dx;
	m_pos.y += dy;
}
const sf::Vector2f& Drawable::getPos() const
{
	return m_pos;
}


