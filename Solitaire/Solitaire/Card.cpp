#include "Card.h"
#include <iostream>


Card::Card()
{
	cardNumber = 0;
	suit = DIAMOND;
}


Card::~Card()
{
}

Suit Card::GetSuit() const
{
	return suit;
}

void Card::SetSuit(Suit suit)
{
	this->suit = suit;
}

int Card::GetCardNumber() const
{
	return cardNumber;
}

void Card::SetCardNumber(int cardNumber)
{
	this->cardNumber = cardNumber;
}
