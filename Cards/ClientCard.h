#pragma once
#include "CardWithSprite.h"

class ClientCard : public CardWithSprite
{
private:
	//c - clubs, d - diamonds, h - hearts, s - spades
	sf::Sprite colorSprite;
public:
	ClientCard(int number, char color, CardTexturesAndFontsContainer* texturesContainer);
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;
	void setPosition(float x, float y) override;
	void scale(float scale);
};