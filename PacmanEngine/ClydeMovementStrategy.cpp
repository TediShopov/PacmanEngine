#include "ClydeMovementStrategy.h"
#include "GridEntity.h"

sf::Vector2i ClydeMovementStrategy::computeDesiredDirection(const GridEntity& self) const
{
    if (shouldChaseTarget(self))
        return this->computeDesiredDirectionToReach(self,target->gridPosition);
    else
        return this->computeDesiredDirectionToReach(self,scatterTarget);
}

bool ClydeMovementStrategy::shouldChaseTarget(const GridEntity& self) const
{
    auto delta = self.gridPosition - target->gridPosition;
    return delta.length() < aggroProximity;
}
