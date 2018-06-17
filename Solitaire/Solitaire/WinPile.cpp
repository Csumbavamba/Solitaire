#include <Windows.h>
#include <vector>
#include <algorithm>

#include "WinPile.h"
#include "Card.h"



WinPile::WinPile(int xLocation, int yLocation)
{
	this->xLocation = xLocation;
	this->yLocation = yLocation;
}


WinPile::~WinPile()
{
	cardPile.clear();
}

int WinPile::GetXLocation() const
{
	return xLocation;
}

int WinPile::GetYLocation() const
{
	return yLocation;
}

int WinPile::GetWidth() const
{
	return width;
}

int WinPile::GetHeight() const
{
	return height;
}

int WinPile::GetPileSize() const
{
	return cardPile.size();
}

void WinPile::AddCard(Card * card)
{
	cardPile.push_back(card);
}

bool WinPile::isValidPlacement(Card * card)
{
	if (card != nullptr)
	{
		int cardNumber = card->GetCardNumber();
		Suit suit = card->GetSuit();

		// If empty allow only ace
		if (cardPile.empty())
		{
			if (cardNumber == 0)
				return true;
		}
		else
		{
			// Check if the suit is the same
			// Check if the number is just one above than the current one
			if (suit == cardPile.back()->GetSuit() && cardNumber == cardPile.back()->GetCardNumber() + 1)
			{
				return true;
			}
		}
	}

	return false;
}

void WinPile::Draw(HDC hdc)
{
	HBRUSH backgroundBrush = CreateSolidBrush(RGB(0, 128, 0));

	HBRUSH hOldBrush = static_cast<HBRUSH>(SelectObject(hdc, backgroundBrush));

	// Do logic here
	// Draw a rectangle as the base of the pile
	if (cardPile.empty())
	{
		::Rectangle(hdc, xLocation, yLocation, xLocation + width, yLocation + height);
	}
	else
	{
		cardPile.back()->Draw(hdc, xLocation, yLocation);
	}

	SelectObject(hdc, hOldBrush);
	DeleteObject(backgroundBrush);
}
