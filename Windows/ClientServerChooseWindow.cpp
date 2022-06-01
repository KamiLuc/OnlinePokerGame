#include "ClientServerChooseWindow.h"
#include <string>
#include <iostream>

void ClientServerChooseWindow::cdd(void)
{
	window->clear(backgroundColor);
	window->draw(*loader);
	window->draw(*menu);
	window->display();
}

ClientServerChooseWindow::ClientServerChooseWindow(std::shared_ptr<sf::RenderWindow> window, sf::Color backgroundColor,
	SwitchingMenu &menu,
	ClientServerSettingsLoader &loader) :
	confrimedOption(""), menu(&menu), loader(&loader),
	Window(window, backgroundColor)
{

}

void ClientServerChooseWindow::startDrawing(void)
{

	while (window->isOpen() and confrimedOption == "")
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

				case sf::Keyboard::Up:
					menu->switchUp();
					break;

				case sf::Keyboard::Down:
					menu->switchDown();
					break;

				case sf::Keyboard::Enter:
					
					if (menu->getMarkedOption() == "Server")
					{
						if (loader->loadServerSettings())
						{
							confrimedOption = menu->getMarkedOption();
						}
						break;
					}

					else
					{
						if (loader->loadClientSettings())
						{
							confrimedOption = menu->getMarkedOption();
						}
						break;
					}
				}
			}
		}
		cdd();
	}
}
