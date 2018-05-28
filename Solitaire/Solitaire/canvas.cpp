#include "canvas.h"
#include "backBuffer.h"

Canvas::Canvas()
{
	backBuffer = new CBackBuffer();
}

Canvas::~Canvas()
{

}

bool Canvas::Initialise(HWND _hwnd, int _iWidth, int _iHeight)
{
	this->hwnd = _hwnd;
	backBuffer->Initialise(_hwnd, _iWidth, _iHeight);

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

	backBuffer->Present();

	//ReleaseDC(hwnd, hdc);

	return false;
}
