#pragma once
#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>
class GameLevelGrid 
{

public:

	enum class TileType   { Empty, Wall, Dot, PowerPill, Door };

	std::unordered_map<TileType,sf::Sprite> tileToSpriteMap;

	sf::Vector2f gridTileDimensions; 
	GameLevelGrid(int rows, int cols, sf::Vector2f gridDims) : Position({0,0}), rows(rows), cols(cols), gridTileDimensions(gridDims)
	{

		gridData = std::vector<TileType>(rows * cols, TileType::Dot);
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
					sprite.setPosition(getPixelCoordinates( row, col));
					win.draw((foundSprite->second));
				}
			}
		}
	}

	void loadLevel(std::vector<TileType> tiles)
	{
		this->gridData.clear();
		this->gridData.assign(tiles.size(), TileType::Empty);
		for (size_t i = 0; i < tiles.size(); i++)
			this->gridData[i] = tiles[i];
	}

	void setPosition( sf::Vector2f pos)
	{
		this->Position = pos;
	}
	sf::Vector2f getPosition()
	{
		return this->Position;
	}


	TileType at(int row, int col) const
	{
		return gridData[getFlatIndex(row,col)];
	}

	constexpr int getFlatIndex(int row, int col) const
	{
		return row * cols + col;

	}
	constexpr sf::Vector2f getPixelCoordinates(int row, int col) const
	{
		return { Position.y + (float)row * gridTileDimensions.y ,Position.x + (float)col * gridTileDimensions.x};

	}
private:
	//Represent the top-left cornect of the grid
	sf::Vector2f Position;

	int rows;
	int cols;

	std::vector<TileType> gridData;

};

