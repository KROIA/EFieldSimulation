#pragma once


#include "drawable.h"
#include <vector>

using std::vector;

class DistributionPlot : public Drawable
{
	public:
	enum Allignement
	{
		x,
		y
	};
	DistributionPlot();
	~DistributionPlot();

	void setDimension(const sf::Vector2f& dim);
	const sf::Vector2f& getDimension() const;
	void setXResolution(size_t count);
	size_t getXResolution() const;

	void addCount(size_t x, int count);
	int getCount(size_t x) const;
	void clearCount();

	void setAllignement(Allignement a);
	Allignement getAllignement() const;

	virtual void draw(sf::RenderWindow* window,
					  const sf::Vector2f& offset = sf::Vector2f(0, 0));

	protected:
	sf::Color getColor(int elements);
	size_t m_nElements;
	sf::Vector2f m_dimension;
	int m_maxCount;

	vector<int> m_plotCountList;
	Allignement m_allignement;
};