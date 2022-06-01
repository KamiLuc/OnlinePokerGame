#include "GameEndWindow.h"

void GameEndWindow::cdd(void)
{
	window->clear();
	window->draw(gameEndMessage);
	window->display();
};

GameEndWindow::GameEndWindow(std::shared_ptr<sf::RenderWindow> window, sf::Color backgroundColor, TextOptions& gameEndMessageOptions)
	: Window(window, backgroundColor)
{
	gameEndMessageOptions.LoadTextOptions(gameEndMessage);
}

void GameEndWindow::startDrawing(void)
{
	while (window->isOpen())
	{
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->close();
			}
			else if (event.key.code == sf::Keyboard::Enter)
			{
				break;
			}
		}
		cdd();
	}
}

