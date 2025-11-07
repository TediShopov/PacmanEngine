#pragma once
#include "map"
#include "unordered_set"
#include <functional>
//A class responsible for sequencing the Ghosts spawns in a level
enum GhostHouseEntityEnum
{
	Blinky,Pinky,Inky,Clyde,None
};

struct GhostHouseReleaseCondition
{
	int personalDotCounter;
	int dotLimit;
};


class GhostHouseController
{
public :
	void resetLevel();
	void onDotEaten();
	void fixedUpdate(float dt);
	bool canGhostExit(float dt) const;
	void returnGhost(GhostHouseEntityEnum ghost);
	void returnGhost(std::initializer_list<GhostHouseEntityEnum> ghosts);
	std::function<void(GhostHouseEntityEnum)> onGhostReleased;
private:
	void updateGhostHouse();
	void releaseGhost(GhostHouseEntityEnum ghost);
	GhostHouseEntityEnum activeGhostTimer ;
	std::unordered_set<GhostHouseEntityEnum> ghostInHouse;

	std::map<GhostHouseEntityEnum, GhostHouseReleaseCondition> ghostReleaseConditions
	{
		{Blinky,		{0,0}},
		{Pinky,		{0,0}},
		{Inky,		{0,30}},
		{Clyde,		{0,60}}
	};
};

