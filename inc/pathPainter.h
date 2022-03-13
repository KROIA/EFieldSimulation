#pragma once

#include "drawable.h"
#include <vector>

using std::vector;

class PathPainter : public Drawable
{
	public:
	PathPainter(Drawable *target);
	~PathPainter();

	void setColor(const sf::Color& color);
	const sf::Color& getColor() const;

	void setPathLength(size_t length);
	size_t getPathLength() const;

	void clear();


	void draw(sf::RenderWindow* window,
		      const sf::Vector2f& offset = sf::Vector2f(0, 0));
	void drawDebug(sf::RenderWindow* window,
				   const sf::Vector2f& offset = sf::Vector2f(0, 0));


	protected:
	void getNewPoint();
	void swapBuffer();

	Drawable* m_target;
	sf::Color m_color;
	size_t m_pathLength;

	vector<sf::Vertex> m_path;
	size_t m_startPointOffset; 
	size_t m_currentPoint;
};