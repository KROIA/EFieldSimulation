#include "vectorPainter.h"

float VectorPainter::m_maxLength = 1000000;
VectorPainter::VectorPainter()
{
	setVector(VectorMath::getUnitVector());
	setColor(sf::Color::White);
	setDisplayLength(10);
}
VectorPainter::~VectorPainter()
{

}

void VectorPainter::setColor(const sf::Color& color)
{
	m_color = color;
}
void VectorPainter::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	m_color.r = r;
	m_color.g = g;
	m_color.b = b;
	m_color.a = a;
}
const sf::Color& VectorPainter::getColor() const
{
	return m_color;
}

void VectorPainter::setVector(const sf::Vector2f vec)
{
	m_vector = vec;
	m_length = VectorMath::getLength(m_vector);
}
void VectorPainter::setVector(float x, float y)
{
	m_vector.x = x;
	m_vector.y = y;
	m_length = VectorMath::getLength(m_vector);
}
const sf::Vector2f& VectorPainter::getVector() const
{
	return m_vector;
}

void VectorPainter::setDisplayLength(float length)
{
	m_displayLength = length;
}
float VectorPainter::getDisplayLength() const
{
	return m_displayLength;
}

void VectorPainter::norm(float normScale)
{
	m_vector = VectorMath::getNormalized(m_vector, normScale);
}
void VectorPainter::setLength(float length)
{
	norm(length);
	m_length = length;
}
float VectorPainter::getLength() const
{
	return m_length;
}

void VectorPainter::setAngle(float rad)
{
	m_vector = VectorMath::getRotated(m_vector, rad);
}
float VectorPainter::getAngle() const
{
	return VectorMath::getAngle(m_vector);
}


void VectorPainter::setMaxVectorLength(float length)
{
	m_maxLength = length;
}

// Virtual Implementation
void  VectorPainter::draw(sf::RenderWindow* window,
						  const sf::Vector2f& offset)
{
	if (m_length == 0)
		return;
	const float arrowTipScale = 0.1f;
	const float arrowTipAngle = 3.14159265 * 5.f/ 6.f;

	float angle = getAngle();

	sf::Vector2f normalizedVec = VectorMath::getNormalized(m_vector, m_displayLength);

	
	//std::cout << length << "\n";

	
	m_color = getColorFromSignal(m_length, 0, m_maxLength);

	float arrowTipLength = arrowTipScale * m_displayLength;
	if (arrowTipLength < 2)
		arrowTipLength = 2;

	sf::Vector2f arrowTip1 = 
		VectorMath::getRotated(VectorMath::getUnitVector() * arrowTipLength,
							   angle + arrowTipAngle);
	sf::Vector2f arrowTip2 = 
		VectorMath::getRotated(VectorMath::getUnitVector() * arrowTipLength,
							   angle - arrowTipAngle);
	sf::Vector2f rotOffset = normalizedVec / 2.f;
	sf::Vertex line[] = 
	{
		sf::Vertex(m_pos - rotOffset,m_color),
		sf::Vertex(m_pos + rotOffset,m_color),

		sf::Vertex(m_pos + rotOffset,m_color),
		sf::Vertex(m_pos + rotOffset + arrowTip1,m_color),

		sf::Vertex(m_pos + rotOffset,m_color),
		sf::Vertex(m_pos + rotOffset + arrowTip2,m_color)
	};

	window->draw(line, 6, sf::Lines);
}