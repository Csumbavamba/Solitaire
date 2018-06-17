#include "canvas.h"
#include "backBuffer.h"
#include "Card.h"
#include "VectorPile.h"
#include <vector>
#include <algorithm>
#include "Shuffler.h"

Canvas::Canvas()
{
	backBuffer = new CBackBuffer();
	shuffler = new Shuffler();
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
			//vectorPiles[i]->AddCard(new Card(SPADE, i));
			vectorPiles[i]->AddCard(shuffler->GiveRandomCard());

			vectorPiles[i]->SetFaceDownCards(i);
		}
		
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



void Canvas::PickUpTopCard()
{
	// Get the vectorPile I'm hovering over
	VectorPile * hoveredVector = GetHoveredOverVectorPile();


	
		if (hoveredVector != nullptr)
		{
			// create selectedCard for transferring
			Card * selectedCard = hoveredVector->RemoveTop();
			// Move selectedCard into hand
			if (selectedCard != nullptr)
				hand->AddCard(selectedCard);
		}	
}

void Canvas::PlaceCards()
{
	// Get the vectorPile I'm hovering over
	VectorPile * hoveredVector = GetHoveredOverVectorPile();

	if (hoveredVector != nullptr)
	{
		int size = hand->GetPileSize();

		if (CanPlaceCards(hand->PeekFront()) == true)
		{
			// Reverse the order of your hand so it places the front card first.
			hand->ReverseOrder();

			// create selectedCard for transferring
			for (unsigned int i = 0; i < size; ++i)
			{
				Card * selectedCard = hand->RemoveTop();
				hoveredVector->AddCard(selectedCard);

				// Move selectedCard into hand
				// if (selectedCard != nullptr)

			}
		}
		
		
		
	}
}

void Canvas::PickUpCards()
{
	// Get the vectorPile I'm hovering over
	VectorPile * hoveredVector = GetHoveredOverVectorPile();
	Card * selectedCard = nullptr;

	// if it is nullptr, do nothing
	if (hoveredVector == nullptr)
	{
		return;
	}


	if ((mouseX > hoveredVector->GetXLocation()) && (mouseX < hoveredVector->GetXLocation() + hoveredVector->GetWidth()) && (mouseY > hoveredVector->GetYLocation()) && (mouseY < hoveredVector->GetYLocation() + 96 + (hoveredVector->GetPileSize() - 1) * 19))
	{
		// If your hand is not holding anything currently
		if (hand->GetPileSize() == 0)
		{
			// int bottomY = GetBottomLocationOfPile(hoveredVector);
			int currentCheckedYPosition = GetBottomLocationOfPile(hoveredVector) - hoveredVector->GetHeight();
			// Check from the bottom how many cards should be collected
			if (mouseY < currentCheckedYPosition)	//  if mouseY is above currentCheckedYPosition
			{
				//currentCheckedYPosition -= hoveredVector->GetHeight();	
				// Pick up top card
				for (unsigned int i = 1; i < hoveredVector->GetPileSize(); ++i)
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
							// create selectedCard for transferring
							selectedCard = hoveredVector->PeekTop();
							if (selectedCard->GetIsDiscovered() == true)
							{
								selectedCard = hoveredVector->RemoveTop();
								hand->AddCard(selectedCard);
							}
						}
						hand->ReverseOrder();
						return;
						
						
					}
					
				}
				
			}
			else
			{
				selectedCard = hoveredVector->PeekTop();
				if (selectedCard->GetIsDiscovered() == true)
				{
					selectedCard = hoveredVector->RemoveTop();
					hand->AddCard(selectedCard);
				}
			}



		}
	}
}

bool Canvas::CanPlaceCards(Card * card)
{
	// Sometimes it crashes at the start here - cardSuit is nullptr. No idea why
	int cardSuit = card->GetSuit();
	int cardNumber = card->GetCardNumber();

	VectorPile * hoveredVector = GetHoveredOverVectorPile();
	Card * topCard = hoveredVector->PeekTop();

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

	return false;
}

bool Canvas::GameLost()
{
	int movesAvailable = 0;
	Card * tempCard = nullptr;
	//Card * visibleCardStackStart1 = nullptr;
	//Card * visibleCardStackStart2 = nullptr;
	//Card * visibleCardStackStart3 = nullptr;
	//Card * visibleCardStackStart4 = nullptr;
	//Card * visibleCardStackStart5 = nullptr;
	//Card * visibleCardStackStart6 = nullptr;
	//Card * visibleCardStackStart7 = nullptr;
	Card * visibleCardStackStart[7];
	for (int i = 0; i < 7; ++i)
	{
		visibleCardStackStart[i] = nullptr;
	}

	//Card * lastVisibleCard1 = nullptr;
	//Card * lastVisibleCard2 = nullptr;
	//Card * lastVisibleCard3 = nullptr;
	//Card * lastVisibleCard4 = nullptr;
	//Card * lastVisibleCard5 = nullptr;
	//Card * lastVisibleCard6 = nullptr;
	//Card * lastVisibleCard7 = nullptr;
	Card * lastVisibleCard[7];
	for (int i = 0; i < 7; ++i)
	{
		lastVisibleCard[i] = nullptr;
	}

	for (int i = 0; i < 7; ++i)
	{
		int size = vectorPiles[i]->GetPileSize();
		int pos = 0;

		tempCard = vectorPiles[i]->PeekTop();
		
		// Iterate through all undiscovered cards.
		while (tempCard->GetIsDiscovered() == false)
		{
			tempCard = vectorPiles[i]->IterateOne(tempCard, pos);

			++pos;
		}

		for (int k = 0; k < 7; ++k)
		{
			if (visibleCardStackStart[k] == nullptr) { visibleCardStackStart[k] = tempCard; }
		}
		//if (visibleCardStackStart1 == nullptr) { visibleCardStackStart1 = tempCard; }
		//if (visibleCardStackStart2 == nullptr) { visibleCardStackStart2 = tempCard; }
		//if (visibleCardStackStart3 == nullptr) { visibleCardStackStart3 = tempCard; }
		//if (visibleCardStackStart4 == nullptr) { visibleCardStackStart4 = tempCard; }
		//if (visibleCardStackStart5 == nullptr) { visibleCardStackStart5 = tempCard; }
		//if (visibleCardStackStart6 == nullptr) { visibleCardStackStart6 = tempCard; }
		//if (visibleCardStackStart7 == nullptr) { visibleCardStackStart7 = tempCard; }

		tempCard = vectorPiles[i]->PeekFront();

		for (int j = 0; j < size; ++j)
		{
			tempCard = vectorPiles[i]->IterateOne(tempCard, j);
		}
		for (int k = 0; k < 7; ++k)
		{
			if (lastVisibleCard[k] == nullptr) { lastVisibleCard[k] = tempCard; }
		}
		//if (lastVisibleCard1 == nullptr) { lastVisibleCard1 = tempCard; }
		//if (lastVisibleCard2 == nullptr) { lastVisibleCard2 = tempCard; }
		//if (lastVisibleCard3 == nullptr) { lastVisibleCard3 = tempCard; }
		//if (lastVisibleCard4 == nullptr) { lastVisibleCard4 = tempCard; }
		//if (lastVisibleCard5 == nullptr) { lastVisibleCard5 = tempCard; }
		//if (lastVisibleCard6 == nullptr) { lastVisibleCard6 = tempCard; }
		//if (lastVisibleCard7 == nullptr) { lastVisibleCard7 = tempCard; }
	}

	//
	for (int i = 0; i < 7; ++i)
	{
		int cardSuit = visibleCardStackStart[i]->GetSuit();

		for (int j = 0; j < 7; ++j)
		{
			if (visibleCardStackStart[i]->GetCardNumber() == lastVisibleCard[j]->GetCardNumber() - 1)
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


