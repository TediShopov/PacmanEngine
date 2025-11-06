#include "Ghost.h"
#include "InkyMovementStrategy.h"
#include "PinkyMovementStrategy.h"
#include "ClydeMovementStrategy.h"

Ghost::Ghost(
	const GameLevelGrid* gameGrid,
	sf::Sprite* sprite,
	const GridEntity* target,
	GhostMovementEnum prefferecChase) :
GridEntity(), target(target), state(Chase), prefferedChaseStrategy(Blinky),
respawnTile(sf::Vector2i{0,0}), scatterTile(sf::Vector2i{0,0}), ally(nullptr)

	{
	this->levelGrid = gameGrid;
	this->sprite = sprite;
	stateMap.at(Chase).movementStrategy = prefferedChaseStrategy;
	applyState(state);
	}

void Ghost::applyState(GhostStateEnum newStateEnum)
{
	GhostState newState = this->stateMap.at(newStateEnum);
	this->movementSpeed = newState.movementSpeed;
	this->sprite->setColor(newState.newColor);
	this->movementStrategy = createMovementStrategy(newState.movementStrategy);
}

void Ghost::changeState(GhostStateEnum newStateEnum) {

		//Can only transition to unique states
		if (newStateEnum == this->state)
			return;

		this->applyState(newStateEnum);

		//If previous state was aggro or new state is aggro
		if (this->state == GhostStateEnum::Chase || newStateEnum == GhostStateEnum::Chase)
			this->desiredDirecton = -this->desiredDirecton;

		this->state = newStateEnum;
}


std::unique_ptr<IMovementStrategy> Ghost::createMovementStrategy(GhostMovementEnum e) {

	switch (e)
	{
	case Blinky:
		return std::make_unique<ChaseMovementStrategy>(target);
		break;
	case Inky:
		return std::make_unique<InkyMovementStrategy>(target, ally);
		break;
	case Pinky:
		return std::make_unique<PinkyMovementStrategy>(target);
		break;
	case Clyde:
		return std::make_unique<ClydeMovementStrategy>(target,this->scatterTile);
		break;
	case Retreat:
		return std::make_unique<ScatterMovementStrategy>(this->scatterTile);
		break;
	case Flee:
		return std::make_unique<FrightenedMovementStrategy>();
		break;
	case Respawn:
		return std::make_unique<ScatterMovementStrategy>(this->respawnTile);
		break;
	default:
		break;
	}

	}

void Ghost::setChaseStrategy(GhostMovementEnum newState)
{
	this->prefferedChaseStrategy = newState;
	stateMap.at(Chase).movementStrategy = newState;
	if (state == Chase)
		applyState(Chase);

		
}
