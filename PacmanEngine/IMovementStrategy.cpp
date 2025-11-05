#include "IMovementStrategy.h"
#include "GridEntity.h"

 bool IMovementStrategy::canTraverseInDirection(const GridEntity& self, sf::Vector2i nextTile, DirectionEnum dir) const {

    sf::Vector2i dirVecor = GameLevelGrid::Directions.at(dir);
    auto tileInDir = nextTile + dirVecor;

    auto grid = self.levelGrid;
    using GT = GameLevelGrid::TileType;

    bool isNotWall = grid->at(tileInDir) != GT::Wall;
    bool isNotReverseDirection = !GameLevelGrid::areDirectionRevererse(dirVecor, self.currentDirecton);

    return isNotWall && isNotReverseDirection;
}
