#include "controlledParticle.h"


ControlledParticle::ControlledParticle()
{

}
ControlledParticle::~ControlledParticle()
{

}

void ControlledParticle::calculatePhysiscs(float timeIntervalSec)
{
	static float time = 0;
	time += timeIntervalSec*10;
	// sf::Vector2f newVelocity = VectorMath::rotate(m_velocity, 0.1);
	//sf::Vector2f acceleration = (newVelocity - m_velocity) / timeIntervalSec;


	//sf::Vector2f newVelocity = VectorMath::rotate90_clockwise(m_velocity);
	//sf::Vector2f acceleration = (VectorMath::getNormalized(newVelocity))/ timeIntervalSec;
	//
	//
	//m_velocity += acceleration * timeIntervalSec;
	//m_deltaPos = m_velocity * timeIntervalSec;
	
	// Matlab Beispiel
	//sf::Vector2f newVelocity = VectorMath::rotate90_clockwise(m_velocity);
	sf::Vector2f acceleration = VectorMath::rotate90_clockwise(m_velocity); // B/m = 1 
	sf::Vector2f deltaV = acceleration * timeIntervalSec;
	m_velocity += deltaV;
	m_deltaPos = m_velocity * timeIntervalSec;
	
	
	std::cout << VectorMath::getLength(m_velocity) << "\n";
}