#pragma once
#include "GridEntity.h"
#include "ChaseMovementStrategy.h"
class Ghost :
    public GridEntity
{
public:
	Ghost(const GameLevelGrid* gameGrid,const GridEntity* target, sf::Sprite* sprite) : GridEntity()
	{
		this->movementStrategy = std::make_unique<ChaseMovementStrategy>(target);
		this->sprite = sprite;
		this->levelGrid = gameGrid;
	}

};

