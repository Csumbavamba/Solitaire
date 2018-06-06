#include "Card.h"
#include <iostream>

extern HBITMAP cardsBitmap;


Card::Card(Suit suit, int cardNumber)
{
	this->cardNumber = cardNumber;
	this->suit = suit;
}


Card::~Card()
{
}

Suit Card::GetSuit() const
{
	return suit;
}

void Card::SetSuit(Suit suit)
{
	this->suit = suit;
}

int Card::GetCardNumber() const
{
	return cardNumber;
}

void Card::SetCardNumber(int cardNumber)
{
	this->cardNumber = cardNumber;
}

void Card::SetBitmap(HBITMAP bitmap)
{
	this->bitmap = bitmap;
}

void Card::Draw(HDC hdc, int drawX, int drawY)
{
	HDC bitmapHDC;

	bitmapHDC = CreateCompatibleDC(hdc);
	SelectObject(bitmapHDC, cardsBitmap);

	BitBlt(hdc, drawX, drawY, 71, 96, bitmapHDC, cardNumber * 73 + 1, suit * 98 + 1, SRCCOPY);

	DeleteDC(bitmapHDC);
}
