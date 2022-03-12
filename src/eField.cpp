#include "eField.h"

EField::EField(const sf::Vector2f& dim,
			   const sf::Vector2u& resolution)
{
	
	m_spaceDimension = dim;
	m_vectorGridColor = sf::Color(255, 255, 0);
	setVectorScale(0.01);
	displayEField(true);
	setGridSize(resolution);
}

EField::~EField()
{
	clearVectorGrid();
}
const EField& EField::operator+=(const EField& other)
{
	if (other.m_gridSize != m_gridSize ||
		other.m_spaceDimension != m_spaceDimension)
	{
		std::cout << "Error: other EField has not the same dimensions";
		return *this;
	}
	for (size_t x = 0; x < m_gridSize.x; ++x)
		for (size_t y = 0; y < m_gridSize.y; ++y)
			m_vectorGrid[x][y]->setVector(m_vectorGrid[x][y]->getVector() + 
										  other.m_vectorGrid[x][y]->getVector());
	return *this;

}

void EField::setSpaceDimension(const sf::Vector2f& dim)
{
	m_spaceDimension = dim;
	clearVectorGrid();
	buildVectorGrid();
}
void EField::setSpaceDimension(float x, float y)
{
	m_spaceDimension.x = x;
	m_spaceDimension.y = y;
	clearVectorGrid();
	buildVectorGrid();
}
const sf::Vector2f& EField::getSpaceDimension() const
{
	return m_spaceDimension;
}

void EField::setGridSize(const sf::Vector2u& size)
{
	if (size.x == 0 || size.y == 0)
		return;
	m_gridSize = size;
	clearVectorGrid();
	buildVectorGrid();
}
void EField::setGridSize(size_t x, size_t y)
{
	if (x == 0 || y == 0)
		return;
	m_gridSize.x = x;
	m_gridSize.y = y;
	clearVectorGrid();
	buildVectorGrid();
}

const sf::Vector2u& EField::getGridSize() const
{
	return m_gridSize;
}

void EField::setGridColor(const sf::Color& color)
{
	m_vectorGridColor = color;
	for (size_t x = 0; x < m_vectorGrid.size(); ++x)
		for (size_t y = 0; y < m_vectorGrid[x].size(); ++y)
			m_vectorGrid[x][y]->setColor(m_vectorGridColor);
}
const sf::Color& EField::getGridColor() const
{
	return m_vectorGridColor;
}
void EField::setVectorScale(float scale)
{
	m_vectorScale = scale;
}
float EField::getVectorScale() const
{
	return m_vectorScale;
}
void EField::setGridVector(const sf::Vector2u& index,
				           const sf::Vector2f& vec)
{
	if (index.x >= m_gridSize.x || index.y >= m_gridSize.y)
		return;
	m_vectorGrid[index.x][index.y]->setVector(vec);
}
const sf::Vector2f& EField::getGridVector(const sf::Vector2u& index)
{
	if (m_vectorGrid.size() >= index.x ||
		m_vectorGrid[0].size() >= index.y)
	{
		std::cout << "Error: position out of grid\n";
		return sf::Vector2f(0, 0);
	}
	return m_vectorGrid[index.x][index.y]->getVector();;
}
void EField::setVector(const sf::Vector2f& index,
					   const sf::Vector2f& vec)
{
	sf::Vector2u uIndex((index.x*m_gridSize.x)/m_spaceDimension.x,
						(index.y * m_gridSize.y) / m_spaceDimension.y);
	setGridVector(uIndex, vec);
}
const sf::Vector2f& EField::getVector(const sf::Vector2f& index)
{
	sf::Vector2u uIndex((index.x * m_gridSize.x) / m_spaceDimension.x,
						(index.y * m_gridSize.y) / m_spaceDimension.y);
	return getGridVector(uIndex);
}

void EField::addParticle(Particle* p)
{
	for (size_t i = 0; i < m_particles.size(); ++i)
		if (m_particles[i] == p)
			return;
	m_particles.push_back(p);
}

void EField::removeParticle(Particle* p)
{
	for (size_t i = 0; i < m_particles.size(); ++i)
		if (m_particles[i] == p)
		{
			m_particles.erase(m_particles.begin() + i);
			return;
		}
}

void EField::clearParticles()
{
	m_particles.clear();
}


void EField::displayEField(bool enable)
{
	m_display_eField = enable;
}

bool EField::displayEField() const
{
	return m_display_eField;
}
void EField::calculatePhysics(float timeIntervalSec)
{
	calculateField();
	for (Particle* p : m_particles)
	{
		if (p->isStatic())
			continue;
		sf::Vector2f pos = p->getPos();
		sf::Vector2i indexPos((m_gridSize.x * pos.x) / m_spaceDimension.x,
							  (m_gridSize.y * pos.y) / m_spaceDimension.y);

		if (indexPos.x >= m_gridSize.x ||
			indexPos.y >= m_gridSize.y)
		{
			std::cout << "Error: position out of grid\n";
			continue;
		}

		p->calculatePhysiscs(m_particles, timeIntervalSec);
	}
	applyPhysics();
	checkParticleBounds();
}
void EField::draw(sf::RenderWindow* window,
				  const sf::Vector2f& offset)
{
	

	if(m_display_eField)
		for (size_t x = 0; x < m_vectorGrid.size(); ++x)
			for (size_t y = 0; y < m_vectorGrid[x].size(); ++y)
			{
				m_vectorGrid[x][y]->draw(window, offset);
			}

	for (Particle* p : m_particles)
	{
		p->draw(window, offset);
	}
}

void EField::clearVectorGrid()
{
	for (size_t x = 0; x < m_vectorGrid.size(); ++x)
		for (size_t y = 0; y < m_vectorGrid[x].size(); ++y)
			delete m_vectorGrid[x][y];
	m_vectorGrid.clear();
}
void EField::buildVectorGrid()
{
	sf::Vector2f vectorSpacing(m_spaceDimension.x / m_gridSize.x,
							   m_spaceDimension.y / m_gridSize.y);
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
			m_vectorGrid[x].push_back(vector);
		}
	}
}

void EField::calculateField()
{
	/*for (Particle* p : m_particles)
	{
		p->calculateVectorField();
	}*/
	for (size_t x = 0; x < m_vectorGrid.size(); ++x)
		for (size_t y = 0; y < m_vectorGrid[x].size(); ++y)
		{
			VectorPainter* vec = m_vectorGrid[x][y];
			sf::Vector2f sum(0, 0);
			sf::Vector2f pos = vec->getPos();
			//sf::Vector2f pos(-(m_gridSize.x /2.f) + (x * m_spaceDimension.x) / m_gridSize.x,
			//				 -(m_gridSize.y /2.f) + (y * m_spaceDimension.y) / m_gridSize.y);

			for (Particle* p : m_particles)
			{
				sum += p->getFieldVector(pos);
			}
			//std::cout << "VectorLength: " << VectorMath::getLength(sum * m_vectorScale)<<"\n";
			vec->setVector(sum * m_vectorScale);
		}
}


void EField::applyPhysics()
{
	for (Particle* p : m_particles)
	{
		if (p->isStatic())
			continue;
		p->applyPhysics();
	}
}
void EField::checkParticleBounds()
{
	auto isInRect = [](const sf::Vector2f& pos,
					   const sf::Vector2f& minBound,
					   const sf::Vector2f& maxBound)
	{
		return	pos.x > minBound.x && pos.y > minBound.y &&
			    pos.x < maxBound.x && pos.y < maxBound.y;
	};

	for (Particle* p : m_particles)
	{
		if (p->isStatic())
			continue;
		if (!isInRect(p->getPos(), sf::Vector2f(0, 0), m_spaceDimension))
		{
			p->boundryCollision();
		}
	}
}