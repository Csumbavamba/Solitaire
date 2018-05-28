#include "GamePile.h"
#include "Card.h"


GamePile::GamePile()
{
}


GamePile::~GamePile()
{
}

void GamePile::SetXLocation(int xLocation)
{
	this->xLocation = xLocation;
}

int GamePile::GetXLocation() const
{
	return xLocation;
}

void GamePile::SetYLocation(int yLocation)
{
	this->yLocation = yLocation;
}

int GamePile::GetYLocation() const
{
	return yLocation;
}

void GamePile::SetWidth(int width)
{
	this->width = width;
}

int GamePile::GetWidth() const
{
	return width;
}

void GamePile::SetHeight(int height)
{
	this->height = height;
}

int GamePile::GetHeight() const
{
	return height;
}

void GamePile::AddCard(Card * card)
{
	pile.push_back(card);
}

void GamePile::RemoveTop()
{
	pile.pop_back();
}
