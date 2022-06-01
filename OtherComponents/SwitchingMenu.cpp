#include "SwitchingMenu.h"

SwitchingMenu::SwitchingMenu(std::vector<std::string> optionsStrings,
	sf::Font* fontPointer,
	float xPos, float yPos, float dx, float dy, int fontSize,
	sf::Color textColor, sf::Color markedTextColor) :
	optionsTexts(optionsStrings.size()),
	textColor(textColor),
	markedTextColor(markedTextColor),
	fontPointer(fontPointer),
	currentOption(0)
{
	for (int i = 0; i < optionsStrings.size(); i++)
	{
		optionsTexts[i].setFillColor(textColor);
		optionsTexts[i].setString(optionsStrings[i]);
		optionsTexts[i].setFont(*fontPointer);
		optionsTexts[i].setOrigin(0, 0);
		optionsTexts[i].setPosition(float(dx * i + xPos), float(dy * i + yPos));
		optionsTexts[i].setCharacterSize(fontSize);
		options[i] = optionsStrings[i];
	}
	turnTextNotRegular(currentOption);
}

void SwitchingMenu::turnTextRegular(int optionText)
{
	optionsTexts[optionText].setStyle(sf::Text::Regular);
	optionsTexts[optionText].setFillColor(textColor);
}

void SwitchingMenu::turnTextNotRegular(int optionText)
{
	optionsTexts[optionText].setStyle(sf::Text::Bold);
	optionsTexts[optionText].setFillColor(markedTextColor);
}

void SwitchingMenu::switchUp(void)
{
	turnTextRegular(currentOption);
	if (currentOption == 0)
	{
		currentOption = optionsTexts.size() - 1;
	}
	else
		currentOption--;
	turnTextNotRegular(currentOption);
}

void SwitchingMenu::switchDown(void)
{
	turnTextRegular(currentOption);
	if (currentOption == optionsTexts.size() - 1)
	{
		currentOption = 0;
	}
	else
		currentOption++;
	turnTextNotRegular(currentOption);
}

void SwitchingMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto &text : optionsTexts)
	{
		target.draw(text, states);
	}
}

std::string SwitchingMenu::getMarkedOption(void)
{
	return options[currentOption];
}
