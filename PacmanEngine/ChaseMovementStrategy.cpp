#include "ChaseMovementStrategy.h"
#include "GridEntity.h"
//#include "Grid.h"



sf::Vector2i ChaseMovementStrategy::computeDesiredDirection(const GridEntity& self) const
{
    auto grid = self.levelGrid;
    auto nextTile = self.gridPosition + self.currentDirecton;

    std::vector<DirectionEnum> walkableDirections;

    // Filter tiles that are walkable = not a wall
    for (auto& cardinalDirection : GameLevelGrid::Directions)
    {
        if(canTraverseInDirection(self,nextTile,cardinalDirection.first))
			walkableDirections.push_back(cardinalDirection.first);

    }

    // Sort the walkable tiles based on vector distance
    std::sort(walkableDirections.begin(), walkableDirections.end(), 
         [&](const DirectionEnum a, const DirectionEnum b)
        {
            sf::Vector2i aTile = self.gridPosition + GameLevelGrid::Directions.at(a);
            sf::Vector2i bTile = self.gridPosition + GameLevelGrid::Directions.at(b);

            int distanceToA = (aTile - target->gridPosition).lengthSquared();
            int distanceToB = (bTile - target->gridPosition).lengthSquared();

            if (distanceToA != distanceToB)
                return distanceToA < distanceToB;
            else 
                return a < b; // Converts enum to int and compares; The enum itself encodes the priority for the tiebrake

        });

    if (walkableDirections.size() <= 0)
        return GameLevelGrid::Directions.at(UP);

    return GameLevelGrid::Directions.at(walkableDirections.front());
}

bool ChaseMovementStrategy::canTraverseInDirection(const GridEntity& self, sf::Vector2i nextTile,DirectionEnum dir) const
{
    auto tileInDir = nextTile + GameLevelGrid::Directions.at(dir);

    auto grid = self.levelGrid;
    using GT = GameLevelGrid::TileType;

    bool isNotWall = grid->at(tileInDir) != GT::Wall;
    return isNotWall;
	//bool isNotReverseDirection = (Directions.at(dir) + self.currentDirecton).lengthSquared() != 0;
    //return isNotWall && isNotReverseDirection;
}
