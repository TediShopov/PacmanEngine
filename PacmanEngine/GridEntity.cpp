#include "GridEntity.h"
#include "IInput.h"
#include "Grid.h"

void GridEntity::updateInput(const IInput* input)
{
	 if (input->isKeyDown((int)sf::Keyboard::Key::A))
		 this->desiredDirecton = sf::Vector2i{ -1,0 };
	 else if (input->isKeyDown((int)sf::Keyboard::Key::D))
		 this->desiredDirecton = sf::Vector2i{ 1,0};
	 else if (input->isKeyDown((int)sf::Keyboard::Key::W))
		 this->desiredDirecton = sf::Vector2i{ 0,-1 };
	 else if (input->isKeyDown((int)sf::Keyboard::Key::S))
		 this->desiredDirecton = sf::Vector2i{ 0,1 };
}

void GridEntity::fixedUpdate(float dt)
{
	//Update this only if possible
	this->currentDirecton = desiredDirecton;
	gridPosition = this->levelGrid->getCellCoordinates(worldPos); 
	auto currentTile = this->levelGrid->at(gridPosition.x, gridPosition.y);
	auto nextTile = this->levelGrid->at(gridPosition.x + currentDirecton.x, gridPosition.y + currentDirecton.y);
	if (nextTile != GameLevelGrid::TileType::Wall)
	{
		this->worldPos.x += (float)currentDirecton.x * movementSpeed;
		this->worldPos.y += (float)currentDirecton.y * movementSpeed;
		//this->worldPos = this->levelGrid->getPixelCoordinates(gridPosition.x + currentDirecton.x, gridPosition.y + currentDirecton.y);
	}
	


}

void GridEntity::update(float dt)
{
}

 void GridEntity::draw(sf::RenderWindow& win)
{
	 this->sprite->setPosition(worldPos);
	 win.draw(*this->sprite);
}
