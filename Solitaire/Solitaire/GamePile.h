#pragma once

#include <vector>

class Card;

class GamePile
{
public:
	GamePile();
	~GamePile();

	void AddCard(Card * card);
	void RemoveTop();

private:
	std::vector<Card*> pile;

};

