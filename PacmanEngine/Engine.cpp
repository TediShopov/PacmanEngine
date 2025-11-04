#include "Engine.h"

//A quick and dirty way to reference textures and sprites
const std::string BigCoin = "BigCoin";
const std::string Wall = "Wall";

//Game configurable properties
const sf::Vector2i PacTileSetSpriteDimensions {16,16};
const sf::Vector2f PacTileSetSpriteOrigin {8,8};
const sf::Vector2i InitialGridDimensions {10,10};
const sf::Color DebugWallColor {5,10,120,255};

Engine::Engine() :
	window(std::make_unique<WindowSFML>()),
	input(std::make_unique<Input>())
{
	initTextures();

	initSprites();

	initGameLevelGrid();

}

void Engine::initGameLevelGrid()
{
	gameGrid = std::make_unique<GameLevelGrid>(InitialGridDimensions.x, InitialGridDimensions.y,
		sf::Vector2f{ (float)PacTileSetSpriteDimensions.x * 2,(float)PacTileSetSpriteDimensions.y * 2 });
	gameGrid->setPosition(sf::Vector2f{ PacTileSetSpriteDimensions.x * 2.0f,PacTileSetSpriteDimensions.y * 2.0f});


	gameGrid->tileToSpriteMap.insert({ GameLevelGrid::TileType::Dot, *spriteMap.at(BigCoin) });
	gameGrid->tileToSpriteMap.insert({ GameLevelGrid::TileType::Wall,*spriteMap.at(Wall) });

	auto sfmlWindow = static_cast<WindowSFML*>(window.get());
	auto win = sfmlWindow->raw();



	using TT = GameLevelGrid::TileType;
	this->gameGrid->loadLevel(
		{
			TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,
			TT::Wall,TT::Dot,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,
			TT::Wall,TT::Wall,TT::Dot,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,
			TT::Wall,TT::Wall,TT::Wall,TT::Dot,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,
			TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Dot,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,
			TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Dot,TT::Wall,TT::Wall,TT::Wall,TT::Wall,
			TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Dot,TT::Wall,TT::Wall,TT::Wall,
			TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Dot,TT::Wall,TT::Wall,
			TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Dot,TT::Wall,
			TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,TT::Wall,


		}
	);
}

void Engine::initSprites()
{
	auto defSprite = std::make_unique<sf::Sprite>(*textureMap.at(BigCoin), sf::IntRect{ { 0,0 },PacTileSetSpriteDimensions });
	defSprite->setOrigin(PacTileSetSpriteOrigin);
	defSprite->scale({ 2,2 });
	auto globalBounds = defSprite->getGlobalBounds().size;

	//Create Wall Sprite
	sf::Sprite s(*defSprite.get());
	auto wallSprite = std::make_unique<sf::Sprite>(*defSprite.get());
	wallSprite->setColor(DebugWallColor);
	wallSprite->setTexture(*textureMap.at(Wall));
	this->spriteMap.insert({ BigCoin, std::move(defSprite) });
	this->spriteMap.insert({ Wall, std::move(wallSprite) });
}

void Engine::initTextures()
{
	std::unique_ptr<sf::Texture> defTexture = std::make_unique<sf::Texture>("../assets/BigCoin.png");
	//std::unique_ptr<sf::Texture> wallTexture = std::make_unique<sf::Texture>(sf::Vector2u{ 16,16 }, false);
	std::unique_ptr<sf::Texture> wallTexture = std::make_unique<sf::Texture>("../assets/Wall.png");

	this->textureMap.insert({ BigCoin,std::move(defTexture) });
	this->textureMap.insert({ Wall,std::move(wallTexture) });

}

 int Engine::run()
{
	while (window->isOpen()) {
		//$input->pumpEvents(*window);
		input->pumpEvents(*window);
		if (input->isQuitRequested())
			break;

		//time_->tick();
//		 while (time_->stepFixed()) {
//		 }

		window->clear();
		this->update();
		//auto& win = static_cast<WindowSFML&>(*window).raw();
		window->display();
	}

	window->close();
	return 0;


}

 void Engine::update()
 {

	 auto sfmlWindow = static_cast<WindowSFML*>(window.get());

//	 auto currPos = defSprite->getPosition();
//	 float moveSpeed = 0.001;
//
//	 if (input->isKeyDown((int)sf::Keyboard::Key::A))
//		 currPos.x += -moveSpeed;
//	 else if (input->isKeyDown((int)sf::Keyboard::Key::D))
//		 currPos.x += moveSpeed;
//	 else if (input->isKeyDown((int)sf::Keyboard::Key::W))
//		 currPos.y += -moveSpeed;
//	 else if (input->isKeyDown((int)sf::Keyboard::Key::S))
//		 currPos.y += moveSpeed;


	 //Debug Control Game Level Grid
#ifdef _DEBUG
	 



	 if (input->isPressed((int)sf::Keyboard::Key::J))
		 this->gameGrid->gridTileDimensions.x += 1;
	 else if (input->isPressed((int)sf::Keyboard::Key::K))
		 this->gameGrid->gridTileDimensions.y -= 1;
	 else if (input->isPressed((int)sf::Keyboard::Key::I))
		 this->gameGrid->gridTileDimensions.y += 1;
	 else if (input->isPressed((int)sf::Keyboard::Key::L))
		 this->gameGrid->gridTileDimensions.x -= 1;

	  if (input->isPressed((int)sf::Keyboard::Key::U))
		  this->gameGrid->tileToSpriteMap.at(GameLevelGrid::TileType::Dot).scale({1.1,1.1});
	  else if (input->isPressed((int)sf::Keyboard::Key::O))
		  this->gameGrid->tileToSpriteMap.at(GameLevelGrid::TileType::Dot).scale({0.9,0.9});

#endif // _DEBUG




	 //defSprite->setPosition(currPos);

	 auto win = sfmlWindow->raw();


	 input->update(0);


	 //win->draw(*defSprite.get());
	 gameGrid->draw(*win);

 }
