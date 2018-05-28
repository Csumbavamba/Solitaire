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

class Canvas
{
public:
	Canvas();
	~Canvas();
	bool Initialise(HWND _hwnd, int _iWidth, int _iHeight);
	CBackBuffer* GetBackBuffer();
	bool Draw();


private:
	CBackBuffer* backBuffer; // A canvas has a backbuffer.


	HWND hwnd;
};

#endif