#pragma once

// Display stuff
#include "display.h"
#include "displayInterface.h"

// Simulation stuff
#include "particle.h"
#include "eField.h"
#include "distributionPlot.h"
#include "pathPainter.h"
#include "shape.h"

struct MouseParticle
{
	Particle* particle;

	float leftClickCharge;
	float rightClickCharge;
};
enum RenderLayerIndex
{
	eField = 0,
	paricle = 1,
	mouseParticle = 2,
	path = 3,
	shape = 4,
	plot = 5
};

class Simulation : public DisplayInterface
{
	public:
	Simulation();
	~Simulation();

	void start();
	void stop();
	void addParticle(Particle *particle);
	void addParticle(const vector<Particle*>& list);
	void clearParticles();

	void addShape(Shape *shape);
	void addShape(const vector<Shape*>& list);
	void clearShapes();
	const sf::Vector2f& getWorldSize();
	const sf::Vector2u& getGridSize();
	const sf::Vector2u& getWindowSize();


	// Events from Displayinterface
	void onPreEventUpdate();
	void onEvent(const sf::Event& event);
	void onKeyEvent(const sf::Event& event);
	void onKeyPressEvent(const sf::Event& event);
	void onKeyReleaseEvent(const sf::Event& event);
	void onMouseEvent(const sf::Event& event);
	void onDisplyCloseEvent();
	void onPostEventUpdate();

	void onPreFrameUpdate();
	void onPostFrameUpdate();

	void onLoop();

	protected:
	void setup();
	void clean();
	void simulate();

	void calculatePhysics();
	void checkCollisions();
	void applyMovements();

	void addParticle(const sf::Vector2f& spawnPos, float charge, bool isStatic = false);
	void readDistribution(DistributionPlot* plot);
	//void readDistributionY(DistributionPlot* plot);

	bool m_simulationRunning;
	bool m_startEmpty;
	float m_simulationTimeInterval;
	sf::Vector2u m_windowSize;
	sf::Vector2f m_worldSize;
	sf::Vector2u m_gridSize;

	Display* m_display;
	vector<DistributionPlot*> m_distributionPlots;
	EField* m_eField;

	sf::Vector2u m_particleArraySize;
	vector<float> m_particleCharges;
	vector<Particle*> m_particles;
	vector<Particle*> m_eFieldParticles;
	vector<PathPainter*> m_pathPatiners;
	vector<Shape*> m_shapes;
	
	
	MouseParticle m_mouseParticle;
};