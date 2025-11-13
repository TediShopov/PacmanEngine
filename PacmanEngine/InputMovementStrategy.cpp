#include "InputMovementStrategy.h"
#include "GridEntity.h"
#include "Grid.h"

sf::Vector2i InputMovementStrategy::computeDesiredDirection(const GridEntity& self) const
{
	auto grid = self.levelGrid;
	sf::Vector2i desiredDirection = self.getDesiredDirection();
	if (input->isKeyDown((int)sf::Keyboard::Key::A) ||  input->isKeyDown((int)sf::Keyboard::Key::Left))
		desiredDirection = GameLevelGrid::Directions.at(LEFT);
	else if (input->isKeyDown((int)sf::Keyboard::Key::D)||  input->isKeyDown((int)sf::Keyboard::Key::Right))
		desiredDirection = GameLevelGrid::Directions.at(RIGHT);
	else if (input->isKeyDown((int)sf::Keyboard::Key::W)||  input->isKeyDown((int)sf::Keyboard::Key::Up))
		desiredDirection = GameLevelGrid::Directions.at(UP);
	else if (input->isKeyDown((int)sf::Keyboard::Key::S)||  input->isKeyDown((int)sf::Keyboard::Key::Down))
		desiredDirection = GameLevelGrid::Directions.at(DOWN);

	return desiredDirection;
}

