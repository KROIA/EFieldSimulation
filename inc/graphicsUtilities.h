#pragma once


#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include "debugHelper.h"


static const float PI = 3.14159265;

sf::Color getColorFromSignal(float signal,
				   float min = -1.f,
				   float max =  1.f);

namespace VectorMath
{
	sf::Vector2f getNormalized(sf::Vector2f vec, float normScale = 1);
	extern inline float getLength(const sf::Vector2f& vec);
	extern inline float getLengthSquare(const sf::Vector2f& vec);
	float getAngle(const sf::Vector2f& vec);
	sf::Vector2f setRotation(sf::Vector2f vec, float rad);
	sf::Vector2f rotate(const sf::Vector2f &vec, float rad);
	sf::Vector2f getUnitVector();
	

	// Gets 2 scalars and a bool for the given 4 Points
	// if "doesCross" == true, the 2 Lines may intersect,
	// 0 < factor1,factor2 < 1 for intersection
	extern inline void intersectionFactor(const sf::Vector2f& S1,
	                        const sf::Vector2f& S2,
							float& factor1,
							const sf::Vector2f& R1,
							const sf::Vector2f& R2,
							float& factor2,
							bool &doesCross);
	extern inline sf::Vector2f rotate90_clockwise(const sf::Vector2f& vec);
	extern inline sf::Vector2f rotate90_counterClockwise(const sf::Vector2f& vec);
	extern inline float dotProduct(const sf::Vector2f& v1,
							const sf::Vector2f& v2);
};

