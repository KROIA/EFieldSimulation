#pragma once

#include <vector>
#include "drawable.h"

using std::vector;

class Particle	:	 public Drawable
{
	public:
	Particle();
	~Particle();


	void setCharge(float q);
	float getCharge() const;

	void setSize(float size);
	float getSize() const;

	void setStatic(bool enable);
	bool isStatic() const;

	void setVelocity(const sf::Vector2f& vel);
	const sf::Vector2f& getVelocity() const;
	void setDrag(float drag);
	float getDrag() const;
	void boundryCollision();
	void setDeltaPos(const sf::Vector2f &deltaPos);
	const sf::Vector2f& getDeltaPos() const;

	void calculatePhysiscs(const vector<Particle*> &particleList,
						   float timeIntervalSec);
	void applyPhysics();

	sf::Vector2f getFieldVector(const sf::Vector2f& point);

	virtual void draw(sf::RenderWindow* window,
					  const sf::Vector2f& offset = sf::Vector2f(0, 0));

	static sf::Color getChargeColor(float charge);

	static const double fieldConstant; // [As/Vm]
	static const double PI;
	static const double E_PI_4; // 1/(E0*PI*4) [Vm/As]
	static const double massPerElectron; // [kg]
	static const double chargePerElectron; // [C] = [As]
	static const double massPerCharge; // [kg/C] = [kg/As]

	protected:
	
	float m_size; 
	float m_charge;  // [C] = [As] 
	
	bool m_static;
	sf::Vector2f m_velocity;
	float m_drag;
	sf::Vector2f m_deltaPos;
};