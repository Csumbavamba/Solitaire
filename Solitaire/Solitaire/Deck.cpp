#include "Deck.h"



Deck::Deck(int xLocation, int yLocation)
{
	this->xLocation = xLocation;
	this->yLocation = yLocation;

	// Shuffle somewhere
}

Deck::~Deck()
{
}

int Deck::GetXLocation() const
{
	return xLocation;
}

int Deck::GetYLocation() const
{
	return yLocation;
}

int Deck::GetWidth() const
{
	return width;
}

int Deck::GetHeight() const
{
	return height;
}

void Deck::SetLocation(int xLocation, int yLocation)
{
	this->xLocation = xLocation;
	this->yLocation = yLocation;
}

int Deck::GetPileSize() const
{
	return (int)cardPile.size();
}

void Deck::AddCard(Card * card)
{
	cardPile.push_back(card);
}

void Deck::Draw(HDC hdc)
{
	HBRUSH backgroundBrush = CreateSolidBrush(RGB(0, 128, 0));

	HBRUSH hOldBrush = static_cast<HBRUSH>(SelectObject(hdc, backgroundBrush));

	// Draw a rectangle as the base of the pile
	::Rectangle(hdc, xLocation, yLocation, xLocation + width, yLocation + height);
	// Check if the pile is empty or not

	SelectObject(hdc, hOldBrush);
	DeleteObject(backgroundBrush);

	//if (cardPile.size() > 0)
	//{
	//	// Iterate throught the lenght of the pile
	//	for (unsigned int i = 0; i < cardPile.size(); ++i)
	//	{

	//	}

	//}
}

//Card * Deck::RemoveTop()
//{
//	return nullptr;
//}
