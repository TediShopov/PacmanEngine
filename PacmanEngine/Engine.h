#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>
#include "WindowSFML.h"
#include "Input.h"
#include "Grid.h"
#include "GridEntity.h"
#include "Timer.h"

class Ghost;

class Engine
{
	const std::string DefaultEngineName = "PacmanEngine";
	const sf::Vector2u DefaultEngineWindowSize = { 448,576 };
	const uint16_t ScorePerPellet = 10;
	const uint16_t ScorePerGhost = 10;

public:

	sf::Clock time;
	std::unique_ptr<IWindow> window;
	std::unique_ptr<IInput> input;

	Engine();

	void initGhosts(const sf::Vector2i& defaultGhostSpawnPoint);

	//sf::Font font("../assets/arial.ttf");


	void initGameLevelGrid();

	void initSprites();

	void initTextures();


	int run();
	void fixedUpdate(float dt);
	void update(float lag);
	void render();

	void changeAllGhostsState(int state);

	void eatPill();
	//Reverts specific ghosts to chase/scatter mode after frigtened state
	//duration has been exhausted
	void revertToChaseAfterFrightened(Timer* t);

private:
	std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textureMap;
	std::unordered_map<std::string, std::unique_ptr<sf::Sprite>> spriteMap;
	std::unique_ptr<GameLevelGrid> gameGrid;
	std::unique_ptr<GridEntity> pacman;
	//std::unique_ptr<GridEntity> blinky;
	std::vector<std::unique_ptr<GridEntity>> ghosts;
	//std::unique_ptr<Ghost> pinky;
	//std::vector<std::unique_ptr<GridEntity>> ghosts;
	sf::Font debugFont;
	sf::Text* debugText;


	Timer frightenedTimer;

	uint32_t score;


};

