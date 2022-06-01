#pragma once

enum Cards
{
	reverse = 1,
	two = 2,
	three = 3,
	four = 4,
	five = 5,
	six = 6,
	seven = 7,
	eight = 8,
	nine = 9,
	ten = 10,
	jack = 11,
	queen = 12,
	king = 13,
	ace = 14
};

enum Colors
{
	nothing = 'n',
	club = 'c',
	diamond = 'd',
	heart = 'h',
	spade = 's'
};

//used in server side
class Card
{
protected:
	int number = reverse;
	char color = nothing;
public:
	Card(int number, char color) : number(number), color(color) {}
	int getNumber(void) const { return number; }
	char getColor(void) const {return color; }
	void setNumber(int value) { number = value; }
	void setColor(char value) { color = value; }
};

