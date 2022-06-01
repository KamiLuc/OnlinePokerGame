#include "ClientPokerWindow.h"

void ClientPokerWindow::cdd(void)
{
	poker->updatePoker();
	window->clear(backgroundColor);
	if (!poker->shouldDisplayEndOfRoundInfo())
	{
		window->draw(*poker);
		window->draw(*pokerPlays);
		window->display();
	}
	else
	{
		window->draw(*poker);
		window->display();
	}
}

ClientPokerWindow::ClientPokerWindow(std::shared_ptr<sf::RenderWindow> window, sf::Color backgroundColor,
	Client* clientPointer, ClientPoker* clientPokerPointer, ClientPokerPlays* clientPokerPlaysPointer) : Window(window, backgroundColor),
	client(clientPointer), poker(clientPokerPointer), pokerPlays(clientPokerPlaysPointer)
{
}

void ClientPokerWindow::startDrawing()
{
	while (client->shouldPokerStart() and window->isOpen() and !client->isConnectionLost() and !poker->isGameEnd())
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
					//
					if (poker->shouldDisplayEndOfRoundInfo())
					{
						poker->EndOfRoundInfoDisplayingIsOver();
					}
					else
					{
						poker->sendPacketWithPlay(pokerPlays->getMarkedOption(), pokerPlays->returnTokens());
					}
					break;

				case sf::Keyboard::Right:
					pokerPlays->switchDown();
					break;

				case sf::Keyboard::Left:
					pokerPlays->switchUp();
					break;

				default:
					break;
				}
			}
			else if (event.type == sf::Event::TextEntered)
			{
				auto uni = event.text.unicode;
				//backspace
				if (uni == '\b')
				{
					pokerPlays->deleteDigit();
				}
				// 0 - 9
				else if (uni >= 48 and uni <= 57)
				{
					std::string temp = "";
					temp += uni;
					pokerPlays->digitToTokens(temp);
				}
				break;
			}
		}
		cdd();
	}
}

