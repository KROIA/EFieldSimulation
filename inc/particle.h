#pragma once

#include "drawable.h"

class Particle : public Drawable
{
	public:
	Particle();
	virtual ~Particle();

	void setRadius(float radius);
	float getRadius() const;

	void setColor(const sf::Color& col);
	const sf::Color& getColor() const;



	void setVelocity(const sf::Vector2f& vel);
	const sf::Vector2f& getVelocity() const;


	virtual void calculatePhysiscs(float timeIntervalSec);
	virtual void applyPhysics();

	virtual void draw(sf::RenderWindow* window,
					  const sf::Vector2f& offset = sf::Vector2f(0, 0));

	protected:

	sf::Vector2f m_velocity;
	sf::Vector2f m_deltaPos;
	sf::Color m_color;
	float m_radius;
};