#pragma once
#include "SFML/Graphics.hpp"
#include "Grid.h"

class GridEntity;
class IMovementStrategy
{
public:
	virtual ~IMovementStrategy() = default;
	virtual sf::Vector2i computeDesiredDirection(const GridEntity& self) const = 0;
protected:
	bool canTraverseInDirection(const GridEntity& self, sf::Vector2i nextTile, DirectionEnum dir) const;

};

