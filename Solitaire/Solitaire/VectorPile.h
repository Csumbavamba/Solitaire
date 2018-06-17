#pragma once

#include "Pile.h"
#include <vector>
#include <Windows.h>

class Card;

class VectorPile : public Pile
{
public:
	VectorPile(int xLocation, int yLocation);
	virtual ~VectorPile();

	int GetXLocation() const;
	int GetYLocation() const;
	int GetWidth() const;
	int GetHeight() const;

	int GetFaceDownCards() const;
	virtual void SetVisibility(bool visibility); // For failed card placement
	void SetFaceDownCards(int number);
	void SetLocation(int xLocation, int yLocation);

	int GetPileSize() const;

	virtual void AddCard(Card * card);
	virtual void Draw(HDC hdc);
	Card * RemoveTop();
	Card * GetTopCard();

	// @@Sunny
	void ReverseOrder();
	Card * PeekTop();
	Card * PeekFront();
	Card * IterateOne(Card * card, int position);

private:
	std::vector<Card*> cardPile;
	int xLocation;
	int yLocation;

	const int width = 71;
	const int height = 96;

	int faceDownCards;
};

