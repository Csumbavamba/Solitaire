#include "Shuffler.h"
#include "Card.h"

#include <vector>
#include <algorithm>
#include <time.h>
#include <cstdlib>

Shuffler::Shuffler()
{
}


Shuffler::~Shuffler()
{
	availableCards.clear();
}

void Shuffler::Initialize()
{
	std::srand(unsigned(time(0)));

	Suit suit;

	for (int cardNumber = 0; cardNumber < 13; ++cardNumber)
	{
		for (int suitNumber = 0; suitNumber < 4; ++suitNumber)
		{
			switch (suitNumber)
			{
			case 0: suit = Suit::CLUB; break;
			case 1: suit = Suit::DIAMOND; break;
			case 2: suit = Suit::HEART; break;
			case 3: suit = Suit::SPADE; break;
			}

			Card * card = new Card(suit, cardNumber);

			availableCards.push_back(card);
		}
	}

	std::random_shuffle(availableCards.begin(), availableCards.end());
}

Card * Shuffler::GiveRandomCard()
{
	Card * cardToGive = nullptr;

	// Get a random card from the available cards
	
	cardToGive = availableCards.back();

	// Remove that card from the deck
	availableCards.pop_back();

	return cardToGive;
}
