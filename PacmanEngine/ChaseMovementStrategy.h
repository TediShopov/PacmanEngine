#pragma once
#include "IMovementStrategy.h"
enum DirectionEnum {
    UP,
    LEFT,
    DOWN,
    RIGHT
};

class ChaseMovementStrategy :
    public IMovementStrategy
{

public:
    ChaseMovementStrategy(const GridEntity* target) : target(target){}

    // Inherited via IMovementStrategy
    sf::Vector2i computeDesiredDirection(const GridEntity& self) const override;

private:
    bool canTraverseInDirection(const GridEntity& self, sf::Vector2i nextTile,DirectionEnum dir) const ;
    const GridEntity* target;

};

