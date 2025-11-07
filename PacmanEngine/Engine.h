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

public:
	sf::Clock time;
	std::unique_ptr<IWindow> window;
	std::unique_ptr<IInput> input;

	Engine();

	virtual void init();
	int run();
	virtual void fixedUpdate(float dt);
	virtual void update(float lag);
	virtual void render();

	bool getPaused();
	void setPaused(bool pause);
protected:
	//Resource management by string alias
	std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textureMap;
	std::unordered_map<std::string, std::unique_ptr<sf::Sprite>> spriteMap;
	
	//60hz game loop by default
	const float fixedTimeStep = 0.0166f;
	float fixedDt = fixedTimeStep; //approximately 60hz update loop
	float lag = 0.0f;

private:
	bool isPaused = false;
};

