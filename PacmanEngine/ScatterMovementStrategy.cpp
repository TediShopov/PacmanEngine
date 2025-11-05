#include "ScatterMovementStrategy.h"

sf::Vector2i ScatterMovementStrategy::computeDesiredDirection(const GridEntity& self) const
{
    return this->computeDesiredDirectionToReach(self, target);
}
