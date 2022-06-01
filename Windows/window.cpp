#include "Window.h"

Window::Window(std::shared_ptr<sf::RenderWindow> window, sf::Color backgroundColor) :
	window(window),
	backgroundColor(backgroundColor)
{
}

void Window::startDrawing(void)
{
	return;
}

