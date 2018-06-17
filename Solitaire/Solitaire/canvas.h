#pragma once
/***********************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand

(c) 2018 Media Design School

File Name   :   [Filename]
Description :   [One line description of what is the file is for]
Author      :   [Your name]
Mail        :   [your.name]@mediadesign.school.nz
********************/

#ifndef __CANVAS_H__
#define __CANVAS_H__

//	Library Includes
#include "backBuffer.h"
#include <windows.h>
#include <windowsx.h>
#include <vector>

class Card;
class WinPile;
class VectorPile;
class DeckPile;
class Shuffler;
class Pile;

class Canvas
{
public:
	Canvas();
	~Canvas();
	bool Initialise(HWND hwnd, int width, int height);
	CBackBuffer* GetBackBuffer();
	bool Draw();
	void AddToCards(Card * card);
	void SetMousePosition(int mouseX, int mouseY);
	VectorPile * GetHoveredOverVectorPile();
	int GetBottomLocationOfPile(VectorPile * hoveredVectorPile) const;
	Pile * GetPileWithLastInteraction();
	void SetPileWithLastInteraction(Pile * interactedPile);

	// For testing
	void PlaceCards();
	void PickUpCards();

	// For Deck stuff
	DeckPile * GetHoveredOverDeck();
	void SelectCorrespondingActionForDeck();
	void MoveCardsBackToStaticDeck();

	// For Stacks
	WinPile * GetHoveredOverWinPile();
	bool CheckIfTheGameIsWon();

	// @@@@@ SUNNY
	bool CanPlaceCards(Card * card);
	bool GameLost();

private:
	std::vector<Card*> cards;
	WinPile * winPiles[4];
	VectorPile * vectorPiles[7];
	DeckPile * staticDeck = nullptr;
	DeckPile * playableDeck = nullptr;
	Shuffler * shuffler = nullptr;
	VectorPile * hand = nullptr;
	Pile * pileWithLastInteraction = nullptr;

	HDC hdc;
	HWND hwnd;
	CBackBuffer* backBuffer = nullptr; // A canvas has a backbuffer.
	int mouseX;
	int mouseY;
	int handOffsetX;
	int handOffsetY;
};

#endif