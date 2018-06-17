#include "DeckPile.h"
#include "Card.h"

#include <vector>
#include <algorithm>



DeckPile::DeckPile(int xLocation, int yLocation, bool visibility)
{
	this->xLocation = xLocation;
	this->yLocation = yLocation;
	this->visibility = visibility;
}

DeckPile::~DeckPile()
{
}

int DeckPile::GetXLocation() const
{
	return xLocation;
}

int DeckPile::GetYLocation() const
{
	return yLocation;
}

int DeckPile::GetWidth() const
{
	return width;
}

int DeckPile::GetHeight() const
{
	return height;
}

int DeckPile::GetVisibility() const
{
	return visibility;
}

void DeckPile::SetLocation(int xLocation, int yLocation)
{
	this->xLocation = xLocation;
	this->yLocation = yLocation;
}

void DeckPile::SetVisibility(bool visibility)
{
	this->visibility = visibility;
}

int DeckPile::GetPileSize() const
{
	return (int)cardPile.size();
}

void DeckPile::AddCard(Card * card)
{
	cardPile.push_back(card);
}

void DeckPile::Draw(HDC hdc)
{
	HBRUSH backgroundBrush = CreateSolidBrush(RGB(0, 128, 0));

	HBRUSH hOldBrush = static_cast<HBRUSH>(SelectObject(hdc, backgroundBrush));

	// Draw a rectangle as the base of the pile
	if (cardPile.empty())
	{
		::Rectangle(hdc, xLocation, yLocation, xLocation + width, yLocation + height);
	}
	else
	{
		if (visibility == false)
		{
			cardPile.back()->DrawCardBack(hdc, xLocation, yLocation);
		}
		else
		{
			cardPile.back()->Draw(hdc, xLocation, yLocation);
		}
		
	}
	
	// Check if the pile is empty or not

	SelectObject(hdc, hOldBrush);
	DeleteObject(backgroundBrush);


}

Card * DeckPile::RemoveTop()
{
	if (cardPile.empty()) { return nullptr; }

	Card * cardToRemove = cardPile.back();
	cardPile.pop_back();

	return cardToRemove;
}
