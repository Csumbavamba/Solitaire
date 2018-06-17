#pragma once

#include <vector>
#include <Windows.h>

class Card;

class Deck
{
public:
	Deck(int xLocation, int yLocation);
	~Deck();
	
	int GetXLocation() const;
	int GetYLocation() const;
	int GetWidth() const;
	int GetHeight() const;

	void SetLocation(int xLocation, int yLocation);
	int GetPileSize() const;
	void AddCard(Card * card);
	void Draw(HDC hdc);
	Card * RemoveTop();


private:

	std::vector<Card*> cardPile;
	int xLocation;
	int yLocation;

	const int width = 71;
	const int height = 96;
};

