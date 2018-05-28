#include "backBuffer.h"


CBackBuffer::CBackBuffer()
	:m_hWnd(0),
	m_BFDC(0),
	m_hBFBitmap(0),
	m_iWidth(0),
	m_iHeight(0)
{
}


CBackBuffer::~CBackBuffer()
{
	//Do the cleanup job here
	//Restore the old bitmap into the DC
	SelectObject(m_BFDC, m_hOldBitmap);
	DeleteObject(m_hBFBitmap);
	DeleteDC(m_BFDC);
}

bool CBackBuffer::Initialise(HWND _hWnd, int _iWidth, int _iHeight)
{
	//Initialise the member variables of the class with the passed in values.
	m_hWnd = _hWnd;
	//m_iWidth = _iWidth;
	//m_iHeight = _iHeight;

	RECT _desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &_desktop);
	m_iWidth = _desktop.right;
	m_iHeight = _desktop.bottom;

	//The backbuffer is a bitmap that holds all that needs to be drawn to the screen.
	// So in initalising the backbuffer, we will create a bitmap 
	HDC _hWindowDC = ::GetDC(m_hWnd);

	m_BFDC = CreateCompatibleDC(_hWindowDC); //Create the Backbuffer DC
	m_hBFBitmap = CreateCompatibleBitmap(_hWindowDC, _desktop.right, _desktop.bottom); // _rect.right, _rect.bottom);
																					   //Select the bitmap into the in-memory DC. Save the oldbitmap into the m_hOldBitmap;
	m_hOldBitmap = static_cast<HBITMAP>(SelectObject(m_BFDC, m_hBFBitmap));

	::ReleaseDC(m_hWnd, _hWindowDC);

	//Clear the backbuffer
	Clear();

	return true;

}

HDC CBackBuffer::GetBFDC() const
{
	return m_BFDC;
}

int CBackBuffer::GetHeight() const
{
	return m_iHeight;
}

int CBackBuffer::GetWidth() const
{
	return m_iWidth;
}

void CBackBuffer::Clear()
{
	HBRUSH hOldBrush = static_cast<HBRUSH>(SelectObject(m_BFDC, GetStockObject(WHITE_BRUSH)));
	RECT _rect;
	_rect.top = 0;
	_rect.left = 0;
	_rect.bottom = m_iHeight;
	_rect.right = m_iWidth;

	FillRect(m_BFDC, &_rect, hOldBrush);
	SelectObject(m_BFDC, hOldBrush);
}

void CBackBuffer::Present()
{
	//Blit the image contained in the backbuffer to the window
	RECT _rect;
	GetClientRect(m_hWnd, &_rect);
	HDC _hWindowDC = ::GetDC(m_hWnd);
	BitBlt(_hWindowDC, 0, 0, _rect.right, _rect.bottom, m_BFDC, 0, 0, SRCCOPY);
	ReleaseDC(m_hWnd, _hWindowDC);

}


