#pragma once
#include "GridEntity.h"
#include "ChaseMovementStrategy.h"
#include "FrightenedMovementStrategy.h"
enum GhostState {
	Chase, Frightened, Scatter

 };

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

	//Changes the state to frightened. This involved changing the movement strategy, color and speed
	void Frighten()
	{
		if (state == GhostState::Frightened)
			return;
		this->movementStrategy = std::make_unique<FrightenedMovementStrategy>();
		this->sprite->setColor({ 0,255,0,255 });
		this->movementSpeed = 1;
		this->desiredDirecton = -this->desiredDirecton;
	}

	//Changes the state to frightened. This involved changing the movement strategy, color and speed
	void Aggro(const GridEntity* target)
	{
		if (state == GhostState::Chase)
			return;
		this->movementStrategy = std::make_unique<ChaseMovementStrategy>(target);
		this->sprite->setColor({255,255,255,255});
		this->movementSpeed = 2;
		this->desiredDirecton = -this->desiredDirecton;
	}

private:
	GhostState state;
};

