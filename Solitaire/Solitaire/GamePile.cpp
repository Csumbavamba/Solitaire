#include "GamePile.h"
#include "Card.h"


GamePile::GamePile()
{
}


GamePile::~GamePile()
{
}

void GamePile::AddCard(Card * card)
{
	pile.push_back(card);
}

void GamePile::RemoveTop()
{
	pile.pop_back();
}
