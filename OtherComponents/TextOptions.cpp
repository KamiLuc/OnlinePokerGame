#include "TextOptions.h"

TextOptions::TextOptions(float messageXPos, float messageYPos, sf::Font* font, int fontSize, sf::Color textColor, std::string textString, int xFreeSpace, int yFreeSpace) :
	textString(textString),
	messageXPos(messageXPos),
	messageYPos(messageYPos),
	font(font),
	fontSize(fontSize),
	xFreeSpace(xFreeSpace),
	yFreeSpace(yFreeSpace),
	textColor(textColor)
{

}

void TextOptions::LoadTextOptions(sf::Text& sourceText) const
{
	sourceText.setCharacterSize(fontSize);
	sourceText.setFont(*font);
	sourceText.setPosition(messageXPos, messageYPos);
	sourceText.setString(textString);
}