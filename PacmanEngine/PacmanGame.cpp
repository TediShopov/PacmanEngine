#include "PacmanGame.h"
#include "chrono"
#include "Pacman.h"
#include "Ghost.h"

//Game configurable properties
constexpr sf::Vector2i PacTileSetSpriteDimensions{ 16,16 };
constexpr sf::Vector2f PacTileSetSpriteOrigin{ 8,8 };
constexpr sf::Vector2i InitialGridDimensions{ 10,10 };
constexpr sf::Color DebugWallColor{ 5,10,120,255 };

PacmanGame::PacmanGame() : Engine(),
score(0), gameState(GameState::Paused)
{
	setPaused(true);
	this->debugFont.openFromFile("../assets/arial.ttf");
	frightenedTimer = Timer(1.0);
	frightenedTimer.onTimerFinished([&](Timer* t) {this->revertToChaseAfterFrightened(t); });

	init();
}

void PacmanGame::init()
{
	initTextures();
	initSprites();

	initGameLevelGrid();
	initPacman();
	//Create Ghosts
	initGhosts();
	initGlobalStateCycles();
	initLabels();
	setWinCondition();
}

void PacmanGame::reset()
{
	score = 0;
	gameState = GameState::Running;
	setPaused(false);
	init();
}

void PacmanGame::initPacman()
{
	if (!pacman)
		pacman = std::make_unique<Pacman>(gameGrid.get(), input.get(), spriteMap.at(PacmanString).get());
	//gameGrid->getSpawnTile(PacManEntityEnum::Pacman) = { 1,1 };
	pacman->gridPosition = { gameGrid->getSpawnTile(PacManEntityEnum::Pacman) };
	pacman->worldPos = gameGrid->getPixelCoordinates(gameGrid->getSpawnTile(PacManEntityEnum::Pacman));
	pacman->movementSpeed = 2;
}

void PacmanGame::initGhosts()
{
	ghosts.clear();
	ghosts.insert({ GhostHouseEntityEnum::Blinky,std::move(createGhostEntity(PacManEntityEnum::Blinky)) });
	ghosts.insert({ GhostHouseEntityEnum::Pinky,std::move(createGhostEntity(PacManEntityEnum::Pinky)) });
	ghosts.insert({ GhostHouseEntityEnum::Inky,std::move(createGhostEntity(PacManEntityEnum::Inky)) });
	ghosts.insert({ GhostHouseEntityEnum::Clyde,std::move(createGhostEntity(PacManEntityEnum::Clyde)) });
}

void PacmanGame::initGameLevelGrid()
{
	if (!gameGrid)
	{
		gameGrid = std::make_unique<GameLevelGrid>(InitialGridDimensions.x, InitialGridDimensions.y,
			sf::Vector2f{ (float)PacTileSetSpriteDimensions.x ,(float)PacTileSetSpriteDimensions.y });
	}
	gameGrid->setPosition({ 0,0 });

	gameGrid->tileToSpriteMap.insert({ GameLevelGrid::TileType::Dot, *spriteMap.at(Dot) });
	gameGrid->tileToSpriteMap.insert({ GameLevelGrid::TileType::PowerPill, *spriteMap.at(PowerPill) });
	gameGrid->tileToSpriteMap.insert({ GameLevelGrid::TileType::Wall,*spriteMap.at(Wall) });

	auto sfmlWindow = static_cast<WindowSFML*>(window.get());
	auto win = sfmlWindow->raw();

	using TT = GameLevelGrid::TileType;
	this->gameGrid->loadLevelCsf("../assets/PacLevel6.csv");
}

void PacmanGame::initSprites()
{
	auto defSprite = std::make_unique<sf::Sprite>(*textureMap.at(SpritesheetString), sf::IntRect{ { 0,180 },{8,8} });
	defSprite->setOrigin({ 4,4 });
	auto globalBounds = defSprite->getGlobalBounds().size;

	auto powerPillSprite = std::make_unique<sf::Sprite>(*defSprite);
	powerPillSprite->setScale({ 2,2 });

	//Create Wall Sprite
	auto wallSprite = std::make_unique<sf::Sprite>(*defSprite.get());
	wallSprite->setTextureRect(sf::IntRect{ {0,0},PacTileSetSpriteDimensions });
	wallSprite->setOrigin(PacTileSetSpriteOrigin);
	wallSprite->setColor(DebugWallColor);
	wallSprite->setTexture(*textureMap.at(Wall));

	//Create Pacman Sprite
	auto pacmanSprite = std::make_unique<sf::Sprite>(*textureMap.at(SpritesheetString),
		sf::IntRect{ {0,0},PacTileSetSpriteDimensions });
	pacmanSprite->setOrigin({ 8,8 });

	auto blinkySprite = std::make_unique<sf::Sprite>(*textureMap.at(SpritesheetString),
		sf::IntRect{ {0,80},PacTileSetSpriteDimensions });
	blinkySprite->setOrigin(PacTileSetSpriteOrigin);

	auto pinkySprite = std::make_unique<sf::Sprite>(*blinkySprite);
	pinkySprite->setTextureRect(offsetSpriteTextureRect(*blinkySprite, { 0,20 }));

	auto inkySprite = std::make_unique<sf::Sprite>(*pinkySprite);
	inkySprite->setTextureRect(offsetSpriteTextureRect(*pinkySprite, { 0,20 }));

	auto clydeSpreit = std::make_unique<sf::Sprite>(*inkySprite);
	clydeSpreit->setTextureRect(offsetSpriteTextureRect(*inkySprite, { 0,20 }));

	this->spriteMap.insert({ Dot, std::move(defSprite) });
	this->spriteMap.insert({ PowerPill, std::move(powerPillSprite) });
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
	this->textureMap.insert({ SpritesheetString,std::move(spriteSheet) });
}

void PacmanGame::initLabels()
{
	this->debugText = std::make_unique<sf::Text>(this->debugFont);
	this->scoreText = std::make_unique<sf::Text>(this->debugFont);
	this->finalText = std::make_unique<sf::Text>(this->debugFont);

	scoreText->setCharacterSize(48);
	scoreText->setPosition(gameGrid->getPixelCoordinates({ 0,gameGrid->getGridDimensions().y }));

	finalText->setFillColor({ 0,255,0,255 });
	finalText->setStyle(sf::Text::Underlined);
	finalText->setCharacterSize(32);
	finalText->setPosition(gameGrid->getPixelCoordinates({ 0,gameGrid->getGridDimensions().y / 2 }));
}

void PacmanGame::initGlobalStateCycles()
{
	ghostHouse.resetLevel();
	ghostHouse.onGhostReleased = [&](GhostHouseEntityEnum entity) {releaseGhost(entity); };
	ghostHouse.returnGhost({
		GhostHouseEntityEnum::Blinky,
		GhostHouseEntityEnum::Pinky,
		GhostHouseEntityEnum::Inky,
		GhostHouseEntityEnum::Clyde });

	globalStateCycle.init({ 7,20,7,20,5 });
	globalStateCycle.onGlobalStateChanged =
		[&](GhostStateEnum) {
		for (auto& [entity, ghost] : ghosts)
		{
			if (ghost->getState() == Scatter || ghost->getState() == Chase)
				ghost->changeState(globalStateCycle.getCurrentGlobalState());
		}
		};

	globalStateCycle.start();
}

void PacmanGame::setWinCondition()
{
	dotsRemaining = 0;
	for (int row = 0; row < gameGrid->getGridDimensions().y; row++)
	{
		for (int col = 0; col < gameGrid->getGridDimensions().x; col++)
		{
			if (gameGrid->at(sf::Vector2i{ col,row }) == GameLevelGrid::TileType::Dot)
				//if(gameGrid->at(row,col) == GameLevelGrid::TileType::Dot)
				dotsRemaining++;
		}
	}
}

void PacmanGame::fixedUpdate(float dt)
{
	frightenedTimer.update(dt);
	globalStateCycle.fixedUpdate(dt);
	pacman->fixedUpdate(dt);
	for (auto& [entity, ghost] : this->ghosts)
		ghost->fixedUpdate(dt);
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
		score += ScorePerDot;
		ghostHouse.onDotEaten();
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
		changeAllActiveGhostStates(Frightened);
		gameGrid->set(pacman->gridPosition, Tile::Empty);
	}

	//Game Grid - Ghost collision
	for (auto& [entity, ghost] : ghosts)
	{
		if (ghost->getState() == Dead && ghost->gridPosition == ghost->getRespawnTile() && ghost->approximatelyNearCenter(2.0f))
		{
			//The ghost might enter scatter mode depening on timer ?
			ghost->changeState(Spawning);
			ghost->desiredDirecton = GameLevelGrid::Directions.at(UP);
			ghost->currentDirecton = GameLevelGrid::Directions.at(UP);
		}
		if (ghost->getState() == Spawning && ghost->gridPosition == gameGrid->ghostHouseExit && ghost->approximatelyNearCenter(2.0f))
		{
			ghost->changeState(globalStateCycle.getCurrentGlobalState());
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
			else if (ghost->getState() != Dead)
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
	for (auto& [entity, ghost] : this->ghosts)
		ghost->draw(*win);

	win->draw(*debugText);
	win->draw(*scoreText);
	win->draw(*finalText);

	window->display();
}

void PacmanGame::changeAllActiveGhostStates(int state)
{
	for (auto& [entity, ghost] : ghosts)
	{
		if(Ghost::isStateActive(ghost->getState()))
			ghost->changeState((GhostStateEnum)state);
	}
}

void PacmanGame::eatPowerPill()
{
	changeAllActiveGhostStates(Frightened);
	frightenedTimer.reset();
	frightenedTimer.resume();
}

void PacmanGame::revertToChaseAfterFrightened(Timer* t)
{
	changeAllActiveGhostStates(Chase);
}

sf::IntRect PacmanGame::offsetSpriteTextureRect(const sf::Sprite& sprite, sf::Vector2i offset)
{
	return sf::IntRect{ sprite.getTextureRect().position + offset, sprite.getTextureRect().size };
}

std::unique_ptr<Ghost> PacmanGame::createGhostEntity(PacManEntityEnum entity)
{
	auto gridDim = gameGrid->getGridDimensions();
	auto ghost = std::make_unique<Ghost>(
		gameGrid.get(),
		spriteMap.at(entityIdToSprite.at(entity)).get(),
		pacman.get(),
		BlinkyMovement
	);
	ghost->sprite = spriteMap.at(entityIdToSprite.at(entity)).get();
	ghost->scatterTile = gameGrid->getScatterTile(entity);
	ghost->respawnTile = gameGrid->getSpawnTile(entity);
	ghost->gridPosition = ghost->respawnTile;
	ghost->worldPos = gameGrid->getPixelCoordinates(ghost->gridPosition);

	auto foundBlinky = ghosts.find(Blinky);
	if (foundBlinky != ghosts.end())
	{
		ghost->ally = foundBlinky->second.get();
	}

	switch (entity)
	{
	case PacManEntityEnum::Blinky:
		ghost->setChaseStrategy(BlinkyMovement);
		break;
	case PacManEntityEnum::Pinky:
		ghost->setChaseStrategy(PinkyMovement);
		break;
	case PacManEntityEnum::Inky:
		ghost->setChaseStrategy(InkyMovement);
		break;
	case PacManEntityEnum::Clyde:
		ghost->setChaseStrategy(ClydeMovement);
		break;
	default:
		break;
	}

	return ghost;
}

void PacmanGame::releaseGhost(GhostHouseEntityEnum ghost)
{
	auto found = ghosts.find(ghost);
	if (found != ghosts.end())
	{
		found->second->changeState(GhostStateEnum::Spawning);
	}
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

	for (auto& [entity, ghost] : this->ghosts)
		ghost->update(lag);
	debugText->setCharacterSize(25);

	std::string debugString = "";
	debugString += "Character Pixel Pos X:" + std::to_string(pacman->worldPos.x) + "Y: " + std::to_string(pacman->worldPos.y) + "\n";
	debugString += "Character Grid Coord X:" + std::to_string(pacman->gridPosition.x) + "Y: " + std::to_string(pacman->gridPosition.y) + "\n";
	debugString += "To Collect" + std::to_string(dotsRemaining);
	debugString += "SCORE: " + std::to_string(score) + "\n";

	switch (globalStateCycle.getCurrentGlobalState())
	{
	case GhostStateEnum::Scatter:
		debugString += "GLOBAL STATE: Scatter \n";
		break;
	case GhostStateEnum::Chase:
		debugString += "GLOBAL STATE: Chase \n";
		break;
	default:
		break;
	}

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

	debugText->setPosition(gameGrid->getPixelCoordinates({ 0,12 }));
	//debugText->setString(debugString);

	std::ostringstream oss;
	oss << "Score: " << std::setfill('0') << std::setw(4) << score;
	std::string scoreString = oss.str();
	scoreText->setCharacterSize(48);
	scoreText->setPosition(gameGrid->getPixelCoordinates({ 0,gameGrid->getGridDimensions().y }));
	scoreText->setString(scoreString);

	std::string finalString = "";
	if (this->gameState == GameState::Lost)
		finalString = "YOU LOST!. \n PRESS G TO RESTART...";
	if (this->gameState == GameState::Won)
		finalString = "YOU WON!. \n PRESS G TO RESTART...";

	finalText->setFillColor({ 0,255,0,255 });
	finalText->setStyle(sf::Text::Underlined);
	finalText->setCharacterSize(32);
	finalText->setPosition(gameGrid->getPixelCoordinates({ 0,gameGrid->getGridDimensions().y / 2 }));
	finalText->setString(finalString);
}