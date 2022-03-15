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
	//setDrawDebug(true);
}
Shape::~Shape()
{

}

void Shape::setPoints(const vector<sf::Vector2f>& points)
{
	if (points.size() < 3)
	{
		PRINT_ERROR("Vector of points need more points to define a shape.\nMinimum are 3 points")
		return;
	}
	m_localPoints = vector<sf::Vertex>(points.size());
	
	for (size_t i=0; i<m_localPoints.size(); ++i)
	{
		m_localPoints[i].position = points[i];
		m_localPoints[i].color = m_color;
	}
	if (m_localPoints[0].position != m_localPoints[m_localPoints.size() - 1].position)
	{
		m_localPoints.push_back(m_localPoints[0]);
	}
	m_globalPoints = m_localPoints;
	normalizeCenterOfMass();
}
vector<sf::Vector2f> Shape::getPoints() const
{
	vector<sf::Vector2f> list(m_localPoints.size());
	for (size_t i = 0; i < m_localPoints.size(); ++i)
		list[i] = m_localPoints[i].position;
	return list;
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
			
				sf::Vector2f normal = VectorMath::rotate90_clockwise(VectorMath::getNormalized(edgeVec));
				VectorMath::intersectionFactor(currentPos, currentMovingVector, particleScalar,
											   pointA, edgeVec, edgeScalar,
											   doesCross);

				sf::Vector2f vecA_to_currentPos = currentPos - pointA;
				float distanceToEdge = VectorMath::dotProduct(vecA_to_currentPos, normal);

				if (abs(distanceToEdge) < 2 )
				{
					if (VectorMath::getLengthSquare(vecA_to_currentPos - normal) >
						VectorMath::getLengthSquare(vecA_to_currentPos + normal))
					{
						normal = -normal;
					}
					sf::Vector2f normalOffset = (2- abs(distanceToEdge))* normal;
					currentMovingVector += normalOffset;
					//if (edgeScalar < 0 || edgeScalar     > 1)
					{
						p->setDeltaPos(currentMovingVector);
						//continue;
					}
				}
				
				if (!doesCross)
					continue; // Go to next line of this shape

				

				// Check if they intersect
				float margin = 1;
				if (particleScalar < 0 || particleScalar > 1 ||
					edgeScalar < -margin || edgeScalar     > 1 + margin)
					continue; // They do not intersect

				// They do intersect
				// Resolve collision

				//   Ajust the targeted moving vector of the particle
				//currentMovingVector *= (particleScalar - 0.1f);
				sf::Vector2f newMovingVec = currentMovingVector * (particleScalar - 0.01f);
				/*if (VectorMath::getLength(currentMovingVector) > 0.2 && particleScalar > 0.2)
				{
					newMovingVec += normal;
				}*/
				

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
									float margin1 = 0.1;
									if (particleScalar1 < 0 || particleScalar1 > 1 ||
										edgeScalar1 < -margin1 || edgeScalar1     > 1 + margin1)
									{
									}
									else
									{
										neighbourMatch = true;
										additionalMove *= (particleScalar1 - 0.01f);
										//velocity = sf::Vector2f(0, 0);
										float edgeLengthSquare1 = VectorMath::getLengthSquare(edgeVec1);
										float velocityDotProduct1 = VectorMath::dotProduct(velocity, edgeVec1);
										sf::Vector2f sqareNormalEdge1 = edgeVec1 / edgeLengthSquare1;

										velocity = sqareNormalEdge1 * velocityDotProduct1; // Projection on the edge
									}
								}
							}
							neighbourStartIndex = (i + 1)%(m_globalPoints.size()-1);

							if (!neighbourMatch)
							{
								//neighbourStartIndex = (i + 1) % m_globalPoints.size();
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
									float margin1 = 0.1;
									if (particleScalar1 < 0 || particleScalar1 > 1 ||
										edgeScalar1 < -margin1 || edgeScalar1     > 1 + margin1)
									{
									}
									else
									{
										neighbourMatch = true;
										additionalMove *= (particleScalar1 - 0.01f);
										//velocity = sf::Vector2f(0, 0);
										float edgeLengthSquare1 = VectorMath::getLengthSquare(edgeVec1);
										float velocityDotProduct1 = VectorMath::dotProduct(velocity, edgeVec1);
										sf::Vector2f sqareNormalEdge1 = edgeVec1 / edgeLengthSquare1;

										velocity = sqareNormalEdge1 * velocityDotProduct1; // Projection on the edge
									}
								}
							}
							//if (!neighbourMatch)
							//	std::cout << "no neighbour\n";
						}
					}
					
					newMovingVec += additionalMove;
				
				}


				p->setVelocity(velocity);
				p->setDeltaPos(newMovingVec);
				//reCheckCollision = true;

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



const vector<sf::Vector2f> Shape::getGenerateRect(const sf::Vector2f& size)
{
	static vector<sf::Vector2f> rect(8);

	sf::Vector2f middle = size / 2.f;
	float defaultEdgeSize = 20;
	float edgeSize = defaultEdgeSize;
	if (middle.x / 2.f < defaultEdgeSize)
	{
		edgeSize = middle.x / 2.f;
	}
	else if (middle.y / 2.f < defaultEdgeSize)
	{
		edgeSize = middle.y / 2.f;
	}

	sf::Vector2f edgeTL = - middle;
	sf::Vector2f edgeTR = sf::Vector2f(middle.x,-middle.y);
	sf::Vector2f edgeBL = sf::Vector2f(-middle.x,middle.y);
	sf::Vector2f edgeBR = middle;

	sf::Vector2f edgeOffsetX(edgeSize, 0);
	sf::Vector2f edgeOffsetY(0, edgeSize);

	rect[0] = edgeTL + edgeOffsetX;
	rect[1] = edgeTR - edgeOffsetX;

	rect[2] = edgeTR + edgeOffsetY;
	rect[3] = edgeBR - edgeOffsetY;

	rect[4] = edgeBR - edgeOffsetX;
	rect[5] = edgeBL + edgeOffsetX;

	rect[6] = edgeBL - edgeOffsetY;
	rect[7] = edgeTL + edgeOffsetY;

	return rect;
}