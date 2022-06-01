#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Window.h"
#include "../OtherComponents/TextOptions.h"


class GameEndWindow : public Window
{
private:

	sf::Text gameEndMessage;
	virtual void cdd(void);

public:
	GameEndWindow(std::shared_ptr<sf::RenderWindow> window, sf::Color backgroundColor, TextOptions& gameEndMessageOptions);
	void startDrawing(void);
};
