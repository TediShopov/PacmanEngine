#pragma once
#include "IMovementStrategy.h"
#include "Grid.h"

class ChaseMovementStrategy :
    public IMovementStrategy
{

public:

    ChaseMovementStrategy(const GridEntity* target) : target(target){}

    // Inherited via IMovementStrategy
    virtual sf::Vector2i computeDesiredDirection(const GridEntity& self) const override;
protected:
    sf::Vector2i computeDesiredDirectionToReach(const GridEntity& self,sf::Vector2i target) const ;

    const GridEntity* target;
    mutable sf::Vector2i lastComtutedFor;

};

