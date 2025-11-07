#include "GhostHouseController.h"

void GhostHouseController::resetLevel()
{
	//Reset the release condition
	for (auto releaseCond : ghostReleaseConditions)
		ghostReleaseConditions.at(releaseCond.first).personalDotCounter = 0;



}

void GhostHouseController::onDotEaten()
{
	if (activeGhostTimer == None)
		return;
	else
		ghostReleaseConditions.at(activeGhostTimer).personalDotCounter++;

	updateGhostHouse();
}

void GhostHouseController::fixedUpdate(float dt)
{
}

bool GhostHouseController::canGhostExit(float dt) const
{
    return false;
}

void GhostHouseController::returnGhost(GhostHouseEntityEnum ghost)
{
	//Update ghosts in house
	ghostInHouse.insert(ghost);

	updateGhostHouse();

}

void GhostHouseController::returnGhost(std::initializer_list<GhostHouseEntityEnum> ghosts)
{
	for (auto g : ghosts)
		ghostInHouse.insert(g);
	updateGhostHouse();
	
}

void GhostHouseController::updateGhostHouse()
{
	activeGhostTimer = None;
	for (auto& [ghost,condition] : ghostReleaseConditions)
	{
		//Ghost could be active only if present in ghost house
		if (ghostInHouse.find(ghost) == ghostInHouse.end())
			continue;
		//If dots personal counter exceed its own limite
		if (condition.personalDotCounter >= condition.dotLimit)
			releaseGhost(ghost);
		else
		{
			activeGhostTimer = ghost;
			break;

		}
	}
}

void GhostHouseController::releaseGhost(GhostHouseEntityEnum ghost)
{
	if (onGhostReleased)
		onGhostReleased(ghost);
	ghostInHouse.erase(ghost);
	
}
