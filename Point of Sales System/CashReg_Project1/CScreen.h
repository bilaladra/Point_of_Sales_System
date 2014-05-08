// Screen Control Program
// File:   Screen.h
// Author: S. Renk       Date: 4/2001 edited 4/2011

// This program provides interface control for console applications. I provides: screen addressing, custom I/O, screen delay, clear screen, colored text and backgroungs, etc.

#ifndef _Screenio
#define _Screenio

#include <windows.h>

enum {BLACK, DBLUE, GREEN, MBLUE, DRED, PURPLE, OLIVE, LTBLUE, GRAY,
	BLUE, LTGREEN, AQUA, RED, FUSCIA, GOLD, WHITE};

class CScreen
{ // create a DOD window and provide user support for screen I/O
private:
	HANDLE hConsole;                       // ID of DOS window
	CONSOLE_SCREEN_BUFFER_INFO conInfo;    // hold state info
	CONSOLE_CURSOR_INFO curInfo;           // holds cursor data

	// state info
	int iForColor, iBakColor;              // foreground/background
	bool bCursor;                          // true = show cursor
	COORD cursorPos;                       // not updated

	bool InStr(char s, char t[]);          // true if s is in t

public:
	CScreen(int iSizeX=80, int iSizeY=24); // create a DOS window
	~CScreen();                            // close DOS window
	void SetColor(int iFColor, int iBColor = 0); // set text colors
	void GetColor(int &iFColor, int &iBColor); // return settings
	void SetFgColor(int iFColor);          // set foregorund color
	void SetBkColor(int iBColor);          // set background color
	void ClearScreen();                    // erase the screen
	void ClearScreenPos(int x, int y);     // erase single char
	void DrawString(char *szStr, int iX=-1, int iY=-1,  int BClr=-1, int FClr=-1); // write @ (x,y)
	void DrawString(int iX, int iY, char *szStr,  int BClr=-1, int FClr=-1);
	void DrawString(double d, int iX=-1, int iY=-1);
	void Delay(int ix=0, int iy=24, char szPrompt[]="Press <CR> to continue");                          // wait fo <CR>
	void GotoXY(int iX, int iY);           // move cursor to (x,y)
	void CursorOn(bool bCursor);           // turn cursor on/off
	void GetStr(int iX, int iY, char szAns[], int iLen, char szFmt[]="", bool bEcho=true);
	float GetVal(float & val, int iX=-1, int iY=-1);// get a number
	float CScreen::GetVal(int iX, int iY);
	void Box(int iULx, int iULy, int iLRx, int iLRy, int iBkColor);
};

#endif