#include <iostream>


#include "display.h"
#include "particle.h"
#include "eField.h"
#include "distributionPlot.h"
#include "simulation.h"

void buildLevel_1(Simulation& sim);
void buildLevel_2(Simulation& sim);
void buildLevel_orbit(Simulation& sim);
void aufgabe_4_1(Simulation& sim);
void buildLevel_shapes(Simulation& sim);

int main()
{
    Simulation simulation;
	//buildLevel_1(simulation);
	//buildLevel_2(simulation);
	//buildLevel_orbit(simulation);
	//buildLevel_orbit(simulation);
	buildLevel_shapes(simulation);
    simulation.start();
  
}

void buildLevel_1(Simulation& sim)
{
	sf::Vector2f arraySize(50, 20);
	vector<float> particleCharges = { 2 };

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

void buildLevel_2(Simulation& sim)
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
	left->setCharge(100);
	left->setStatic(true);
	left->setSize(20);

	Particle* right = new Particle;
	right->setPos(worldSize / 2.f + sf::Vector2f(400, 0));
	right->setCharge(100);
	right->setStatic(true);
	right->setSize(20);

	Particle* q = new Particle;
	q->setPos(worldSize / 2.f + sf::Vector2f(0, -400));
	q->setCharge(-200);
	q->setStatic(false);
	q->setSize(20);

	particles = { left,right,q };


	sim.addParticle(particles);
}

void buildLevel_shapes(Simulation& sim)
{
	sf::Vector2f worldSize = sim.getWorldSize();
	//vector<Shape*> shapes;

	Shape* shape1 = new Shape;
	shape1->setPos(worldSize / 2.f);
	shape1->setRotation(PI/4.f);

	Particle* particle = new Particle;
	particle->setPos(shape1->getPos());
	particle->setCharge(5);


	sim.addParticle(particle);
	sim.addShape(shape1);
}