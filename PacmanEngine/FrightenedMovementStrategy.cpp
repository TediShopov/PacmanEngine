#include "FrightenedMovementStrategy.h"
#include "GridEntity.h"

sf::Vector2i FrightenedMovementStrategy::computeDesiredDirection(const GridEntity& self) const
{
    auto currentTile = self.gridPosition ;

    if(lastComtutedFor == currentTile)
        return self.desiredDirecton;

    DirectionEnum randomDirection = computeRandomDirection();

    DirectionEnum validDirection = reachNextValidDirectoinClockwise(self, randomDirection);

    lastComtutedFor = currentTile;
    return GameLevelGrid::Directions.at(validDirection);

}

DirectionEnum FrightenedMovementStrategy::computeRandomDirection() const
{
    //Utilized rand which as it is deterministic and ghost functionality MUST be 
    //deterministic for the arcade sytle gameplay of Pacman
    int randomInt = rand() % 4;
    return (DirectionEnum)(randomInt);

}

DirectionEnum FrightenedMovementStrategy::reachNextValidDirectoinClockwise(const GridEntity& self,DirectionEnum startingDir) const
{
    //Exhaust all 4 possible directions
    auto nextTile = self.gridPosition;

    //WARNING: this code only work if the enum itself declared direction clockwise
    for (size_t i = 0; i < 4; i++)
    {
        int directionNum = (int)startingDir + i;
        directionNum = directionNum % 4;
        DirectionEnum directionClockwise = (DirectionEnum)(directionNum);
		if (canTraverseInDirection(self, nextTile, directionClockwise))
			return directionClockwise;
    }
}

