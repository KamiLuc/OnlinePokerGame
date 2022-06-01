
#include <string>
#include <iostream>
#include "LobbySnakeWindow.h"

void LobbySnakeWindow::cdd(void)
{
	window->clear(backgroundColor);
	binarySemaphore.acquire();
	window->draw(*snake);
	binarySemaphore.release();
	window->draw(*client);
	window->display();
}

void LobbySnakeWindow::snakeUpdate(void)
{
	while (!stopSnakeUpdate)
	{
		binarySemaphore.acquire();
		(*snake).updateGame();
		binarySemaphore.release();
		blockMove = false;
		giveReactionTime();
	}
}

//time in milisec
void LobbySnakeWindow::giveReactionTime()
{
	sf::sleep(sf::Time(sf::milliseconds((*snake).getSnakeSpeed())));
}

void LobbySnakeWindow::startDrawing()
{
	snakeUpdateThread.launch();
	while (client->shouldLobbyStart() and window->isOpen() and !client->shouldPokerStart() and !client->isConnectionLost())
	{
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->close();
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{

				case sf::Keyboard::Enter:
					binarySemaphore.acquire();
					snake->startGame();
					binarySemaphore.release();
					break;

				case sf::Keyboard::Up:
					if (!blockMove)
					{
						binarySemaphore.acquire();
						(*snake).changeDirection('u');
						binarySemaphore.release();
						blockMove = true;
					}
					break;

				case sf::Keyboard::Right:
					if (!blockMove)
					{
						binarySemaphore.acquire();
						snake->changeDirection('r');
						binarySemaphore.release();
						blockMove = true;
					}
					break;

				case sf::Keyboard::Down:
					if (!blockMove)
					{
						binarySemaphore.acquire();
						snake->changeDirection('d');
						binarySemaphore.release();
						blockMove = true;
					}
					break;

				case sf::Keyboard::Left:
					if (!blockMove)
					{
						binarySemaphore.acquire();
						snake->changeDirection('l');
						binarySemaphore.release();
						blockMove = true;
					}
					break;

				default:
					break;
				}
			}
		}
		cdd();
	}
	stopSnakeUpdate = true;
}

LobbySnakeWindow::LobbySnakeWindow(std::shared_ptr<sf::RenderWindow> window, sf::Color backgroundColor, Client& client, ClientSnake& clientSnake) :
	Window(window, backgroundColor), binarySemaphore{ 1 },
	client(&client),
	snake(&clientSnake),
	snakeUpdateThread(&LobbySnakeWindow::snakeUpdate, &*this),
	stopSnakeUpdate(false),
	blockMove(false)
{
}



