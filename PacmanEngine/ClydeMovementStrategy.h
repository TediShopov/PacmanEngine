#pragma once
#include "ChaseMovementStrategy.h"
class ClydeMovementStrategy :
    public ChaseMovementStrategy
{
public:

    //The Euclidean distance from target below which Clyde should chase the target
    const int aggroProximity;

    ClydeMovementStrategy(const GridEntity* target, sf::Vector2i scatterTarget, int aggroProx = 8) :
        ChaseMovementStrategy(target),scatterTarget(scatterTarget), aggroProximity(aggroProx)
    {
            
    }
    // Inherited via IMovementStrategy
    sf::Vector2i computeDesiredDirection(const GridEntity& self) const final;
private:

    sf::Vector2i scatterTarget;


    //Determines if Clyde should be aggresive and chase the target or 
    //head towards his scatter target
    bool shouldChaseTarget(const GridEntity& self) const;

};


