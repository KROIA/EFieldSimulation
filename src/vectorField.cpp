#include "vectorField.h"

VectorField::VectorField(const sf::Vector2f& dim,
			   const sf::Vector2u& resolution)
	: m_spaceDimension(dim)
	, m_maxVectorLength(1000000)
	, m_minVectorLength(10e-5)
{
	m_vectorGridColor = sf::Color(255, 255, 0);
	setGridSize(resolution);
}

VectorField::~VectorField()
{
	clearVectorGrid();
}
const VectorField& VectorField::operator+=(const VectorField& other)
{
	if (other.m_gridSize != m_gridSize ||
		other.m_spaceDimension != m_spaceDimension)
	{
		PRINT_ERROR("Other VectorField has not the same dimensions");
		return *this;
	}
	for (size_t x = 0; x < m_gridSize.x; ++x)
		for (size_t y = 0; y < m_gridSize.y; ++y)
			m_vectorGrid[x][y]->setVector(m_vectorGrid[x][y]->getVector() +
										  other.m_vectorGrid[x][y]->getVector());
	return *this;

}

void VectorField::setMaxVectorLength(float length)
{
	m_maxVectorLength = length;
}
float VectorField::getMaxVectorLength() const
{
	return m_maxVectorLength;
}
void VectorField::setMinVectorLength(float length)
{
	m_minVectorLength = length;
}
float VectorField::getMinVectorLength() const
{
	return m_minVectorLength;
}

void VectorField::setSpaceDimension(const sf::Vector2f& dim)
{
	m_spaceDimension = dim;
	clearVectorGrid();
	buildVectorGrid();
}
void VectorField::setSpaceDimension(float x, float y)
{
	m_spaceDimension.x = x;
	m_spaceDimension.y = y;
	clearVectorGrid();
	buildVectorGrid();
}
const sf::Vector2f& VectorField::getSpaceDimension() const
{
	return m_spaceDimension;
}

void VectorField::setGridSize(const sf::Vector2u& size)
{
	if (size.x == 0 || size.y == 0)
	{
		PRINT_ERROR("Gridsize can't be 0 in any dimension")
			return;
	}

	m_gridSize = size;
	clearVectorGrid();
	buildVectorGrid();
}
void VectorField::setGridSize(size_t x, size_t y)
{
	if (x == 0 || y == 0)
	{
		PRINT_ERROR("Gridsize can't be 0 in any dimension")
			return;
	}
	m_gridSize.x = x;
	m_gridSize.y = y;
	clearVectorGrid();
	buildVectorGrid();
}

const sf::Vector2u& VectorField::getGridSize() const
{
	return m_gridSize;
}

void VectorField::setGridColor(const sf::Color& color)
{
	m_vectorGridColor = color;
	for (size_t x = 0; x < m_vectorGrid.size(); ++x)
		for (size_t y = 0; y < m_vectorGrid[x].size(); ++y)
			m_vectorGrid[x][y]->setColor(m_vectorGridColor);
}
const sf::Color& VectorField::getGridColor() const
{
	return m_vectorGridColor;
}

void VectorField::setGridVector(const sf::Vector2u& index,
						   const sf::Vector2f& vec)
{
	if (index.x >= m_gridSize.x || index.y >= m_gridSize.y)
	{
		PRINT_ERROR("Index out of range: x = " + std::to_string(index.x) +
					" y = " + std::to_string(index.y))
			return;
	}
	m_vectorGrid[index.x][index.y]->setVector(vec);
}
const sf::Vector2f& VectorField::getGridVector(const sf::Vector2u& index)
{
	if (m_vectorGrid.size() >= index.x ||
		m_vectorGrid[0].size() >= index.y)
	{
		PRINT_ERROR("Index out of range: x = " + std::to_string(index.x) +
					" y = " + std::to_string(index.y))
			return sf::Vector2f(0, 0);
	}
	return m_vectorGrid[index.x][index.y]->getVector();
}
void VectorField::setVector(const sf::Vector2f& index,
					   const sf::Vector2f& vec)
{
	sf::Vector2u uIndex((index.x * m_gridSize.x) / m_spaceDimension.x,
						(index.y * m_gridSize.y) / m_spaceDimension.y);
	setGridVector(uIndex, vec);
}
const sf::Vector2f& VectorField::getVector(const sf::Vector2f& index)
{
	sf::Vector2u uIndex((index.x * m_gridSize.x) / m_spaceDimension.x,
						(index.y * m_gridSize.y) / m_spaceDimension.y);
	return getGridVector(uIndex);
}

void VectorField::addChargeParticle(ChargeParticle* p)
{
	for (size_t i = 0; i < m_ChargeParticles.size(); ++i)
		if (m_ChargeParticles[i] == p)
			return;
	m_ChargeParticles.push_back(p);
}

void VectorField::removeChargeParticle(ChargeParticle* p)
{
	for (size_t i = 0; i < m_ChargeParticles.size(); ++i)
		if (m_ChargeParticles[i] == p)
		{
			m_ChargeParticles.erase(m_ChargeParticles.begin() + i);
			return;
		}
}

void VectorField::clearChargeParticles()
{
	m_ChargeParticles.clear();
}


void VectorField::calculatePhysics(float timeIntervalSec)
{
	calculateField();
	/*for (ChargeParticle* p : m_ChargeParticles)
	{
		if (p->isStatic())
			continue;
		p->calculatePhysiscs(m_ChargeParticles, timeIntervalSec);
	}
	applyPhysics();
	checkChargeParticleBounds();*/
}
void VectorField::draw(sf::RenderWindow* window,
				  const sf::Vector2f& offset)
{
	for (size_t x = 0; x < m_vectorGrid.size(); ++x)
		for (size_t y = 0; y < m_vectorGrid[x].size(); ++y)
		{
			m_vectorGrid[x][y]->draw(window, offset);
		}
}

void VectorField::clearVectorGrid()
{
	for (size_t x = 0; x < m_vectorGrid.size(); ++x)
		for (size_t y = 0; y < m_vectorGrid[x].size(); ++y)
			delete m_vectorGrid[x][y];
	m_vectorGrid.clear();
}
void VectorField::buildVectorGrid()
{
	sf::Vector2f vectorSpacing(m_spaceDimension.x / m_gridSize.x,
							   m_spaceDimension.y / m_gridSize.y);
	m_vectorLength = VectorMath::getLength(vectorSpacing) / 2.f;
	m_vectorGrid.reserve(m_gridSize.x);
	for (size_t x = 0; x < m_gridSize.x; ++x)
	{
		m_vectorGrid.push_back(vector<VectorPainter*>());
		m_vectorGrid[x].reserve(m_gridSize.y);
		for (size_t y = 0; y < m_gridSize.y; ++y)
		{
			VectorPainter* vector = new VectorPainter;
			vector->setPos(vectorSpacing.x / 2.f + vectorSpacing.x * x,
						   vectorSpacing.y / 2.f + vectorSpacing.y * y);
			vector->setColor(m_vectorGridColor);
			vector->setOriginFactor(0.5);
			//vector->setDisplayLength(m_vectorLength);
			m_vectorGrid[x].push_back(vector);
		}
	}
}




void VectorField::applyPhysics()
{
	for (ChargeParticle* p : m_ChargeParticles)
	{
		if (p->isStatic())
			continue;
		p->applyPhysics();
	}
}
void VectorField::checkChargeParticleBounds()
{
	for (ChargeParticle* p : m_ChargeParticles)
	{
		if (p->isStatic())
			continue;
		sf::Vector2f pos = p->getPos();
		sf::Vector2f deltaPos = p->getDeltaPos();
		sf::Vector2f velocity = p->getVelocity();

		sf::Vector2f newDeltaPos(0, 0);

		float offset = 1;

		if (pos.x <= 0)
		{
			newDeltaPos.x = -pos.x + offset;
			velocity.x = 0;
		}
		else if (pos.x >= m_spaceDimension.x)
		{
			newDeltaPos.x = m_spaceDimension.x - pos.x - offset;
			velocity.x = 0;
		}
		if (pos.y <= 0)
		{
			newDeltaPos.y = -pos.y + offset;
			velocity.y = 0;
		}
		else if (pos.y >= m_spaceDimension.y)
		{
			newDeltaPos.y = m_spaceDimension.y - pos.y - offset;
			velocity.y = 0;
		}
		p->move(newDeltaPos);
		p->setVelocity(velocity);

	}
}