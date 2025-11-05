#pragma once
#include "SFML/Graphics.hpp"



class IInput;
class GameLevelGrid;
class GridEntity
{
public:

	void updateInput(const IInput* input);
	void fixedUpdate(float dt);
	void update(float lag);
	void draw(sf::RenderWindow& win);

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
};

