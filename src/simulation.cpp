#include "simulation.h"


Simulation::Simulation()
{
	m_windowSize		= sf::Vector2u(1900, 900)*2u;
	m_display			= new Display(m_windowSize, "E-Feld Simulation");
	m_display->setSizeFixed(true);
	m_display->frameRateTarget(30);
	m_display->addSubscriber(this);
	
	size_t gridX		= 100;
	m_gridSize			= sf::Vector2u(gridX, 0);
	
	m_startEmpty		= false;
	m_particleArraySize = sf::Vector2u(50, 15);
	m_particleCharges   = { 2 };
	//m_particleCharges   = { 2,-2 };
	m_simulationTimeInterval = 5; // sec.

	setup();

	m_mouseParticle.leftClickCharge = 100;
	m_mouseParticle.rightClickCharge = -100;
	m_eField->setVisible(true);
	m_eField->setMaxVectorLength(1e8);
}
Simulation::~Simulation()
{
	delete m_display;
	clean();
}


void Simulation::start()
{
	m_simulationRunning = true;
	while (m_display->isOpen() &&
		   m_simulationRunning)
	{
		if (m_display->needsFrameUpdate())
		{
			simulate();
			m_display->processEvents();
			m_display->draw();
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
			
			
			PRINT_MESSAGE("New Timeinterval: "<< m_simulationTimeInterval << "s\n")
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
		m_gridSize.y = m_gridSize.x * m_worldSize.y / m_worldSize.x;
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

		/*
		// Particle Array
		if(!m_startEmpty)
		{
			float boarder = 20;
			size_t counter = 0;
			m_eFieldParticles.reserve(m_particleArraySize.x * m_particleArraySize.y);
			for (size_t x = 0; x < m_particleArraySize.x; ++x)
			{
				for (size_t y = 0; y < m_particleArraySize.y; ++y)
				{
					float cha = m_particleCharges[counter % m_particleCharges.size()];
					sf::Vector2f pos(boarder + (float)x * (m_worldSize.x) / (float)m_particleArraySize.x,
									 boarder + (float)y * (m_worldSize.y) / (float)m_particleArraySize.y);

					addParticle(pos, cha);
					++counter;
				}
			}
		}*/
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
	for (DistributionPlot* p : m_distributionPlots)
		delete p;
	m_distributionPlots.clear();

}
void Simulation::simulate()
{
	calculatePhysics();
	checkCollisions();
	applyMovements();
}

void Simulation::calculatePhysics()
{
	m_eField->calculatePhysics(m_simulationTimeInterval);
	for (Particle* p : m_particles)
	{
		if (p->isStatic())
			continue;
		p->calculatePhysiscs(m_particles, m_simulationTimeInterval);
	}

		
	for (Shape* s : m_shapes)
		s->calculatePhysics(m_simulationTimeInterval);
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
