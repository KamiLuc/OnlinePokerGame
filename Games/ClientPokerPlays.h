#pragma once
#include "../OtherComponents/SwitchingMenu.h"
#include "../OtherComponents/TextOptions.h"
#include <string>

class ClientPokerPlays : public SwitchingMenu
{
private:
	std::string tokensString = "";
	//in this rectangle, tokes are
	sf::RectangleShape tokensRectangleMark;
	sf::Text tokensText;
	int maxTokens = 0;

public:
	ClientPokerPlays(std::vector<std::string>& pokerPlaysStringVector, sf::Font* fontPointer,
		float xPos, float yPos, float xFreeSpace, float yFreeSpace, int fontSize,
		sf::Color textColor, sf::Color markedTextColor, float markXSize = 120.f, int maxTokens = 5); 
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;
	int returnTokens(void);
	//changes input from keyboard into tokens
	bool digitToTokens(std::string digit);
	//shrinks tokens field
	bool deleteDigit(void);
};