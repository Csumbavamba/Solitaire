#include <windows.h>   // Include all the windows headers.
#include <windowsx.h>  // Include useful macros.
#include <time.h>

#include "canvas.h"
#include "VectorPile.h"
#include "resource.h"
#include "Card.h"

#define WINDOW_CLASS_NAME L"WINCLASS1"

HINSTANCE globalHandleToInstance;
HMENU globalMenu;
HWND hwnd;
Canvas* globalCanvas = nullptr;
int mouseX = 0;
int mouseY = 0;

HBITMAP cardsBitmap;
HBITMAP cardsBackBitmap;

void GameLoop()
{
	if (globalCanvas != nullptr)
	{
		globalCanvas->Draw();
	}
}

void GameSetup()
{
	// 52 cards to be created,
	// In the 7 vectors, there will be a total of 28 cards
	// Rest of the cards goes into the deck to be drawn. (24 cards)

	// Create 2 piles that contain 1 card each.
	//GamePile* pile1 = new GamePile();
	//GamePile* pile2 = new GamePile();
}

LRESULT CALLBACK WindowProc(HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	// This is the main message handler of the system.
	PAINTSTRUCT ps; // Used in WM_PAINT.
	HDC hdc;        // Handle to a device context.

	
	

	switch (msg)
	{
	case WM_CREATE:
	{
		// Do initialization stuff here.
		globalCanvas = new Canvas();
		cardsBitmap = LoadBitmap(globalHandleToInstance, MAKEINTRESOURCE(IDB_BITMAP2));
		cardsBackBitmap = LoadBitmap(globalHandleToInstance, MAKEINTRESOURCE(IDB_BITMAP4));

		globalCanvas->Initialise(hwnd, 1500, 800);


		// Return Success.
		return (0);
	}

	case WM_LBUTTONDOWN:
	{

		globalCanvas->PickUpCards();
		return(0);
	}

	case WM_LBUTTONUP:
	{
		globalCanvas->PlaceCards();

		// If Game Offer to Play again
		if (globalCanvas->CheckIfTheGameIsWon())
		{
			Sleep(200);
			int choice = MessageBox(NULL, (LPCWSTR)L"Do you want to play again?", (LPCWSTR)L"Victory!!", MB_ICONASTERISK || MB_YESNO || MB_DEFBUTTON1);
			
			if (choice == 1)
			{
				PostMessage(hwnd, WM_COMMAND, ID_MENU_NEWGAME, 0);
			}
			else
			{
				PostQuitMessage(0);
			}
		}

		return (0);
	}

	case WM_MOUSEMOVE:
	{
		mouseX = static_cast<int>(LOWORD(lParam));
		mouseY = static_cast<int>(HIWORD(lParam));

		globalCanvas->SetMousePosition(mouseX, mouseY);

		// Whenever Drag is true to the card --> follow mouse

		return(0);

	}

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_MENU_NEWGAME:
		{
			delete globalCanvas;
			globalCanvas = new Canvas();
			globalCanvas->Initialise(hwnd, 1500, 800);

			break;
		}

		case ID_MENU_QUIT:
		{
			delete globalCanvas;
			PostQuitMessage(0);
			break;
		}
		default:
			break;
		}
		return(0);
	}
	break;
	case WM_DESTROY:
	{
		// Kill the application, this sends a WM_QUIT message.
		delete globalCanvas;
		globalCanvas = nullptr;
		PostQuitMessage(0);



		// Return success.
		return (0);
	}
	break;

	default:break;
	} // End switch.

	  // Process any messages that we did not take care of...

	return (DefWindowProc(hwnd, msg, wParam, lParam));
}

int WINAPI WinMain(HINSTANCE _hInstance,
	HINSTANCE _hPrevInstance,
	LPSTR _lpCmdLine,
	int _nCmdShow)
{
	WNDCLASSEX winclass; // This will hold the class we create.
	HWND hwnd;           // Generic window handle.
	MSG msg;             // Generic message.

	globalHandleToInstance = _hInstance;
	// First fill in the window class structure.
	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = _hInstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground =
		static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = WINDOW_CLASS_NAME;
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// register the window class
	if (!RegisterClassEx(&winclass))
	{
		return (0);
	}

	globalMenu = LoadMenu(_hInstance, MAKEINTRESOURCE(IDR_MENU1));
	// create the window
	hwnd = CreateWindowEx(NULL, // Extended style.
		WINDOW_CLASS_NAME,      // Class.
		L"Solid Air",   // Title.
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		100, 100,                    // Initial x,y.
		1500, 800,                // Initial width, height.
		NULL,                   // Handle to parent.
		globalMenu,                   // Handle to menu.
		_hInstance,             // Instance of this application.
		NULL);                  // Extra creation parameters.

	if (!(hwnd))
	{
		return (0);
	}


	// Enter main event loop
	while (true)
	{
		// Test if there is a message in queue, if so get it.
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// Test if this is a quit.
			if (msg.message == WM_QUIT)
			{
				return (static_cast<int>(msg.wParam));
			}

			// Translate any accelerator keys.
			TranslateMessage(&msg);
			// Send the message to the window proc.
			DispatchMessage(&msg);
		}

		// Main game processing goes here.
		GameLoop(); //One frame of game logic occurs here...
	}

	// Return to Windows like this...
	
}
