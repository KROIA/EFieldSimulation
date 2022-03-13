#include "graphicsUtilities.h"


sf::Color getColorFromSignal(float signal,
				   float min,
				   float max)
{
	sf::Color color;
	float middle = (max + min) / 2;


	float red = mapF(signal, max, min, 30, 255);
	if (red < 0)
		red = 0;
	else if (red > 255)
		red = 255;

	float green = mapF(signal, min, max, 30, 255);
	if (green < 0)
		green = 0;
	else if (green > 255)
		green = 255;

	color.g = green;
	color.r = red;
	
	/*if (signal < middle)
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
	}*/
	return color;
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