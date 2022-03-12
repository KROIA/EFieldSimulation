#include "graphicsUtilities.h"


sf::Color getColorFromSignal(float signal,
				   float min,
				   float max)
{
	sf::Color color;
	float middle = (max + min) / 2;
	
	if (signal < middle)
	{
		if (signal < min)
			signal = min;
		color.g = 0;
		color.r = mapF(signal, middle, min, 30, 255);
	}
	else
	{
		if (signal > max)
			signal = max;
		color.r = 0;
		color.g = mapF(signal, middle, max, 30, 255);
	}
	return color;
}
float mapF(float x, float in_min, float in_max, float out_min, float out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

namespace VectorMath
{
	sf::Vector2f getNormalized(sf::Vector2f vec, float normScale)
	{
		float length = getLength(vec);
		if (length == 0)
		{
			return getUnitVector() * normScale;
		}
		return vec * normScale / length;;
	}

	float getLength(const sf::Vector2f& vec)
	{
		return sqrt(vec.x * vec.x + vec.y * vec.y);
	}
	float getAngle(const sf::Vector2f& vec)
	{
		static const float PI = 3.14159265;

		float rad;
		float length = getLength(vec);

		if (length == 0)
			return 0;
		if (vec.x > 0)
		{
			rad = asin(vec.y / length);
		}
		else
		{
			rad = PI - asin(vec.y / length);
		}
		return rad;
	}
	sf::Vector2f getRotated(sf::Vector2f vec, float rad)
	{
		float length = getLength(vec);
		vec.x = cos(rad) * length;
		vec.y = sin(rad) * length;
		return vec;
	}

	sf::Vector2f getUnitVector()
	{
		return sf::Vector2f(1, 0);
	}
}