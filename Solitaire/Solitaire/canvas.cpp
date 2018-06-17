#include "canvas.h"
#include "DeckPile.h"
#include "backBuffer.h"
#include "Card.h"
#include "VectorPile.h"
#include "Shuffler.h"
#include <vector>
#include <algorithm>

Canvas::Canvas()
{
	backBuffer = new CBackBuffer();
	shuffler = new Shuffler();
	staticDeck = new DeckPile(8, 8, false);
	playableDeck = new DeckPile(8, 8, true);
}

Canvas::~Canvas()
{

}

bool Canvas::Initialise(HWND hwnd, int width, int height)
{
	this->hwnd = hwnd;
	backBuffer->Initialise(hwnd, width, height);
	shuffler->Initialize();

	for (int i = 0; i < 7; ++i)
	{
		vectorPiles[i] = new VectorPile(8 + i * 80, 120);
		for (int j = 0; j <= i; ++j)
		{
			vectorPiles[i]->AddCard(shuffler->GiveRandomCard());
			vectorPiles[i]->SetFaceDownCards(i);
		}
		
	}

	playableDeck->SetLocation(88, 8);

	for (int i = 0; i < 24; ++i)
	{
		staticDeck->AddCard(shuffler->GiveRandomCard());
	}

	delete shuffler;

	hand = new VectorPile(0, 0);
	hand->SetFaceDownCards(0);

	handOffsetX = 0;
	handOffsetY = 0;


	return true;
}

CBackBuffer* Canvas::GetBackBuffer()
{
	//m_pBackBuffer;
	return backBuffer;
}

bool Canvas::Draw()
{
	//HDC hdc = GetDC(hwnd);

	backBuffer->Clear();

	// Draw deck
	staticDeck->Draw(backBuffer->GetBFDC(), IsHandEmpty());
	playableDeck->Draw(backBuffer->GetBFDC(), IsHandEmpty());

	// Do drawing here
	for (VectorPile * vectorPile : vectorPiles)
	{
		vectorPile->Draw(backBuffer->GetBFDC());
	}

	if (hand->GetPileSize() > 0)
		hand->Draw(backBuffer->GetBFDC());


	backBuffer->Present();

	//ReleaseDC(hwnd, hdc);

	return false;
}

void Canvas::AddToCards(Card * card)
{
	cards.push_back(card);
}

void Canvas::SetMousePosition(int mouseX, int mouseY)
{
	this->mouseX = mouseX;
	this->mouseY = mouseY;

	if (hand != nullptr)
	{
		hand->SetLocation(mouseX-handOffsetX, mouseY-handOffsetY);
	}
}

VectorPile * Canvas::GetHoveredOverVectorPile()
{
	VectorPile * tempVectorPile = nullptr;

	for (int i = 0; i < 7; ++i)
	{
		tempVectorPile = vectorPiles[i];

		// Check if the mouseX > xLocation
		// Check if the mouseX < xLocation + width
		// Check if the mouseY > yLocation
		// Check if the mouseY < yLocation + height --> also check based on the size of the vector
		if ((mouseX > tempVectorPile->GetXLocation()) && (mouseX < tempVectorPile->GetXLocation() + 71) && (mouseY > tempVectorPile->GetYLocation()) && (mouseY < tempVectorPile->GetYLocation() + 96 + tempVectorPile->GetPileSize() * 19))
		{
			return tempVectorPile;
		}
		
	}

	return nullptr;
}

int Canvas::GetBottomLocationOfPile(VectorPile * hoveredVectorPile) const
{
	if (hoveredVectorPile != nullptr)
	{
		int bottomY = hoveredVectorPile->GetYLocation() + hoveredVectorPile->GetHeight(); // 96

		bottomY += ((hoveredVectorPile->GetPileSize() - 1) * 19); // i-1 * 19

return bottomY;
	}
	return 0;

}

bool Canvas::IsHandEmpty()
{
	if (hand->GetPileSize() == 0)
		return true;
	return false;
}


// VectorPile Functions

void Canvas::PlaceCards()
{
	// Get the vectorPile I'm hovering over
	VectorPile * hoveredVector = GetHoveredOverVectorPile();

	if (hoveredVector != nullptr)
	{
		int size = hand->GetPileSize();
		// create selectedCard for transferring
		for (int i = 0; i < size; ++i)
		{
			Card * selectedCard = hand->RemoveTop();
			hoveredVector->AddCard(selectedCard);

			// Move selectedCard into hand
			// if (selectedCard != nullptr)

		}



	}
}

void Canvas::PickUpCards()
{
	// Get the vectorPile I'm hovering over
	VectorPile * hoveredVector = GetHoveredOverVectorPile();
	DeckPile * hoveredDeck = GetHoveredOverDeck();
	Card * selectedCard = nullptr;

	// If Hovering Vector
	if (hoveredVector != nullptr)
	{
		if ((mouseX > hoveredVector->GetXLocation()) && (mouseX < hoveredVector->GetXLocation() + hoveredVector->GetWidth()) && (mouseY > hoveredVector->GetYLocation()) && (mouseY < hoveredVector->GetYLocation() + 96 + (hoveredVector->GetPileSize() - 1) * 19))
		{
			// If your hand is not holding anything currently
			if (hand->GetPileSize() == 0)
			{
				// int bottomY = GetBottomLocationOfPile(hoveredVector);
				int currentCheckedYPosition = GetBottomLocationOfPile(hoveredVector) - hoveredVector->GetHeight();
				// Check from the bottom how many cards should be collected
				if (mouseY < currentCheckedYPosition)
				{
					// currentCheckedYPosition -= hoveredVector->GetHeight();

					// Pick up top card

					for (int i = 1; i < hoveredVector->GetPileSize(); ++i)
					{
						// If current Y < mouseY
						if (currentCheckedYPosition - 19 > mouseY)
						{
							// Increase I
							currentCheckedYPosition -= 19;
						}
						else
						{
							for (int j = 0; j < i + 1; ++j)
							{
								selectedCard = hoveredVector->RemoveTop();
								hand->AddCard(selectedCard);
							}
							return;
						}
					}
				}
				else
				{
					selectedCard = hoveredVector->RemoveTop();
					hand->AddCard(selectedCard);
				}
			}
		}
	}
	else if (hoveredDeck != nullptr)
	{
		SelectCorrespondingActionForDeck();
	}
	else
	{
		return;
	}

	
}


// Deck Functions

DeckPile * Canvas::GetHoveredOverDeck()
{
	DeckPile * correspondingDeck = nullptr;

	// If it is the playAble deck return playable deck
	if ((mouseX > playableDeck->GetXLocation()) &&
		(mouseX < playableDeck->GetXLocation() + 71) &&
		(mouseY > playableDeck->GetYLocation()) &&
		(mouseY < playableDeck->GetYLocation() + 96))
	{
		correspondingDeck = playableDeck;
	}

	// If it is the base deck, return base deck
	if ((mouseX > staticDeck->GetXLocation()) &&
		(mouseX < staticDeck->GetXLocation() + 71) &&
		(mouseY > staticDeck->GetYLocation()) &&
		(mouseY < staticDeck->GetYLocation() + 96))
	{
		correspondingDeck = staticDeck;
	}

	return correspondingDeck;
}

void Canvas::SelectCorrespondingActionForDeck()
{
	// If the static deck isn't empty AND the static deck is hovered
	if (GetHoveredOverDeck() == staticDeck)
	{
		if (staticDeck->GetPileSize() == 0)
		{
			MoveCardsBackToStaticDeck();
		}
		else
		{
			// move Top of static deck to playable Deck
			playableDeck->AddCard(staticDeck->RemoveTop());
		}	
	}
	// If hovering the playableDeck and it isn't empty
	else if (GetHoveredOverDeck() == playableDeck && playableDeck->GetPileSize() != 0)
	{
		// move top of playable  deck to hand
		hand->AddCard(playableDeck->RemoveTop());
	}
}

void Canvas::MoveCardsBackToStaticDeck()
{
	int unusedCardsInDeck = playableDeck->GetPileSize();
	for (int i = 0; i < unusedCardsInDeck; ++i)
	{
		Card * cardToBeMoved = playableDeck->RemoveTop();
		staticDeck->AddCard(cardToBeMoved);
	}
}
