#pragma once
#include "SFML/Graphics.hpp"

class GridEntity;
class IMovementStrategy
{
public:
	virtual ~IMovementStrategy() = default;
	virtual sf::Vector2i computeDesiredDirection(const GridEntity& self) const = 0;

};

