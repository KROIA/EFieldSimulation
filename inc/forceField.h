#pragma once

#include "vectorField.h"

using std::vector;

class ForceField : public VectorField
{
	public:
	ForceField(const sf::Vector2f& dim,
		   const sf::Vector2u& resolution);
	~ForceField();

	protected:
	void calculateField();
};