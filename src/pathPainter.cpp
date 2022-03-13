#include "pathPainter.h"

PathPainter::PathPainter(Drawable* target)
{
	m_target = nullptr;
	m_startPointOffset = 0;
	m_currentPoint = 0;
	m_color = sf::Color(0, 150, 180);
	setPathLength(100);
	

	PTR_CHECK_NULLPTR(target, return)
	m_target = target;
}
PathPainter::~PathPainter()
{
	
}

void PathPainter::setColor(const sf::Color& color)
{
	m_color = color;
	for (sf::Vertex& v : m_path)
		v.color = m_color;
}
const sf::Color& PathPainter::getColor() const
{
	return m_color;
}

void PathPainter::setPathLength(size_t length)
{
	m_pathLength = length;
	clear();
}
size_t PathPainter::getPathLength() const
{
	return m_pathLength;
}

void PathPainter::clear()
{
	sf::Vertex preset;
	preset.color = m_color;
	m_path = vector<sf::Vertex>(m_pathLength * 2, preset);
	m_startPointOffset = 0;
	m_currentPoint = 0;

}

void PathPainter::draw(sf::RenderWindow* window,
		  const sf::Vector2f& offset)
{
	getNewPoint();
	window->draw(m_path.data() + m_startPointOffset, m_currentPoint - m_startPointOffset, sf::LineStrip);
}
void PathPainter::drawDebug(sf::RenderWindow* window,
							const sf::Vector2f& offset)
{

}
void PathPainter::getNewPoint()
{
	if (m_currentPoint >= m_path.size())
	{
		swapBuffer();
		m_currentPoint = m_startPointOffset;
		m_startPointOffset = 0;
	}
	else if (m_currentPoint >= m_path.size() / 2)
		++m_startPointOffset;
	m_path[m_currentPoint++].position = m_target->getPos();
	
	

}
void PathPainter::swapBuffer()
{
	sf::Vertex* start = m_path.data();
	size_t size = m_path.size() / 2;
	memcpy(start, start+size, size*sizeof(sf::Vertex));
}