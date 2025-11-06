#include "InkyMovementStrategy.h"
#include "GridEntity.h"

InkyMovementStrategy::InkyMovementStrategy(const GridEntity* target, const GridEntity* ally) :
	ChaseMovementStrategy(target),
	ally(ally)
{


}

sf::Vector2i InkyMovementStrategy::computeDesiredDirection(const GridEntity& self) const 
{
	const int targetTileOffset = 2;
	const int blinkyDistanceMultiplier = 2;
	sf::Vector2i offsetTargetCoordinate;

	if (target->currentDirecton == GameLevelGrid::Directions.at(UP))
	{
		offsetTargetCoordinate =
			target->gridPosition 
			+ (targetTileOffset * GameLevelGrid::Directions.at(UP)) 
			+ (targetTileOffset * GameLevelGrid::Directions.at(LEFT));
	}
	else {
		offsetTargetCoordinate 
			= target->gridPosition + (targetTileOffset * target->currentDirecton);
	}

	sf::Vector2i targetCoordinate = ally->gridPosition + (offsetTargetCoordinate - ally->gridPosition) * 2;
	return computeDesiredDirectionToReach(self,targetCoordinate);
}
