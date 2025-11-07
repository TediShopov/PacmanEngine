#pragma once
#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>
#include "PacManEntities.h"

enum DirectionEnum {
    UP,
    LEFT,
    DOWN,
    RIGHT
};
class GameLevelGrid 
{

public:
	std::unordered_map<PacManEntityEnum,sf::Vector2i> spawnPoints;
	std::unordered_map<PacManEntityEnum,sf::Vector2i> scatterPoints;

	void initScatterPoints()
	{
		auto gridDim = getGridDimensions();
		scatterPoints.insert({ PacManEntityEnum::Blinky,{ gridDim.x, 0 } });
		scatterPoints.insert({ PacManEntityEnum::Pinky,{ 0, 0 } });
		scatterPoints.insert({ PacManEntityEnum::Inky,{ gridDim.x, gridDim.y } });
		scatterPoints.insert({ PacManEntityEnum::Clyde,{ 0, gridDim.y + 5 } });
	}


	sf::Vector2i getScatterTile(PacManEntityEnum e)
	{
		auto foundSpawnPoint = scatterPoints.find(e);
		if (foundSpawnPoint != scatterPoints.end())
			return foundSpawnPoint->second;
		return {};

	}
	sf::Vector2i getSpawnTile(PacManEntityEnum e)
	{
		auto foundSpawnPoint = spawnPoints.find(e);
		if (foundSpawnPoint != spawnPoints.end())
			return foundSpawnPoint->second;
		return {};

	}

//	sf::Vector2i blinkySpawnPoint;
//	sf::Vector2i inkySpawnPoint;
//	sf::Vector2i clydeSpawnPoint;
//	sf::Vector2i pinkySpawnPoint;
	sf::Vector2i ghostHouseEntrance;
	sf::Vector2i ghostHouseExit;
//sf::Vector2i playerSpawnPoint;

	inline static const std::unordered_map<DirectionEnum, sf::Vector2i> Directions{
		{UP,		sf::Vector2i(0,-1)},
		{LEFT,		sf::Vector2i(-1,0)},
		{DOWN,		sf::Vector2i(0,1)},
		{RIGHT,		sf::Vector2i(1,0)}
	};

	enum class TileType { Empty, Wall, Dot, PowerPill, Door, GhostHouse};

	std::unordered_map<TileType, sf::Sprite> tileToSpriteMap;

	sf::Vector2f gridTileDimensions;
	GameLevelGrid(int rows, int cols, sf::Vector2f gridDims) : Position({ 0,0 }), rows(rows), cols(cols), gridTileDimensions(gridDims)
	{
		gridData = std::vector<TileType>(rows * cols, TileType::Dot);
		initScatterPoints();
	}



	void draw(sf::RenderWindow& win)
	{
		for (size_t row = 0; row < rows; ++row)
		{
			for (size_t col = 0; col < cols; ++col)
			{
				auto foundSprite = tileToSpriteMap.find(at(row, col));
				if (foundSprite != tileToSpriteMap.end())
				{
					sf::Sprite& sprite = foundSprite->second;
					sprite.setPosition(getPixelCoordinates(row, col));
					win.draw((foundSprite->second));
				}
			}
		}
	}

	sf::Vector2f computeTeleportedPixelPosition(sf::Vector2f pixelPos) const
	{
		sf::Vector2f gridDimensions = { cols * gridTileDimensions.x + gridTileDimensions.x/2,rows * gridTileDimensions.y + gridTileDimensions.y/2};
		sf::Vector2f relativePosition = pixelPos - this->Position;

		//Teleport X
		if (relativePosition.x <= 0)
			relativePosition.x = gridDimensions.x + relativePosition.x;

		if (relativePosition.x > gridDimensions.x)
			relativePosition.x = -relativePosition.x;

		//Teleport Y
		if (relativePosition.y <= 0)
			relativePosition.y = gridDimensions.y + relativePosition.y;

		if (relativePosition.y > gridDimensions.y)
			relativePosition.y = -relativePosition.y;

		return this->Position + relativePosition;
	}

	void loadLevel(std::vector<TileType> tiles)
	{
		this->gridData.clear();
		this->gridData.assign(tiles.size(), TileType::Empty);
		for (size_t i = 0; i < tiles.size(); i++)
			this->gridData[i] = tiles[i];

		initScatterPoints();

	}


	void loadLevelCsf(std::string fileName)
	{
		gridData.clear();
		//std::vector<std::vector<TileType>> grid;

		std::ifstream file(fileName);
		std::string line;

		int rows = 0;
		int cols = 0;
		while (std::getline(file, line)) {
			std::stringstream ss(line);
			std::string token;
			cols = 0;
			while (std::getline(ss, token, ',')) {

				if (token == "W") gridData.push_back(TileType::Wall);
				else if (token == ".") gridData.push_back(TileType::Dot);
				else if (token == "O") 
					gridData.push_back(TileType::PowerPill);
				else if (token == "E")
				{
					gridData.push_back(TileType::Door);
					this->ghostHouseEntrance = { cols,rows };
				}
				else if (token == "P")
				{
					gridData.push_back(TileType::Empty);
					this->spawnPoints.insert({ PacManEntityEnum::Pinky,sf::Vector2i{cols,rows} });
				}
				else if (token == "I")
				{
					gridData.push_back(TileType::Empty);
					this->spawnPoints.insert({ PacManEntityEnum::Inky,sf::Vector2i{cols,rows} });
				}
				else if (token == "B")
				{
					gridData.push_back(TileType::Empty);
					this->spawnPoints.insert({ PacManEntityEnum::Blinky,sf::Vector2i{cols,rows} });
					this->ghostHouseExit = { cols,rows };
				}
				else if (token == "C")
				{
					gridData.push_back(TileType::Empty);
					this->spawnPoints.insert({ PacManEntityEnum::Clyde,sf::Vector2i{cols,rows} });
				}
				else if (token == "H")
				{
					gridData.push_back(TileType::Wall);
					//this->playerSpawnPoint = { cols,rows };

				}
				else if (token == "S")
				{
					gridData.push_back(TileType::Empty);
					this->spawnPoints.insert({ PacManEntityEnum::Pacman,sf::Vector2i{cols,rows} });

				}
//				else if (token == "G") row.push_back(TileType::GhostGate);
//				else if (token == "H") row.push_back(TileType::GhostHouse);
				//else if (token == "P") gridData.push_back(TIle)
				else gridData.push_back(TileType::Empty);

				cols++;
			}
			rows++;
		}
		this->cols = cols;
		this->rows = rows;

		initScatterPoints();
	}


	void setPosition(sf::Vector2f pos)
	{
		this->Position = pos;
	}
	sf::Vector2f getPosition()
	{
		return this->Position;
	}

	bool isInRange(int row, int col) const
	{
		return	row >= 0 && row < rows &&
			col >= 0 && col < cols;

	}

	TileType at(int row, int col) const
	{
		if (isInRange(row, col))
			return gridData[getFlatIndex(row, col)];
		return TileType::Empty;
	}
	TileType at(sf::Vector2i i) const
	{
		return at(i.y, i.x);
	}
	void set(int row, int col, TileType t)
	{
		if (isInRange(row, col))
			gridData[getFlatIndex(row, col)] = t;
	}
	void set(sf::Vector2i i, TileType t)
	{
		return set(i.y, i.x, t);
	}


	static bool areDirectionRevererse(sf::Vector2i a, sf::Vector2i b) 
	{
		auto result = a + b;
		return result.x == 0 && result.y == 0;
	}
	static bool areDirectionRevererse(DirectionEnum a, DirectionEnum b)
	{
		auto result = Directions.at(a) + Directions.at(b);
		return result.x == 0 && result.y == 0;
	}


	constexpr int getFlatIndex(int row, int col) const
	{
		return row * (cols) + col;

	}
	//Return the center of the tile in pixel coordinates
	constexpr sf::Vector2f getPixelCoordinates(int row, int col) const
	{
		return { Position.x + (float)col * gridTileDimensions.x + gridTileDimensions.x / 2 ,Position.y + (float)row * gridTileDimensions.y + gridTileDimensions.y / 2};
	}
	constexpr sf::Vector2f getPixelCoordinates(sf::Vector2i pos) const
	{
		return getPixelCoordinates(pos.y, pos.x);

	}
	constexpr sf::Vector2i getCellCoordinates(float pixelX, float pixelY) const
	{
		int x = (pixelX - Position.x) / gridTileDimensions.x;
		int y = (pixelY - Position.y) / gridTileDimensions.y;
		return { x,y };
	}
	constexpr sf::Vector2i getCellCoordinates(sf::Vector2f position) const
	{
		return getCellCoordinates(position.x, position.y);
	}

	constexpr sf::Vector2i getGridDimensions()
	{
		return sf::Vector2i{ cols,rows };
	}



private:
	//Represent the top-left cornect of the grid
	sf::Vector2f Position;

	int rows;
	int cols;

	std::vector<TileType> gridData;

};

