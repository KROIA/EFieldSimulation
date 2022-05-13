#include "bField.h"

BField::BField(const sf::Vector2f& dim,
			   const sf::Vector2u& resolution)
	: VectorField(dim, resolution)
{}

BField::~BField()
{}
void BField::calculateField()
{
	if (!m_visible)
		return;

	for (size_t x = 0; x < m_vectorGrid.size(); ++x)
		for (size_t y = 0; y < m_vectorGrid[x].size(); ++y)
		{
			VectorPainter* vec = m_vectorGrid[x][y];
			sf::Vector2f sum(0, 0);
			sf::Vector2f pos = vec->getPos();

			for (ChargeParticle* p : m_ChargeParticles)
			{
				sum += p->getBFieldVector(pos);
			}
			float length = VectorMath::getLength(sum);
			if (length < m_minVectorLength)
				vec->setVector(0, 0);
			else
				vec->setVector(VectorMath::getNormalized(sum, m_vectorLength));
			vec->setColor(getColorFromSignal(VectorMath::getLength(sum), 0, m_maxVectorLength));
			//std::cout << sum.x << " " << sum.y;
		}
}