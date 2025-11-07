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

	bool canTraverse(GameLevelGrid::TileType type) const override
	{
		return type != GameLevelGrid::TileType::Door && type != GameLevelGrid::TileType::Wall;
	}

	void fixedUpdate(float dt) override
	{
		//Call based classes fixed update
		GridEntity::fixedUpdate(dt);
		//Additionally, rotate sprite to the direction of movement
		rotateToDirection(sf::Vector2f(currentDirecton));
	}

	void rotateToDirection(sf::Vector2f dir) 
	{
		sf::Angle angle = sf::radians(std::atan2(dir.y,dir.x));
		this->sprite->setRotation(angle + sf::degrees(180));
	}



};

