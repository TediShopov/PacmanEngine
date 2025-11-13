#include "Ghost.h"
#include "InkyMovementStrategy.h"
#include "PinkyMovementStrategy.h"
#include "ClydeMovementStrategy.h"

Ghost::Ghost(
	const GameLevelGrid* gameGrid,
	std::shared_ptr<sf::Sprite> sprite,
	const GridEntity* target,
	GhostMovementEnum prefferecChase) :
GridEntity(), target(target), state(Idle), prefferedChaseStrategy(BlinkyMovement),
respawnTile(sf::Vector2i{0,0}), scatterTile(sf::Vector2i{0,0}), ally(nullptr)

	{
	this->levelGrid = gameGrid;
	this->sprite = sprite;
	stateMap.at(Chase).movementStrategy = prefferedChaseStrategy;
	applyState(state);
	}


Ghost::Ghost(const Ghost& other) {
	(*this) = other;
}
Ghost& Ghost::operator= (const Ghost& other)
{
	this->levelGrid = other.levelGrid;
	this->sprite = other.sprite;
	this->gridPosition = other.gridPosition;
	this->worldPos = other.worldPos;
	this->movementSpeed = other.movementSpeed;
	this->ally = other.ally;
	this->prefferedChaseStrategy = other.prefferedChaseStrategy;
	this->state = other.state;
	this->target = other.target;
	this->respawnTile = other.respawnTile;
	this->scatterTile = other.scatterTile;
	this->applyState(state);
	return *this;

}

inline bool Ghost::canTraverse(GameLevelGrid::TileType tile) const
{
	using tileType = GameLevelGrid::TileType;

	if (this->state == Dead || this->state == Spawning)
		return tile != tileType::Wall;
	else
		return tile != tileType::Wall && tile != tileType::Door;


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
	case BlinkyMovement:
		return std::make_unique<ChaseMovementStrategy>(target);
		break;
	case InkyMovement:
		return std::make_unique<InkyMovementStrategy>(target, ally);
		break;
	case PinkyMovement:
		return std::make_unique<PinkyMovementStrategy>(target);
		break;
	case ClydeMovement:
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
	case ExitGhostHouse:
		return std::make_unique<ScatterMovementStrategy>(this->levelGrid->ghostHouseExit);
	case NoMovement:
		return nullptr;
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
