// Main.c - Entry point for program
//

// Copyright (c) 2008 Progarmmers' Club of Diamond Ranch

// Demo Program 1 : Tick-Tack Toe

#include <windows.h>
#include <commctrl.h>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "Resource.h" 

// Constants

#define IN_PROGRESS			0
#define PLAYER_WON			1
#define PLAYER_LOST			2
#define TIE					3

#define EMPTY				0
#define PLAYER_OWNED		1
#define COMPUTER_OWNED		2

#define GRID_WIDTH			3
#define GRID_HEIGHT			3
#define GRID_DIAGONAL		3

#define MAX_GRID_COUNT		9

// Globals

static char szClassName[]	= "Tick-Tack Toe";
static char szStatusName[]	= "Game Status";

static DWORD ErrorCode;
static char  ErrorMessage[50];

const int  scHeight			= 280;
const int  scWidth			= 220;

const int  stHeight			= 12;
const int  stWidth			= 220;

const int  geHeight			= 212;
const int  geWidth			= 212;
const int  geOffset			= 70;

static int fGameState;
static int fAiLevel;
static int rgGrid[3][3];
static BOOL fComputerStart	= FALSE;

void (*aiPtr)(int *, int *, int rgGrid[3][3]) = NULL;

HBITMAP hbmCross, hbmCircle;
BITMAP  bmCross, bmCircle;

/* INFO :

	Need a smarter AI.

	*/

// Execute AI at random
void aiRandom(int *xGrid, int *yGrid, int rgGrd[3][3])
{
	int cEmptySpaces = 0;
	int iGridX, iGridY;
	int rgRandomTable[MAX_GRID_COUNT][2];
	int wRandom;

	for(iGridX = 0; iGridX < GRID_WIDTH; iGridX ++)
	{
		for(iGridY = 0; iGridY < GRID_HEIGHT; iGridY ++)
		{
			if(rgGrd[iGridX][iGridY] == EMPTY)
			{
				rgRandomTable[cEmptySpaces][0] = iGridX;
				rgRandomTable[cEmptySpaces][1] = iGridY;
				cEmptySpaces ++;
			}
		}
	}

	if(cEmptySpaces != 0)
	{
		srand((unsigned int) time(NULL));
		wRandom = rand( ) % cEmptySpaces;
		*xGrid = rgRandomTable[wRandom][0];
		*yGrid = rgRandomTable[wRandom][1];
	}
	else
	{
		*xGrid = -1;
		*yGrid = -1;
	}
}

void aiSimple(int *xGrid, int *yGrid, int rgGrd[3][3])
{
	int cComputerOccupy = 0;
	int cPlayerOccupy = 0;
	int cEmptySpaces = 0;
	int xEmpty, yEmpty;
	int iGridX, iGridY;
	int rgRandomTable[MAX_GRID_COUNT][2];
	int wRandom; 
	BOOL bEmptySpace = FALSE;

	// Check if there is two in a row (vertically)
	for(iGridX = 0; iGridX < GRID_WIDTH; iGridX ++)
	{
		for(iGridY = 0; iGridY < GRID_HEIGHT; iGridY ++)
		{
			switch(rgGrd[iGridX][iGridY])
			{
			case COMPUTER_OWNED:
				cComputerOccupy ++;
				break;

			case PLAYER_OWNED:
				cPlayerOccupy ++;
				break;

			case EMPTY:
				xEmpty = iGridX;
				yEmpty = iGridY;
				bEmptySpace = TRUE;
				break;

			default:
				break;
			}
		}
		if( ( (cComputerOccupy == 2) || (cPlayerOccupy == 2) ) && (bEmptySpace == TRUE) )
		{
			*xGrid = xEmpty;
			*yGrid = yEmpty;
			return;
		}
			
		cPlayerOccupy = 0;
		cComputerOccupy = 0;
		bEmptySpace = FALSE;
	}

	// Check if there is two in a row (horzontally)
	for(iGridY = 0; iGridY < GRID_HEIGHT; iGridY ++)
	{
		for(iGridX = 0; iGridX < GRID_WIDTH; iGridX ++)
		{
			switch(rgGrd[iGridX][iGridY])
			{
			case COMPUTER_OWNED:
				cComputerOccupy ++;
				break;

			case PLAYER_OWNED:
				cPlayerOccupy ++;
				break;

			case EMPTY:
				xEmpty = iGridX;
				yEmpty = iGridY;
				bEmptySpace = TRUE;
				break;

			default:
				break;
			}
		}
		if( ( (cComputerOccupy == 2) || (cPlayerOccupy == 2) ) && (bEmptySpace == TRUE) )
		{
			*xGrid = xEmpty;
			*yGrid = yEmpty;
			return;
		}
			
		cPlayerOccupy = 0;
		cComputerOccupy = 0;
		bEmptySpace = FALSE;
	}


	// Check if there is two in a row (diagonally)
	for(iGridX = 0; iGridX < GRID_HEIGHT; iGridX ++)
	{
		switch(rgGrd[iGridX][iGridX])
		{
		case COMPUTER_OWNED:
			cComputerOccupy ++;
			break;

		case PLAYER_OWNED:
			cPlayerOccupy ++;
			break;

		case EMPTY:
			xEmpty = iGridX;
			yEmpty = iGridX;
			bEmptySpace = TRUE;
			break;

		default:
			break;
		}
	}
	if( ( (cComputerOccupy == 2) || (cPlayerOccupy == 2) ) && (bEmptySpace == TRUE) )
	{
		*xGrid = xEmpty;
		*yGrid = yEmpty;
		return;
	}

	cPlayerOccupy = 0;
	cComputerOccupy = 0;
	bEmptySpace = FALSE;

	for(iGridX = 0; iGridX < GRID_HEIGHT; iGridX ++)
	{
		switch(rgGrd[iGridX][2 - iGridX])
		{
		case COMPUTER_OWNED:
			cComputerOccupy ++;
			break;

		case PLAYER_OWNED:
			cPlayerOccupy ++;
			break;

		case EMPTY:
			xEmpty = iGridX;
			yEmpty = 2 - iGridX;
			bEmptySpace = TRUE;
			break;

		default:
			break;
		}
	}
	if( ( (cComputerOccupy == 2) || (cPlayerOccupy == 2) ) && (bEmptySpace == TRUE) )
	{
		*xGrid = xEmpty;
		*yGrid = yEmpty;
		return;
	}

	// Random
	for(iGridX = 0; iGridX < GRID_WIDTH; iGridX ++)
	{
		for(iGridY = 0; iGridY < GRID_HEIGHT; iGridY ++)
		{
			if(rgGrd[iGridX][iGridY] == EMPTY)
			{
				rgRandomTable[cEmptySpaces][0] = iGridX;
				rgRandomTable[cEmptySpaces][1] = iGridY;
				cEmptySpaces ++;
			}
		}
	}

	if(cEmptySpaces != 0)
	{
		srand((unsigned int) time(NULL));
		wRandom = rand( ) % cEmptySpaces;
		*xGrid = rgRandomTable[wRandom][0];
		*yGrid = rgRandomTable[wRandom][1];
		return;
	}
	else
	{
		*xGrid = -1;
		*yGrid = -1;
	}
}

// Checking if someone wins.
int CheckWin(int rgGrd[3][3])
{
	int iGrid;
	int iGridEx;
	BOOL fEmpty = FALSE;

	// Check vertical
	for(iGrid = 0; iGrid < GRID_WIDTH; iGrid ++)
	{
		if( (rgGrd[iGrid][0] == rgGrd[iGrid][1]) && (rgGrd[iGrid][1] == rgGrd[iGrid][2]))
			return rgGrd[iGrid][0];
	}
	
	// Check horzontal
	for(iGrid = 0; iGrid < GRID_HEIGHT; iGrid ++)
	{
		if((rgGrd[0][iGrid] == rgGrd[1][iGrid]) && (rgGrd[1][iGrid] == rgGrd[2][iGrid]))
			return rgGrd[0][iGrid];
	}

	// Check diagonal
	if((rgGrd[0][0] == rgGrd[1][1]) && (rgGrd[1][1] == rgGrd[2][2]))
		return rgGrd[0][0];

	if((rgGrd[2][0] == rgGrd[1][1]) && (rgGrd[1][1] == rgGrd[0][2]))
		return rgGrd[2][0];

	// Check if everything is filled
	for(iGrid = 0; iGrid < GRID_DIAGONAL; iGrid ++)
	{	
		for(iGridEx = 0; iGridEx < 3; iGridEx ++)
		{
			if(rgGrd[iGrid][iGridEx] == EMPTY)
				fEmpty = TRUE;
		}
	}

	if(fEmpty == FALSE)
		return TIE;

	return IN_PROGRESS;
}

// Paint the grid and the x and the o
void PaintGrid(HDC hdc, int rgGrd[3][3])
{
	int iGridX, iGridY;
	RECT rc;
	HDC hdcMem;

	rc.left		= 0;
	rc.top		= 0;
	rc.right	= scWidth;
	rc.bottom	= scHeight;

	hdcMem = CreateCompatibleDC(hdc);

	FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOW+1));

	// Painting the grid
	MoveToEx(hdc, geOffset, 0, (LPPOINT) NULL);
	LineTo(hdc, geOffset, geHeight);

	MoveToEx(hdc, geOffset * 2, 0, (LPPOINT) NULL);
	LineTo(hdc, geOffset * 2, geHeight);

	MoveToEx(hdc, 0, geOffset, (LPPOINT) NULL);
	LineTo(hdc, geWidth, geOffset);

	MoveToEx(hdc, 0, geOffset * 2, (LPPOINT) NULL);
	LineTo(hdc, geWidth, geOffset * 2);

	// Painting the x and the o
	for(iGridX = 0; iGridX < 3; iGridX ++)
	{
		for(iGridY = 0; iGridY < 3; iGridY ++)
		{
			if(rgGrd[iGridX][iGridY] == PLAYER_OWNED)
			{
				SelectObject(hdcMem, hbmCross);
				BitBlt(hdc, 3 + (geOffset * iGridX), 3 + (geOffset * iGridY), 
					bmCross.bmWidth, bmCross.bmHeight, hdcMem, 0, 0, SRCCOPY);
			}
			if(rgGrd[iGridX][iGridY] == COMPUTER_OWNED)
			{
				SelectObject(hdcMem, hbmCircle);
				BitBlt(hdc, 3 + (geOffset * iGridX), 3 + (geOffset * iGridY), 
					bmCircle.bmWidth, bmCircle.bmHeight, hdcMem, 0, 0, SRCCOPY);
			}
		}
	}
	DeleteDC(hdcMem);
}	

BOOL CALLBACK SettingsDlgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	void (*aiOriginal)(int *, int *, int rgGrid[3][3]);

	aiOriginal = &(*aiPtr);

	switch(Msg)
	{
	case WM_INITDIALOG:
		SendDlgItemMessage(hWnd,
			fAiLevel,
			BM_SETCHECK,
			(int) TRUE,
			NULL);
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case ID_RANDOM:
			aiPtr = &aiRandom;
			fAiLevel = ID_RANDOM;
			break;

		case ID_SIMPLE:
			aiPtr = &aiSimple;
			fAiLevel = ID_SIMPLE;
			break;

		case ID_APPLY:
			return EndDialog(hWnd, ID_OK);

		case ID_CANCEL:
			aiPtr = &(*aiOriginal);
			return EndDialog(hWnd, ID_CANCEL);

		default:
			return FALSE;
		}
		return TRUE;
		
	default:
		return FALSE;
	}
	return TRUE;
}

BOOL CALLBACK AboutDlgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		return EndDialog(hWnd, ID_OK);
		
	default:
		return FALSE;
	}
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	int iGridX, iGridY;
	HDC hdcWindow;
	HWND hStatus; 

	switch(Msg)
	{
	// Create new game when program is initiated
	case WM_CREATE:
	{
		int StatWidth[] = {100, -1};
		// Allocate and create bitmaps		
		hbmCross	= LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_CROSS));
		hbmCircle	= LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_CIRCLE));
		
		if(!hbmCross || !hbmCircle)
		{
			ErrorCode = GetLastError( );
			wsprintf(ErrorMessage, "Cannot Load Resource. (ERROR %d)", ErrorCode);
			MessageBox(hWnd, ErrorMessage, "Error!", 
				MB_ICONEXCLAMATION | MB_OK);
			return -1;
		}

		GetObject(hbmCross, sizeof(bmCross), &bmCross);
		GetObject(hbmCircle, sizeof(bmCircle), &bmCircle);

		// Initialize ai style
		aiPtr = &aiRandom;

		// Initialize game status
		fGameState = IN_PROGRESS;

		hStatus = CreateWindowEx(0, STATUSCLASSNAME, NULL,
			WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 0, 0, stWidth, stHeight, 
			hWnd, (HMENU)IDC_STATUS, GetModuleHandle(NULL), NULL);
		
		if(!hStatus)
		{
			ErrorCode = GetLastError( );
			wsprintf(ErrorMessage, "Cannot Create Status Bar. (ERROR %d)", ErrorCode);
			MessageBox(hWnd, ErrorMessage, "Error!", 
				MB_ICONEXCLAMATION | MB_OK);
		}

		SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)"PLAYER'S TURN");

		fAiLevel = ID_RANDOM;
		
		// Initialize an empty grid
		for(iGridX = 0; iGridX < GRID_WIDTH; iGridX ++)
		{
			for(iGridY = 0; iGridY < GRID_HEIGHT; iGridY ++)
				rgGrid[iGridX][iGridY] = EMPTY;
		}

	}	
		break;
		
	// Menu response
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDM_EXIT:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;

		// New game
		case IDM_NEW:
			fGameState = IN_PROGRESS;
		
			for(iGridX = 0; iGridX < GRID_WIDTH; iGridX ++)
			{
				for(iGridY = 0; iGridY < GRID_HEIGHT; iGridY ++)
					rgGrid[iGridX][iGridY] = EMPTY;
			}

			hdcWindow = GetDC(hWnd);
			PaintGrid(hdcWindow, rgGrid);

			// If the computer starts first
			hStatus = GetDlgItem(hWnd, IDC_STATUS);

			if(fComputerStart)
			{
				SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)"COMPUTER'S TURN");

				// Use random as AI
				(*aiPtr)(&iGridX, &iGridY, rgGrid);
				rgGrid[iGridX][iGridY] = COMPUTER_OWNED;

				// Update grid
				PaintGrid(hdcWindow, rgGrid);

				fComputerStart = FALSE;
			}
			else
				fComputerStart = TRUE;

			SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)"PLAYER'S TURN");
			ReleaseDC(hWnd, hdcWindow);
			break;

		case IDD_SETTINGS:
		{
			int ErrorCheck = DialogBox(GetModuleHandle(NULL),
				MAKEINTRESOURCE(IDD_SETTINGS), hWnd, SettingsDlgProc);
			if(ErrorCheck == -1)
			{
				ErrorCode = GetLastError( );
				wsprintf(ErrorMessage, "Dialog Box Error. (ERROR %d)", ErrorCode);
				MessageBox(hWnd, ErrorMessage, "Error!", MB_ICONEXCLAMATION | MB_OK);
			}

			fGameState = IN_PROGRESS;
		
			for(iGridX = 0; iGridX < GRID_WIDTH; iGridX ++)
			{
				for(iGridY = 0; iGridY < GRID_HEIGHT; iGridY ++)
					rgGrid[iGridX][iGridY] = EMPTY;
			}

			hdcWindow = GetDC(hWnd);
			PaintGrid(hdcWindow, rgGrid);
			SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)"PLAYER'S TURN");
			ReleaseDC(hWnd, hdcWindow);
			break;
		}


		case IDD_ABOUT:
		{
			int ErrorCheck = DialogBox(GetModuleHandle(NULL), 
				MAKEINTRESOURCE(IDD_ABOUT), hWnd, AboutDlgProc);
			if(ErrorCheck == -1)
			{
				ErrorCode = GetLastError( );
				wsprintf(ErrorMessage, "Dialog Box Error. (ERROR %d)", ErrorCode);
				MessageBox(hWnd, ErrorMessage, "Error!", MB_ICONEXCLAMATION | MB_OK);
			}
		}
			break;
		}
		break;

	case WM_LBUTTONDOWN:
		if(fGameState == IN_PROGRESS)
		{
		int xMouse = LOWORD(lParam);
		int yMouse = HIWORD(lParam);

		// Determine where the mouse click is
		if(xMouse < (geWidth / 3))
			iGridX = 0;

		else if(xMouse < ((geWidth / 3) * 2))
			iGridX = 1;

		else
			iGridX = 2;

		if(yMouse < (geHeight / 3))
			iGridY = 0;

		else if(yMouse < (geHeight / 3) * 2)
			iGridY = 1;

		else
			iGridY = 2;

		hStatus = GetDlgItem(hWnd, IDC_STATUS);

		// Load mouse click location to array and display
		if(rgGrid[iGridX][iGridY] == EMPTY)
			rgGrid[iGridX][iGridY] = PLAYER_OWNED;
		else
		{
			SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)"SLOT OCCUPIED");
			break;
		}

		// Update grid
		hdcWindow = GetDC(hWnd);
		PaintGrid(hdcWindow, rgGrid);

		// Check if either side won
		fGameState = CheckWin(rgGrid);
		if(fGameState == PLAYER_WON)
		{
			SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)"YOU WIN");
			break;
		}
		if(fGameState == TIE)
		{
			SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)"IT'S A TIE");
			break;
		}

		SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)"COMPUTER'S TURN");

		// Use random as AI
		(*aiPtr)(&iGridX, &iGridY, rgGrid);
		if(iGridX < 0)
			break;
		rgGrid[iGridX][iGridY] = COMPUTER_OWNED;

		// Update grid
		PaintGrid(hdcWindow, rgGrid);

		// Check if either side won
		fGameState = CheckWin(rgGrid);
		if(fGameState == PLAYER_LOST)
		{
			SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)"YOU LOST");
			break;
		}
		if(fGameState == TIE)
		{
			SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)"IT'S A TIE");
			break;
		}
		SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)"PLAYER'S TURN");

		ReleaseDC(hWnd, hdcWindow);
		}
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdcWindow;

		hdcWindow = BeginPaint(hWnd, &ps);
		PaintGrid(hdcWindow, rgGrid);
		EndPaint(hWnd, &ps);
	}
		break;

	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		DeleteObject(hbmCross);
		DeleteObject(hbmCircle);

		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
			LPSTR lpCmdLine, int nCmdShow)
{
	MSG Msg;
	WNDCLASSEX wc;
	HWND hWnd;

	InitCommonControls( );

	wc.lpszClassName	= szClassName;
	wc.hInstance		= hInstance;
	wc.style			= 0;
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.lpfnWndProc		= WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName		= MAKEINTRESOURCE(IDR_MENU);
	wc.hIcon			= LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON));
	wc.hIconSm			= LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICONSM));

	if(!RegisterClassEx(&wc))
	{
		ErrorCode = GetLastError( );
		wsprintf(ErrorMessage, "Window Registration Failed. (ERROR %d)", ErrorCode);
		MessageBox(NULL, (LPCSTR) ErrorMessage, "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}

	hWnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		szClassName,
		"Tick-Tack Toe",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, scWidth, scHeight,
		NULL, NULL, hInstance, NULL);

	if(hWnd == NULL)
	{
		ErrorCode = GetLastError( );
		wsprintf(ErrorMessage, "Window Creation Failed. (ERROR %d)", ErrorCode);
		MessageBox(NULL, ErrorMessage, "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return Msg.wParam;
}