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
class VectorPile;
class Shuffler;

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

	// For testing
	void PickUpTopCard();
	void PlaceCards();

	void PickUpCards();

	bool CanPlaceCards(Card * card);


private:
	CBackBuffer* backBuffer = nullptr; // A canvas has a backbuffer.
	std::vector<Card*> cards;
	VectorPile * vectorPiles [7];
	VectorPile * hand = nullptr;
	HDC hdc;
	HWND hwnd;
	int mouseX;
	int mouseY;
	int handOffsetX;
	int handOffsetY;

	Shuffler * shuffler = nullptr;
};

#endif