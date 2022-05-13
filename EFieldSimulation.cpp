#include <iostream>


#include "display.h"
#include "chargeParticle.h"
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
void buildLevel_spread2(Simulation& sim);
void buildLevel_spreadSquare(Simulation& sim);
void buildLevel_koaxialCable(Simulation& sim);
void buildLevel_koaxialCableShell(Simulation& sim, size_t outerCount = 64, double radius = 250, float charge = 5);

int main()
{
	// ChargeParticle::setStandard_charge(0);
	// ChargeParticle::setStandard_size(10);
	 ChargeParticle::setStandard_drag(0.001);
	// ChargeParticle::setStandard_maxVelocity(10000);

	Simulation::Settings settings;
	//settings.windowSize				= sf::Vector2u(1900, 900)*2u;
	settings.windowSize				= sf::Vector2u(1900, 900);
	settings.targetFrameRate		= 60;
	settings.gridSizeX				= settings.windowSize.x/19;
	settings.physicsTimeInterval	= 0.1; // sec
	settings.leftClickCharge		= 5*64;
	settings.rightClickCharge		= -5*64;
	settings.eField_maxVectorLength	= 1e7;
	settings.eField_minVectorLength	= 1e5;
	settings.bField_maxVectorLength = 1e-8;
	settings.bField_minVectorLength = 1e-11;
	

    Simulation simulation(settings);
	//buildLevel_1(simulation);
	//buildLevel_cathodeRayTube(simulation); // Analysis Aufgabe
	//buildLevel_orbit(simulation);
	//aufgabe_4_1(simulation);
	//buildLevel_shapes(simulation);
	//buildLevel_influence(simulation);
	//buildLevel_spread(simulation);
	//buildLevel_spread2(simulation);
	//buildLevel_spreadSquare(simulation);
	buildLevel_koaxialCable(simulation);
	//buildLevel_koaxialCableShell(simulation);
    simulation.start();
	return 0;
}

void buildLevel_1(Simulation& sim)
{
	sf::Vector2f arraySize(50, 20);
	vector<float> ChargeParticleCharges = { 2 };
	//vector<float> ChargeParticleCharges = { 2 , -2};

	sf::Vector2f worldSize = sim.getWorldSize();
	vector<ChargeParticle*> ChargeParticles;

	// ChargeParticle Array
	float boarder = 20;
	size_t counter = 0;
	ChargeParticles.reserve(arraySize.x * arraySize.y);
	for (size_t x = 0; x < arraySize.x; ++x)
	{
		for (size_t y = 0; y < arraySize.y; ++y)
		{
			float cha = ChargeParticleCharges[counter % ChargeParticleCharges.size()];
			sf::Vector2f pos(boarder + (float)x * (worldSize.x) / (float)arraySize.x,
							 boarder + (float)y * (worldSize.y) / (float)arraySize.y);

			ChargeParticle* p = new ChargeParticle();
			p->setCharge(cha);
			p->setPos(pos);
			p->setStatic(false);


			ChargeParticles.push_back(p);

			++counter;
		}
	}
	sim.addChargeParticle(ChargeParticles);
}

void buildLevel_cathodeRayTube(Simulation& sim)
{
	sf::Vector2f worldSize = sim.getWorldSize();

	sf::Vector2u grid(10, 4);
	sf::Vector2f spacing(50, 25);
	float gab = 100;
	float charge = 5;
	
	vector<ChargeParticle*> ChargeParticles;
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
			ChargeParticle* ChargeParticle1 = new ChargeParticle;
			ChargeParticle1->setPos(pos1);
			ChargeParticle1->setCharge(charge);
			ChargeParticle1->setStatic(true);
			ChargeParticles.push_back(ChargeParticle1);


			sf::Vector2f pos2 = sartPosNegative + sf::Vector2f(x * spacing.x, y * spacing.y);
			ChargeParticle* ChargeParticle2 = new ChargeParticle;
			ChargeParticle2->setPos(pos2);
			ChargeParticle2->setCharge(-charge);
			ChargeParticle2->setStatic(true);
			ChargeParticles.push_back(ChargeParticle2);
		}
	}

	ChargeParticle* bullet = new ChargeParticle;
	bullet->setPos(sf::Vector2f(20, worldSize.y / 2.f));
	bullet->setCharge(charge);
	bullet->setVelocity(sf::Vector2f(8, 0));
	bullet->setDrag(0);
	ChargeParticles.push_back(bullet);

	sim.addChargeParticle(ChargeParticles);
}


void buildLevel_orbit(Simulation& sim)
{
	sf::Vector2f worldSize = sim.getWorldSize();
	vector<ChargeParticle*> ChargeParticles;

	ChargeParticle* planet = new ChargeParticle;
	planet->setPos(worldSize/2.f);
	planet->setCharge(-100);
	planet->setStatic(true);

	{
		ChargeParticle* moon = new ChargeParticle;
		moon->setPos(worldSize / 2.f + sf::Vector2f(0, 100));
		moon->setCharge(10);
		moon->setVelocity(sf::Vector2f(2, 0));
		ChargeParticles.push_back(moon);
	}
	{
		ChargeParticle* moon = new ChargeParticle;
		moon->setPos(worldSize / 2.f + sf::Vector2f(0, -100));
		moon->setCharge(10);
		moon->setVelocity(sf::Vector2f(-2, 0));
		ChargeParticles.push_back(moon);
	}
	
	ChargeParticles.push_back(planet);


	sim.addChargeParticle(ChargeParticles);
}
void aufgabe_4_1(Simulation& sim)
{
	sf::Vector2f worldSize = sim.getWorldSize();
	vector<ChargeParticle*> ChargeParticles;

	ChargeParticle* left = new ChargeParticle;
	left->setPos(worldSize / 2.f + sf::Vector2f(-400, 0));
	left->setCharge(10);
	left->setStatic(true);
	left->setRadius(20);

	ChargeParticle* right = new ChargeParticle;
	right->setPos(worldSize / 2.f + sf::Vector2f(400, 0));
	right->setCharge(10);
	right->setStatic(true);
	right->setRadius(20);

	ChargeParticle* q = new ChargeParticle;
	q->setPos(worldSize / 2.f + sf::Vector2f(0, -400));
	q->setCharge(-20);
	q->setStatic(false);
	q->setRadius(20);

	ChargeParticles = { left,right,q };


	sim.addChargeParticle(ChargeParticles);
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

	//ChargeParticle* ChargeParticle = new ChargeParticle;
	//ChargeParticle->setPos(shape1->getPos());
	//ChargeParticle->setCharge(5);


	//sim.addChargeParticle(ChargeParticle);
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
	
	float chargeA = 5;
	float chargeB = -5;

	vector<ChargeParticle*> ChargeParticles;
	
	sf::Vector2f sartPosPositive = middlePoint + sf::Vector2f(-(gab / 2.f + grid.x * spacing.x),
															  -(grid.y * spacing.y / 2.f));
	sf::Vector2f sartPosNegative = middlePoint + sf::Vector2f(gab / 2.f, -(grid.y * spacing.y / 2.f));


	for (size_t x = 0; x < grid.x; ++x)
	{
		for (size_t y = 0; y < grid.y; ++y)
		{
			sf::Vector2f pos1 = sartPosPositive + sf::Vector2f(x * spacing.x, y * spacing.y);
			ChargeParticle* ChargeParticle1 = new ChargeParticle;
			ChargeParticle1->setPos(pos1);
			ChargeParticle1->setCharge(chargeA);
			ChargeParticle1->setStatic(false);
			ChargeParticles.push_back(ChargeParticle1);


			sf::Vector2f pos2 = sartPosNegative + sf::Vector2f(x * spacing.x, y * spacing.y);
			ChargeParticle* ChargeParticle2 = new ChargeParticle;
			ChargeParticle2->setPos(pos2);
			ChargeParticle2->setCharge(chargeB);
			ChargeParticle2->setStatic(false);
			ChargeParticles.push_back(ChargeParticle2);
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
	sim.addChargeParticle(ChargeParticles);
}


void buildLevel_spread(Simulation& sim)
{
	sf::Vector2f worldSize = sim.getWorldSize();
	sf::Vector2f center = worldSize / 2.f;


	float charge = 2;

	float largeRadius = 350;
	float smallRadius = 20;
	float spacing = worldSize.x-(largeRadius+ smallRadius+100); // Space between the center of the circles

	size_t pointCount = 20;

	
	sf::Vector2f centerOffset(spacing / 2.f, 0);
	vector<sf::Vector2f> points;
	vector<ChargeParticle*> ChargeParticles;
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
		ChargeParticle* chargeParticle = new ChargeParticle;
		chargeParticle->setPos(pos);
		chargeParticle->setCharge(charge);
		chargeParticle->setStatic(false);
		ChargeParticles.push_back(chargeParticle);
		currentAngle += deltaAngle;
	}

	Shape* shape = new Shape;
	shape->setPos(center);
	shape->setPoints(points);

	sim.addShape(shape);
	sim.addChargeParticle(ChargeParticles);
}
void buildLevel_spread2(Simulation& sim)
{
	sf::Vector2f worldSize = sim.getWorldSize();
	sf::Vector2f middlePoint = worldSize / 2.f;

	sf::Vector2f spacing(50, 25);
	sf::Vector2u grid((worldSize.x - 200) / spacing.x, 4);

	float charge = 5;

	vector<ChargeParticle*> ChargeParticles;

	sf::Vector2f sartPosPositive = middlePoint + sf::Vector2f(-(grid.x * spacing.x)/ 2.f,
															  -(grid.y * spacing.y / 2.f));
	//sf::Vector2f sartPosNegative = middlePoint + sf::Vector2f(gab / 2.f, -(grid.y * spacing.y / 2.f));


	for (size_t x = 0; x < grid.x; ++x)
	{
		for (size_t y = 0; y < grid.y; ++y)
		{
			sf::Vector2f pos1 = sartPosPositive + sf::Vector2f(x * spacing.x, y * spacing.y);
			ChargeParticle* ChargeParticle1 = new ChargeParticle;
			ChargeParticle1->setPos(pos1);
			ChargeParticle1->setCharge(charge);
			ChargeParticle1->setStatic(false);
			ChargeParticles.push_back(ChargeParticle1);


	/*		sf::Vector2f pos2 = sartPosNegative + sf::Vector2f(x * spacing.x, y * spacing.y);
			ChargeParticle* ChargeParticle2 = new ChargeParticle;
			ChargeParticle2->setPos(pos2);
			ChargeParticle2->setCharge(-charge);
			ChargeParticle2->setStatic(false);
			ChargeParticles.push_back(ChargeParticle2);*/
		}
	}


	sf::Vector2f rectSize((grid.x + 1) * spacing.x,
						  (grid.y + 1) * spacing.y);
	vector<sf::Vector2f> rectPoints = Shape::getGenerateRect(rectSize);


	Shape* shape1 = new Shape;
	shape1->setPos(sartPosPositive + rectSize / 2.f - spacing);
	shape1->setPoints(rectPoints);
	//shape2->setRotation(PI / 2.f);
	/*
	Shape* shape2 = new Shape;
	shape2->setPos(sartPosNegative + rectSize / 2.f - spacing);
	shape2->setPoints(rectPoints);
	//shape2->setRotation(PI / 2.f);*/

	sim.addShape(shape1);
	//sim.addShape(shape2);
	sim.addChargeParticle(ChargeParticles);
}

void buildLevel_spreadSquare(Simulation& sim)
{
	sf::Vector2f worldSize = sim.getWorldSize();
	sf::Vector2f middlePoint = worldSize / 2.f;

	sf::Vector2f spacing(50, 25);
	sf::Vector2u grid((worldSize.x - 200) / spacing.x, 4);

	float charge = 5;

	vector<ChargeParticle*> ChargeParticles;

	sf::Vector2f sartPosPositive = middlePoint + sf::Vector2f(-(grid.x * spacing.x) / 2.f,
															  -(grid.y * spacing.y / 2.f));
	//sf::Vector2f sartPosNegative = middlePoint + sf::Vector2f(gab / 2.f, -(grid.y * spacing.y / 2.f));


	/*for (size_t x = 0; x < grid.x; ++x)
	{
		for (size_t y = 0; y < grid.y; ++y)
		{
			sf::Vector2f pos1 = sartPosPositive + sf::Vector2f(x * spacing.x, y * spacing.y);
			ChargeParticle* ChargeParticle1 = new ChargeParticle;
			ChargeParticle1->setPos(pos1);
			ChargeParticle1->setCharge(charge);
			ChargeParticle1->setStatic(false);
			ChargeParticles.push_back(ChargeParticle1);


		}
	}*/


	sf::Vector2f rectSize(500,
						  500);
	vector<sf::Vector2f> rectPoints = Shape::getGenerateRect(rectSize);


	Shape* shape1 = new Shape;
	shape1->setPos(middlePoint - rectSize / 2.f);
	shape1->setPoints(rectPoints);
	//shape2->setRotation(PI / 2.f);
	/*
	Shape* shape2 = new Shape;
	shape2->setPos(sartPosNegative + rectSize / 2.f - spacing);
	shape2->setPoints(rectPoints);
	//shape2->setRotation(PI / 2.f);*/

	sim.addShape(shape1);
	//sim.addShape(shape2);
	//sim.addChargeParticle(ChargeParticles);
}

void buildLevel_koaxialCable(Simulation& sim)
{
	sf::Vector2f worldSize = sim.getWorldSize();
	sf::Vector2f middlePoint = worldSize / 2.f;
	float charge = 5;

	double radius = 250;
	size_t outerCount = 64;
	double dAlpha = 2.f*PI / (double)outerCount;

	buildLevel_koaxialCableShell(sim,outerCount,radius,charge);

	ChargeParticle* ChargeParticle1 = new ChargeParticle;
	ChargeParticle1->setPos(middlePoint+sf::Vector2f(radius/2,0));
	ChargeParticle1->setCharge(-charge * (outerCount));
	ChargeParticle1->setStatic(true);

	sim.addChargeParticle(ChargeParticle1);
}
void buildLevel_koaxialCableShell(Simulation& sim, size_t outerCount, double radius, float charge)
{
	sf::Vector2f worldSize = sim.getWorldSize();
	sf::Vector2f middlePoint = worldSize / 2.f;

	double dAlpha = 2.f * PI / (double)outerCount;
	vector<ChargeParticle*> chargeParticles;

	for (size_t x = 0; x < outerCount; ++x)
	{
		sf::Vector2f pos1 = middlePoint + sf::Vector2f(radius * cos((double)x * dAlpha), radius * sin((double)x * dAlpha));
		ChargeParticle* ChargeParticle1 = new ChargeParticle;
		ChargeParticle1->setPos(pos1);
		ChargeParticle1->setCharge(charge);
		ChargeParticle1->setStatic(true);
		chargeParticles.push_back(ChargeParticle1);

	}

	sim.addChargeParticle(chargeParticles);
}