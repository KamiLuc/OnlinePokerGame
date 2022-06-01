#include "ServerWindow.h"

void ServerWindow::cdd(void)
{
	window->clear();
	server->draw(*window);
	window->display();
}

ServerWindow::ServerWindow(std::shared_ptr<sf::RenderWindow> window, sf::Color backgroundColor, Server& server) : 
	Window(window, backgroundColor), server(&server)
{
}

void ServerWindow::startDrawing(void)
{
	while (!server->shouldGameStart() and window->isOpen())
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
