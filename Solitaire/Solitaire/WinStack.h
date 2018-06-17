#pragma once
#include <Windows.h>
#include <stack>

class Card;

class WinStack 
{
public:
	WinStack(int xLocation, int yLocation);
	~WinStack();

	void SetLocation(int xLocation, int yLocation);
	int GetXLocation() const;
	int GetYLocation() const;

	int GetStackSize() const;

	void AddCard(Card * card);
	void Draw(HDC hdc);
	Card * RemoveTop();

private:
	std::stack<Card*> cardStack;

	int xLocation;
	int yLocation;

	const int width = 71;
	const int height = 96;

};