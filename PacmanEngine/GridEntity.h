#pragma once
#include "SFML/Graphics.hpp"
#include "IMovementStrategy.h"

class IInput;
class GameLevelGrid;

class GridEntity
{
public:
	~GridEntity() {};

	//virtual void updateInput(const IInput* input);
	virtual void fixedUpdate(float dt);
	virtual void update(float lag);
	virtual void draw(sf::RenderWindow& win);



//private:
	float movementSpeed;
	sf::Sprite* sprite;
	sf::Vector2i gridPosition;
	sf::Vector2f worldPos;
	//The directoin that is currently being traveled
	sf::Vector2i currentDirecton;
	//The direction that is changed by player input
	sf::Vector2i desiredDirecton;

	const GameLevelGrid* levelGrid;
protected:
	std::unique_ptr<IMovementStrategy> movementStrategy;
};

