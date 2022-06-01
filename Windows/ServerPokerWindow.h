#pragma once
#include "Window.h"
#include "../Games/ServerPoker.h"

#include <SFML/Graphics.hpp>"

class ServerPokerWindow : public Window
{
private:
	ServerPoker* serverPoker;
	void cdd(void) override;

public:
	ServerPokerWindow(std::shared_ptr<sf::RenderWindow> window, sf::Color backgroundColor, ServerPoker& serverPoker);
	void startDrawing(void) override;
};