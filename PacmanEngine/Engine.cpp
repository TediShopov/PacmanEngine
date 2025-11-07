#include "Engine.h"
#include "chrono"
#include "Pacman.h"
#include "Ghost.h"

//A quick and dirty way to reference textures and sprites
const std::string BigCoin = "BigCoin";
const std::string Wall = "Wall";
const std::string PacmanString = "Pacman";
const std::string BlinkyString = "Blinky";
const std::string InkyString= "Inky";
const std::string ClydeString = "Clyde";
const std::string PinkyString = "Pinky";
const std::string SpritesheetString = "SpriteSheet";

//Game configurable properties
const sf::Vector2i PacTileSetSpriteDimensions{ 16,16 };
const sf::Vector2f PacTileSetSpriteOrigin{ 8,8 };
const sf::Vector2i InitialGridDimensions{ 10,10 };
const sf::Color DebugWallColor{ 5,10,120,255 };

Engine::Engine() :
	window(std::make_unique<WindowSFML>()),
	input(std::make_unique<Input>())
{
	this->debugFont.openFromFile("../assets/arial.ttf");

	init();
}

void Engine::init() {}

int Engine::run()
{
	using time = std::chrono::steady_clock;
	using secondsf = std::chrono::duration<float>;
	auto previous = time::now();

	float fixedDt = 0.0166f; //approximately 60hz update loop
	float lag = 0.0f;

	while (window->isOpen())
	{
		auto current = time::now();
		secondsf dt = current - previous;
		previous = current;
		lag += dt.count();

		input->pumpEvents(*window);
		if (input->isQuitRequested())
			break;

		while (lag >= fixedDt)
		{
			fixedUpdate(fixedDt);
			lag -= fixedDt;
		}

		update(lag);
		render();
	}
	return 0;
}

void Engine::fixedUpdate(float dt)
{
}

void Engine::render()
{
}

void Engine::update(float lag)
{
}