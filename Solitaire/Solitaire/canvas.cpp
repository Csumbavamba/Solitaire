#include "canvas.h"
#include "backBuffer.h"
#include "Card.h"
#include "VectorPile.h"
#include <vector>
#include <algorithm>

Canvas::Canvas()
{
	backBuffer = new CBackBuffer();
}

Canvas::~Canvas()
{

}

bool Canvas::Initialise(HWND hwnd, int width, int height)
{
	this->hwnd = hwnd;
	backBuffer->Initialise(hwnd, width, height);

	for (int i = 0; i < 7; ++i)
	{
		vectorPiles[i] = new VectorPile(8 + i * 80, 120);
		for (int j = 0; j <= i; ++j)
		{
			vectorPiles[i]->AddCard(new Card(SPADE, i));
			vectorPiles[i]->SetFaceDownCards(i);
		}
		
	}

	hand = new VectorPile(0, 0);

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

void Canvas::PlaceCard()
{
	// Get the vectorPile I'm hovering over
	VectorPile * hoveredVector = GetHoveredOverVectorPile();

	if (hoveredVector != nullptr)
	{
		// create selectedCard for transferring
		Card * selectedCard = hand->RemoveTop();
		// Move selectedCard into hand
		if (selectedCard != nullptr)
			hoveredVector->AddCard(selectedCard);
	}
}

void Canvas::PickUpCards()
{
	// Get the vectorPile I'm hovering over
	VectorPile * hoveredVector = GetHoveredOverVectorPile();

	if ((mouseX > hoveredVector->GetXLocation()) && (mouseX < hoveredVector->GetXLocation() + 71) && (mouseY > hoveredVector->GetYLocation() + hoveredVector->GetPileSize() * 19) && (mouseY < hoveredVector->GetYLocation() + 96 + hoveredVector->GetPileSize() * 19))
	{
		// PickUpTopCard();
	}
}
