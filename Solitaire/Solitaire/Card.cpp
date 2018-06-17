#include "Card.h"
#include <iostream>

extern HBITMAP cardsBitmap;
extern HBITMAP cardsBackBitmap;


Card::Card(Suit suit, int cardNumber)
{
	this->cardNumber = cardNumber;
	this->suit = suit;
	isDiscovered = false;
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

void Card::DrawCardBack(HDC hdc, int drawX, int drawY)
{
	HDC bitmapHDC;

	bitmapHDC = CreateCompatibleDC(hdc);
	SelectObject(bitmapHDC, cardsBackBitmap);

	StretchBlt(hdc, drawX, drawY, 71, 96, bitmapHDC,0, 0, 71, 96, SRCCOPY);
	// BitBlt(hdc, drawX, drawY, 71, 96, bitmapHDC, cardNumber * 73 + 1, suit * 98 + 1, SRCCOPY);

	DeleteDC(bitmapHDC);
}

void Card::SetIsDiscovered(bool discovered)
{
	isDiscovered = discovered;
}

bool Card::GetIsDiscovered() const
{
	return isDiscovered;
}