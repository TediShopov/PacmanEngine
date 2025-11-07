#pragma once
#include "Engine.h"
#include "GhostHouseController.h"
#include "ScatterChaseGlobalCycle.h"
#include "PacManEntities.h"

enum class GameState {
	Lost, Won, Running, Paused
};


class PacmanGame :
    public Engine
{
	//A quick and dirty way to reference textures and sprites
	const std::string Dot = "Dot";
	const std::string PowerPill = "BigCoin";
	const std::string Wall = "Wall";
	const std::string PacmanString = "Pacman";
	const std::string BlinkyString = "Blinky";
	const std::string InkyString = "Inky";
	const std::string ClydeString = "Clyde";
	const std::string PinkyString = "Pinky";
	const std::string SpritesheetString = "SpriteSheet";

	const uint16_t ScorePerDot = 10;
	const uint16_t ScorePerGhost = 10;

	const std::unordered_map<PacManEntityEnum, std::string> entityIdToSprite{
		{PacManEntityEnum::Pacman,PacmanString},
		{PacManEntityEnum::Blinky,BlinkyString},
		{PacManEntityEnum::Pinky,PinkyString},
		{PacManEntityEnum::Inky,InkyString},
		{PacManEntityEnum::Clyde,ClydeString},
	};

public:
	PacmanGame();
	void reset();

	//Engine methods
	virtual void init() final;
	virtual void fixedUpdate(float dt) final;
	virtual void update(float lag) final;
	virtual void render() final;

protected:

	//Initializatoin Methods
	void initPacman();
	void initGhosts();
	void initGameLevelGrid();
	void initSprites();
	void initTextures();
	void initGlobalStateCycles();
	void setWinCondition();
	void initLabels();


	//Game Logic Events

	void eatPowerPill();

	void releaseGhost(GhostHouseEntityEnum g);

	//Helpers

	//An active ghost is a ghost that is in either Chase, Scatter, or Frighten
	//States such as Idle, Dead, Spawning are not considered active
	void changeAllActiveGhostStates(int state);

	//A centralized way of acquiring all resrouce to a specific ghost type
	std::unique_ptr<Ghost> createGhostEntity(PacManEntityEnum e);

	//A shorter way to offset sprites int rect
	sf::IntRect offsetSpriteTextureRect(const sf::Sprite& sprite, sf::Vector2i offset);

	//Reverts specific ghosts to chase/scatter mode after frigtened state
	//duration has been exhausted
	void revertToChaseAfterFrightened(Timer* t);

protected:
	GameState gameState;
	GhostHouseController ghostHouse;
	ScatterChaseGlobalCycle globalStateCycle;

	std::unique_ptr<GameLevelGrid> gameGrid;
	std::unique_ptr<GridEntity> pacman;
	std::unordered_map<GhostHouseEntityEnum, std::unique_ptr<Ghost>> ghosts;

	sf::Font debugFont;
//	sf::Text* debugText;
//	sf::Text* scoreText;
//	sf::Text* finalText;
	std::unique_ptr<sf::Text> debugText;
	std::unique_ptr<sf::Text> scoreText;
	std::unique_ptr<sf::Text> finalText;

	Timer frightenedTimer;
	uint32_t score;
	uint32_t dotsRemaining;
};

