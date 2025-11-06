#pragma once
#include "GridEntity.h"
#include "ChaseMovementStrategy.h"
#include "FrightenedMovementStrategy.h"
#include "ScatterMovementStrategy.h"


enum GhostStateEnum {
	Chase, Frightened, Scatter, Dead

 };
enum GhostMovementEnum {
	Blinky, Inky, Pinky, Clyde, Retreat, Flee, Respawn
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
		const GridEntity* ally,
		sf::Vector2i respawn,
		sf::Vector2i scatter
	);

	void changeState(GhostStateEnum newStateEnum);
	std::unique_ptr<IMovementStrategy> createMovementStrategy(GhostMovementEnum e);

	GhostStateEnum getState() const { return state; }
	sf::Vector2i getRespawnTile() const { return this->respawnTile; }

private:
	 std::unordered_map<GhostStateEnum, GhostState> stateMap =
	{
		{GhostStateEnum::Chase,			GhostState { GhostMovementEnum::Blinky,2,sf::Color{255,255,255,255} }},
		{GhostStateEnum::Scatter,		GhostState { GhostMovementEnum::Retreat,2,sf::Color{255,255,255,255} }},
		{GhostStateEnum::Frightened,	GhostState { GhostMovementEnum::Flee,1.5,sf::Color{0,0,1,255} }},
		{GhostStateEnum::Dead,			GhostState { GhostMovementEnum::Respawn,3.5,sf::Color{255,255,255,20} }}
	};
	const GridEntity* target;
	const GridEntity* ally;
	GhostStateEnum state;
	GhostMovementEnum prefferedChaseStrategy;
	sf::Vector2i scatterTile;
	sf::Vector2i respawnTile;
};

