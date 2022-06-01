#include "ServerPokerWindow.h"
#include <string>
#include <iostream>

void ServerPokerWindow::cdd(void)
{
	window->clear();
	serverPoker->draw(*window);
	window->display();
}

ServerPokerWindow::ServerPokerWindow(std::shared_ptr<sf::RenderWindow> window, sf::Color backgroundColor, ServerPoker& serverPoker) :
	Window(window, backgroundColor), serverPoker(&serverPoker)
{

}

void ServerPokerWindow::startDrawing(void)
{
	while (serverPoker->getGameOn() and window->isOpen())
	{
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->close();
			}
		}
		cdd();
	}
}

