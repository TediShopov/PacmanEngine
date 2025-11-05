#pragma once
#include "IMovementStrategy.h"
#include "Grid.h"
class FrightenedMovementStrategy :
    public IMovementStrategy
{
    
public:
    // Inherited via IMovementStrategy
    sf::Vector2i computeDesiredDirection(const GridEntity& self) const override;

private:
    DirectionEnum computeRandomDirection() const; 
    DirectionEnum reachNextValidDirectoinClockwise(const GridEntity& self,DirectionEnum startingDir) const; 
    bool canTraverseInDirection(const GridEntity& self, sf::Vector2i nextTile,DirectionEnum dir) const ;
    mutable sf::Vector2i lastComtutedFor;
};

