#pragma once
#include <SFML/Graphics.hpp>
#include "../OtherComponents/TwoWayList.h"

class ClientSnake : public sf::Drawable
{
private:
	enum class move { up = 1, right = 2, down = 3, left = 4 };

	move nextMove;

	//2d board
	sf::RectangleShape** board;

	std::pair<int, int> applePos;
	std::pair<int, int> nextHeadPosition;

	//last snake position
	std::pair<int, int> lastPos;

	const sf::Color headColor;
	const sf::Color tailColor;
	const sf::Color appleColor;
	const sf::Color squareColor1;
	const sf::Color squareColor2;

	Two_way_list<std::pair<int, int>> snakePos;

	//represents where apple can be placed
	Two_way_list<std::pair<int, int>> emptyPos;

	bool AppleOnBoard;
	bool GameStarted;

	//board size
	const int boardSizeX;
	const int boardSizeY;
	int snakeSize;
	int boardSize;
	int snakeSpeed;

	//returns pointer to color
	const sf::Color* findBoardColor(int x, int y) const;
	void resetGame(void);
	//used to generate apple in random pos
	int generateRandomInt(int min, int max);
	void fillBoardWithColors(void);
	void placeAppleOnBoard(void);
	bool checkCollision(std::pair<int, int> position) const;

public:
	void updateGame(void);
	ClientSnake(int boardSizeX, int boardSizeY, int squareSize, int snakeSpeed,
		sf::Color headColor, sf::Color tailColor, sf::Color appleColor,
		sf::Color squareColor1, sf::Color squareColor2);
	void changeDirection(char direction);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;
	void drawSnake() const;
	void startGame(void) { GameStarted = true; }
	int getSnakeSpeed(void) const { return snakeSpeed; }

};