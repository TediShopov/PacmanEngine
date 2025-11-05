#pragma once
#include "GridEntity.h"
#include "InputMovementStrategy.h"
class Pacman :
    public GridEntity
{
public:
	Pacman(const GameLevelGrid* gameGrid,const IInput* input, sf::Sprite* sprite) : GridEntity()
	{
		this->movementStrategy = std::make_unique<InputMovementStrategy>(input);
		this->sprite = sprite;
		this->levelGrid = gameGrid;
	}



};

