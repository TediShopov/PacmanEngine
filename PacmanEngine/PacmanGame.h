#pragma once
#include "Engine.h"
class PacmanGame :
    public Engine
{
	const uint16_t ScorePerPellet = 10;
	const uint16_t ScorePerGhost = 10;

public:
	PacmanGame();

	virtual void init() override;



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

	void changeAllGhostsState(int state);
	void eatPill();
	//Reverts specific ghosts to chase/scatter mode after frigtened state
	//duration has been exhausted
	void revertToChaseAfterFrightened(Timer* t);

	sf::IntRect offsetSpriteTextureRect(const sf::Sprite& sprite,sf::Vector2i offset)
	{
		return sf::IntRect{ sprite.getTextureRect().position + offset, sprite.getTextureRect().size };
	}

protected:
	std::unique_ptr<GameLevelGrid> gameGrid;
	std::unique_ptr<GridEntity> pacman;
	std::vector<std::unique_ptr<GridEntity>> ghosts;
	sf::Font debugFont;
	sf::Text* debugText;
	Timer frightenedTimer;
	uint32_t score;
};

