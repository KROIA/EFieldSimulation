#include "distributionPlot.h"

DistributionPlot::DistributionPlot()
{
	m_nElements = 10;
	m_dimension.x = 100;
	m_dimension.y = 10;
	m_maxCount = 0;
	m_allignement = Allignement::x;
}
DistributionPlot::~DistributionPlot()
{

}

void DistributionPlot::setDimension(const sf::Vector2f& dim)
{
	m_dimension = dim;
}
const sf::Vector2f& DistributionPlot::getDimension() const
{
	return m_dimension;
}
void DistributionPlot::setXResolution(size_t count)
{
	m_nElements = count;
	m_plotCountList = vector<int>(m_nElements, 0);
}
size_t DistributionPlot::getXResolution() const
{
	return m_nElements;
}

void DistributionPlot::addCount(size_t x, int count)
{
	if (m_plotCountList.size() <= x)
		return;
	m_plotCountList[x] += count;
	if (m_plotCountList[x] < 0)
		m_plotCountList[x] = 0;
	if (m_maxCount < m_plotCountList[x])
		m_maxCount = m_plotCountList[x];
}
int DistributionPlot::getCount(size_t x) const
{
	if (m_plotCountList.size() <= x)
		return 0;
	return m_plotCountList[x];
}
void DistributionPlot::clearCount()
{
	m_plotCountList = vector<int>(m_nElements, 0);
	m_maxCount = 0;
}
void DistributionPlot::setAllignement(Allignement a)
{
	m_allignement = a;
}
DistributionPlot::Allignement DistributionPlot::getAllignement() const
{
	return m_allignement;
}

void DistributionPlot::draw(sf::RenderWindow* window,
		                    const sf::Vector2f& offset)
{
	sf::Vector2f deltaDim(0,0);
	sf::Vector2f rectDim(0,0);
	if (m_allignement == Allignement::x)
	{
		rectDim.x = m_dimension.x / (float)m_plotCountList.size();
		rectDim.y = m_dimension.y;
		deltaDim.x = rectDim.x;
	}
	else
	{
		rectDim.x = m_dimension.x;
		rectDim.y = m_dimension.y / (float)m_plotCountList.size();
		deltaDim.y = rectDim.y;
	}
	
	sf::Vertex* linePath = new sf::Vertex[m_plotCountList.size()];
	sf::Color pathColor(0, 255, 0);
	for (size_t x = 0; x < m_plotCountList.size(); ++x)
	{

		sf::RectangleShape rectangle(rectDim);
		sf::Vector2f rectPos = (float)x * deltaDim + m_pos + offset;
		rectangle.setPosition(rectPos);
		rectangle.setFillColor(getColor(m_plotCountList[x]));
		window->draw(rectangle);

		float counts = m_plotCountList[x];
		if (m_allignement == Allignement::x)
		{
			
			linePath[x].position = sf::Vector2f(rectPos.x, rectPos.y+mapF(counts* counts,-1,m_maxCount* m_maxCount +1, rectDim.y,0)) + deltaDim / 2.f;
		}
		else
		{
			linePath[x].position = sf::Vector2f(rectPos.x + mapF(counts* counts, -1, m_maxCount* m_maxCount +1, rectDim.x, 0), rectPos.y) + deltaDim / 2.f;
		}
		linePath[x].color = pathColor;


		//std::cout << x << " c:" << m_plotCountList[x];
	}
	window->draw(linePath, m_plotCountList.size(), sf::LineStrip);
	delete[] linePath;
	//std::cout << "\n";
}
sf::Color DistributionPlot::getColor(int elements)
{
	sf::Color col(0,0,0);
	col.r = mapF(elements, 0, m_maxCount, 0, 255);
	col.g = mapF(elements* elements, 0, m_maxCount* m_maxCount, 0, 255);
	return col;
}