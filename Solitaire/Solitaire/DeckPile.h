#pragma once

#include <vector>
#include <Windows.h>

class Card;

class DeckPile
{
public:
	DeckPile(int xLocation, int yLocation, bool visibility);
	~DeckPile();
	
	int GetXLocation() const;
	int GetYLocation() const;
	int GetWidth() const;
	int GetHeight() const;
	int GetVisibility() const;

	void SetLocation(int xLocation, int yLocation);
	void SetVisibility(bool visibility);
	int GetPileSize() const;
	void AddCard(Card * card);
	void Draw(HDC hdc, bool isHandEmpty);
	Card * RemoveTop();


private:

	std::vector<Card*> cardPile;
	int xLocation;
	int yLocation;

	const int width = 71;
	const int height = 96;

	bool visibility;
};

