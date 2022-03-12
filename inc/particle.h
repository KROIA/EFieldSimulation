#pragma once

#include <vector>
#include "drawable.h"
//#include "eField.h"

using std::vector;

class Particle	:	 public Drawable
{
	public:
	/*struct VectorField
	{
		sf::Vector2u size;
		sf::Vector2f spaceDimension;
		vector<vector<sf::Vector2f> > field;

		const sf::Vector2f& getVector(const sf::Vector2f& pos)const
		{
			sf::Vector2i indexPos((size.x * pos.x) / spaceDimension.x,
								  (size.y * pos.y) / spaceDimension.y);
			if (indexPos.x >= size.x ||
				indexPos.y >= size.y)
			{
				std::cout << "Error: position out of grid\n";
				return sf::Vector2f(0, 0);
			}
			return field[indexPos.x][indexPos.y];
		}
	};*/
	//Particle(VectorField *fieldBuff);
	Particle(/*const sf::Vector2u& size,
			 const sf::Vector2f& spaceDimension*/);
	~Particle();

	//void setupField(const sf::Vector2u& size,
	//				const sf::Vector2f& spaceDimension);

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


	//void calculateVectorField();
	void calculatePhysiscs(const vector<Particle*> &particleList,
						   float timeIntervalSec);
	void applyPhysics();

	sf::Vector2f getFieldVector(const sf::Vector2f& point);
	//sf::Vector2f getFieldVectorOfGrid(const sf::Vector2u& index);

	virtual void draw(sf::RenderWindow* window,
					  const sf::Vector2f& offset = sf::Vector2f(0, 0));

	static sf::Color getChargeColor(float charge);

	static const double fieldConstant; // [As/Vm]
	static const double PI;
	static const double E_PI_4; // 1/(E0*PI*4) [Vm/As]
	static const double massPerElectron; // [kg]
	static const double chargePerElectron; // [C] = [As]
	static const double massPerCharge; // [kg/C] = [kg/As]


	//void calculateField();
	//void calculatePhysics(float timeIntervalSec);
	//void checkBounds();
	protected:
	
	float m_size; 
	float m_charge;  // [C] = [As] 
	
	bool m_static;
	sf::Vector2f m_velocity;
	float m_drag;
	sf::Vector2f m_deltaPos;
	//VectorField m_vecField;

	//EField m_eField;
};