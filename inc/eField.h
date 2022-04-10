#pragma once

#include "drawable.h"
#include "chargeParticle.h"
#include "vectorPainter.h"
#include <vector>

using std::vector;

class EField : public Drawable
{
	public:
	EField(const sf::Vector2f& dim, 
		   const sf::Vector2u &resolution);
	~EField();

	const EField& operator+=(const EField& other);

	void setMaxVectorLength(float length);
	float getMaxVectorLength() const;
	
	void setSpaceDimension(const sf::Vector2f& dim);
	void setSpaceDimension(float x, float y);
	const sf::Vector2f& getSpaceDimension() const;

	void setGridSize(const sf::Vector2u& size);
	void setGridSize(size_t x, size_t y);
	const sf::Vector2u& getGridSize() const;

	void setGridColor(const sf::Color& color);
	const sf::Color& getGridColor() const;


	void setGridVector(const sf::Vector2u& index,
				   const sf::Vector2f& vec);
	const sf::Vector2f& getGridVector(const sf::Vector2u& index);

	void setVector(const sf::Vector2f& index,
				   const sf::Vector2f& vec);
	const sf::Vector2f& getVector(const sf::Vector2f& index);

	void addChargeParticle(ChargeParticle* p);
	void removeChargeParticle(ChargeParticle* p);
	void clearChargeParticles();

	void calculatePhysics(float timeIntervalSec);
	virtual void draw(sf::RenderWindow* window,
					  const sf::Vector2f& offset = sf::Vector2f(0, 0));


	void checkChargeParticleBounds();
	protected:
	void calculateField();
	void applyPhysics();

	void clearVectorGrid();
	void buildVectorGrid();

	
	



	sf::Vector2u m_gridSize;
	sf::Vector2f m_spaceDimension;
	float m_vectorLength;
	float m_maxVectorLength;

	vector<ChargeParticle*> m_ChargeParticles;

	sf::Color m_vectorGridColor;
	vector < vector<VectorPainter* > >m_vectorGrid;

};