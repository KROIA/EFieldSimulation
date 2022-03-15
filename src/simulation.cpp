#include "simulation.h"



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

	m_mouseParticle.leftClickCharge = settings.leftClickCharge;
	m_mouseParticle.rightClickCharge = settings.rightClickCharge;
	m_eField->setVisible(true);
	m_eField->setMaxVectorLength(settings.eField_maxVectorLength);

//#ifdef USE_THREADS
//	m_threads.resize(std::thread::hardware_concurrency());
//#endif
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
			setFilteredValue(m_simulationTimes.simulationTime,milliseconds(simEnd - simStart),0.3);
			setFilteredValue(m_simulationTimes.eventTime,milliseconds(events - simEnd), 0.3);
			setFilteredValue(m_simulationTimes.frameTime,milliseconds(draw - events), 0.3);
			setFilteredValue(m_simulationTimes.frameInterval,milliseconds(now() - frameIntervalTime), 0.3);
			frameIntervalTime = now();

			updateInfoText();
		}
	}
}
void Simulation::stop() 
{
	m_display->exitLoop();
}

void Simulation::addParticle(Particle* particle)
{
	PathPainter* pathPainter = new PathPainter(particle);
	m_pathPatiners.push_back(pathPainter);

	m_eField->addParticle(particle);
	m_eFieldParticles.push_back(particle);
	m_particles.push_back(particle);
	m_display->addDrawable(particle, RenderLayerIndex::paricle);
	m_display->addDrawable(pathPainter, RenderLayerIndex::path);
}
void Simulation::addParticle(const vector<Particle*>& list)
{
	for (Particle* p : list)
	{
		addParticle(p);
	}
}
void Simulation::clearParticles()
{
	m_display->clearDrawable(RenderLayerIndex::paricle);
	m_display->clearDrawable(RenderLayerIndex::path);
	m_eField->clearParticles();
	for (Particle* p : m_eFieldParticles)
		delete p;
	for (PathPainter* p : m_pathPatiners)
		delete p;
	m_eFieldParticles.clear();
	m_particles.clear();
	m_pathPatiners.clear();
	m_eField->addParticle(m_mouseParticle.particle);
	m_particles.push_back(m_mouseParticle.particle);
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
			clearParticles();
			break;
		}
		case sf::Keyboard::Key::E:
		{
			m_display->toggleLayerVisibility(RenderLayerIndex::eField);
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
			break;
		}
		case sf::Keyboard::Key::Subtract:
		{
			m_eField->setMaxVectorLength(m_eField->getMaxVectorLength() * 1.5f);
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
			float newParticleCharge = 5;
			bool spacePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
			
			switch (event.mouseButton.button)
			{
				case sf::Mouse::Button::Left:
				{
					mouseCharge = m_mouseParticle.leftClickCharge;
					break;
				}
				case sf::Mouse::Button::Right:
				{
					mouseCharge = m_mouseParticle.rightClickCharge;
					newParticleCharge = -newParticleCharge;
					break;
				}
			}
			if (spacePressed)
			{
				bool isStatic = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl);
				addParticle(sf::Vector2f(m_display->getRelativeMousePos()), newParticleCharge, isStatic);
			}
			else
			{
				m_mouseParticle.particle->setCharge(mouseCharge);
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
					m_mouseParticle.particle->setCharge(0);
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
	m_mouseParticle.particle->setPos(sf::Vector2f(m_display->getRelativeMousePos()));
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
		m_display->addDrawable(m_eField, RenderLayerIndex::eField);
	}

	// particles
	{
		// Mouse Particle
		m_mouseParticle.particle = new Particle;
		m_mouseParticle.particle->setCharge(0);
		m_mouseParticle.particle->setStatic(true);
		m_particles.push_back(m_mouseParticle.particle);
		m_display->addDrawable(m_mouseParticle.particle, RenderLayerIndex::mouseParticle);
		m_eField->addParticle(m_mouseParticle.particle);
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
	m_eField->clearParticles();

	for (Particle* p : m_eFieldParticles)
		delete p;
	m_eFieldParticles.clear();

	for (PathPainter* p : m_pathPatiners)
		delete p;
	m_pathPatiners.clear();

	delete m_mouseParticle.particle;
	m_mouseParticle.particle = nullptr;
	delete m_eField;
	m_eField = nullptr;
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
	m_eField->checkParticleBounds();
	
#ifdef USE_THREADS
	//size_t threadCount = 6;
	////vector<std::thread> threads;
	////threads.reserve(threadCount);
	//size_t size = m_particles.size() / threadCount;
	//size_t rest = m_particles.size() - size * threadCount;
	//size_t startIndex = 0;
	//
	//for (size_t i = 0; i < threadCount; ++i)
	//{
	//	m_threads[i].
	//	threads.push_back(std::thread(&Simulation::calculatePhysicsThreaded, this, startIndex, size));
	//	startIndex += size;
	//}
	//if(rest)
	//	calculatePhysicsThreaded(startIndex, rest);
	//
	//for (std::thread& t : m_threads)
	//{
	//	t.join();
	//}
#else
	calculatePhysicsThreaded(0, m_particles.size());
#endif
		
	for (Shape* s : m_shapes)
		s->calculatePhysics(m_simulationTimeInterval);
}
void Simulation::calculatePhysicsThreaded(size_t particleIndex, size_t size)
{
	size_t end = particleIndex + size;
	if (end > m_particles.size())
		end = m_particles.size();

	for (size_t i = particleIndex; i < end; ++i)
	{
		if (m_particles[i]->isStatic())
			continue;
		m_particles[i]->calculatePhysiscs(m_particles, m_simulationTimeInterval);
	}
}
void Simulation::checkCollisions()
{
	for (Shape* s : m_shapes)
		s->checkCollision(m_eFieldParticles);
}
void Simulation::applyMovements()
{
	for (Particle* p : m_eFieldParticles)
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
		text+= "TPS: "+  floatToString(1000.f / m_simulationTimes.frameInterval,3) + "\n";
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

void Simulation::addParticle(const sf::Vector2f& spawnPos, float charge, bool isStatic)
{
	Particle* p = new Particle();
	p->setCharge(charge);
	p->setPos(spawnPos);
	p->setStatic(isStatic);
	PathPainter* pathPainter = new PathPainter(p);
	//pathPainter->setColor(sf::Color(0, 150, 180));
	m_pathPatiners.push_back(pathPainter);
	
	m_eField->addParticle(p);
	m_eFieldParticles.push_back(p);
	m_particles.push_back(p);
	m_display->addDrawable(p,RenderLayerIndex::paricle);
	m_display->addDrawable(pathPainter, RenderLayerIndex::path);
}

void Simulation::readDistribution(DistributionPlot* plot)
{
	vector<Particle*> particles = m_eFieldParticles;
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

		for (size_t i = 0; i < particles.size(); ++i)
		{
			if (particles[i] == nullptr)
				continue;
			float pos;
			if (plot->getAllignement() == DistributionPlot::Allignement::x)
			{
				pos = particles[i]->getPos().x;
			}
			else
			{
				pos = particles[i]->getPos().y;
			}
			if (pos > min && pos < max)
			{
				plot->addCount(x, 1);
				particles[i] = nullptr;
			}
		}
	}
	
	
}
