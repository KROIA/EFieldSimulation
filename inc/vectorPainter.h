#pragma once

#include "drawable.h"


class VectorPainter : public Drawable
{
	public:
	VectorPainter();
	~VectorPainter();

	void setColor(const sf::Color& color);
	void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
	const sf::Color& getColor() const;

	void setVector(const sf::Vector2f vec);
	void setVector(float x, float y);
	const sf::Vector2f& getVector() const;

	void setDisplayLength(float length);
	float getDisplayLength() const;

	void norm(float normScale = 1);
	void setLength(float length);
	float getLength() const;
	void setAngle(float rad);
	float getAngle() const;


	// Virtual Implementation
	virtual void draw(sf::RenderWindow* window,
					  const sf::Vector2f& offset = sf::Vector2f(0, 0));

	static void setMaxVectorLength(float length);
	protected:
	sf::Color m_color;
	sf::Vector2f m_vector;
	float m_displayLength;
	float m_length;

	static float m_maxLength;
};