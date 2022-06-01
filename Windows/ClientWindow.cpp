#include "ClientWindow.h"

ClientWindow::ClientWindow(std::shared_ptr<sf::RenderWindow> window, sf::Color backgroundColor, Client& client) :
	Window(window,backgroundColor),
	client(&client)
{
}

void ClientWindow::cdd(void)
{
	window->clear(backgroundColor);
	window->draw(*client);
	window->display();
}

void ClientWindow::startDrawing(void)
{	
	while (!client->shouldLobbyStart() and window->isOpen() and !client->isConnectionLost())
	{
		cdd();
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->close();
			}
			else if (event.type == sf::Event::KeyPressed and client->isError())
			{
				window->close();
			}
		}	
	}
}
