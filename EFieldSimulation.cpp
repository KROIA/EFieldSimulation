#include <iostream>


#include "display.h"
#include "particle.h"
#include "eField.h"
#include "distributionPlot.h"
#include "simulation.h"

void buildLevel_1(Simulation& sim);
void buildLevel_cathodeRayTube(Simulation& sim);
void buildLevel_orbit(Simulation& sim);
void aufgabe_4_1(Simulation& sim);
void buildLevel_shapes(Simulation& sim);
void buildLevel_influence(Simulation& sim);
void buildLevel_spread(Simulation& sim);

int main()
{
	// Particle::setStandard_charge(0);
	// Particle::setStandard_size(10);
	 Particle::setStandard_drag(0.001);
	// Particle::setStandard_maxVelocity(10000);

	Simulation::Settings settings;
	settings.windowSize				= sf::Vector2u(1900, 900)*2u;
	//settings.windowSize				= sf::Vector2u(1900, 900);
	settings.targetFrameRate		= 60;
	settings.gridSizeX				= settings.windowSize.x/19;
	settings.physicsTimeInterval	= 5; // sec
	settings.leftClickCharge		= 100;
	settings.rightClickCharge		= -100;
	settings.eField_maxVectorLength	= 1e7;
	

    Simulation simulation(settings);
	//buildLevel_1(simulation);
	//buildLevel_cathodeRayTube(simulation); // Analysis Aufgabe
	//buildLevel_orbit(simulation);
	//aufgabe_4_1(simulation);
	//buildLevel_shapes(simulation);
	buildLevel_influence(simulation);
	//buildLevel_spread(simulation);
    simulation.start();
	return 0;
}

void buildLevel_1(Simulation& sim)
{
	sf::Vector2f arraySize(50, 20);
	vector<float> particleCharges = { 2 };
	//vector<float> particleCharges = { 2 , -2};

	sf::Vector2f worldSize = sim.getWorldSize();
	vector<Particle*> particles;

	// Particle Array
	float boarder = 20;
	size_t counter = 0;
	particles.reserve(arraySize.x * arraySize.y);
	for (size_t x = 0; x < arraySize.x; ++x)
	{
		for (size_t y = 0; y < arraySize.y; ++y)
		{
			float cha = particleCharges[counter % particleCharges.size()];
			sf::Vector2f pos(boarder + (float)x * (worldSize.x) / (float)arraySize.x,
							 boarder + (float)y * (worldSize.y) / (float)arraySize.y);

			Particle* p = new Particle();
			p->setCharge(cha);
			p->setPos(pos);
			p->setStatic(false);


			particles.push_back(p);

			++counter;
		}
	}
	sim.addParticle(particles);
}

void buildLevel_cathodeRayTube(Simulation& sim)
{
	sf::Vector2f worldSize = sim.getWorldSize();

	sf::Vector2u grid(10, 4);
	sf::Vector2f spacing(50, 25);
	float gab = 100;
	float charge = 5;
	
	vector<Particle*> particles;
	sf::Vector2f sartPosPositive;
	sartPosPositive.x = (worldSize.x - grid.x * spacing.x) / 2.f;
	sartPosPositive.y = (worldSize.y - grid.y * spacing.y * 2 - gab) / 2.f;


	sf::Vector2f sartPosNegative = sartPosPositive;
	sartPosNegative.y = sartPosPositive.y + grid.y * spacing.y + gab;

	for (size_t x = 0; x < grid.x; ++x)
	{
		for (size_t y = 0; y < grid.y; ++y)
		{
			sf::Vector2f pos1 = sartPosPositive + sf::Vector2f(x * spacing.x, y * spacing.y);
			Particle* particle1 = new Particle;
			particle1->setPos(pos1);
			particle1->setCharge(charge);
			particle1->setStatic(true);
			particles.push_back(particle1);


			sf::Vector2f pos2 = sartPosNegative + sf::Vector2f(x * spacing.x, y * spacing.y);
			Particle* particle2 = new Particle;
			particle2->setPos(pos2);
			particle2->setCharge(-charge);
			particle2->setStatic(true);
			particles.push_back(particle2);
		}
	}

	Particle* bullet = new Particle;
	bullet->setPos(sf::Vector2f(20, worldSize.y / 2.f));
	bullet->setCharge(charge);
	bullet->setVelocity(sf::Vector2f(8, 0));
	bullet->setDrag(0);
	particles.push_back(bullet);

	sim.addParticle(particles);
}


void buildLevel_orbit(Simulation& sim)
{
	sf::Vector2f worldSize = sim.getWorldSize();
	vector<Particle*> particles;

	Particle* planet = new Particle;
	planet->setPos(worldSize/2.f);
	planet->setCharge(-100);
	planet->setStatic(true);

	{
		Particle* moon = new Particle;
		moon->setPos(worldSize / 2.f + sf::Vector2f(0, 100));
		moon->setCharge(10);
		moon->setVelocity(sf::Vector2f(2, 0));
		particles.push_back(moon);
	}
	{
		Particle* moon = new Particle;
		moon->setPos(worldSize / 2.f + sf::Vector2f(0, -100));
		moon->setCharge(10);
		moon->setVelocity(sf::Vector2f(-2, 0));
		particles.push_back(moon);
	}
	
	particles.push_back(planet);


	sim.addParticle(particles);
}
void aufgabe_4_1(Simulation& sim)
{
	sf::Vector2f worldSize = sim.getWorldSize();
	vector<Particle*> particles;

	Particle* left = new Particle;
	left->setPos(worldSize / 2.f + sf::Vector2f(-400, 0));
	left->setCharge(10);
	left->setStatic(true);
	left->setSize(20);

	Particle* right = new Particle;
	right->setPos(worldSize / 2.f + sf::Vector2f(400, 0));
	right->setCharge(10);
	right->setStatic(true);
	right->setSize(20);

	Particle* q = new Particle;
	q->setPos(worldSize / 2.f + sf::Vector2f(0, -400));
	q->setCharge(-20);
	q->setStatic(false);
	q->setSize(20);

	particles = { left,right,q };


	sim.addParticle(particles);
}

void buildLevel_shapes(Simulation& sim)
{
	sf::Vector2f worldSize = sim.getWorldSize();
	//vector<Shape*> shapes;
	float scale = 300;

	Shape* shape1 = new Shape;
	shape1->setPos(worldSize / 2.f + sf::Vector2f(-300,0));

	vector<sf::Vector2f> points;
	size_t numPoints = 5;
	for (size_t i = 0; i < numPoints; ++i)
	{
		points.push_back(VectorMath::setRotation(VectorMath::getUnitVector() * scale, i * 2 * PI / (float)numPoints));
	}

	shape1->setPoints(points);
	//shape1->setRotation(PI/4.f);
	

	Shape* shape2 = new Shape;
	shape2->setPos(worldSize / 2.f + sf::Vector2f(300, 0));
	shape2->setPoints(points);
	shape2->setRotation(PI / 2.f);

	//Particle* particle = new Particle;
	//particle->setPos(shape1->getPos());
	//particle->setCharge(5);


	//sim.addParticle(particle);
	sim.addShape(shape1);
	sim.addShape(shape2);
}


void buildLevel_influence(Simulation& sim)
{
	sf::Vector2f worldSize = sim.getWorldSize();
	sf::Vector2f middlePoint = worldSize / 2.f;

	float gab = 100;
	sf::Vector2f spacing(50, 25);
	sf::Vector2u grid((middlePoint.x- gab-100) / spacing.x, 4);
	
	float charge = 5;

	vector<Particle*> particles;
	
	sf::Vector2f sartPosPositive = middlePoint + sf::Vector2f(-(gab / 2.f + grid.x * spacing.x),
															  -(grid.y * spacing.y / 2.f));
	sf::Vector2f sartPosNegative = middlePoint + sf::Vector2f(gab / 2.f, -(grid.y * spacing.y / 2.f));


	for (size_t x = 0; x < grid.x; ++x)
	{
		for (size_t y = 0; y < grid.y; ++y)
		{
			sf::Vector2f pos1 = sartPosPositive + sf::Vector2f(x * spacing.x, y * spacing.y);
			Particle* particle1 = new Particle;
			particle1->setPos(pos1);
			particle1->setCharge(charge);
			particle1->setStatic(false);
			particles.push_back(particle1);


			sf::Vector2f pos2 = sartPosNegative + sf::Vector2f(x * spacing.x, y * spacing.y);
			Particle* particle2 = new Particle;
			particle2->setPos(pos2);
			particle2->setCharge(-charge);
			particle2->setStatic(false);
			particles.push_back(particle2);
		}
	}


	sf::Vector2f rectSize((grid.x + 1) * spacing.x,
						  (grid.y + 1) * spacing.y);
	vector<sf::Vector2f> rectPoints = Shape::getGenerateRect(rectSize);


	Shape* shape1 = new Shape;
	shape1->setPos(sartPosPositive + rectSize / 2.f - spacing);
	shape1->setPoints(rectPoints);
	//shape2->setRotation(PI / 2.f);

	Shape* shape2 = new Shape;
	shape2->setPos(sartPosNegative + rectSize/2.f - spacing);
	shape2->setPoints(rectPoints);
	//shape2->setRotation(PI / 2.f);

	sim.addShape(shape1);
	sim.addShape(shape2);
	sim.addParticle(particles);
}


void buildLevel_spread(Simulation& sim)
{
	sf::Vector2f worldSize = sim.getWorldSize();
	sf::Vector2f center = worldSize / 2.f;


	float charge = 2;

	float largeRadius = 350;
	float smallRadius = 20;
	float spacing = worldSize.x-(largeRadius+ smallRadius+50); // Space between the center of the circles

	size_t pointCount = 20;

	
	sf::Vector2f centerOffset(spacing / 2.f, 0);
	vector<sf::Vector2f> points;
	vector<Particle*> particles;
	float deltaAngle = PI / (float)pointCount;
	float currentAngle = PI / 2.f;
	for (size_t i = 0; i < pointCount; ++i)
	{
		
		points.push_back(center - centerOffset + (largeRadius * sf::Vector2f(cos(currentAngle), sin(currentAngle))));

		


		currentAngle += deltaAngle;
	}

	deltaAngle = PI / ((float)pointCount*1.5);
	currentAngle = -PI / 2.f + deltaAngle* pointCount/4.f;
	for (size_t i = 0; i < pointCount; ++i)
	{
		points.push_back(center + centerOffset + (smallRadius * sf::Vector2f(cos(currentAngle), sin(currentAngle))));
		currentAngle += deltaAngle;
	}

	float turns = PI * 2 * 4;
	currentAngle = 0;
	size_t pointsPerTurn = 10;
	for (size_t i = 0; i < turns* pointsPerTurn; ++i)
	{
		sf::Vector2f pos = center - centerOffset + (0.f+i*0.5f)* sf::Vector2f(cos(currentAngle), sin(currentAngle));
		Particle* particle = new Particle;
		particle->setPos(pos);
		particle->setCharge(charge);
		particle->setStatic(false);
		particles.push_back(particle);
		currentAngle += deltaAngle;
	}

	Shape* shape = new Shape;
	shape->setPos(center);
	shape->setPoints(points);

	sim.addShape(shape);
	sim.addParticle(particles);
}