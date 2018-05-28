#pragma once

#include <vector>

class Card;

class GamePile
{
public:
	GamePile();
	~GamePile();

	void SetXLocation(int xLocation);
	int GetXLocation() const;
	void SetYLocation(int yLocation);
	int GetYLocation() const;

	void SetWidth(int width);
	int GetWidth() const;
	void SetHeight(int height);
	int GetHeight() const;

	void AddCard(Card * card);
	void RemoveTop();

private:
	std::vector<Card*> pile;
	int xLocation;
	int yLocation;

	int width;
	int height;
};

