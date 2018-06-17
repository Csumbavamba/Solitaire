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
			if (i < faceDownCards)
			{
				//::Rectangle(hdc, xLocation, yLocation + i * 19, xLocation + width, yLocation + height + i * 19);
				cardPile[i]->DrawCardBack(hdc, xLocation, yLocation + i * 19);
				cardPile[i]->SetIsDiscovered(false);
			}
			// Otherwise draw card
			else
			{
				cardPile[i]->Draw(hdc, xLocation, yLocation + i * 19);
				cardPile[i]->SetIsDiscovered(true);
			}
		}
		
	}
	
}

Card * VectorPile::RemoveTop()
{
	if (cardPile.empty()) { return nullptr; }

	Card * temp = cardPile.back();
	cardPile.pop_back();

	// Decrease faceDownCards size if needed
	if (cardPile.size() <= faceDownCards)
	{ 
		if (faceDownCards == 0)
		{
			// Do nothing
		}
		else
		{
			SetFaceDownCards(faceDownCards - 1);
		}
		
	}

	return temp;
}

void VectorPile::ReverseOrder()
{
	if (cardPile.empty()) { return; }
	std::reverse(cardPile.begin(), cardPile.end());
}

Card * VectorPile::PeekTop()
{
	if (cardPile.empty()) { return nullptr; }

	Card * temp = cardPile.back();
	return temp;
}

Card * VectorPile::PeekFront()
{
	if (cardPile.empty()) { return nullptr; }

	Card * temp = cardPile.front();
	return temp;
}
