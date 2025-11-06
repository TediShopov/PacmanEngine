#include "ChaseMovementStrategy.h"
#include "GridEntity.h"
//#include "Grid.h"



sf::Vector2i ChaseMovementStrategy::computeDesiredDirection(const GridEntity& self) const
{
    return computeDesiredDirectionToReach(self, this->target->gridPosition);
}

sf::Vector2i ChaseMovementStrategy::computeDesiredDirectionToReach(const GridEntity& self, sf::Vector2i target) const {
    auto grid = self.levelGrid;
    auto currentTile = self.gridPosition ;

    if(lastComtutedFor == currentTile)
        return self.desiredDirecton;

    std::vector<DirectionEnum> walkableDirections;

    // Filter tiles that are walkable = not a wall
    for (auto& cardinalDirection : GameLevelGrid::Directions)
    {
        if(canTraverseInDirection(self,currentTile,cardinalDirection.first))
			walkableDirections.push_back(cardinalDirection.first);

    }

    // Sort the walkable tiles based on vector distance
    std::sort(walkableDirections.begin(), walkableDirections.end(), 
         [&](const DirectionEnum a, const DirectionEnum b)
        {
            sf::Vector2i aTile = self.gridPosition + GameLevelGrid::Directions.at(a);
            sf::Vector2i bTile = self.gridPosition + GameLevelGrid::Directions.at(b);

            int distanceToA = (aTile - target).lengthSquared();
            int distanceToB = (bTile - target).lengthSquared();

            if (distanceToA != distanceToB)
                return distanceToA < distanceToB;
            else 
                return a < b; // Converts enum to int and compares; The enum itself encodes the priority for the tiebrake

        });


    if (walkableDirections.size() <= 0)
    {
        lastComtutedFor = currentTile;
        return self.desiredDirecton;

    }
    else 
    {
        lastComtutedFor = currentTile;
		return GameLevelGrid::Directions.at(walkableDirections.front());
    }


}


