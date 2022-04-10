#pragma once


#include "particle.h"


class ControlledParticle : public Particle
{
	public:
	ControlledParticle();
	~ControlledParticle();

	virtual void calculatePhysiscs(float timeIntervalSec);
	protected:

};
