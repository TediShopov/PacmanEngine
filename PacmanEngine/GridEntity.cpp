#include "GridEntity.h"
#include "IInput.h"
#include "Grid.h"

//void GridEntity::updateInput(const IInput* input)
//{
//	if (input->isKeyDown((int)sf::Keyboard::Key::A))
//		this->desiredDirecton = sf::Vector2i{ -1,0 };
//	else if (input->isKeyDown((int)sf::Keyboard::Key::D))
//		this->desiredDirecton = sf::Vector2i{ 1,0 };
//	else if (input->isKeyDown((int)sf::Keyboard::Key::W))
//		this->desiredDirecton = sf::Vector2i{ 0,-1 };
//	else if (input->isKeyDown((int)sf::Keyboard::Key::S))
//		this->desiredDirecton = sf::Vector2i{ 0,1 };
//}

void GridEntity::fixedUpdate(float dt)
{
	this->desiredDirecton = movementStrategy->computeDesiredDirection(*this);
	//Update this only if possible
	this->currentDirecton = desiredDirecton;
	gridPosition = this->levelGrid->getCellCoordinates(worldPos);

	// Get current and next tile types
	auto currentTile		= this->levelGrid->at(gridPosition.x, gridPosition.y);
	auto nextTile			= this->levelGrid->at(gridPosition.x + currentDirecton.x, gridPosition.y + currentDirecton.y);

	// Compute tile centers
	auto currentTileCenter	= this->levelGrid->getPixelCoordinates(gridPosition.x, gridPosition.y);
	auto nextTileCenter		= this->levelGrid->getPixelCoordinates(gridPosition.x + currentDirecton.x, gridPosition.y + currentDirecton.y);

	const sf::Vector2f step =static_cast<sf::Vector2f>(currentDirecton) * movementSpeed;
	const sf::Vector2f nextWolrdPos = worldPos + step;

	// If tile is walkable -> walk freely
	if (nextTile != GameLevelGrid::TileType::Wall)
	{
		worldPos = nextWolrdPos;
		return;
	}

	// X-axis clamp

	//Moving Right
	if (currentDirecton.x > 0)
		worldPos.x = std::min(nextWolrdPos.x, currentTileCenter.x);
	//Moving Left
	else
		worldPos.x = std::max(nextWolrdPos.x, currentTileCenter.x);

	// Y-axis clamp

	//Moving Up
	if (currentDirecton.y > 0)
		worldPos.y = std::min(nextWolrdPos.y, currentTileCenter.y);
	//Moving Down
	else
		worldPos.y = std::max(nextWolrdPos.y, currentTileCenter.y);
}

void GridEntity::update(float dt)
{
}

void GridEntity::draw(sf::RenderWindow& win)
{
	this->sprite->setPosition(worldPos);
	win.draw(*this->sprite);
}


