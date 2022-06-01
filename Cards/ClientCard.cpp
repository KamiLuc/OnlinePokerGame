#include "ClientCard.h"
#include "../Settings/CardTexturesAndFontsContainer.h"

ClientCard::ClientCard(int number, char color, CardTexturesAndFontsContainer* texturesContainer) : 
	CardWithSprite(number, color, texturesContainer)
{
	auto cardSpritePosition = CardWithSprite::cardSprite.getPosition();
	colorSprite.setOrigin(cardSpritePosition.x - 15, cardSpritePosition.y - 15);
	colorSprite.setTexture((*texturesContainer).getColorTexture(color));
}

void ClientCard::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	CardWithSprite::draw(target, states);
	target.draw(colorSprite);
}

void ClientCard::setPosition(float x, float y)
{
	CardWithSprite::setPosition(x, y);
	colorSprite.setPosition(x, y);
}

void ClientCard::scale(float scale)
{
	colorSprite.setScale(scale, scale);
	cardSprite.setScale(scale, scale);
}