#pragma once

#include <vector>

class Card;

class Shuffler
{
public:
	Shuffler();
	~Shuffler();

	void Initialize();
	Card * GiveRandomCard();

private:
	std::vector<Card*> availableCards;
};

