#pragma once
#include <SFML/Graphics.hpp>

class Window
{
protected:
	std::shared_ptr<sf::RenderWindow> window;
	sf::Color backgroundColor;
	sf::Event event{};

	//clear, draw, display
	virtual void cdd(void) {};

public:
	Window(std::shared_ptr<sf::RenderWindow> window, sf::Color backgroundColor);
	virtual void startDrawing(void);
	~Window(void)
	{
		window = nullptr;
	}
};




