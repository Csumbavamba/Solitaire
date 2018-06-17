#pragma once

#include <vector>
#include <Windows.h>
#include "Pile.h"

class Card;

class DeckPile : public Pile
{
public:
	DeckPile(int xLocation, int yLocation, bool visibility);
	virtual ~DeckPile();
	
	int GetXLocation() const;
	int GetYLocation() const;
	int GetWidth() const;
	int GetHeight() const;
	int GetVisibility() const;

	void SetLocation(int xLocation, int yLocation);
	virtual void SetVisibility(bool visibility);
	int GetPileSize() const;
	virtual void AddCard(Card * card);
	virtual void Draw(HDC hdc);
	Card * RemoveTop();


private:

	std::vector<Card*> cardPile;
	int xLocation;
	int yLocation;

	const int width = 71;
	const int height = 96;

	bool visibility;
};

