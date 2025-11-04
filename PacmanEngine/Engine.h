#pragma once
#include <SFML/Graphics.hpp>
#include "WindowSFML.h"
#include "Input.h"
#include "Grid.h"

class Engine
{
	const std::string DefaultEngineName = "PacmanEngine";
	const sf::Vector2u DefaultEngineWindowSize = { 448,576 };

public:

	sf::Clock time;
	std::unique_ptr<IWindow> window;
	std::unique_ptr<IInput> input;

	Engine();

	void initGameLevelGrid();

	void initSprites();

	void initTextures();

	int run();
	void update();


	//std::unique_ptr<sf::Texture> defTexture;
	std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textureMap;
	std::unordered_map<std::string, std::unique_ptr<sf::Sprite>> spriteMap;
	std::unique_ptr<GameLevelGrid> gameGrid;




	
};

