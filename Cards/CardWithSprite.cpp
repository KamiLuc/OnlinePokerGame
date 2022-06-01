#pragma once
#include "CardWithSprite.h"

CardWithSprite::CardWithSprite(int number, char color, CardTexturesAndFontsContainer* texturesContainer) : Card(number, color),
	texturesContainer(texturesContainer)
{
	cardSprite.setOrigin(0.f,0.f);
	cardSprite.setTexture((*texturesContainer).getCardTexture(number));
}

void CardWithSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(cardSprite, states);
}

void CardWithSprite::setPosition(float x, float y)
{
	cardSprite.setPosition(x, y);
}
