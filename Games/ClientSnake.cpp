#include "ClientSnake.h"

const sf::Color* ClientSnake::findBoardColor(int x, int y) const
{
	const sf::Color* p;
	if ((x + y) % 2 == 0)
	{
		p = &squareColor1;
	}
	else
		p = &squareColor2;
	return p;
}

void ClientSnake::resetGame(void)
{
	snakePos.clear();
	snakePos.push_back(std::pair((int)boardSizeX / 2, (int)boardSizeY / 2));
	emptyPos.clear();
	for (int i = 0; i < boardSizeX; i++)
	{
		for (int j = 0; j < boardSizeY; j++)
		{
			emptyPos.push_back(std::pair<int, int>(i, j));
		}
	}
	emptyPos.erase_value(snakePos.front());
	GameStarted = false;
	fillBoardWithColors();
	snakeSize = 1;
	AppleOnBoard = false;
}

int ClientSnake::generateRandomInt(int min, int max)
{
	if (min > max)
	{
		throw std::invalid_argument("min > max");
	}
	else
	{
		srand(time(NULL));
		return (rand() % max + min);
	}
}

void ClientSnake::fillBoardWithColors(void)
{
	for (int i = 0; i < boardSizeX; i++)
	{
		for (int j = 0; j < boardSizeY; j++)
		{
			board[i][j].setFillColor(*findBoardColor(i, j));
		}
	}
}

void ClientSnake::placeAppleOnBoard(void)
{
	applePos = emptyPos[generateRandomInt(0, emptyPos.size())];
	board[applePos.first][applePos.second].setFillColor(appleColor);
	AppleOnBoard = true;
}

bool ClientSnake::checkCollision(std::pair<int, int> position) const
{
	if (snakePos.find(position) or position.first < 0 or position.second < 0 or position.first > boardSizeX - 1 or position.second > boardSizeY - 1)
	{
		return true;
	}
	else
		return false;
}

void ClientSnake::updateGame(void)
{
	if (GameStarted)
	{
		if (!AppleOnBoard)
		{
			placeAppleOnBoard();
		}

		nextHeadPosition = snakePos.front();

		if (nextMove == move::up)
		{
			nextHeadPosition.second--;
		}
		else if (nextMove == move::right)
		{
			nextHeadPosition.first++;
		}
		else if (nextMove == move::down)
		{
			nextHeadPosition.second++;
		}
		else if (nextMove == move::left)
		{
			nextHeadPosition.first--;
		}

		if (!checkCollision(nextHeadPosition))
		{

			lastPos = snakePos.back();

			if (snakePos.front() == applePos)
			{
				if (++snakeSize == boardSize)
				{
					resetGame();
					return;
				}
				AppleOnBoard = false;
				snakePos.push_back(lastPos);
				emptyPos.erase_value(lastPos);
			}

			else
			{
				emptyPos.push_back(lastPos);
			}

			if (snakePos.size() > 1)
			{
				snakePos.push_front(nextHeadPosition);
				snakePos.pop_back();
			}

			else
			{
				snakePos.front() = nextHeadPosition;
			}
			emptyPos.erase_value(nextHeadPosition);
		}
		else
		{
			resetGame();
		}
	}
}

ClientSnake::ClientSnake(int boardSizeX, int boardSizeY, int squareSize, int snakeSpeed,
	sf::Color headColor, sf::Color tailColor, sf::Color appleColor, sf::Color squareColor1, sf::Color squareColor2) : boardSizeX(boardSizeX),
	boardSizeY(boardSizeY), headColor(headColor), tailColor(tailColor),
	appleColor(appleColor), squareColor1(squareColor1), squareColor2(squareColor2),
	AppleOnBoard(false), snakeSpeed(snakeSpeed),
	snakeSize(1), boardSize(boardSizeX* boardSizeY),
	GameStarted(false)
{
	snakePos.push_back(std::pair<int, int>((int)boardSizeX / 2, (int)boardSizeY / 2));
	nextMove = move::up;

	board = new sf::RectangleShape * [boardSizeX];
	for (int i = 0; i < boardSizeX; i++)
	{
		board[i] = new sf::RectangleShape[boardSizeY];
	}

	fillBoardWithColors();

	for (int i = 0; i < boardSizeX; i++)
	{
		for (int j = 0; j < boardSizeY; j++)
		{
			emptyPos.push_back(std::pair<int, int>(i, j));
			board[i][j].setSize(sf::Vector2f(squareSize, squareSize));
			board[i][j].setPosition(i * (squareSize)+40, j * (squareSize)+80);
		}
	}
	emptyPos.erase_value(snakePos.front());
}

void ClientSnake::changeDirection(char direction)
{
	if (direction == 'u' and nextMove != move::down)
	{
		nextMove = move::up;
	}
	else if (direction == 'r' and nextMove != move::left)
	{
		nextMove = move::right;
	}
	else if (direction == 'd' and nextMove != move::up)
	{
		nextMove = move::down;
	}
	else if (direction == 'l' and nextMove != move::right)
	{
		nextMove = move::left;
	}
}

void ClientSnake::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < boardSizeX; i++)
	{
		for (int j = 0; j < boardSizeY; j++)
		{
			target.draw(board[i][j]);
		}
	}
	drawSnake();
}

void ClientSnake::drawSnake() const
{
	Two_way_list<std::pair<int, int>>::iterator it = snakePos.begin();
	board[(*it).first][(*it).second].setFillColor(headColor);
	if (snakePos.size() > 1)
	{
		it++;
		board[(*it).first][(*it).second].setFillColor(tailColor);
	}
	board[lastPos.first][lastPos.second].setFillColor(*findBoardColor(lastPos.first, lastPos.second));
}
