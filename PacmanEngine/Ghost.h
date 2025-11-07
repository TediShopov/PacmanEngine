#pragma once
#include "GridEntity.h"
#include "ChaseMovementStrategy.h"
#include "FrightenedMovementStrategy.h"
#include "ScatterMovementStrategy.h"


enum GhostStateEnum {
	Chase, Frightened, Scatter, Dead, Spawning, Idle

 };
enum GhostMovementEnum {
	BlinkyMovement, InkyMovement, PinkyMovement, ClydeMovement, Retreat, Flee, Respawn, ExitGhostHouse, NoMovement
 };

struct GhostState 
{
public:
	GhostMovementEnum movementStrategy;
	float movementSpeed;
	sf::Color newColor;
};

class Ghost :
	public GridEntity
{
public:

	Ghost(
		const GameLevelGrid* gameGrid,
		sf::Sprite* sprite,
		const GridEntity* target,
		GhostMovementEnum prefferedChase
	);

	Ghost(const Ghost& other) {
		(*this) = other;
	}
	Ghost& operator= (const Ghost& other)
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

	~Ghost() = default;

	bool canTraverse(GameLevelGrid::TileType tile) const override;
	static bool isStateActive(GhostStateEnum e)
	{
		return e == GhostStateEnum::Scatter
			|| e == GhostStateEnum::Chase
			|| e == GhostStateEnum::Frightened;

	}




	void applyState(GhostStateEnum newStateEnum);
	void changeState(GhostStateEnum newStateEnum);
	std::unique_ptr<IMovementStrategy> createMovementStrategy(GhostMovementEnum e);

	GhostStateEnum getState() const { return state; }
	sf::Vector2i getRespawnTile() const { return this->respawnTile; }

	const GridEntity* ally;
	sf::Vector2i scatterTile;
	sf::Vector2i respawnTile;

	GhostMovementEnum getChaseStrategy() const { return prefferedChaseStrategy; }
	void setChaseStrategy(GhostMovementEnum e);


private:
	GhostMovementEnum prefferedChaseStrategy;
	GhostStateEnum state;
	const GridEntity* target;

	 std::unordered_map<GhostStateEnum, GhostState> stateMap =
	{
		{GhostStateEnum::Chase,			GhostState { GhostMovementEnum::BlinkyMovement,2,sf::Color{255,255,255,255} }},
		{GhostStateEnum::Spawning,			GhostState { GhostMovementEnum::ExitGhostHouse,0.5,sf::Color{255,0,0,255} }},
		{GhostStateEnum::Scatter,		GhostState { GhostMovementEnum::Retreat,2,sf::Color{255,255,255,255} }},
		//{GhostStateEnum::Frightened,	GhostState { GhostMovementEnum::Flee,1.5,sf::Color{0,0,1,255} }},
		{GhostStateEnum::Frightened,	GhostState { GhostMovementEnum::Flee,0.1f,sf::Color{0,0,255,255} }},
		{GhostStateEnum::Dead,			GhostState { GhostMovementEnum::Respawn,3.5,sf::Color{255,255,255,20} }},
		{GhostStateEnum::Idle,			GhostState { GhostMovementEnum::NoMovement,0,sf::Color{255,0,255,255} }}
	};
};

