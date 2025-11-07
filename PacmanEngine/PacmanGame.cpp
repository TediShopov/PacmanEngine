#include "PacmanGame.h"
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

PacmanGame::PacmanGame() : Engine(),
	score(0), gameState(GameState::Paused)
{
	setPaused(true);
	this->debugFont.openFromFile("../assets/arial.ttf");
	this->debugText = new sf::Text(this->debugFont);

	frightenedTimer = Timer(1.0);
	frightenedTimer.onTimerFinished([&](Timer* t) {this->revertToChaseAfterFrightened(t); });

	init();

}

void PacmanGame::init() 
{
	initTextures();
	initSprites();
	initGameLevelGrid();

	//initWinCondition():
	for (size_t row = 0; row < gameGrid->gridTileDimensions.y; row++)
	{
		for (size_t col = 0; col < gameGrid->gridTileDimensions.x; col++)
		{
			if(gameGrid->at(row,col) == GameLevelGrid::TileType::Dot)
				++dotsRemaining;
		}
	}

	//Init debug grid entity
	initPacman();
	//Create Ghosts
	//initGhosts();
}

void PacmanGame::reset()
{
	gameState = GameState::Running;
	setPaused(false);
	init();
}

void PacmanGame::initPacman()
{
	if(!pacman)
		pacman = std::make_unique<Pacman>(gameGrid.get(), input.get(), spriteMap.at(PacmanString).get());
	gameGrid->playerSpawnPoint = { 1,1 };
	pacman->gridPosition = { gameGrid->playerSpawnPoint };
	pacman->worldPos = gameGrid->getPixelCoordinates(gameGrid->playerSpawnPoint);
	pacman->movementSpeed = 2;
}

void PacmanGame::initGhosts()
{
	ghosts.clear();
	auto blinky = std::make_unique<Ghost>(
		gameGrid.get(),
		spriteMap.at(PacmanString).get(),
		pacman.get(),
		Blinky
	);
	auto gridDim = gameGrid->getGridDimensions();
	blinky->sprite = spriteMap.at(BlinkyString).get();
	blinky->respawnTile = gameGrid->blinkySpawnPoint;
	blinky->scatterTile = {gridDim.x, 0}; // Blinky scatter tile is top-right
	blinky->gridPosition = blinky->respawnTile;
	blinky->worldPos = gameGrid->getPixelCoordinates(blinky->gridPosition);

	auto pinky = std::make_unique<Ghost>(*blinky);
	pinky->respawnTile = gameGrid->pinkySpawnPoint;
	pinky->sprite = spriteMap.at(PinkyString).get();
	pinky->scatterTile = { 2, 0 }; // Pinky's scatter tile is on the top-left
	pinky->setChaseStrategy(Pinky);
	pinky->gridPosition = pinky->respawnTile;
	pinky->worldPos = gameGrid->getPixelCoordinates(pinky->gridPosition);

	auto inky = std::make_unique<Ghost>(*blinky);
	 inky->respawnTile = gameGrid->inkySpawnPoint;
	inky->sprite = spriteMap.at(InkyString).get();
	inky->ally = blinky.get();
	inky->scatterTile = { gridDim.x, gridDim.y}; // Pinky's scatter tile is on the bottom-right
	inky->setChaseStrategy(Inky);
	inky->gridPosition = inky->respawnTile;
	inky->worldPos = gameGrid->getPixelCoordinates(pinky->gridPosition);

	auto clyde = std::make_unique<Ghost>(*blinky);
	clyde->respawnTile = gameGrid->clydeSpawnPoint;
	clyde->sprite = spriteMap.at(ClydeString).get();
	clyde->setChaseStrategy(Clyde); 
	clyde->ally = blinky.get();
	clyde->scatterTile = { 0, gridDim.y+5}; // Clyde's scatter tile is at the bottom right
	clyde->gridPosition = clyde->respawnTile;
	clyde->worldPos = gameGrid->getPixelCoordinates(pinky->gridPosition);

	ghosts.push_back(std::move(blinky));
	ghosts.push_back(std::move(pinky));
	ghosts.push_back(std::move(inky));
	ghosts.push_back(std::move(clyde));
}

void PacmanGame::initGameLevelGrid()
{
	if (!gameGrid)
	{
		gameGrid = std::make_unique<GameLevelGrid>(InitialGridDimensions.x, InitialGridDimensions.y,
		sf::Vector2f{ (float)PacTileSetSpriteDimensions.x ,(float)PacTileSetSpriteDimensions.y });
	}
	gameGrid->setPosition({ 0,0 });

	gameGrid->tileToSpriteMap.insert({ GameLevelGrid::TileType::Dot, *spriteMap.at(BigCoin) });
	gameGrid->tileToSpriteMap.insert({ GameLevelGrid::TileType::Wall,*spriteMap.at(Wall) });

	auto sfmlWindow = static_cast<WindowSFML*>(window.get());
	auto win = sfmlWindow->raw();

	using TT = GameLevelGrid::TileType;
	this->gameGrid->loadLevelCsf("../assets/PacLevel5.csv");
}

void PacmanGame::initSprites()
{
	auto defSprite = std::make_unique<sf::Sprite>(*textureMap.at(SpritesheetString), sf::IntRect{ { 0,180 },{8,8} });
	defSprite->setOrigin({4,4});
	auto globalBounds = defSprite->getGlobalBounds().size;

	//Create Wall Sprite
	auto wallSprite = std::make_unique<sf::Sprite>(*defSprite.get());
	wallSprite->setTextureRect(sf::IntRect{ {0,0},PacTileSetSpriteDimensions });
	wallSprite->setOrigin(PacTileSetSpriteOrigin);
	wallSprite->setColor(DebugWallColor);
	wallSprite->setTexture(*textureMap.at(Wall));

	//Create Pacman Sprite
	auto pacmanSprite = std::make_unique<sf::Sprite>(*textureMap.at(SpritesheetString),
		sf::IntRect{{0,0},PacTileSetSpriteDimensions});
	pacmanSprite->setOrigin({8,8});
	
	auto blinkySprite = std::make_unique<sf::Sprite>(*textureMap.at(SpritesheetString),
		sf::IntRect{{0,80},PacTileSetSpriteDimensions});
	blinkySprite->setOrigin(PacTileSetSpriteOrigin);
	

	auto pinkySprite = std::make_unique<sf::Sprite>(*blinkySprite);
	pinkySprite->setTextureRect(offsetSpriteTextureRect(*blinkySprite, {0,20}));

	auto inkySprite = std::make_unique<sf::Sprite>(*pinkySprite);
	inkySprite->setTextureRect(offsetSpriteTextureRect(*pinkySprite, {0,20}));


	auto clydeSpreit = std::make_unique<sf::Sprite>(*inkySprite);
	clydeSpreit->setTextureRect(offsetSpriteTextureRect(*inkySprite, {0,20}));

	this->spriteMap.insert({ BigCoin, std::move(defSprite) });
	this->spriteMap.insert({ Wall, std::move(wallSprite) });
	this->spriteMap.insert({ PacmanString, std::move(pacmanSprite) });
	this->spriteMap.insert({ BlinkyString, std::move(blinkySprite) });
	this->spriteMap.insert({ ClydeString, std::move(clydeSpreit) });
	this->spriteMap.insert({ InkyString, std::move(inkySprite) });
	this->spriteMap.insert({ PinkyString, std::move(pinkySprite) });
}

void PacmanGame::initTextures()
{
	std::unique_ptr<sf::Texture> wallTexture = std::make_unique<sf::Texture>("../assets/Wall.png");
	std::unique_ptr<sf::Texture> spriteSheet = std::make_unique<sf::Texture>("../assets/spritesheet1.png");
	this->textureMap.insert({ Wall,std::move(wallTexture) });
	this->textureMap.insert({ SpritesheetString,std::move(spriteSheet)});
}


void PacmanGame::fixedUpdate(float dt)
{
	frightenedTimer.update(dt);
	pacman->fixedUpdate(dt);
	for (auto& g : this->ghosts)
		g->fixedUpdate(dt);
	//Debug Control Game Level Grid
#ifdef _DEBUG


	if (input->isKeyDown((int)sf::Keyboard::Key::P))
		eatPill();
	else if (input->isKeyDown((int)sf::Keyboard::Key::O))
		changeAllGhostsState(Scatter);
	else if (input->isKeyDown((int)sf::Keyboard::Key::I))
		changeAllGhostsState(Chase);
	else if (input->isKeyDown((int)sf::Keyboard::Key::U))
		changeAllGhostsState(Dead);


	if (input->isKeyDown((int)sf::Keyboard::Key::J))
		this->gameGrid->gridTileDimensions.x += 10 * dt;
	else if (input->isKeyDown((int)sf::Keyboard::Key::K))
		this->gameGrid->gridTileDimensions.y -= 10 * dt;
	else if (input->isKeyDown((int)sf::Keyboard::Key::L))
		this->gameGrid->gridTileDimensions.x -= 1 * dt;

		//this->gameGrid->tileToSpriteMap.at(GameLevelGrid::TileType::Dot).scale({ 0.9,0.9 });
#endif // _DEBUG

	//Game Collision 
	using Tile = GameLevelGrid::TileType;

	//Game Grid- Pacman Collision
	if (gameGrid->at(pacman->gridPosition) == Tile::Dot)
	{
		score += ScorePerPellet;
		--dotsRemaining;
		if (dotsRemaining <= 0)
		{
			gameState = GameState::Won;
			setPaused(true);

		}

		gameGrid->set(pacman->gridPosition, Tile::Empty);
	}
	if (gameGrid->at(pacman->gridPosition) == Tile::PowerPill)
	{
		changeAllGhostsState(Frightened);
	}


	//Game Grid - Ghost collision
	for (auto& g : ghosts)
	{
		auto ghost = dynamic_cast<Ghost*>(g.get());
		if (ghost->getState() == Dead && ghost->gridPosition == ghost->getRespawnTile() && ghost->approximatelyNearCenter(2.0f))
		{
			//The ghost might enter scatter mode depening on timer ? 
			ghost->changeState(Spawning);
			ghost->desiredDirecton = GameLevelGrid::Directions.at(UP);
			ghost->currentDirecton = GameLevelGrid::Directions.at(UP);

		}
		if (ghost->getState() == Spawning && ghost->gridPosition==gameGrid->ghostHouseExit && ghost->approximatelyNearCenter(2.0f))
		{
			ghost->changeState(Chase);
		}
		//Pacman-ghost collision
		if (pacman->gridPosition == ghost->gridPosition)
		{
			//If ghost frightened pacman eats ghost
			if (ghost->getState() == Frightened)
			{
				ghost->changeState(Dead);
				//Get points
				score += ScorePerGhost;
			}
			else
			{
				this->gameState = GameState::Lost;
				setPaused(true);
			}
		}

	}

	





}

void PacmanGame::render()
{



	auto sfmlWindow = static_cast<WindowSFML*>(window.get());
	auto win = sfmlWindow->raw();
	window->clear();
	gameGrid->draw(*win);
	pacman->draw(*win);
	for (auto& g : this->ghosts)
		g->draw(*win);
	win->draw(*debugText);

	window->display();
}

 void PacmanGame::changeAllGhostsState(int state)
{
	 for (auto& g : ghosts)
	 {
		 auto ghost = dynamic_cast<Ghost*>(g.get());
		 ghost->changeState((GhostStateEnum)state);

	 }

}

void PacmanGame::eatPill() 
{
	changeAllGhostsState(Frightened);
	frightenedTimer.reset();
	frightenedTimer.resume();
}

 void PacmanGame::revertToChaseAfterFrightened(Timer* t)
 {
	 changeAllGhostsState(Chase);
 }

void PacmanGame::update(float lag)
{
	if (input->isKeyDown((int)sf::Keyboard::Key::G))
	//if (input->isPressed((int)sf::Keyboard::Key::G))
	{
		if (gameState == GameState::Paused)
		{
			setPaused(false);
			gameState = GameState::Running;
			input->setKeyUp((int)sf::Keyboard::Key::G);
		}
		else if (gameState == GameState::Running)
		{
			setPaused(true);
			gameState = GameState::Paused;
			input->setKeyUp((int)sf::Keyboard::Key::G);
		}
		else if (gameState == GameState::Lost)
		{
			reset();
		}
		else if (gameState == GameState::Won)
		{
			reset();


		}

	}

	for (auto& g : this->ghosts)
		g->update(lag);
	debugText->setCharacterSize(25);

	std::string debugString = "";
	debugString += "Character Pixel Pos X:" + std::to_string(pacman->worldPos.x) + "Y: " + std::to_string(pacman->worldPos.y) + "\n";
	debugString += "Character Grid Coord X:" + std::to_string(pacman->gridPosition.x) + "Y: " + std::to_string(pacman->gridPosition.y) + "\n";
	debugString += "SCORE: " + std::to_string(score) + "\n";

	switch (this->gameState)
	{
	case GameState::Lost:
		debugString += "GAME STATE: LOST \n";
		break;
	case GameState::Won:
		debugString += "GAME STATE: WON \n";
		break;
	case GameState::Running:
		debugString += "GAME STATE: RUNNING \n";
		break;
	case GameState::Paused:
		debugString += "GAME STATE: PAUSED \n";
		break;
	default:
		break;
	}

	debugText->setPosition(gameGrid->getPixelCoordinates({0,12}));

	debugText->setString(debugString);
}
