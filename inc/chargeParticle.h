#pragma once

#include <vector>
#include "particle.h"

using std::vector;

class ChargeParticle	:	 public Particle
{
	public:
	ChargeParticle();
	virtual ~ChargeParticle();


	void setCharge(float q);
	float getCharge() const;

	//void setSize(float size);
	//float getSize() const;

	void setStatic(bool enable);
	bool isStatic() const;

	//void setVelocity(const sf::Vector2f& vel);
	//const sf::Vector2f& getVelocity() const;
	void setDrag(float drag);
	float getDrag() const;
	void setMaxVelocity(float vel);
	float getMaxVelocity() const;
	void boundryCollision();
	void setDeltaPos(const sf::Vector2f &deltaPos);
	const sf::Vector2f& getDeltaPos() const;

	void calculateElectrostaticPhysiscs(const vector<ChargeParticle*> &ChargeParticleList,
						   float timeIntervalSec);
	void calculateMagneticPhysiscs(const vector<ChargeParticle*>& ChargeParticleList,
						   float timeIntervalSec);
	void applyPhysics();

	sf::Vector2f getEFieldVector(const sf::Vector2f& point) const;
	sf::Vector2f getBFieldVector(const sf::Vector2f& point) const;
	sf::Vector2f getForceFieldVector(const sf::Vector2f& point) const;

	virtual void draw(sf::RenderWindow* window,
					  const sf::Vector2f& offset = sf::Vector2f(0, 0));

	static sf::Color getChargeColor(float charge);


	static const double fieldConstant; // [As/Vm]
	static const double PI;
	static const double E_PI_4; // 1/(E0*PI*4) [Vm/As]
	static const double massPerElectron; // [kg]
	static const double chargePerElectron; // [C] = [As]
	static const double massPerCharge; // [kg/C] = [kg/As]
	// Magnetostatic
	static const double mu_PI_2; // mu_0/(2*PI)
	static const double chargeZVelocity;

	static void setStandard_charge(float charge);
	static void setStandard_size(float size);
	static void setStandard_drag(float drag);
	static void setStandard_maxVelocity(float velocity);

	protected:
	
	//float m_size; 
	float m_charge;  // [C] = [As] 
	
	bool m_static;
	//sf::Vector2f m_velocity;
	float m_maxVelocitySquare;
	float m_drag;
	//sf::Vector2f m_deltaPos;


	static float m_standard_charge;
	static float m_standard_size;
	static float m_standard_drag;
	static float m_standard_maxVelocity;

	
};