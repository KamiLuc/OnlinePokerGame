#pragma once
#include "Window.h"
#include "../Network/Client.h"

class ClientWindow : public Window
{
private:
	Client* client;

public:
	ClientWindow(std::shared_ptr<sf::RenderWindow> window, sf::Color backgroundColor,
		Client& client);
	void cdd(void);
	void startDrawing (void) override ;
};