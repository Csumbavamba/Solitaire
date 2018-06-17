#pragma once

#include <vector>
#include <Windows.h>

class Card;

class VectorPile
{
public:
	VectorPile(int xLocation, int yLocation);
	~VectorPile();

	int GetXLocation() const;
	int GetYLocation() const;
	int GetWidth() const;
	int GetHeight() const;

	int GetFaceDownCards() const;
	void SetFaceDownCards(int number);
	void SetLocation(int xLocation, int yLocation);

	int GetPileSize() const;

	void AddCard(Card * card);
	void Draw(HDC hdc);
	Card * RemoveTop();

	void ReverseOrder();

	Card * PeekTop();
	Card * PeekFront();

private:
	std::vector<Card*> cardPile;
	int xLocation;
	int yLocation;

	const int width = 71;
	const int height = 96;

	int faceDownCards;
};

