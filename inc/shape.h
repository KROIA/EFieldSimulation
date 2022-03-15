#pragma once

#include <vector>
#include "particle.h"
#include "drawable.h"
#include "vectorPainter.h"

using std::vector;

class Shape : public Drawable
{
	public:
	Shape();
	~Shape();

	void setPoints(const vector<sf::Vector2f> &points);
	vector<sf::Vector2f> getPoints() const;

	//void setOrigin(const sf::Vector2f& origin);
	//const sf::Vector2f& getOrigin() const;

	void setRotation(float angle);
	float getRotation() const;

	void calculatePhysics(float deltaTime);
	void checkCollision(const vector<Particle*>& particles);

	virtual void draw(sf::RenderWindow* window,
					  const sf::Vector2f& offset = sf::Vector2f(0, 0));

	virtual void drawDebug(sf::RenderWindow* window,
						   const sf::Vector2f& offset = sf::Vector2f(0, 0));

	static const vector<sf::Vector2f> getGenerateRect(const sf::Vector2f& size);
	protected:

	void updateGlobalPoints();
	void updateBoundingBox();
	void normalizeCenterOfMass();

	sf::Color m_color;
	vector<sf::Vertex> m_localPoints;
	vector<sf::Vertex> m_globalPoints;
	sf::FloatRect m_boundingBox;

	vector<sf::Vector2f> m_collisionPoints;
	vector<VectorPainter> m_vectors;

	//sf::Vector2f m_localCenterOfMass;
	float m_angle;
};