#pragma once
#include <SFML/Graphics.hpp>
#include "../Settings/CardTexturesAndFontsContainer.h"
#include "Card.h"

//class has only card texture, not color sprite
class CardWithSprite : public sf::Drawable, public Card
{
protected:
	CardTexturesAndFontsContainer* texturesContainer;
	sf::Sprite cardSprite;
public:
	CardWithSprite(int number, char color, CardTexturesAndFontsContainer* texturesContainer);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;
	void virtual setPosition(float x, float y);
	sf::Vector2f virtual getPosition(void) { return cardSprite.getPosition(); }
};