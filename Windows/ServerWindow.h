#pragma once
#include "Window.h"
#include "../Network/Server.h"


class ServerWindow : public Window
{
private:
	Server* server;

	void cdd(void) override;

public:
	ServerWindow(std::shared_ptr<sf::RenderWindow> window, sf::Color backgroundColor,
		Server& server);
	void startDrawing(void) override;
};