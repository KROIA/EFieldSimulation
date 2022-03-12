#pragma once

#include "graphicsUtilities.h"




class Drawable
{
	public:
	Drawable();
	Drawable(const Drawable& other);
	~Drawable();

	const Drawable& operator=(const Drawable& other);

	void setPos(const sf::Vector2f& pos);
	void setPos(float x, float y);
	void move(const sf::Vector2f& deltaPos);
	void move(float dx, float dy);
	const sf::Vector2f &getPos() const;

	virtual void draw(sf::RenderWindow* window,
					  const sf::Vector2f &offset=sf::Vector2f(0,0)) = 0;
	protected:
	sf::Vector2f m_pos;
	private:
	

};
