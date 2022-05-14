#include "simulation.h"


#ifdef USE_THREADS
size_t Simulation::m_threadSyncCounter = 0;
size_t Simulation::m_threadSyncCounterTarget = 1;
size_t Simulation::m_counter = 0;
#endif
Simulation::Simulation(const Settings& settings)
	: m_simulationTimes({ TextPainter(),0,0,0,0,0,0,0 })
{
	m_windowSize		= settings.windowSize;
	m_display			= new Display(m_windowSize, "E-Feld Simulation");
	m_display->setSizeFixed(true);
	m_display->frameRateTarget(settings.targetFrameRate);
	m_display->addSubscriber(this);
	
	m_gridSize			= sf::Vector2u(settings.gridSizeX, 0);
	
	m_simulationTimeInterval = settings.physicsTimeInterval; // sec.

	setup();

	m_mouseChargeParticle.leftClickCharge = settings.leftClickCharge;
	m_mouseChargeParticle.rightClickCharge = settings.rightClickCharge;
	m_eField->setVisible(true);
	m_eField->setMaxVectorLength(settings.eField_maxVectorLength);
	m_eField->setMinVectorLength(settings.eField_minVectorLength);

	m_bField->setVisible(true);
	m_bField->setMaxVectorLength(settings.bField_maxVectorLength);
	m_bField->setMinVectorLength(settings.bField_minVectorLength);

	m_forceField->setVisible(true);
	m_forceField->setMaxVectorLength(settings.forceField_maxVectorLength);
	m_forceField->setMinVectorLength(settings.forceField_minVectorLength);


#ifdef USE_THREADS
	m_threadData.resize(std::thread::hardware_concurrency());
	m_threads.resize(std::thread::hardware_concurrency());
	m_threadSyncCounterTarget = m_threads.size();

	//m_threadSyncCounter = 0;
	//m_threadSyncCounterTarget = 1;
	for (size_t i = 0; i < std::thread::hardware_concurrency(); ++i)
	{
		ThreadData data;
		data.threadsCunt = std::thread::hardware_concurrency();
		data.index = i;
		data.ChargeParticleCount = 0;
		data.doLoop = true;
		data.workDone = false;
		m_threadData[i] = data;
		m_threads[i] = new std::thread(&Simulation::calculatePhysicsThreaded, this, i, &m_threadData[i]);
	}
#endif
}
Simulation::~Simulation()
{
	clean();
}


void Simulation::start()
{
	m_simulationRunning = true;
	auto start = now();
	auto frameIntervalTime = now();
	auto tpsStart = now();
#ifdef SYNC_WITH_DISPLAY
	while (m_display->isOpen() &&
		   m_simulationRunning)
	{

		if (m_display->needsFrameUpdate())
		{
			auto simStart = now();
			simulate();
			auto simEnd = now();
			m_display->processEvents();
			auto events = now();
			m_display->draw();
			auto draw = now();
			setFilteredValue(m_simulationTimes.ticksPerSecond, milliseconds(simEnd - tpsStart), 0.3);
			setFilteredValue(m_simulationTimes.simulationTime,milliseconds(simEnd - simStart),0.3);
			setFilteredValue(m_simulationTimes.eventTime,milliseconds(events - simEnd), 0.3);
			setFilteredValue(m_simulationTimes.frameTime,milliseconds(draw - events), 0.3);
			setFilteredValue(m_simulationTimes.frameInterval,milliseconds(now() - frameIntervalTime), 0.3);
			frameIntervalTime = now();
			tpsStart = simEnd;

			updateInfoText();
		}
	}
#else
	
	while (m_display->isOpen() &&
		   m_simulationRunning)
	{
		auto simStart = now();
		simulate();
		auto simEnd = now();
		
		setFilteredValue(m_simulationTimes.ticksPerSecond, milliseconds(simEnd - tpsStart), 0.3);
		setFilteredValue(m_simulationTimes.simulationTime, milliseconds(simEnd - simStart), 0.3);
		tpsStart = simEnd;

		if (m_display->needsFrameUpdate())
		{
			
			m_display->processEvents();
			auto events = now();
			m_display->draw();
			auto draw = now();
			
			setFilteredValue(m_simulationTimes.eventTime, milliseconds(events - simEnd), 0.3);
			setFilteredValue(m_simulationTimes.frameTime, milliseconds(draw - events), 0.3);
			setFilteredValue(m_simulationTimes.frameInterval, milliseconds(now() - frameIntervalTime), 0.3);
			frameIntervalTime = now();

			updateInfoText();
		}
	}
#endif
}
void Simulation::stop() 
{
	m_display->exitLoop();
}

void Simulation::addChargeParticle(ChargeParticle* ChargeParticle)
{
	PathPainter* pathPainter = new PathPainter(ChargeParticle);
	m_pathPatiners.push_back(pathPainter);

	m_eField->addChargeParticle(ChargeParticle);
	m_bField->addChargeParticle(ChargeParticle);
	m_forceField->addChargeParticle(ChargeParticle);

	m_eFieldChargeParticles.push_back(ChargeParticle);
	m_ChargeParticles.push_back(ChargeParticle);
	m_display->addDrawable(ChargeParticle, RenderLayerIndex::paricle);
	m_display->addDrawable(pathPainter, RenderLayerIndex::path);
}
void Simulation::addChargeParticle(const vector<ChargeParticle*>& list)
{
	for (ChargeParticle* p : list)
	{
		addChargeParticle(p);
	}
}
void Simulation::clearChargeParticles()
{
	m_display->clearDrawable(RenderLayerIndex::paricle);
	m_display->clearDrawable(RenderLayerIndex::path);
	m_eField->clearChargeParticles();
	m_bField->clearChargeParticles();
	m_forceField->clearChargeParticles();
	for (ChargeParticle* p : m_eFieldChargeParticles)
		delete p;
	for (PathPainter* p : m_pathPatiners)
		delete p;
	m_eFieldChargeParticles.clear();
	m_ChargeParticles.clear();
	m_pathPatiners.clear();
	m_eField->addChargeParticle(m_mouseChargeParticle.ChargeParticle);
	m_bField->addChargeParticle(m_mouseChargeParticle.ChargeParticle);
	m_forceField->addChargeParticle(m_mouseChargeParticle.ChargeParticle);
	m_ChargeParticles.push_back(m_mouseChargeParticle.ChargeParticle);
}
void Simulation::addShape(Shape* shape)
{
	m_display->addDrawable(shape, RenderLayerIndex::shape);
	m_shapes.push_back(shape);
}
void Simulation::addShape(const vector<Shape*>& list)
{
	for (Shape* p : list)
	{
		addShape(p);
	}
}
void Simulation::clearShapes()
{
	m_display->clearDrawable(RenderLayerIndex::shape);
	for (Shape* p : m_shapes)
		delete p;
	m_shapes.clear();
}

const sf::Vector2f& Simulation::getWorldSize()
{
	return m_worldSize;
}
const sf::Vector2u& Simulation::getGridSize()
{
	return m_gridSize;
}
const sf::Vector2u& Simulation::getWindowSize()
{
	return m_windowSize;
}

// Events from Displayinterface
void Simulation::onPreEventUpdate()
{

}
void Simulation::onEvent(const sf::Event& event)
{

}
void Simulation::onKeyEvent(const sf::Event& event)
{
	switch (event.key.code)
	{
		
	}
}
void Simulation::onKeyPressEvent(const sf::Event& event)
{
	switch (event.key.code)
	{
		case sf::Keyboard::Key::Delete:
		{
			clearChargeParticles();
			break;
		}
		case sf::Keyboard::Key::E:
		{
			m_display->toggleLayerVisibility(RenderLayerIndex::eField);
			//m_eField->setVisible(!m_eField->isVisible());
			break;
		}
		case sf::Keyboard::Key::B:
		{
			m_display->toggleLayerVisibility(RenderLayerIndex::bField);
			//m_eField->setVisible(!m_eField->isVisible());
			break;
		}
		case sf::Keyboard::Key::F:
		{
			m_display->toggleLayerVisibility(RenderLayerIndex::forceField);
			//m_eField->setVisible(!m_eField->isVisible());
			break;
		}
		case sf::Keyboard::Key::P:
		{
			for (PathPainter* p : m_pathPatiners)
				p->clear();
			m_display->toggleLayerVisibility(RenderLayerIndex::path);
			//m_eField->setVisible(!m_eField->isVisible());
			break;
		}
		case sf::Keyboard::Key::I:
		{
			m_simulationTimes.textPainter.setVisible(!m_simulationTimes.textPainter.isVisible());
			break;
		}
		case sf::Keyboard::Key::Add:
		{
			m_eField->setMaxVectorLength(m_eField->getMaxVectorLength() * 0.75f);
			m_eField->setMinVectorLength(m_eField->getMinVectorLength() * 0.75f);
			m_bField->setMaxVectorLength(m_bField->getMaxVectorLength() * 0.75f);
			m_bField->setMinVectorLength(m_bField->getMinVectorLength() * 0.75f);
			m_forceField->setMaxVectorLength(m_forceField->getMaxVectorLength() * 0.75f);
			m_forceField->setMinVectorLength(m_forceField->getMinVectorLength() * 0.75f);
			break;
		}
		case sf::Keyboard::Key::Subtract:
		{
			m_eField->setMaxVectorLength(m_eField->getMaxVectorLength() * 1.5f);
			m_eField->setMinVectorLength(m_eField->getMinVectorLength() * 1.5f);
			m_bField->setMaxVectorLength(m_bField->getMaxVectorLength() * 1.5f);
			m_bField->setMinVectorLength(m_bField->getMinVectorLength() * 1.5f);
			m_forceField->setMaxVectorLength(m_forceField->getMaxVectorLength() * 1.5f);
			m_forceField->setMinVectorLength(m_forceField->getMinVectorLength() * 1.5f);
			break;
		}
	}
}
void Simulation::onKeyReleaseEvent(const sf::Event& event)
{
	switch (event.key.code)
	{

	}
}
void Simulation::onMouseEvent(const sf::Event& event)
{
	switch (event.type)
	{
		case sf::Event::MouseButtonPressed:
		{
			float mouseCharge;
			float newChargeParticleCharge = 50;
			bool spacePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
			
			switch (event.mouseButton.button)
			{
				case sf::Mouse::Button::Left:
				{
					mouseCharge = m_mouseChargeParticle.leftClickCharge;
					break;
				}
				case sf::Mouse::Button::Right:
				{
					mouseCharge = m_mouseChargeParticle.rightClickCharge;
					newChargeParticleCharge = -newChargeParticleCharge;
					break;
				}
			}
			if (spacePressed)
			{
				bool isStatic = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl);
				addChargeParticle(sf::Vector2f(m_display->getRelativeMousePos()), newChargeParticleCharge, isStatic);
			}
			else
			{
				m_mouseChargeParticle.ChargeParticle->setCharge(mouseCharge);
			}
			break;
		}
		case sf::Event::MouseButtonReleased:
		{
			switch (event.mouseButton.button)
			{
				case sf::Mouse::Button::Left:
				case sf::Mouse::Button::Right:
				{
					m_mouseChargeParticle.ChargeParticle->setCharge(0);
					break;
				}
			}
			break;
		}
		case sf::Event::MouseWheelScrolled:
		case sf::Event::MouseWheelMoved:
		{
			if (event.mouseWheel.delta > 0)
			{
				m_simulationTimeInterval *= 1.1;
			}
			else if(event.mouseWheel.delta < 0)
			{
				m_simulationTimeInterval *= 0.9;
			}
			
			
			//PRINT_MESSAGE("New Timeinterval: "<< m_simulationTimeInterval << "s\n")
			break;
		}
	}
}
void Simulation::onDisplyCloseEvent()
{
	
}
void Simulation::onPostEventUpdate()
{
	
}

void Simulation::onPreFrameUpdate()
{
	for (DistributionPlot* p : m_distributionPlots)
		readDistribution(p);
}
void Simulation::onPostFrameUpdate()
{
	m_mouseChargeParticle.ChargeParticle->setPos(sf::Vector2f(m_display->getRelativeMousePos()));
}

void Simulation::onLoop()
{
	// Not in use
}

void Simulation::setup()
{
	float distributionDisplaySize = 50;
	float boarderOffset			  = 5;
	m_worldSize = sf::Vector2f(m_windowSize.x - distributionDisplaySize,
							   m_windowSize.y - distributionDisplaySize);

	// Distribution Plots
	{
		DistributionPlot* distributionPlotX1 = new DistributionPlot;
		distributionPlotX1->setDimension(sf::Vector2f(m_windowSize.x - distributionDisplaySize - 2 * boarderOffset, distributionDisplaySize / 2));
		distributionPlotX1->setPos(sf::Vector2f(boarderOffset, m_windowSize.y - distributionDisplaySize));
		distributionPlotX1->setXResolution(m_windowSize.x / 100);

		DistributionPlot* distributionPlotX2 = new DistributionPlot;
		distributionPlotX2->setDimension(sf::Vector2f(m_windowSize.x - distributionDisplaySize - 2 * boarderOffset, distributionDisplaySize / 2));
		distributionPlotX2->setPos(sf::Vector2f(boarderOffset, m_windowSize.y - distributionDisplaySize / 2));
		distributionPlotX2->setXResolution(m_windowSize.x / 10);

		DistributionPlot* distributionPlotY1 = new DistributionPlot;
		distributionPlotY1->setDimension(sf::Vector2f(25, m_windowSize.y - distributionDisplaySize - 2 * boarderOffset));
		distributionPlotY1->setPos(sf::Vector2f(m_windowSize.x - distributionDisplaySize, boarderOffset));
		distributionPlotY1->setXResolution(m_windowSize.y / 100);
		distributionPlotY1->setAllignement(DistributionPlot::Allignement::y);

		DistributionPlot* distributionPlotY2 = new DistributionPlot;
		distributionPlotY2->setDimension(sf::Vector2f(25, m_windowSize.y - distributionDisplaySize - 2 * boarderOffset));
		distributionPlotY2->setPos(sf::Vector2f(m_windowSize.x - distributionDisplaySize / 2, boarderOffset));
		distributionPlotY2->setXResolution(m_windowSize.y / 10);
		distributionPlotY2->setAllignement(DistributionPlot::Allignement::y);

		m_distributionPlots = { distributionPlotX1 ,distributionPlotX2,
							    distributionPlotY1, distributionPlotY2 };
		m_display->addDrawable(distributionPlotX1,RenderLayerIndex::plot);
		m_display->addDrawable(distributionPlotX2,RenderLayerIndex::plot);
		m_display->addDrawable(distributionPlotY1,RenderLayerIndex::plot);
		m_display->addDrawable(distributionPlotY2,RenderLayerIndex::plot);
	}

	// E Field
	{
		m_gridSize.y = m_gridSize.x * (m_worldSize.y / m_worldSize.x);
		m_eField = new EField(m_worldSize, m_gridSize);
		m_bField = new BField(m_worldSize, m_gridSize);
		m_forceField = new ForceField(m_worldSize, m_gridSize);
		m_display->addDrawable(m_eField, RenderLayerIndex::eField);
		m_display->addDrawable(m_bField, RenderLayerIndex::bField);
		m_display->addDrawable(m_forceField, RenderLayerIndex::forceField);
	}

	// ChargeParticles
	{
		// Mouse ChargeParticle
		m_mouseChargeParticle.ChargeParticle = new ChargeParticle;
		m_mouseChargeParticle.ChargeParticle->setCharge(0);
		m_mouseChargeParticle.ChargeParticle->setStatic(true);
		m_ChargeParticles.push_back(m_mouseChargeParticle.ChargeParticle);
		m_display->addDrawable(m_mouseChargeParticle.ChargeParticle, RenderLayerIndex::mouseChargeParticle);
		m_eField->addChargeParticle(m_mouseChargeParticle.ChargeParticle);
		m_bField->addChargeParticle(m_mouseChargeParticle.ChargeParticle);
		m_forceField->addChargeParticle(m_mouseChargeParticle.ChargeParticle);
	}

	// Text
	{
		m_simulationTimes.textPainter.setColor(sf::Color(255, 255, 255));
		m_simulationTimes.textPainter.setOrigin(TextPainter::Origin::topLeft);
		m_simulationTimes.textPainter.setPos(sf::Vector2f(10,10));
		m_simulationTimes.textPainter.setSize(m_windowSize.x / 100);
		m_simulationTimes.textPainter.setVisible(false);

		m_display->addDrawable(&m_simulationTimes.textPainter, RenderLayerIndex::text);
	}
	
}
void Simulation::clean()
{
	m_display->clearDrawable();
	m_eField->clearChargeParticles();
	m_bField->clearChargeParticles();
	m_forceField->clearChargeParticles();

	for (ChargeParticle* p : m_eFieldChargeParticles)
		delete p;
	m_eFieldChargeParticles.clear();

	for (PathPainter* p : m_pathPatiners)
		delete p;
	m_pathPatiners.clear();

	delete m_mouseChargeParticle.ChargeParticle;
	m_mouseChargeParticle.ChargeParticle = nullptr;
	delete m_eField;
	m_eField = nullptr;
	delete m_bField;
	m_bField = nullptr;
	delete m_forceField;
	m_forceField = nullptr;
	delete m_display;
	m_display = nullptr;
	for (DistributionPlot* p : m_distributionPlots)
		delete p;
	m_distributionPlots.clear();

}
void Simulation::simulate()
{
	auto start = now();
	calculatePhysics();
	auto phys = now();
	checkCollisions();
	auto coll = now();
	applyMovements();
	auto move = now();

	setFilteredValue(m_simulationTimes.physicsTime     , milliseconds(phys - start),0.3);
	setFilteredValue(m_simulationTimes.collisionTime   , milliseconds(coll - phys) ,0.3);
	setFilteredValue(m_simulationTimes.appyMovementTime, milliseconds(move - coll) ,0.3);
}

void Simulation::calculatePhysics()
{
	m_eField->calculatePhysics(m_simulationTimeInterval);
	m_bField->calculatePhysics(m_simulationTimeInterval);
	m_forceField->calculatePhysics(m_simulationTimeInterval);
	m_eField->checkChargeParticleBounds();
	//m_bField->checkChargeParticleBounds();
	//m_forceField->checkChargeParticleBounds();

#ifdef USE_THREADS
	/*size_t threadCount = 6;
	vector<std::thread*> threads(threadCount);
	//threads.reserve(threadCount);
	size_t size = m_ChargeParticles.size() / threadCount;
	size_t rest = m_ChargeParticles.size() - size * threadCount;
	size_t startIndex = 0;
	
	for (size_t i = 0; i < threadCount; ++i)
	{
		//m_threads[i].
		threads[i] = new std::thread(&Simulation::calculatePhysicsThreaded, this, i, size);
	}
	if(rest)
		calculatePhysicsThreaded(threadCount, rest);
	
	for (std::thread* t : threads)
	{
		t->join();
		delete t;
	}*/
	{
		std::unique_lock<std::mutex> lck(m_thread_mutex);
		m_threadSyncCounter = 1;
		m_thread_cv.notify_all();
	}
	
	//++m_threadSyncCounterTarget;
	for (m_counter = 0; m_counter < m_threadData.size(); ++m_counter)
	{
		{
			std::unique_lock<std::mutex> lock(m_thread_mutex2);
			m_thread_cv2.wait(lock, [this]() { return m_threadData[m_counter].workDone; });
		}
	}
	/*size_t doneCount = 0;
	while (doneCount != m_threadData.size())
	{
		doneCount = 0;
		for (size_t i = 0; i < m_threadData.size(); ++i)
		{
			doneCount += m_threadData[i].workDone;
		}
	}*/
	
	for (size_t i = 0; i < m_threadData.size(); ++i)
	{
		m_threadData[i].workDone = false;
		m_threadData[i].ChargeParticleCount = m_ChargeParticles.size();
	}
	{
		std::unique_lock<std::mutex> lck(m_thread_mutex);
		m_threadSyncCounter = 0;
		m_thread_cv.notify_all();
	}
	

#else
	size_t size = m_ChargeParticles.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (m_ChargeParticles[i]->isStatic())
			continue;
		m_ChargeParticles[i]->calculatePhysiscs(m_ChargeParticles, m_simulationTimeInterval);
	}
#endif
	for (Shape* s : m_shapes)
		s->calculatePhysics(m_simulationTimeInterval);
}
#ifdef USE_THREADS
void Simulation::calculatePhysicsThreaded(size_t index, ThreadData *data)
{
	if (!data)
		return;

	bool doLoop = true;
	size_t targetCounter = 1;

	size_t size = data->ChargeParticleCount / data->threadsCunt;
	size_t start = index * size;
	size_t end = start + size;
	size_t rest = data->ChargeParticleCount - size * data->threadsCunt;

	while (doLoop)
	{

		{
			std::unique_lock<std::mutex> lk(m_thread_mutex);
			//m_thread_cv.wait(lk, targetCounter == m_threadSyncCounter);
			m_thread_cv.wait(lk, [] {return m_threadSyncCounter; });
			
		}
		doLoop = data->doLoop;
		//++targetCounter;
		//{
		//	std::unique_lock<std::mutex> lk(m_thread_mutex);
		//	++m_threadSyncCounterTarget;
		//}
		size = data->ChargeParticleCount / data->threadsCunt;
		start = index * size;
		end = start + size;
		if (index == data->threadsCunt - 1)
		{
			rest = data->ChargeParticleCount - size * data->threadsCunt;
			end += rest;
		}
		
		for (size_t i = start; i < end; ++i)
		{
			if (m_ChargeParticles[i]->isStatic())
				continue;
			m_ChargeParticles[i]->calculatePhysiscs(m_ChargeParticles, m_simulationTimeInterval);
		}
		{
			//std::unique_lock<std::mutex> lock(m_thread_mutex2);
			data->workDone = true;
		}
		

		{
			std::unique_lock<std::mutex> lk(m_thread_mutex);
			//m_thread_cv.wait(lk, targetCounter == m_threadSyncCounter);
			m_thread_cv.wait(lk, [] {return !m_threadSyncCounter; });

		}
		//++m_threadSyncCounterTarget;
	}




	/*
	size_t start = index * size;
	size_t end = start + size;
	if (end > m_ChargeParticles.size())
		end = m_ChargeParticles.size();

	for (size_t i = start; i < end; ++i)
	{
		if (m_ChargeParticles[i]->isStatic())
			continue;
		m_ChargeParticles[i]->calculatePhysiscs(m_ChargeParticles, m_simulationTimeInterval);
	}*/
}
#endif
void Simulation::checkCollisions()
{
	for (Shape* s : m_shapes)
		s->checkCollision(m_eFieldChargeParticles);
}
void Simulation::applyMovements()
{
	for (ChargeParticle* p : m_eFieldChargeParticles)
	{
		if (p->isStatic())
			continue;
		p->applyPhysics();
	}
}
void Simulation::updateInfoText()
{
	if (!m_simulationTimes.textPainter.isVisible())
		return;
	string text = "";
	if (m_simulationTimes.frameInterval > 0)
	{
		text+= "TPS: "+  floatToString(1000.f / m_simulationTimes.ticksPerSecond,3) + "\n";
	}

	text+= "physics delta t:  " + floatToString(m_simulationTimeInterval, 3) + " s\n";
	text+= "Process time: \n";
	text+= " physicsTime:      " + floatToString( m_simulationTimes.physicsTime, 3) + " ms\n";
	text+= " collisionTime:    " + floatToString( m_simulationTimes.collisionTime, 3) + " ms\n";
	text+= " appyMovementTime: " + floatToString( m_simulationTimes.appyMovementTime, 3) + " ms\n";
	text+= " simulationTime:   " + floatToString( m_simulationTimes.simulationTime, 3) + " ms\n";
	text+= " frameTime:        " + floatToString( m_simulationTimes.frameTime, 3) + " ms\n";
	text+= " eventTime:        " + floatToString( m_simulationTimes.eventTime, 3) + " ms\n";
	text+= " frameInterval:    " + floatToString( m_simulationTimes.frameInterval, 3) + " ms\n\n";
	m_simulationTimes.textPainter.setText(text);
}

void Simulation::addChargeParticle(const sf::Vector2f& spawnPos, float charge, bool isStatic)
{
	ChargeParticle* p = new ChargeParticle();
	p->setCharge(charge);
	p->setPos(spawnPos);
	p->setStatic(isStatic);
	PathPainter* pathPainter = new PathPainter(p);
	//pathPainter->setColor(sf::Color(0, 150, 180));
	m_pathPatiners.push_back(pathPainter);
	
	m_eField->addChargeParticle(p);
	m_bField->addChargeParticle(p);
	m_forceField->addChargeParticle(p);
	m_eFieldChargeParticles.push_back(p);
	m_ChargeParticles.push_back(p);
	m_display->addDrawable(p,RenderLayerIndex::paricle);
	m_display->addDrawable(pathPainter, RenderLayerIndex::path);
}

void Simulation::readDistribution(DistributionPlot* plot)
{
	vector<ChargeParticle*> ChargeParticles = m_eFieldChargeParticles;
	float offset;
	float dim;
	size_t div = plot->getXResolution();
	plot->clearCount();

	if (plot->getAllignement() == DistributionPlot::Allignement::x)
	{
		offset = plot->getPos().x;
		dim = plot->getDimension().x;
	}
	else
	{
		offset = plot->getPos().y;
		dim = plot->getDimension().y;
	}

	for (size_t x = 0; x < div; ++x)
	{
		float min = (x * dim) / div + offset;
		float max = min + (float)dim / (float)div;

		for (size_t i = 0; i < ChargeParticles.size(); ++i)
		{
			if (ChargeParticles[i] == nullptr)
				continue;
			float pos;
			if (plot->getAllignement() == DistributionPlot::Allignement::x)
			{
				pos = ChargeParticles[i]->getPos().x;
			}
			else
			{
				pos = ChargeParticles[i]->getPos().y;
			}
			if (pos > min && pos < max)
			{
				plot->addCount(x, 1);
				ChargeParticles[i] = nullptr;
			}
		}
	}
	
	
}
