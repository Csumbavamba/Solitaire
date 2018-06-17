#pragma once
#include <Windows.h>

enum Suit 
{
	CLUB = 0,
	HEART,
	SPADE,
	DIAMOND
};


class Card
{
public:
	Card(Suit suit, int cardNumber);
	~Card();

	// Getters and Setters
	Suit GetSuit() const;
	void SetSuit(Suit suit);
	int GetCardNumber() const;
	void SetCardNumber(int cardNumber);
	void SetBitmap(HBITMAP bitmap);

	void Draw(HDC hdc, int drawX, int drawY);
	void DrawCardBack(HDC hdc, int drawX, int drawY);

	// @@ Sunny
	void SetIsDiscovered(bool discovered);
	bool GetIsDiscovered() const;
	
private:
	Suit suit;
	int cardNumber;
	HBITMAP bitmap;
	bool isDiscovered;
};

