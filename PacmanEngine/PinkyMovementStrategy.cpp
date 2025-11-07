#include "PinkyMovementStrategy.h"
#include "GridEntity.h"

 PinkyMovementStrategy::PinkyMovementStrategy(const GridEntity* target) : ChaseMovementStrategy(target)
{


}

 sf::Vector2i PinkyMovementStrategy::computeDesiredDirection(const GridEntity& self) const
{
	 sf::Vector2i targetCutOffTile;
	 if (target->getCurrentDirection() != GameLevelGrid::Directions.at(UP)) 
	 {
		 //Offset four tiles in the current diretoin
		 targetCutOffTile = target->gridPosition + (target->getCurrentDirection() * 4);
	 }
	 else 
	 {
		 //Pinky's edge case. Offset Up and Left when target is facing up
		 //Simulates an overflow sideffect to match the original game mechanics
		 targetCutOffTile = target->gridPosition + (GameLevelGrid::Directions.at(UP) * 4) + (GameLevelGrid::Directions.at(LEFT) * 4);
	 }
	 return this->computeDesiredDirectionToReach(self, targetCutOffTile);

}
