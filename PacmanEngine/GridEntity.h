#pragma once
#include "SFML/Graphics.hpp"



class IInput;
class GameLevelGrid;
class GridEntity
{
public:

	void updateInput(const IInput* input);
	void update(float dt);
	void draw(sf::RenderWindow& win);

//private:
	sf::Sprite* sprite;
	sf::Vector2i gridPosition;
	sf::Vector2f worldPos;

	//The directoin that is currently being traveled
	sf::Vector2f currentDirecton;
	//The direction that is changed by player input
	sf::Vector2f desiredDirecton;

	const GameLevelGrid* levelGrid;
	

};

