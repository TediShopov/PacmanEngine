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

 GridEntity::GridEntity()
{
	//_debugDesiredDirectionTexture = new sf::Texture(sf::Vector2u{ 8, 8 },false);
	_debugDesiredDirectionTexture = new sf::Texture("../assets/Wall.png");
	_debugVisualizeDesiredDirection = new sf::Sprite(*_debugDesiredDirectionTexture, sf::IntRect( { 0,0 },{16,16} ));
	_debugVisualizeDesiredDirection->setOrigin({ 8,8 });
	_debugVisualizeDesiredDirection->setColor(sf::Color{0,255,0,255});
	_debugVisualizeDesiredDirection->scale({0.5,0.5});

}

 GridEntity::~GridEntity() 
 {
	 delete _debugDesiredDirectionTexture;
	 delete _debugVisualizeDesiredDirection;

}

void GridEntity::fixedUpdate(float dt)
{
	this->desiredDirecton = movementStrategy->computeDesiredDirection(*this);

	//Update this only if possible
	if (canChangeDirection(dt))
		this->currentDirecton = desiredDirecton;

	

	gridPosition = this->levelGrid->getCellCoordinates(worldPos);
	if (levelGrid->isInRange(gridPosition.y, gridPosition.x) == false)
	{
		//Teleport world postion and recalculate grid position
		worldPos = levelGrid->computeTeleportedPixelPosition(worldPos);
		gridPosition = this->levelGrid->getCellCoordinates(worldPos);
	}


	// Get current and next tile types
	auto currentTile		= this->levelGrid->at(gridPosition);
	auto nextTile			= this->levelGrid->at(gridPosition + currentDirecton);

	// Compute tile centers
	auto currentTileCenter	= this->levelGrid->getPixelCoordinates(gridPosition);
	auto nextTileCenter		= this->levelGrid->getPixelCoordinates(gridPosition + currentDirecton);

	const sf::Vector2f step =static_cast<sf::Vector2f>(currentDirecton) * movementSpeed;
	const sf::Vector2f nextWolrdPos = worldPos + step;

	// If tile is walkable -> walk freely
	if (this->canTraverse(nextTile))
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


	//Update debug position
	auto finalPos = (this->worldPos + (sf::Vector2f(this->desiredDirecton).componentWiseMul({15,15})));
	_debugVisualizeDesiredDirection->setPosition(finalPos);
	win.draw(*this->_debugVisualizeDesiredDirection);

}

 bool GridEntity::canChangeDirection(float dt) 
 {

	 //Direction are completely oppositde
	 if ((desiredDirecton + currentDirecton).lengthSquared() == 0)
	 {
		 return true;
	 }
	 else 
	 {
		 auto nextDesired = gridPosition + desiredDirecton;

		 bool nextCanBeTraversed = this->canTraverse(levelGrid->at(nextDesired));
		 bool approximatelyCloseToCenter = approximatelyNearCenter(2.0f);
		 //bool approximatelyCloseToCenter =true;
		 return nextCanBeTraversed && approximatelyCloseToCenter;

	 }


 }

 bool GridEntity::approximatelyNearCenter(float eps)
 {
	 sf::Vector2f tileCenter = levelGrid->getPixelCoordinates(this->gridPosition);
	 sf::Vector2f delta = tileCenter - worldPos;
	 return std::abs(delta.x) < eps && std::abs(delta.y) < eps;

 }



