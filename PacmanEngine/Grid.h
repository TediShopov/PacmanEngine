#pragma once
#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>

enum DirectionEnum {
    UP,
    LEFT,
    DOWN,
    RIGHT
};
class GameLevelGrid 
{

public:

	inline static const std::unordered_map<DirectionEnum, sf::Vector2i> Directions{
		{UP, sf::Vector2i(0,-1)},
		{LEFT, sf::Vector2i(0,1)},
		{DOWN, sf::Vector2i(-1,0)},
		{RIGHT, sf::Vector2i(1,0)}
	};

	enum class TileType { Empty, Wall, Dot, PowerPill, Door };

	std::unordered_map<TileType, sf::Sprite> tileToSpriteMap;

	sf::Vector2f gridTileDimensions;
	GameLevelGrid(int rows, int cols, sf::Vector2f gridDims) : Position({ 0,0 }), rows(rows), cols(cols), gridTileDimensions(gridDims)
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
					sprite.setPosition(getPixelCoordinates(row, col));
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
		return TileType::Wall;
	}
	TileType at(sf::Vector2i i) const
	{
		return at(i.x, i.y);
	}
	void set(int row, int col, TileType t)
	{
		if (isInRange(row, col))
			gridData[getFlatIndex(row, col)] = t;
	}
	void set(sf::Vector2i i, TileType t)
	{
		return set(i.x, i.y, t);
	}


	constexpr int getFlatIndex(int row, int col) const
	{
		return row * cols + col;

	}
	//Return the center of the tile in pixel coordinates
	constexpr sf::Vector2f getPixelCoordinates(int row, int col) const
	{

		return { Position.x + (float)row * gridTileDimensions.y + gridTileDimensions.y / 2,Position.y + (float)col * gridTileDimensions.x + gridTileDimensions.x / 2 };

	}
	constexpr sf::Vector2f getPixelCoordinates(sf::Vector2i pos) const
	{
		return getPixelCoordinates(pos.x, pos.y);

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
private:
	//Represent the top-left cornect of the grid
	sf::Vector2f Position;

	int rows;
	int cols;

	std::vector<TileType> gridData;

};

