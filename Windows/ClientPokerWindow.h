#pragma once
#include "Window.h"
#include "../Network/Client.h"
#include "../Games/ClientPoker.h"
#include "../Games/ClientPokerPlays.h"

class ClientPokerWindow : public Window
{
private:
	Client* client;
	ClientPoker* poker;
	ClientPokerPlays* pokerPlays;
	void cdd(void);

public:
	ClientPokerWindow(std::shared_ptr<sf::RenderWindow> window, sf::Color backgroundColor,
		Client* clientPointer,
		ClientPoker* clientPokerPointer,
		ClientPokerPlays* clientPokerPlaysPointer); 
	void startDrawing();
};