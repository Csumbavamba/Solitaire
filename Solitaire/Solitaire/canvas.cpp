#include "canvas.h"
#include "DeckPile.h"
#include "backBuffer.h"
#include "Card.h"
#include "VectorPile.h"
#include "Shuffler.h"
#include "Pile.h"
#include "WinPile.h"

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

	for (int i = 0; i < 4; ++i)
	{
		winPiles[i] = new WinPile(248 + i * 80, 8);
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
	staticDeck->Draw(backBuffer->GetBFDC());
	playableDeck->Draw(backBuffer->GetBFDC());

	// Do drawing here
	for (VectorPile * vectorPile : vectorPiles)
	{
		vectorPile->Draw(backBuffer->GetBFDC());
	}

	for (WinPile * winPile : winPiles)
	{
		winPile->Draw(backBuffer->GetBFDC());
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


Pile * Canvas::GetPileWithLastInteraction()
{
	return pileWithLastInteraction;
}

void Canvas::SetPileWithLastInteraction(Pile * interactedPile)
{
	this->pileWithLastInteraction = interactedPile;
}


// VectorPile Functions

void Canvas::PlaceCards()
{
	// Get the vectorPile I'm hovering over
	VectorPile * hoveredVector = GetHoveredOverVectorPile();
	WinPile * hoveredWinPile = GetHoveredOverWinPile();

	if (hoveredVector != nullptr && CanPlaceCards(hand->PeekFront()) == true)
	{
		int size = hand->GetPileSize();
		hand->ReverseOrder();

		// create selectedCard for transferring
		for (int i = 0; i < size; ++i)
		{
			Card * selectedCard = hand->RemoveTop();
			hoveredVector->AddCard(selectedCard);

			// if (i == 0)
			{
				pileWithLastInteraction->SetVisibility(true);
			}
		}
	}
	else if (hoveredWinPile != nullptr)
	{
		Card * selectedCard = hand->GetTopCard();
		int size = hand->GetPileSize();

		// If it can be placed, and we are only having one card in the hand, place it
		if (hoveredWinPile->isValidPlacement(selectedCard) && hand->GetPileSize() == 1)
		{
			hoveredWinPile->AddCard(selectedCard);
			hand->RemoveTop();
			pileWithLastInteraction->SetVisibility(true);
		}
		else
		{
			// Otherwise place all cards back to their original location
			for (int i = 0; i < size; ++i)
			{
				selectedCard = hand->GetTopCard();
				pileWithLastInteraction->AddCard(selectedCard);
				hand->RemoveTop();
			}
		}

		
	}
	else
	{
		// Place card back to last interacted Pile

		int size = hand->GetPileSize();
		hand->ReverseOrder();
		// create selectedCard for transferring
		for (int i = 0; i < size; ++i)
		{
			Card * selectedCard = hand->RemoveTop();
			pileWithLastInteraction->AddCard(selectedCard);
			
			// Reveal card underneath
			playableDeck->SetVisibility(true);
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
				int currentCheckedYPosition = GetBottomLocationOfPile(hoveredVector) - hoveredVector->GetHeight();
				// Check from the bottom how many cards should be collected
				if (mouseY < currentCheckedYPosition)
				{

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
								selectedCard = hoveredVector->GetTopCard();

								if (selectedCard->GetIsDiscovered() == true)
								{
									selectedCard = hoveredVector->RemoveTop();
									hand->AddCard(selectedCard);
								}
							}

							hand->ReverseOrder(); // TODO Might not be needed
							SetPileWithLastInteraction(hoveredVector);
							hoveredVector->SetFaceDownCards(hoveredVector->GetFaceDownCards());
							
							return;
						}
					}
				}
				// If the it is only one that we want to select
				else
				{
					selectedCard = hoveredVector->RemoveTop();
					hand->AddCard(selectedCard);

					SetPileWithLastInteraction(hoveredVector);
					hoveredVector->SetFaceDownCards(hoveredVector->GetFaceDownCards());
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
		playableDeck->SetVisibility(false);
		SetPileWithLastInteraction(GetHoveredOverDeck());
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


// WinPile

WinPile * Canvas::GetHoveredOverWinPile()
{
	WinPile * correspondingWinPile = nullptr;

	for (int i = 0; i < 4; ++i)
	{
		// If We hover over the winPile return it
		if ((mouseX > winPiles[i]->GetXLocation()) &&
			(mouseX < winPiles[i]->GetXLocation() + 71) &&
			(mouseY > winPiles[i]->GetYLocation()) &&
			(mouseY < winPiles[i]->GetYLocation() + 96))
		{
			correspondingWinPile = winPiles[i];
		}
	}
	
	return correspondingWinPile;
}

bool Canvas::CheckIfTheGameIsWon()
{
	for (WinPile * winPile : winPiles)
	{
		if (winPile->GetPileSize() != 13)
		{
			return false;
		}
	}
	return true;
}

bool Canvas::CanPlaceCards(Card * card)
{
	if (card == nullptr) { return false; }

	Suit cardSuit = card->GetSuit();
	int cardNumber = card->GetCardNumber();

	VectorPile * hoveredVector = GetHoveredOverVectorPile();
	Card * topCard = hoveredVector->GetTopCard();

	if (hoveredVector == nullptr)
	{
		return false;
	}

	// If the first card of the hand is a king and the hovered vector is empty. King can be placed.
	if ((topCard == nullptr) && (cardNumber == 12))
	{
		return true;
	}
	// Otherwise, return false
	else if (topCard == nullptr)
	{
		return false;
	}
	
	else
	{
		switch (cardSuit)
		{
		case 0:		// CLUB
		{
			if ((topCard->GetSuit() == HEART) || (topCard->GetSuit() == DIAMOND))
			{
				// If your front cards number is 1 less than the card you want to place on
				if (cardNumber == topCard->GetCardNumber() - 1)
				{
					return true;
				}
				return false;
			}
			else
			{
				return false;
			}
			break;
		}

		case 1:		// HEART
		{
			if ((topCard->GetSuit() == CLUB) || (topCard->GetSuit() == SPADE))
			{
				// If your front cards number is 1 less than the card you want to place on
				if (cardNumber == topCard->GetCardNumber() - 1)
				{
					return true;
				}
				return false;
			}
			else
			{
				return false;
			}
			break;
		}

		case 2:		// SPADE
		{
			if ((topCard->GetSuit() == HEART) || (topCard->GetSuit() == DIAMOND))
			{
				// If your front cards number is 1 less than the card you want to place on
				if (cardNumber == topCard->GetCardNumber() - 1)
				{
					return true;
				}
				return false;
			}
			else
			{
				return false;
			}
			break;
		}

		case 3:		// DIAMOND
		{
			if ((topCard->GetSuit() == CLUB) || (topCard->GetSuit() == SPADE))
			{
				// If your front cards number is 1 less than the card you want to place on
				if (cardNumber == topCard->GetCardNumber() - 1)
				{
					return true;
				}
				return false;
			}
			else
			{
				return false;
			}
			break;
		}
		}
	}
	

	return false;
}

// Game Lost - ironic
/*
bool Canvas::GameLost()
{
	int movesAvailable = 0;
	int pos = 0;
	Card * tempCard = nullptr;
	Card * iterate = nullptr;
	Card * visibleCardAtMaxMoveablePos[7];
	for (int i = 0; i < 7; ++i)
	{
		visibleCardAtMaxMoveablePos[i] = nullptr;
	}

	Card * lastVisibleCard[7];
	for (int i = 0; i < 7; ++i)
	{
		lastVisibleCard[i] = nullptr;
	}

	for (int i = 0; i < 7; ++i)
	{
		if (vectorPiles[i]->GetPileSize() == 0)
		{
			// Check for avaialable Kings here

		}
		else
		{
			int size = vectorPiles[i]->GetPileSize();

			//int pos = vectorPiles[i]->GetPileSize() - 1;
			int pos = 0;

			tempCard = vectorPiles[i]->GetTopCard();
			iterate = vectorPiles[i]->GetTopCard();

			// Iterate through all undiscovered cards that can be moved together.
			if (tempCard == nullptr) { return false; }
			
			while ((tempCard != nullptr)  )
			{
				if (vectorPiles[i]->GetPileSize() == 1)
				{
					tempCard = vectorPiles[i]->IterateOne(iterate, 0);
					break;
				}
				else
				{
					// 
					if ((vectorPiles[i]->IterateOne(iterate, pos)->GetCardNumber()) == (vectorPiles[i]->IterateOne(iterate, pos + 1)->GetCardNumber() + 1))
					{
						tempCard = vectorPiles[i]->IterateOne(iterate, pos);
					}
					if (tempCard->GetIsDiscovered() != true)
					{
						break;
					}

					++pos;
				}

			}
			if (vectorPiles[i]->GetPileSize() != 1)
			{
				tempCard = vectorPiles[i]->IterateOne(iterate, pos - 1);
			}
			else
			{
				tempCard = vectorPiles[i]->GetTopCard();
			}

			for (int k = 0; k < 7; ++k)
			{
				if (visibleCardAtMaxMoveablePos[k] == nullptr) { visibleCardAtMaxMoveablePos[k] = tempCard; break; }
			}

			// 
			tempCard = vectorPiles[i]->PeekFront();

			/*for (int j = 0; j < size; ++j)
			{
				tempCard = vectorPiles[i]->IterateOne(iterate, j);
			}
			for (int k = 0; k < 7; ++k)
			{
				if (lastVisibleCard[k] == nullptr) { lastVisibleCard[k] = tempCard; break; }
			}
		}

		
	}

	//
	for (int i = 0; i < 7; ++i)
	{
		
		int cardSuit = visibleCardAtMaxMoveablePos[i]->GetSuit();

		for (int j = 0; j < 7; ++j)
		{
			
			if (visibleCardAtMaxMoveablePos[i]->GetCardNumber() == lastVisibleCard[j]->GetCardNumber() - 1)
			{
				switch (cardSuit)
				{
				case 0:	// CLUB
				{
					if ((lastVisibleCard[j]->GetSuit() == HEART) || (lastVisibleCard[j]->GetSuit() == DIAMOND))
					{
						++movesAvailable;
					}
					else
					{
						// Do nothing - No move found
					}
					break;
				}

				case 1: // HEART 
				{
					if ((lastVisibleCard[j]->GetSuit() == SPADE) || (lastVisibleCard[j]->GetSuit() == CLUB))
					{
						++movesAvailable;
					}
					else
					{
						// Do nothing - No move found
					}
					break;
				}

				case 2: // SPADE
				{
					if ((lastVisibleCard[j]->GetSuit() == HEART) || (lastVisibleCard[j]->GetSuit() == DIAMOND))
					{
						++movesAvailable;
					}
					else
					{
						// Do nothing - No move found
					}
					break;
				}

				case 3: // DIAMOND
				{
					if ((lastVisibleCard[j]->GetSuit() == SPADE) || (lastVisibleCard[j]->GetSuit() == CLUB))
					{
						++movesAvailable;
					}
					else
					{
						// Do nothing - No move found
					}
					break;
				}
				}
			}
		}
	}

	if (movesAvailable == 0)
	{
		return true;
	}
	return false;
}


bool Canvas::GameLost()
{
	// Create and populate card arrays that will hold the data of cards
	// Top-most moveable card in a vector is called	- FIRST ARRAY
	// and the card at the very back (FRONT) of a vector is called - SECOND ARRAY

	// Iterating through the vectors
		// Get the card at the back of the vector

		// For the first visible card in a vector
			// Iterate through the cards until you reach the first non-discovered card

			// Set this card's position as the moveable card position in the Vector/Array
	
	// ================= //

		// Still using the back(FRONT) card position of the vector
		// Assign the card values to be values of the second array
}*/