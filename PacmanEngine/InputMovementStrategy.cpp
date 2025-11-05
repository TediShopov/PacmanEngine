#include "InputMovementStrategy.h"
#include "GridEntity.h"

sf::Vector2i InputMovementStrategy::computeDesiredDirection(const GridEntity& self) const
{
	auto grid = self.levelGrid;
	sf::Vector2i desiredDirection;
	if (input->isKeyDown((int)sf::Keyboard::Key::A))
		desiredDirection = sf::Vector2i{ -1,0 };
	else if (input->isKeyDown((int)sf::Keyboard::Key::D))
		desiredDirection = sf::Vector2i{ 1,0 };
	else if (input->isKeyDown((int)sf::Keyboard::Key::W))
		desiredDirection = sf::Vector2i{ 0,-1 };
	else if (input->isKeyDown((int)sf::Keyboard::Key::S))
		desiredDirection = sf::Vector2i{ 0,1 };
	return desiredDirection;
}

