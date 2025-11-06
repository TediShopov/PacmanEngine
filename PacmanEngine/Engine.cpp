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
	this->debugText = new sf::Text(this->debugFont);


	frightenedTimer = Timer(1.0);
	frightenedTimer.onTimerFinished([&](Timer* t) {this->revertToChaseAfterFrightened(t); });

	initTextures();
	initSprites();
	initGameLevelGrid();

	gameGrid->playerSpawnPoint = { 1,1 };
	//Init debug grid entity
	//Pacman p = new Pacman(gameGrid.get(), input.get(), spriteMap.at(PacmanString).get());
	pacman = std::make_unique<Pacman>(gameGrid.get(), input.get(), spriteMap.at(PacmanString).get());

	pacman->gridPosition = {gameGrid->playerSpawnPoint};
	pacman->worldPos = gameGrid->getPixelCoordinates(gameGrid->playerSpawnPoint);
	pacman->movementSpeed = 2;

	const sf::Vector2i defaultGhostSpawnPoint = { 15,15 };

	//Create Ghosts
	//Blinky
	initGhosts(defaultGhostSpawnPoint);
	//pinky = std::make_unique<Ghost>(*blinky);
	//pinky->setChaseStrategy(Pinky);

	//pinky = std::make_unique<Ghost>(*blinky);
	//pinky->setChaseStrategy(Pinky);



}

void Engine::initGhosts(const sf::Vector2i& defaultGhostSpawnPoint)
{

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

void Engine::initGameLevelGrid()
{
	//gameGrid = std::make_unique<GameLevelGrid>(InitialGridDimensions.x, InitialGridDimensions.y,
	//	sf::Vector2f{ (float)PacTileSetSpriteDimensions.x * 2,(float)PacTileSetSpriteDimensions.y * 2 });
	//gameGrid->setPosition(sf::Vector2f{ PacTileSetSpriteDimensions.x * 2.0f,PacTileSetSpriteDimensions.y * 2.0f });
	gameGrid = std::make_unique<GameLevelGrid>(InitialGridDimensions.x, InitialGridDimensions.y,
		sf::Vector2f{ (float)PacTileSetSpriteDimensions.x ,(float)PacTileSetSpriteDimensions.y });
	gameGrid->setPosition({ 0,0 });

	gameGrid->tileToSpriteMap.insert({ GameLevelGrid::TileType::Dot, *spriteMap.at(BigCoin) });
	gameGrid->tileToSpriteMap.insert({ GameLevelGrid::TileType::Wall,*spriteMap.at(Wall) });

	auto sfmlWindow = static_cast<WindowSFML*>(window.get());
	auto win = sfmlWindow->raw();

	using TT = GameLevelGrid::TileType;
	this->gameGrid->loadLevelCsf("../assets/PacLevel4.csv");
}

void Engine::initSprites()
{
	auto defSprite = std::make_unique<sf::Sprite>(*textureMap.at(BigCoin), sf::IntRect{ { 0,0 },PacTileSetSpriteDimensions });
	defSprite->setOrigin(PacTileSetSpriteOrigin);
	//defSprite->scale({ 2,2 });
	auto globalBounds = defSprite->getGlobalBounds().size;

	//Create Wall Sprite
	auto wallSprite = std::make_unique<sf::Sprite>(*defSprite.get());
	wallSprite->setOrigin({8,8});
	wallSprite->setColor(DebugWallColor);
	wallSprite->setScale({0.98f,0.98f});
	wallSprite->setTexture(*textureMap.at(Wall));

	//Create Pacman Sprite
	auto pacmanSprite = std::make_unique<sf::Sprite>(*defSprite.get());
	pacmanSprite->setOrigin({8,8});
	pacmanSprite->setTexture(*textureMap.at(PacmanString));

	auto blinkySprite = std::make_unique<sf::Sprite>(*pacmanSprite);
	blinkySprite->setTexture(*textureMap.at(BlinkyString));

	auto inkySprite = std::make_unique<sf::Sprite>(*pacmanSprite);
	inkySprite->setTexture(*textureMap.at(InkyString));

	auto pinkySprite = std::make_unique<sf::Sprite>(*pacmanSprite);
	pinkySprite->setTexture(*textureMap.at(PinkyString));

	auto clydeSpreit =  std::make_unique<sf::Sprite>(*pacmanSprite);
	clydeSpreit->setTexture(*textureMap.at(ClydeString));

	this->spriteMap.insert({ BigCoin, std::move(defSprite) });
	this->spriteMap.insert({ Wall, std::move(wallSprite) });
	this->spriteMap.insert({ PacmanString, std::move(pacmanSprite) });
	this->spriteMap.insert({ BlinkyString, std::move(blinkySprite) });
	this->spriteMap.insert({ ClydeString, std::move(clydeSpreit) });
	this->spriteMap.insert({ InkyString, std::move(inkySprite) });
	this->spriteMap.insert({ PinkyString, std::move(pinkySprite) });
}

void Engine::initTextures()
{
	std::unique_ptr<sf::Texture> defTexture = std::make_unique<sf::Texture>("../assets/BigCoin.png");
	//std::unique_ptr<sf::Texture> wallTexture = std::make_unique<sf::Texture>(sf::Vector2u{ 16,16 }, false);
	std::unique_ptr<sf::Texture> wallTexture = std::make_unique<sf::Texture>("../assets/Wall.png");
	std::unique_ptr<sf::Texture> pacmanTexture = std::make_unique<sf::Texture>("../assets/PacMan.png");
	std::unique_ptr<sf::Texture> bTexture = std::make_unique<sf::Texture>("../assets/redGhost.png");
	std::unique_ptr<sf::Texture> pTexture = std::make_unique<sf::Texture>("../assets/orangeGhost.png");
	std::unique_ptr<sf::Texture> iTexture = std::make_unique<sf::Texture>("../assets/blueGhost.png");
	std::unique_ptr<sf::Texture> cTextreu = std::make_unique<sf::Texture>("../assets/yellowGhost.png");

	this->textureMap.insert({ BigCoin,std::move(defTexture) });
	this->textureMap.insert({ Wall,std::move(wallTexture) });
	this->textureMap.insert({ PacmanString,std::move(pacmanTexture) });
	this->textureMap.insert({ BlinkyString,std::move(bTexture) });
	this->textureMap.insert({ PinkyString,std::move(pTexture) });
	this->textureMap.insert({ InkyString,std::move(iTexture) });
	this->textureMap.insert({ ClydeString,std::move(cTextreu) });
}

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

	frightenedTimer.update(dt);



	pacman->fixedUpdate(dt);
	for (auto& g : this->ghosts)
		g->fixedUpdate(dt);
	input->update(dt);
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
		gameGrid->set(pacman->gridPosition, Tile::Empty);
	}
	if (gameGrid->at(pacman->gridPosition) == Tile::PowerPill)
	{
		//score += ScorePerPellet;
		//gameGrid->set(pacman->gridPosition, Tile::Empty);
		//frigthen ghost
		changeAllGhostsState(Frightened);
		//lock state for specific game time ? 
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
			//ghost->changeState(Chase);
			//ghost->changeState(Scatter);

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

		}

	}

	





}

void Engine::render()
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

 void Engine::changeAllGhostsState(int state)
{
	 for (auto& g : ghosts)
	 {
		 auto ghost = dynamic_cast<Ghost*>(g.get());
		 ghost->changeState((GhostStateEnum)state);

	 }

}

void Engine::eatPill() 
{
	changeAllGhostsState(Frightened);
	frightenedTimer.reset();
	frightenedTimer.resume();
}

 void Engine::revertToChaseAfterFrightened(Timer* t)
 {
	 changeAllGhostsState(Chase);
 }

void Engine::update(float lag)
{

	for (auto& g : this->ghosts)
		g->update(lag);
	debugText->setCharacterSize(15);

	std::string debugString = "";
	debugString += "Character Pixel Pos X:" + std::to_string(pacman->worldPos.x) + "Y: " + std::to_string(pacman->worldPos.y) + "\n";
	debugString += "Character Grid Coord X:" + std::to_string(pacman->gridPosition.x) + "Y: " + std::to_string(pacman->gridPosition.y) + "\n";
	debugString += "SCORE: " + std::to_string(score);


	debugText->setPosition(gameGrid->getPixelCoordinates({0,12}));

	debugText->setString(debugString);
}