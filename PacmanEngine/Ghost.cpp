#include "Ghost.h"
#include "InkyMovementStrategy.h"
#include "PinkyMovementStrategy.h"
#include "ClydeMovementStrategy.h"

Ghost::Ghost(
	const GameLevelGrid* gameGrid,
	sf::Sprite* sprite,
	const GridEntity* target,
	const GridEntity* ally,
	sf::Vector2i respawn,
	sf::Vector2i scatter
) :
	GridEntity(), target(target), state(Chase), prefferedChaseStrategy(Blinky),
	respawnTile(respawn), scatterTile(scatter), ally(ally)
	{
		stateMap.at(Chase).movementStrategy = prefferedChaseStrategy;
		this->movementStrategy = std::make_unique<ChaseMovementStrategy>(target);
		this->sprite = sprite;
		this->levelGrid = gameGrid;
	}

void Ghost::changeState(GhostStateEnum newStateEnum) {

		//Can only transition to unique states
		if (newStateEnum == this->state)
			return;

 
		GhostState newState = this->stateMap.at(newStateEnum);
		this->movementSpeed = newState.movementSpeed;
		this->sprite->setColor(newState.newColor);
		this->movementStrategy = createMovementStrategy(newState.movementStrategy);

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
