#pragma once
#include <string>
#include <SFML/Graphics.hpp>

class TextOptions
{
private:
	std::string textString;
	float messageXPos;
	float messageYPos;
	sf::Font* font;
	int fontSize;
	int xFreeSpace;
	int yFreeSpace;
	sf::Color textColor ;
public:
	TextOptions(float messageXPos = 0, float messageYPos = 0,
		sf::Font* font = nullptr, int fontSize = 0, sf::Color textColor = sf::Color::White, std::string textString = "", int xFreeSpace = 0, int yFreeSpace = 0);
	void LoadTextOptions(sf::Text& sourceText) const;
	int getXFreeSpace(void) const { return xFreeSpace; }
	int getYFreeSpace(void)const { return yFreeSpace; }
};