#include "WinStack.h"
#include <stack>

WinStack::WinStack(int xLocation, int yLocation)
{
	this->xLocation = xLocation;
	this->yLocation = yLocation;
}

WinStack::~WinStack()
{
}

void WinStack::SetLocation(int xLocation, int yLocation)
{

}

int WinStack::GetXLocation() const
{
	return xLocation;
}

int WinStack::GetYLocation() const
{
	return yLocation;
}

int WinStack::GetStackSize() const
{
	return (int)cardStack.size();
}

void WinStack::AddCard(Card * card)
{

}

void WinStack::Draw(HDC hdc)
{
}

Card * WinStack::RemoveTop()
{
	return nullptr;
}
