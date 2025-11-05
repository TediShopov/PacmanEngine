#include "InputMovementStrategy.h"
#include "GridEntity.h"
#include "Grid.h"

sf::Vector2i InputMovementStrategy::computeDesiredDirection(const GridEntity& self) const
{
	auto grid = self.levelGrid;
	sf::Vector2i desiredDirection = self.desiredDirecton;
	if (input->isKeyDown((int)sf::Keyboard::Key::A))
		desiredDirection = GameLevelGrid::Directions.at(LEFT);
	else if (input->isKeyDown((int)sf::Keyboard::Key::D))
		desiredDirection = GameLevelGrid::Directions.at(RIGHT);
	else if (input->isKeyDown((int)sf::Keyboard::Key::W))
		desiredDirection = GameLevelGrid::Directions.at(UP);
	else if (input->isKeyDown((int)sf::Keyboard::Key::S))
		desiredDirection = GameLevelGrid::Directions.at(DOWN);
	return desiredDirection;
}

