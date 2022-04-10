#include "particle.h"

Particle::Particle()
{
	setColor(sf::Color::Green);
	setRadius(10);
}
Particle::~Particle()
{

}

void Particle::setRadius(float radius)
{
	m_radius = radius;
}
float Particle::getRadius() const
{
	return m_radius;
}

void Particle::setColor(const sf::Color& col)
{
	m_color = col;
}
const sf::Color& Particle::getColor() const
{
	return m_color;
}

void Particle::setVelocity(const sf::Vector2f& vel)
{
	m_velocity = vel;
}
const sf::Vector2f& Particle::getVelocity() const
{
	return m_velocity;
}


void Particle::calculatePhysiscs(float timeIntervalSec)
{
	m_deltaPos = m_velocity * timeIntervalSec;
}
void Particle::applyPhysics()
{
	m_pos += m_deltaPos;
}


void Particle::draw(sf::RenderWindow* window,
				  const sf::Vector2f& offset)
{
	sf::CircleShape circle(m_radius);
	circle.setFillColor(m_color);
	circle.setOrigin(sf::Vector2f(m_radius, m_radius));
	circle.setPosition(m_pos + offset);
	window->draw(circle);
}