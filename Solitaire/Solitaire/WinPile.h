#pragma once

#include <vector>
#include <Windows.h>

class Card;

class WinPile
{
public:
	WinPile(int xLocation, int yLocation);
	~WinPile();

	int GetXLocation() const;
	int GetYLocation() const;
	int GetWidth() const;
	int GetHeight() const;
	int GetPileSize() const;

	void AddCard(Card * card);
	bool isValidPlacement(Card * card);
	void Draw(HDC hdc);

private:
	std::vector<Card*> cardPile;
	int xLocation;
	int yLocation;

	const int width = 71;
	const int height = 96;
};

