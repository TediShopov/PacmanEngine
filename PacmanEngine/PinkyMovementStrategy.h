#pragma once
#include "ChaseMovementStrategy.h"
class PinkyMovementStrategy :
    public ChaseMovementStrategy
{
public:
	PinkyMovementStrategy(const GridEntity* target);
	sf::Vector2i computeDesiredDirection(const GridEntity& self) const final;
};

