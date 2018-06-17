#pragma once
#include <vector>
#include <Windows.h>

class Card;

class Pile
{

public:
	Pile();
	virtual ~Pile();
	
	virtual void AddCard(Card * card);
	virtual void Draw(HDC hdc) = 0;
	virtual void SetVisibility(bool visibility);

};

