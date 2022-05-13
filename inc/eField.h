#pragma once

#include "vectorField.h"

using std::vector;

class EField : public VectorField
{
	public:
	EField(const sf::Vector2f& dim, 
		   const sf::Vector2u &resolution);
	~EField();

	protected:
	void calculateField();
};