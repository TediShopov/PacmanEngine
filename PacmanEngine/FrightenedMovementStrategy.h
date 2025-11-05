#pragma once
#include "IMovementStrategy.h"
#include "Grid.h"
class FrightenedMovementStrategy :
    public IMovementStrategy
{
    
public:
    // Inherited via IMovementStrategy
    sf::Vector2i computeDesiredDirection(const GridEntity& self) const final;

private:
    DirectionEnum computeRandomDirection() const; 
    DirectionEnum reachNextValidDirectoinClockwise(const GridEntity& self,DirectionEnum startingDir) const; 
    mutable sf::Vector2i lastComtutedFor;
};

