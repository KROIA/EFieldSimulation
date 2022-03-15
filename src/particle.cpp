#include "particle.h"


const double Particle::fieldConstant = 8.854187e-12; // [As/Vm]
const double Particle::PI = 3.14159265;
const double Particle::E_PI_4 = 1.f/(fieldConstant * 4 * PI); // 1/(E0*PI*4) [Vm/As]
const double Particle::massPerElectron = 9.109383e-31; // [kg]
const double Particle::chargePerElectron = -1.602176e-19; // [C] = [As]
const double Particle::massPerCharge = massPerElectron / chargePerElectron; // [kg/C] = [kg/As]

float Particle::m_standard_charge = 0;
float Particle::m_standard_size = 10;
float Particle::m_standard_drag = 0.01;
float Particle::m_standard_maxVelocity = 10000;

#define PARTICLE_USE_DRAG
//#define PARTICLE_USE_MAX_VELOCITY

Particle::Particle()
{
	setCharge(m_standard_charge);
	setSize(m_standard_size);
	setDrag(m_standard_drag);
	setVelocity(sf::Vector2f(0, 0));
	setStatic(false);
	setMaxVelocity(m_standard_maxVelocity);
}
Particle::~Particle()
{

}

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
void Particle::setMaxVelocity(float vel)
{
	m_maxVelocitySquare = vel*vel;
}
float Particle::getMaxVelocity() const
{
	return sqrt(m_maxVelocitySquare);
}

void Particle::boundryCollision()
{
	if (m_static)
		return;

	m_pos -= m_deltaPos * 2.f;
	m_velocity.x = 0;
	m_velocity.y = 0;
}
void Particle::setDeltaPos(const sf::Vector2f& deltaPos)
{
	m_deltaPos = deltaPos;
}
const sf::Vector2f& Particle::getDeltaPos() const
{
	return m_deltaPos;
}

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

	//force -= force * m_drag;
	sf::Vector2f acceleration = (float)(abs(m_charge) * massPerCharge) * force;
	m_velocity += acceleration * timeIntervalSec;

#ifdef PARTICLE_USE_MAX_VELOCITY
	if (VectorMath::getLengthSquare(m_velocity) > m_maxVelocitySquare)
	{
		m_velocity = VectorMath::getNormalized(m_velocity, sqrt(m_maxVelocitySquare));
	}
#endif
#ifdef PARTICLE_USE_DRAG
	m_velocity -= m_velocity * m_drag * timeIntervalSec;
#endif
	m_deltaPos = m_velocity * timeIntervalSec;
}
void Particle::applyPhysics()
{
	m_pos += m_deltaPos;
}

sf::Vector2f Particle::getFieldVector(const sf::Vector2f& point) const
{
	sf::Vector2f distanceVec = m_pos - point; // Vector from Particle to Point
	float length = VectorMath::getLength(distanceVec);
	if (length == 0)
		return sf::Vector2f((float)(rand()%1000)/10.f, (float)(rand() % 1000) / 10.f);

	float scalar = m_charge * E_PI_4 / (length * length);
	//float scalar = m_charge * E_PI_4 / (length);
	return scalar * VectorMath::getNormalized(distanceVec);
}


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


void Particle::setStandard_charge(float charge)
{
	m_standard_charge = charge;
}
void Particle::setStandard_size(float size)
{
	m_standard_size = size;
}
void Particle::setStandard_drag(float drag)
{
	m_standard_drag = drag;
}
void Particle::setStandard_maxVelocity(float velocity)
{
	m_standard_maxVelocity = velocity;
}