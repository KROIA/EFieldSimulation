#pragma once


#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>



sf::Color getColorFromSignal(float signal,
				   float min = -1.f,
				   float max =  1.f);
float mapF(float x, float in_min, float in_max, float out_min, float out_max);

namespace VectorMath
{
	sf::Vector2f getNormalized(sf::Vector2f vec, float normScale = 1);
	float getLength(const sf::Vector2f& vec);
	float getAngle(const sf::Vector2f& vec);
	sf::Vector2f getRotated(sf::Vector2f vec, float rad);
	sf::Vector2f getUnitVector();
};


