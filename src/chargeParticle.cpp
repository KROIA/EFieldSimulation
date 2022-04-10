#include "chargeParticle.h"


const double ChargeParticle::fieldConstant = 8.854187e-12; // [As/Vm]
const double ChargeParticle::PI = 3.14159265;
const double ChargeParticle::E_PI_4 = 1.f/(fieldConstant * 4 * PI); // 1/(E0*PI*4) [Vm/As]
const double ChargeParticle::massPerElectron = 9.109383e-31; // [kg]
const double ChargeParticle::chargePerElectron = -1.602176e-19; // [C] = [As]
const double ChargeParticle::massPerCharge = massPerElectron / chargePerElectron; // [kg/C] = [kg/As]

float ChargeParticle::m_standard_charge = 0;
float ChargeParticle::m_standard_size = 10;
float ChargeParticle::m_standard_drag = 0.01;
float ChargeParticle::m_standard_maxVelocity = 10000;

#define ChargeParticle_USE_DRAG
//#define ChargeParticle_USE_MAX_VELOCITY

ChargeParticle::ChargeParticle()
{
	setCharge(m_standard_charge);
	setRadius(m_standard_size);
	setDrag(m_standard_drag);
	setVelocity(sf::Vector2f(0, 0));
	setStatic(false);
	setMaxVelocity(m_standard_maxVelocity);
}
ChargeParticle::~ChargeParticle()
{

}

void ChargeParticle::setCharge(float q)
{
	m_charge = q;
}
float ChargeParticle::getCharge() const
{
	return m_charge;
}

/*void ChargeParticle::setSize(float size)
{
	m_size = size/2.f;
}
float ChargeParticle::getSize() const
{
	return m_size*2.f;
}*/

void ChargeParticle::setStatic(bool enable)
{
	m_static = enable;
}
bool ChargeParticle::isStatic() const
{
	return m_static;
}
/*void ChargeParticle::setVelocity(const sf::Vector2f& vel)
{
	m_velocity = vel;
}
const sf::Vector2f& ChargeParticle::getVelocity() const
{
	return m_velocity;
}*/
void ChargeParticle::setDrag(float drag)
{
	if (drag > 1 || drag < 0)
		return;

	m_drag = drag;
}
float ChargeParticle::getDrag() const
{
	return m_drag;
}
void ChargeParticle::setMaxVelocity(float vel)
{
	m_maxVelocitySquare = vel*vel;
}
float ChargeParticle::getMaxVelocity() const
{
	return sqrt(m_maxVelocitySquare);
}

void ChargeParticle::boundryCollision()
{
	if (m_static)
		return;

	m_pos -= m_deltaPos * 2.f;
	m_velocity.x = 0;
	m_velocity.y = 0;
}
void ChargeParticle::setDeltaPos(const sf::Vector2f& deltaPos)
{
	m_deltaPos = deltaPos;
}
const sf::Vector2f& ChargeParticle::getDeltaPos() const
{
	return m_deltaPos;
}

void ChargeParticle::calculatePhysiscs(const vector<ChargeParticle*>& ChargeParticleList,
								 float timeIntervalSec)
{
//	if (m_static)
//		return;
	sf::Vector2f eField(0, 0);
	for (ChargeParticle *p : ChargeParticleList)
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

#ifdef ChargeParticle_USE_MAX_VELOCITY
	if (VectorMath::getLengthSquare(m_velocity) > m_maxVelocitySquare)
	{
		m_velocity = VectorMath::getNormalized(m_velocity, sqrt(m_maxVelocitySquare));
	}
#endif
#ifdef ChargeParticle_USE_DRAG
	m_velocity -= m_velocity * m_drag * timeIntervalSec;
#endif
	m_deltaPos = m_velocity * timeIntervalSec;
}
void ChargeParticle::applyPhysics()
{
	m_pos += m_deltaPos;
}

inline sf::Vector2f ChargeParticle::getFieldVector(const sf::Vector2f& point) const
{
	sf::Vector2f distanceVec = m_pos - point; // Vector from ChargeParticle to Point
	float lengthSquare = VectorMath::getLengthSquare(distanceVec);
	if (lengthSquare == 0)
		return sf::Vector2f((float)(rand()%1000)/10.f, (float)(rand() % 1000) / 10.f);

	float scalar = m_charge * E_PI_4 / (lengthSquare);

	return scalar * VectorMath::getNormalized(distanceVec);
}


void ChargeParticle::draw(sf::RenderWindow* window,
					const sf::Vector2f& offset)
{
	/*sf::CircleShape circle(m_size);
	circle.setFillColor(getChargeColor(m_charge));
	circle.setOrigin(sf::Vector2f(m_size, m_size));
	circle.setPosition(m_pos + offset);
	window->draw(circle);*/
	m_color = getChargeColor(m_charge);
	Particle::draw(window, offset);
}

sf::Color ChargeParticle::getChargeColor(float charge)
{
	if (charge == 0)
		return sf::Color(255, 150, 0);
	if (charge > 0)
		return sf::Color(255, 0, 0);
	return sf::Color(0, 0, 255);
}


void ChargeParticle::setStandard_charge(float charge)
{
	m_standard_charge = charge;
}
void ChargeParticle::setStandard_size(float size)
{
	m_standard_size = size;
}
void ChargeParticle::setStandard_drag(float drag)
{
	m_standard_drag = drag;
}
void ChargeParticle::setStandard_maxVelocity(float velocity)
{
	m_standard_maxVelocity = velocity;
}