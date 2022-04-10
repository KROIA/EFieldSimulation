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
	inline sf::Vector2f getNormalized(const sf::Vector2f& vec, float normScale)
	{
		float length = getLength(vec);
#ifndef IGNORE_MATH_CHECK
		if (length == 0)
		{
			return getUnitVector() * normScale;
		}
#endif
		return vec * normScale / length;;
	}

	inline float getLength(const sf::Vector2f& vec)
	{
		return sqrt(vec.x * vec.x + vec.y * vec.y);
	}
	inline float getLengthSquare(const sf::Vector2f& vec)
	{
		return vec.x * vec.x + vec.y * vec.y;
	}
	inline float getAngle(const sf::Vector2f& vec)
	{
		

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
	inline sf::Vector2f setRotation(sf::Vector2f vec, float rad)
	{
		float length = getLength(vec);
		vec.x = cos(rad) * length;
		vec.y = sin(rad) * length;
		return vec;
	}
	inline sf::Vector2f rotate(const sf::Vector2f& vec, float rad)
	{
		float currentAngle = getAngle(vec);
		return setRotation(vec, currentAngle + rad);
	}

	inline sf::Vector2f getUnitVector()
	{
		return sf::Vector2f(1, 0);
	}

	inline void intersectionFactor(const sf::Vector2f& S1,
								   const sf::Vector2f& S2,
								   float& factor1,
								   const sf::Vector2f& R1,
								   const sf::Vector2f& R2,
								   float& factor2,
								   bool& doesCross)
	{
		// TI-Nspire code:
		//  f1(s) := [[s1x][s1y]] + s * [[s2x][s2y]] ▸ Fertig
		//	f2(t) := [[r1x][r1y]] + t * [[r2x][r2y]] ▸ Fertig
		//	solve(f1(s) = f2(t), s, t)
		//      ▸ s = ((−(r1x * r2y - r1y * r2x + r2x * s1y - r2y * s1x)) / (r2x * s2y - r2y * s2x)) 
		//    and t = ((−(r1x * s2y - r1y * s2x - s1x * s2y + s1y * s2x)) / (r2x * s2y - r2y * s2x))

		float discriminante = (R2.x * S2.y - R2.y * S2.x);
		if (discriminante == 0)
		{
			doesCross = false;
			return;
		}
		doesCross = true;
		factor1 = -(R1.x * R2.y - R1.y * R2.x + R2.x * S1.y - R2.y * S1.x) / discriminante;
		factor2 = -(R1.x * S2.y - R1.y * S2.x - S1.x * S2.y + S1.y * S2.x) / discriminante;
	}

	inline sf::Vector2f rotate90_clockwise(const sf::Vector2f& vec)
	{
		return sf::Vector2f(-vec.y, vec.x);
	}
	inline sf::Vector2f rotate90_counterClockwise(const sf::Vector2f& vec)
	{
		return sf::Vector2f(vec.y, -vec.x);
	}
	inline float dotProduct(const sf::Vector2f& v1,
							const sf::Vector2f& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

}