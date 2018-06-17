#include "VectorPile.h"
#include "Card.h"


VectorPile::VectorPile(int xLocation, int yLocation)
{
	this->xLocation = xLocation;
	this->yLocation = yLocation;
	faceDownCards = 0;
}


VectorPile::~VectorPile()
{
}

int VectorPile::GetXLocation() const
{
	return xLocation;
}

int VectorPile::GetYLocation() const
{
	return yLocation;
}


int VectorPile::GetWidth() const
{
	return width;
}

int VectorPile::GetHeight() const
{
	return height;
}

int VectorPile::GetFaceDownCards() const
{
	return faceDownCards;
}

void VectorPile::SetVisibility(bool visibility)
{
	if (visibility)
	{
		// Reveal the card below if placing was successful
		if (faceDownCards == cardPile.size())
		{
			faceDownCards -= 1;
		}
	}
	else
	{
		// Don't reveal card underneath if placing was unsuccessful
		faceDownCards = cardPile.size();
	}
}

void VectorPile::SetFaceDownCards(int number)
{
	this->faceDownCards = number;
}

void VectorPile::SetLocation(int xLocation, int yLocation)
{
	this->xLocation = xLocation;
	this->yLocation = yLocation;
}

int VectorPile::GetPileSize() const
{
	return (int)cardPile.size();
}

void VectorPile::AddCard(Card * card)
{
	cardPile.push_back(card);
}

void VectorPile::Draw(HDC hdc)
{
	HBRUSH backgroundBrush = CreateSolidBrush(RGB(0, 128, 0));

	HBRUSH hOldBrush = static_cast<HBRUSH>(SelectObject(hdc, backgroundBrush));

	// Draw a rectangle as the base of the pile
	::Rectangle(hdc, xLocation, yLocation, xLocation + width, yLocation + height);
	// Check if the pile is empty or not

	SelectObject(hdc, hOldBrush);
	DeleteObject(backgroundBrush);

	if (cardPile.size() > 0)
	{
		// Iterate throught the lenght of the pile
		for (unsigned int i = 0; i < cardPile.size(); ++i)
		{
			// Compare the facedown number with the iteration of the loop
			// If i smaller than the number of hidden cards -> draw facedown card
			if ((int)i < faceDownCards)
			{
				//::Rectangle(hdc, xLocation, yLocation + i * 19, xLocation + width, yLocation + height + i * 19);
				cardPile[i]->DrawCardBack(hdc, xLocation, yLocation + i * 19);
			}
			// Otherwise draw card
			else
			{
				cardPile[i]->Draw(hdc, xLocation, yLocation + i * 19);
			}
		}
		
	}
	
}

Card * VectorPile::RemoveTop()
{
	if (cardPile.empty()) { return nullptr; }

	Card * temp = cardPile.back();
	cardPile.pop_back();

	return temp;
}

Card * VectorPile::GetTopCard()
{
	if (cardPile.empty()) { return nullptr; }

	Card * temp = cardPile.back();

	return temp;
}
