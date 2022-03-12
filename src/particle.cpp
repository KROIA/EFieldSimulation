#include "particle.h"


const double Particle::fieldConstant = 8.854187e-12; // [As/Vm]
const double Particle::PI = 3.14159265;
const double Particle::E_PI_4 = 1.f/(fieldConstant * 4 * PI); // 1/(E0*PI*4) [Vm/As]
const double Particle::massPerElectron = 9.109383e-31; // [kg]
const double Particle::chargePerElectron = -1.602176e-19; // [C] = [As]
const double Particle::massPerCharge = massPerElectron / chargePerElectron; // [kg/C] = [kg/As]


Particle::Particle(/*const sf::Vector2u& size,
				   const sf::Vector2f& spaceDimension*/)
	//: m_eField(spaceDimension,size)
{
	setCharge(0);
	setSize(10);
	setDrag(0.5);
	setVelocity(sf::Vector2f(0, 0));
	setStatic(false);
	//setupField(size, spaceDimension);
}
Particle::~Particle()
{

}
/*void Particle::setupField(const sf::Vector2u& size,
				          const sf::Vector2f& spaceDimension)
{
	m_vecField.size = size;
	m_vecField.spaceDimension = spaceDimension;
	m_vecField.field.clear();

	m_vecField.field = vector<vector<sf::Vector2f> >(size.x, vector<sf::Vector2f>(size.y, sf::Vector2f(0, 0)));
}*/

void Particle::setCharge(float q)
{
	m_charge = q;
}
float Particle::getCharge() const
{
	return m_charge;
}

void Particle::setSize(float size)
{
	m_size = size/2.f;
}
float Particle::getSize() const
{
	return m_size*2.f;
}

void Particle::setStatic(bool enable)
{
	m_static = enable;
	//if (!m_static)
	//	m_lastPos = m_pos;
}
bool Particle::isStatic() const
{
	return m_static;
}
void Particle::setVelocity(const sf::Vector2f& vel)
{
	m_velocity = vel;
}
const sf::Vector2f& Particle::getVelocity() const
{
	return m_velocity;
}
void Particle::setDrag(float drag)
{
	if (drag > 1 || drag < 0)
		return;

	m_drag = drag;
}
float Particle::getDrag() const
{
	return m_drag;
}

void Particle::boundryCollision()
{
	if (m_static)
		return;

	m_pos -= m_deltaPos * 2.f;
	m_velocity.x = 0;
	m_velocity.y = 0;
	//m_pos = m_lastPos;
}

/*void Particle::calculateVectorField()
{
	for (size_t x = 0; x < m_eField.getGridSize().x; ++x)
		for (size_t y = 0; y < m_eField.getGridSize().y; ++y)
		{
			sf::Vector2f sum(0, 0);
			sf::Vector2f point((x * m_eField.getSpaceDimension().x) / m_eField.getGridSize().x,
							   (y * m_eField.getSpaceDimension().y) / m_eField.getGridSize().y);

			m_eField.setGridVector(sf::Vector2u(x,y),getFieldVector(point));
		}
}*/
void Particle::calculatePhysiscs(const vector<Particle*>& particleList,
								 float timeIntervalSec)
{
	if (m_static)
		return;
	sf::Vector2f eField(0, 0);
	for (Particle *p : particleList)
	{
		if (p == this || p == nullptr) // ignore the own vector field
			continue;
		eField += p->getFieldVector(m_pos);
	}
	if (VectorMath::getLength(eField) > 1000000000)
	{
		eField = VectorMath::getNormalized(eField, 1000000000);
	}
	sf::Vector2f force = eField * m_charge;

	force -= force * m_drag;
	sf::Vector2f acceleration = (float)(abs(m_charge) * massPerCharge) * force;

	m_velocity += acceleration * timeIntervalSec;

	//m_lastPos = m_pos;
	m_deltaPos = m_velocity * timeIntervalSec;
	
	//std::cout << "vel: " << m_velocity.x << " " << m_velocity.y << "\n";
}
void Particle::applyPhysics()
{
	m_pos += m_deltaPos;
}

sf::Vector2f Particle::getFieldVector(const sf::Vector2f& point)
{
	sf::Vector2f distanceVec = m_pos - point; // Vector from Particle to Point
	float length = VectorMath::getLength(distanceVec);
	if (length == 0)
		return sf::Vector2f(0, 0);

	float scalar = m_charge * E_PI_4 / (length * length);
	return scalar * VectorMath::getNormalized(distanceVec);
}
/*sf::Vector2f Particle::getFieldVectorOfGrid(const sf::Vector2u& index)
{
	return m_eField.getGridVector(index);
}*/

void Particle::draw(sf::RenderWindow* window,
					const sf::Vector2f& offset)
{
	sf::CircleShape circle(m_size);
	circle.setFillColor(getChargeColor(m_charge));
	circle.setOrigin(sf::Vector2f(m_size, m_size));
	circle.setPosition(m_pos + offset);
	window->draw(circle);
}

sf::Color Particle::getChargeColor(float charge)
{
	if (charge == 0)
		return sf::Color(255, 150, 0);
	if (charge > 0)
		return sf::Color(255, 0, 0);
	return sf::Color(0, 0, 255);
}

/*
void Particle::calculateField()
{
	for (Particle* p : m_particles)
	{
		p->calculateVectorField();
	}
	for (size_t x = 0; x < m_vectorGrid.size(); ++x)
		for (size_t y = 0; y < m_vectorGrid[x].size(); ++y)
		{
			VectorPainter* vec = m_vectorGrid[x][y];
			sf::Vector2f sum(0, 0);

			for (Particle* p : m_particles)
			{
				sum += p->getFieldVectorOfGrid(sf::Vector2u(x,y));
			}
			//std::cout << "VectorLength: " << VectorMath::getLength(sum * m_vectorScale)<<"\n";
			vec->setVector(sum * m_vectorScale);
		}
}*/

/*void Particle::calculatePhysics(float timeIntervalSec)
{
	if (m_static)
		return;


	for (Particle* p : m_particles)
	{
		if (p->isStatic())
			continue;
		sf::Vector2f pos = p->getPos();
		sf::Vector2i indexPos((m_gridSize.x*pos.x) / m_spaceDimension.x,
							  (m_gridSize.y*pos.y) / m_spaceDimension.y);

		if (indexPos.x >= m_gridSize.x ||
			indexPos.y >= m_gridSize.y)
		{
			std::cout << "Error: position out of grid\n";
			continue;
		}

		p->calculatePhysiscs(m_particles, timeIntervalSec);
	}
	//checkBounds();
}*/
/*void Particle::checkBounds()
{
	if (m_static)
		return;
	if (!(m_pos.x > 0 && m_pos.y > 0 &&
		  m_pos.x < m_eField.getSpaceDimension().x && m_pos.x < m_eField.getSpaceDimension().y))
	{
		setVelocity(sf::Vector2f(0, 0));
		goToLastPos();
	}
}*/