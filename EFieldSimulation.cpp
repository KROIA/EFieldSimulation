#include <iostream>

#include "display.h"
#include "particle.h"
#include "eField.h"


int main()
{
    sf::Vector2u screenSize(1000, 800);

    sf::Vector2f worldSize(screenSize);
    sf::Vector2u gridSize(50, 50);
    Display display(screenSize, "E-Feld Simulation");

    EField eField(worldSize, gridSize);

    float charge = 5;
    Particle p1;
    p1.setCharge(charge);
    p1.setPos(sf::Vector2f(screenSize / 2u));

    Particle p2;
    p2.setCharge(charge);
    p2.setPos(sf::Vector2f(screenSize / 4u));
    p2.setStatic(true);

    Particle mouseParticle;
    mouseParticle.setCharge(100);
    mouseParticle.setStatic(true);
    //p2.setPos(sf::Vector2f(screenSize / 4u));

    size_t maxX = 10;
    size_t maxY = 10;
    float boarder = 20;

    size_t counter = 0;
    for (size_t x = 0; x < maxX; ++x)
    {
        for (size_t y = 0; y < maxY; ++y)
        {
            Particle* p = new Particle();
            p->setCharge(charge);

            //if(counter++ % 2 == 0)
            //    p->setCharge(-charge);
            
           // p->setPos(sf::Vector2f((boarder + rand()) % (screenSize.x - boarder),
           //                        (boarder + rand()) % (screenSize.y - boarder)));

            p->setPos(sf::Vector2f(boarder + (float)x * (screenSize.x - boarder) / (float)maxX,
                                   boarder + (float)y * (screenSize.y - boarder) / (float)maxY));
            eField.addParticle(p);
        }
    }

    //eField.addParticle(&p1);
   // eField.addParticle(&p2);
    eField.addParticle(&mouseParticle);


    display.addDrawable(&eField);

    float angle = 0;

    while (display.isOpen())
    {
        display.processEvents();
        if (display.needsFrameUpdate())
        {
          //  sf::Vector2f deltaPos = VectorMath::getRotated(VectorMath::getUnitVector(), angle)*1.f;
           // angle += 0.01;
          //  p2.move(deltaPos);

            mouseParticle.setPos(sf::Vector2f(display.getRelaticeMousePos()));

            eField.calculatePhysics(10);
            display.draw();
        }
    }
}

