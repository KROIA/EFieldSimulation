#pragma once
//#define USE_THREADS
//#define SYNC_WITH_DISPLAY

#ifdef USE_THREADS
#include <thread>
#include <chrono>
#include <condition_variable>
#endif

// Display stuff
#include "display.h"
#include "displayInterface.h"
#include "textPainter.h"

// Simulation stuff
#include "chargeParticle.h"
#include "eField.h"
#include "bField.h"
#include "forceField.h"
#include "distributionPlot.h"
#include "pathPainter.h"
#include "shape.h"
#include "controlledParticle.h"

struct MouseChargeParticle
{
	ChargeParticle* ChargeParticle;

	float leftClickCharge;
	float rightClickCharge;
};
enum RenderLayerIndex
{
	eField = 0,
	bField = 7,
	forceField = 8,
	paricle = 1,
	mouseChargeParticle = 2,
	path = 3,
	shape = 4,
	plot = 5,
	text = 6
};
enum PhysicsMode
{
	electrostatic,
	magnetic
};

struct SimulationTimes
{
	TextPainter textPainter;
	double ticksPerSecond;
	double physicsTime;
	double collisionTime;
	double appyMovementTime;

	double simulationTime;
	double frameTime;
	double eventTime;
	double frameInterval;
};

class Simulation : public DisplayInterface
{
	public:
	struct Settings
	{
		sf::Vector2u windowSize;
		float targetFrameRate;
		size_t gridSizeX;

		float physicsTimeInterval;
		float leftClickCharge;
		float rightClickCharge;

		float eField_maxVectorLength;
		float eField_minVectorLength;

		float bField_maxVectorLength;
		float bField_minVectorLength;

		float forceField_maxVectorLength;
		float forceField_minVectorLength;

		PhysicsMode physicsMode;

	};
#ifdef USE_THREADS
	struct ThreadData
	{
		size_t index;
		size_t threadsCunt;
		size_t ChargeParticleCount;

		bool doLoop;
		bool workDone;
	};
#endif
	Simulation(const Settings &settings);
	~Simulation();

	void start();
	void stop();
	void addChargeParticle(ChargeParticle *ChargeParticle);
	void addChargeParticle(const vector<ChargeParticle*>& list);
	void clearChargeParticles();

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
#ifdef USE_THREADS
	void calculatePhysicsThreaded(size_t index, ThreadData* data);
#endif
	void checkCollisions();
	void applyMovements();
	void updateInfoText();

	void addChargeParticle(const sf::Vector2f& spawnPos, float charge, bool isStatic = false);
	void readDistribution(DistributionPlot* plot);
	//void readDistributionY(DistributionPlot* plot);

	bool m_simulationRunning;
	float m_simulationTimeInterval;
	sf::Vector2u m_windowSize;
	sf::Vector2f m_worldSize;
	sf::Vector2u m_gridSize;

	Display* m_display;
	vector<DistributionPlot*> m_distributionPlots;
	EField* m_eField;
	BField* m_bField;
	ForceField* m_forceField;

	vector<ChargeParticle*> m_ChargeParticles;
	vector<ChargeParticle*> m_eFieldChargeParticles;
	vector<PathPainter*> m_pathPatiners;
	vector<Shape*> m_shapes;


	MouseChargeParticle m_mouseChargeParticle;

	SimulationTimes m_simulationTimes;
	Settings m_settings;
	

#ifdef USE_THREADS

	vector<std::thread*> m_threads;
	vector<ThreadData> m_threadData;

	std::condition_variable m_thread_cv;
	std::condition_variable m_thread_cv2;
	std::mutex m_thread_mutex;
	std::mutex m_thread_mutex2;
	
	static size_t m_threadSyncCounter;
	static size_t m_threadSyncCounterTarget;
	static size_t m_counter;
#endif
};