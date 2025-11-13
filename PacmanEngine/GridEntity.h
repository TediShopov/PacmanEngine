#pragma once
#include "SFML/Graphics.hpp"
#include "IMovementStrategy.h"

class IInput;
class GameLevelGrid;

class GridEntity
{
public:
	GridEntity();

	virtual void fixedUpdate(float dt);
	virtual void update(float lag);
	virtual void draw(sf::RenderWindow& win);

	 bool canChangeDirection(float dt);
	 bool approximatelyNearCenter(float eps);

	 virtual bool canTraverse(GameLevelGrid::TileType type) const;

	float movementSpeed;
	//sf::Sprite* sprite;
	std::shared_ptr<sf::Sprite> sprite;


	sf::Vector2i gridPosition;
	sf::Vector2f worldPos;

	const GameLevelGrid* levelGrid;

	sf::Vector2i getCurrentDirection() const;
	sf::Vector2i getDesiredDirection() const;

protected:
	//sf::Sprite* desiredDirectionSprite;
	//sf::Texture* desiredDirectionTexture;
	std::unique_ptr<sf::Sprite> desiredDirectionSprite;
	std::unique_ptr<sf::Texture> desiredDirectionTexture;

	
	//The directoin that is currently being traveled
	sf::Vector2i currentDirecton;
	//The direction that is changed by player input
	sf::Vector2i desiredDirecton;

	std::unique_ptr<IMovementStrategy> movementStrategy;
};

