#include "shape.h"


Shape::Shape()
	: m_color(255,255,255)
{
	float scale = 100;
	setPoints({
		scale * VectorMath::getUnitVector(),
		VectorMath::rotate(scale*VectorMath::getUnitVector(),PI * 4.f / 6.f),
		VectorMath::rotate(scale*VectorMath::getUnitVector(),PI * 8.f / 6.f)
	});
	setDrawDebug(true);
}
Shape::~Shape()
{

}

void Shape::setPoints(const vector<sf::Vertex>& points)
{
	if (points.size() < 3)
	{
		PRINT_ERROR("Vector of points need more points to define a shape.\nMinimum are 3 points")
		return;
	}
	m_localPoints = points;
	if (m_localPoints[0].position != m_localPoints[m_localPoints.size() - 1].position)
	{
		m_localPoints.push_back(m_localPoints[0]);
	}
	for (sf::Vertex& v : m_localPoints)
	{
		v.color = m_color;
	}
	m_globalPoints = m_localPoints;
	normalizeCenterOfMass();
}
const vector<sf::Vertex>& Shape::getPoints() const
{
	return m_localPoints;
}

/*void Shape::setOrigin(const sf::Vector2f& origin)
{
	m_origin = origin;
}
const sf::Vector2f& Shape::getOrigin() const
{
	return m_origin;
}*/

void Shape::setRotation(float angle)
{
	m_angle = angle;
}
float Shape::getRotation() const
{
	return m_angle;
}
void Shape::calculatePhysics(float deltaTime)
{
	//m_angle += 0.1;
	updateGlobalPoints();
	updateBoundingBox();

}
void Shape::checkCollision(const vector<Particle*>& particles)
{
	m_collisionPoints.clear();
	m_vectors.clear();

	
	

	for (Particle* p : particles)
	{
		sf::Vector2f currentMovingVector = p->getDeltaPos();
		sf::Vector2f currentPos = p->getPos();
		sf::Vector2f nextPos = currentMovingVector + currentPos;
		bool  reCheckCollision = false;
		if(!m_boundingBox.contains(currentPos) && 
		   !m_boundingBox.contains(nextPos))
			continue; // Particle not in boundingbox, go to next particle

		if (m_drawDebug)
		{
			sf::Vector2f velocity = p->getVelocity();
			VectorPainter st1;
			st1.setVector(velocity * 1000.f);
			st1.setPos(currentPos);
			st1.setColor(sf::Color(255, 0, 0));
			m_vectors.push_back(st1);
		}

		const size_t maxChecks = 100;
		size_t checks = 0;
		// do while reCheckCollision
		do {
			reCheckCollision = false;
			// For every Line of the shape, check vectorcollision
			for (size_t i = 0; i < m_globalPoints.size() - 1; ++i)
			{
				sf::Vector2f pointA = m_globalPoints[i].position;
				sf::Vector2f pointB = m_globalPoints[i + 1].position;
				sf::Vector2f edgeVec = pointB - pointA;
				currentMovingVector = p->getDeltaPos();
				nextPos = currentMovingVector + currentPos;
				/*if (m_drawDebug)
				{
					sf::Vector2f edgeVec = pointB - pointA;
					VectorPainter st2;
					st2.setVector(edgeVec);
					st2.setPos(pointA);
					st2.setColor(sf::Color(0, 255, 255));
					m_vectors.push_back(st2);

				}*/

				bool doesCross;
				float particleScalar, edgeScalar;
				VectorMath::intersectionFactor(currentPos, currentMovingVector, particleScalar,
											   pointA, edgeVec, edgeScalar,
											   doesCross);

				if (!doesCross)
					continue; // Go to next line of this shape

				// Check if they intersect
				float margin = 0;
				if (particleScalar < 0 || particleScalar > 1 ||
					edgeScalar < -margin || edgeScalar     > 1+ margin)
					continue; // They do not intersect

				// They do intersect
				// Resolve collision

				//   Ajust the targeted moving vector of the particle
				//currentMovingVector *= (particleScalar - 0.1f);
				sf::Vector2f newMovingVec = currentMovingVector * (particleScalar - 0.01f);

				// Calculate the new speed vector
				// Calculate the paralell velocity component to the edge of the shape
				sf::Vector2f velocity = p->getVelocity();



				// set new velocity
				float edgeLengthSquare = VectorMath::getLengthSquare(edgeVec);
				if (edgeLengthSquare > 0)
				{
					float velocityDotProduct = VectorMath::dotProduct(velocity, edgeVec);
					sf::Vector2f sqareNormalEdge = edgeVec / edgeLengthSquare;

					velocity = sqareNormalEdge * velocityDotProduct; // Projection on the edge


					// Get the projection of the part of movingvector which can't be executed because it would be 
					// outside of the edge/object
					float deltaMoveDotProduct = VectorMath::dotProduct(currentMovingVector - newMovingVec, edgeVec);
					// Add some movement in the direction of the edge
					sf::Vector2f additionalMove = sqareNormalEdge * deltaMoveDotProduct;
					
					/*
					sf::Vector2f additionalMoveStartPos = currentPos + newMovingVec;

					// Check neighbour edges
					{
						size_t neighbourStartIndex;
						if (i == 0)
							neighbourStartIndex = m_globalPoints.size() - 2;
						else
							neighbourStartIndex = i - 1;

						bool neighbourMatch = false;
						{
							if (!neighbourMatch)
							{
								sf::Vector2f pointA1 = m_globalPoints[neighbourStartIndex].position;
								sf::Vector2f pointB1 = m_globalPoints[neighbourStartIndex + 1].position;
								sf::Vector2f edgeVec1 = pointB1 - pointA1;
								bool doesCross1;
								float particleScalar1, edgeScalar1;
								VectorMath::intersectionFactor(additionalMoveStartPos, additionalMove, particleScalar1,
															   pointA1, edgeVec1, edgeScalar1,
															   doesCross1);

								if (doesCross1)
								{
									// Check if they intersect
									float margin1 = 0;
									if (particleScalar1 < 0 || particleScalar1 > 1 ||
										edgeScalar1 < -margin1 || edgeScalar1     > 1 + margin1)
									{
									}
									else
									{
										neighbourMatch = true;
										additionalMove *= (particleScalar1 - 0.01f);
									}
								}
							}
							neighbourStartIndex = (i + 1)%(m_globalPoints.size()-1);

							if (!neighbourMatch)
							{
								sf::Vector2f pointA1 = m_globalPoints[neighbourStartIndex].position;
								sf::Vector2f pointB1 = m_globalPoints[neighbourStartIndex + 1].position;
								sf::Vector2f edgeVec1 = pointB1 - pointA1;
								bool doesCross1;
								float particleScalar1, edgeScalar1;
								VectorMath::intersectionFactor(additionalMoveStartPos, additionalMove, particleScalar1,
															   pointA1, edgeVec1, edgeScalar1,
															   doesCross1);

								if (doesCross1)
								{
									// Check if they intersect
									float margin1 = 0;
									if (particleScalar1 < 0 || particleScalar1 > 1 ||
										edgeScalar1 < -margin1 || edgeScalar1     > 1 + margin1)
									{
									}
									else
									{
										neighbourMatch = true;
										additionalMove *= (particleScalar1 - 0.01f);
									}
								}
							}
							if (!neighbourMatch)
								std::cout << "no neighbour\n";
						}
					}*/
					
					//newMovingVec += additionalMove;
				}



				p->setVelocity(velocity);
				p->setDeltaPos(newMovingVec);
				reCheckCollision = true;

				if (m_drawDebug)
				{
					m_collisionPoints.push_back(pointA + edgeScalar * edgeVec);
					VectorPainter st1;
					st1.setVector(velocity * 1000.f);
					st1.setPos(currentPos);
					st1.setColor(sf::Color(255, 0, 0));
					m_vectors.push_back(st1);

					VectorPainter st2;
					st2.setVector(edgeVec * edgeScalar);
					st2.setPos(pointA);
					st2.setColor(sf::Color(0, 255, 0));
					m_vectors.push_back(st2);

				}
				++checks;
				if (checks == maxChecks)
					std::cout << "max Checks";
			}
		} while (reCheckCollision && checks < maxChecks);
		
	}
}

void Shape::draw(sf::RenderWindow* window,
				   const sf::Vector2f& offset)
{
	window->draw(m_globalPoints.data(), m_globalPoints.size(), sf::LineStrip);
}
void Shape::drawDebug(sf::RenderWindow* window,
					   const sf::Vector2f& offset)
{
	float radius = 5;

	sf::CircleShape circle(radius);
	circle.setFillColor(m_color);
	circle.setOrigin(sf::Vector2f(radius, radius));
	for (const sf::Vector2f& collisionPoint : m_collisionPoints)
	{
		circle.setPosition(collisionPoint);
		window->draw(circle);
	}
	sf::RectangleShape boundingBox(sf::Vector2f(m_boundingBox.width,m_boundingBox.height));
	boundingBox.setPosition(sf::Vector2f(m_boundingBox.left, m_boundingBox.top));
	boundingBox.setFillColor(sf::Color(0, 0, 0, 0));
	boundingBox.setOutlineColor(sf::Color(255, 0, 0));
	boundingBox.setOutlineThickness(3);
	window->draw(boundingBox);
	for (VectorPainter& p : m_vectors)
		p.draw(window);

}

void Shape::updateGlobalPoints()
{
	for (size_t i=0; i< m_localPoints.size(); ++i)
	{
		sf::Vector2f rotated = VectorMath::rotate(m_localPoints[i].position, m_angle);
		m_globalPoints[i].position = m_pos + rotated;
	}
	
}
void Shape::updateBoundingBox()
{
	sf::Vector2f minPos = m_globalPoints[0].position;
	sf::Vector2f maxPos = m_globalPoints[0].position;
	for (size_t i = 0; i < m_globalPoints.size()-1; ++i)
	{
		sf::Vector2f pos = m_globalPoints[i].position;
		if (pos.x < minPos.x)
			minPos.x = pos.x;
		else if (pos.x > maxPos.x)
			maxPos.x = pos.x;

		if (pos.y < minPos.y)
			minPos.y = pos.y;
		else if (pos.y > maxPos.y)
			maxPos.y = pos.y;
	}
	float additionalSpace = 30;
	m_boundingBox.top = minPos.y- additionalSpace;
	m_boundingBox.left = minPos.x- additionalSpace;

	m_boundingBox.width = (maxPos.x - minPos.x)+2* additionalSpace;
	m_boundingBox.height = (maxPos.y - minPos.y)+ 2 * additionalSpace;
}
void Shape::normalizeCenterOfMass()
{
	sf::Vector2f massCenter(0, 0);
	for (size_t i = 0; i < m_localPoints.size()-1; ++i)
	{
		massCenter += m_localPoints[i].position;
	}
	massCenter /= (float)m_localPoints.size();

	for (sf::Vertex& vertex : m_localPoints)
	{
		vertex.position -= massCenter;
	}
}