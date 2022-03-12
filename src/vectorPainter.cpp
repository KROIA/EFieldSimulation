#include "vectorPainter.h"


VectorPainter::VectorPainter()
{
	setVector(VectorMath::getUnitVector());
	setColor(sf::Color::White);
	useLogScale(true,5);
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
}
void VectorPainter::setVector(float x, float y)
{
	m_vector.x = x;
	m_vector.y = y;
}
const sf::Vector2f& VectorPainter::getVector() const
{
	return m_vector;
}

void VectorPainter::norm(float normScale)
{
	m_vector = VectorMath::getNormalized(m_vector, normScale);
}
void VectorPainter::setLength(float length)
{
	norm(length);
}
float VectorPainter::getLength() const
{
	return VectorMath::getLength(m_vector);
}

void VectorPainter::setAngle(float rad)
{
	m_vector = VectorMath::getRotated(m_vector, rad);
}
float VectorPainter::getAngle() const
{
	return VectorMath::getAngle(m_vector);
}

void VectorPainter::useLogScale(bool enable, float scale)
{
	m_useLogScale = enable;
	m_logScale = 1;
	if (scale != 0)
		m_logScale = scale;
}



// Virtual Implementation
void  VectorPainter::draw(sf::RenderWindow* window,
						  const sf::Vector2f& offset)
{
	const float arrowTipScale = 0.1f;
	const float arrowTipAngle = 3.14159265 * 5.f/ 6.f;

	float angle = getAngle();
	float length = getLength();

	sf::Vector2f normalizedVec = VectorMath::getNormalized(m_vector);

	
	//std::cout << length << "\n";
	if (m_useLogScale)
	{
		length = log(1 + length) * log(m_logScale);
	}
	m_color = getColorFromSignal(length, 0, 35);

	float arrowTipLength = arrowTipScale * length;
	if (arrowTipLength < 2)
		arrowTipLength = 2;

	sf::Vector2f arrowTip1 = 
		VectorMath::getRotated(VectorMath::getUnitVector() * arrowTipLength,
							   angle + arrowTipAngle);
	sf::Vector2f arrowTip2 = 
		VectorMath::getRotated(VectorMath::getUnitVector() * arrowTipLength,
							   angle - arrowTipAngle);
	normalizedVec *= length;
	sf::Vertex line[] =
	{
		sf::Vertex(m_pos,m_color),
		sf::Vertex(m_pos + normalizedVec,m_color),

		sf::Vertex(m_pos + normalizedVec,m_color),
		sf::Vertex(m_pos + normalizedVec + arrowTip1,m_color),

		sf::Vertex(m_pos + normalizedVec,m_color),
		sf::Vertex(m_pos + normalizedVec + arrowTip2,m_color)
	};

	window->draw(line, 6, sf::Lines);
}