#pragma once
#include "ChaseMovementStrategy.h"
class ScatterMovementStrategy :
    public ChaseMovementStrategy
{
public:
    ScatterMovementStrategy(sf::Vector2i target) : ChaseMovementStrategy(nullptr), target(target) {}

    // Inherited via IMovementStrategy
    sf::Vector2i computeDesiredDirection(const GridEntity& self) const override;

private:
    sf::Vector2i target;
    mutable sf::Vector2i lastComtutedFor;
};

