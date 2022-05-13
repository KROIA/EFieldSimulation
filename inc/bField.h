#pragma once

#include "vectorField.h"

using std::vector;

class BField : public VectorField
{
	public:
	BField(const sf::Vector2f& dim,
		   const sf::Vector2u& resolution);
	~BField();

	protected:
	void calculateField();
};