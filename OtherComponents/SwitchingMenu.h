#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <filesystem>
#include "../OtherComponents/TextOptions.h"

class SwitchingMenu : public sf::Drawable
{
protected:
	sf::Font* fontPointer;

	std::vector<sf::Text> optionsTexts;

	std::map<int, std::string> options;

	sf::Color textColor;
	sf::Color markedTextColor;

	int currentOption;

public:
	SwitchingMenu(std::vector<std::string> optionsStrings,
		sf::Font* fontPointer,
		float xPos, float yPos, float dx, float dy, int fontSize,
		sf::Color textColor,
		sf::Color markedTextColor);

	void turnTextRegular(int optionText);
	void turnTextNotRegular(int optionText);
	void switchUp(void);
	void switchDown(void);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;

	std::string getMarkedOption(void);
	sf::Color getColor(void) const { return textColor; }
};