#include "Engine.h"
#include "chrono"
#include "Pacman.h"
#include "Ghost.h"

//A quick and dirty way to reference textures and sprites
const std::string BigCoin = "BigCoin";
const std::string Wall = "Wall";
const std::string PacmanString = "Pacman";

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

	chaseGhost = std::make_unique<Ghost>(gameGrid.get(), pacman.get(), spriteMap.at(PacmanString).get());
	//chaseGhost->desiredDirecton = GameLevelGrid::Directions.at(UP);
	chaseGhost->gridPosition = {3,30};
	chaseGhost->worldPos = gameGrid->getPixelCoordinates(chaseGhost->gridPosition);
	chaseGhost->movementSpeed = 2;


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
	this->gameGrid->loadLevelCsf("../assets/PacLevel2.csv");
//	this->gameGrid->loadLevel(
//		{
//			TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,
//			TT::Wall,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Wall,
//			TT::Wall,TT::Dot,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Dot,TT::Wall,
//			TT::Wall,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Wall,
//			TT::Wall,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Wall,
//			TT::Wall,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Wall,
//			TT::Wall,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Wall,
//			TT::Wall,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Wall,
//			TT::Wall,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Dot,TT::Wall,
//			TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,
//		}
//	);




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
	//pacmanSprite->setScale({0.9f,0.9f});
	//pacmanSprite->setScale({2.f,2.f});
	pacmanSprite->setOrigin({8,8});
	pacmanSprite->setTexture(*textureMap.at(PacmanString));

	this->spriteMap.insert({ BigCoin, std::move(defSprite) });
	this->spriteMap.insert({ Wall, std::move(wallSprite) });
	this->spriteMap.insert({ PacmanString, std::move(pacmanSprite) });
}

void Engine::initTextures()
{
	std::unique_ptr<sf::Texture> defTexture = std::make_unique<sf::Texture>("../assets/BigCoin.png");
	//std::unique_ptr<sf::Texture> wallTexture = std::make_unique<sf::Texture>(sf::Vector2u{ 16,16 }, false);
	std::unique_ptr<sf::Texture> wallTexture = std::make_unique<sf::Texture>("../assets/Wall.png");
	std::unique_ptr<sf::Texture> pacmanTexture = std::make_unique<sf::Texture>("../assets/PacMan.png");

	this->textureMap.insert({ BigCoin,std::move(defTexture) });
	this->textureMap.insert({ Wall,std::move(wallTexture) });
	this->textureMap.insert({ PacmanString,std::move(pacmanTexture) });
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




	pacman->fixedUpdate(dt);
	chaseGhost->fixedUpdate(dt);
	input->update(dt);
	//Debug Control Game Level Grid
#ifdef _DEBUG

	if (input->isKeyDown((int)sf::Keyboard::Key::P))
	{
		auto ghost = dynamic_cast<Ghost*>(chaseGhost.get());
		ghost->Frighten();

	}
	
	 if (input->isKeyDown((int)sf::Keyboard::Key::O))
	{

		auto ghost = dynamic_cast<Ghost*>(chaseGhost.get());
		ghost->Scatter();

	}
	 if (input->isKeyDown((int)sf::Keyboard::Key::I))
	 {
		auto ghost = dynamic_cast<Ghost*>(chaseGhost.get());
		ghost->Aggro(pacman.get());
	 }




	if (input->isKeyDown((int)sf::Keyboard::Key::J))
		this->gameGrid->gridTileDimensions.x += 10 * dt;
	else if (input->isKeyDown((int)sf::Keyboard::Key::K))
		this->gameGrid->gridTileDimensions.y -= 10 * dt;
	else if (input->isKeyDown((int)sf::Keyboard::Key::L))
		this->gameGrid->gridTileDimensions.x -= 1 * dt;

	if (input->isKeyDown((int)sf::Keyboard::Key::U))
		this->gameGrid->tileToSpriteMap.at(GameLevelGrid::TileType::Dot).scale({ 1.1,1.1 });
		//this->gameGrid->tileToSpriteMap.at(GameLevelGrid::TileType::Dot).scale({ 0.9,0.9 });
#endif // _DEBUG

	//Game Collision 
	using Tile = GameLevelGrid::TileType;
	if (gameGrid->at(pacman->gridPosition) == Tile::Dot)
	{
		score += ScorePerPellet;
		gameGrid->set(pacman->gridPosition, Tile::Empty);
	}





}

void Engine::render()
{
	auto sfmlWindow = static_cast<WindowSFML*>(window.get());
	auto win = sfmlWindow->raw();
	window->clear();
	gameGrid->draw(*win);
	pacman->draw(*win);
	chaseGhost->draw(*win);
	win->draw(*debugText);

	window->display();
}

void Engine::update(float lag)
{
	debugText->setCharacterSize(15);

	std::string debugString = "";
	debugString += "Character Pixel Pos X:" + std::to_string(pacman->worldPos.x) + "Y: " + std::to_string(pacman->worldPos.y) + "\n";
	debugString += "Character Grid Coord X:" + std::to_string(pacman->gridPosition.x) + "Y: " + std::to_string(pacman->gridPosition.y) + "\n";
	debugString += "SCORE: " + std::to_string(score);


	debugText->setPosition(gameGrid->getPixelCoordinates({0,12}));

	debugText->setString(debugString);
}