// Screen Control Program
// File: Screen.cpp
// Author: S. Renk       Date: 4/2001 edited 4/2011

// This program provides interface control for console applications. I provides: screen addressing, custom I/O, screen delay, clear screen, colored text and backgroungs, etc.

#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>

#include "CScreen.h"

CScreen::CScreen(int iSizeX, int iSizeY)  // init a DOS window
{
	COORD consoleSize = {iSizeY, iSizeX};     // size of window to open
	iForColor = WHITE;                        // default white
	iBakColor = BLACK;                        // default black

	srand((unsigned) time(NULL));             // seed the random # generator

	hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
	/* Replaced this with above
	CreateFile((LPCWSTR)"CONOUT$", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ
	| FILE_SHARE_WRITE, 0L,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0L); */

	SetConsoleScreenBufferSize(hConsole, consoleSize);

	GetConsoleScreenBufferInfo(hConsole, &conInfo);
	curInfo.bVisible = bCursor;  // Hide the cursor.
	SetConsoleCursorInfo(hConsole, &curInfo);
}

CScreen::~CScreen()
{  CloseHandle(hConsole);
}

//*****************************************************************************
void CScreen::ClearScreen()    // erase the screen to background color
{
	SetColor(iForColor, iBakColor);
	system("cls");
}

//*****************************************************************************
void CScreen::DrawString(char *szStr, int iX, int iY, int iBClr, int iFClr)
{                                     // print text at (x,y)
	if (iX != -1 && iY != -1)
		GotoXY(iX, iY);
	if (iBClr == -1 && iFClr == -1)
		printf("%s", szStr);
	else
	{
		int iFgColor=iForColor,iBkColor=iBakColor;
		SetColor(iBClr, iFClr);
		printf("%s", szStr);
		SetColor(iFgColor, iBkColor);
	}
}

void CScreen::DrawString(int iX, int iY, char *szStr,  int iBClr, int iFClr)
{
	if (iX != -1 && iY != -1)
		GotoXY(iX, iY);
	if (iBClr == -1 && iFClr == -1)
		printf("%s", szStr);
	else
	{
		int iFgColor=iForColor,iBkColor=iBakColor;
		SetColor(iBClr, iFClr);
		printf("%s", szStr);
		SetColor(iFgColor, iBkColor);
	}
}

//*****************************************************************************
void CScreen::DrawString(double d, int iX, int iY)
{                                     // print text at (x,y)
	if (iX != -1 && iY != -1)
		GotoXY(iX, iY);
	printf("%.2f", d);
}

//*****************************************************************************
void CScreen::SetColor(int fColor, int bColor)  // set fore/bacground colors
{
	iBakColor = bColor; iForColor = fColor;
	SetConsoleTextAttribute(hConsole, (WORD) ((iBakColor << 4) | iForColor));
}

void CScreen::GetColor(int &iFColor, int &iBColor) // return settings
{
	iBColor = iBakColor;
	iFColor = iForColor;
}

//*****************************************************************************
void CScreen::SetFgColor(int iFColor)          // set foreground color
{
	iForColor = iFColor;
	SetConsoleTextAttribute(hConsole, (WORD) ((iBakColor << 4) | iForColor));
}

//*****************************************************************************
void CScreen::SetBkColor(int iBColor)          // set background color
{
	iBakColor = iBColor;
	SetConsoleTextAttribute(hConsole, (WORD) ((iBakColor << 4) | iForColor));
}

//*****************************************************************************
void CScreen::ClearScreenPos(int iXPos, int iYPos)  // clear screen
{
	//SetColor(iForColor, iBakColor);
	DrawString(" ", iXPos, iYPos);
}

//*****************************************************************************
void CScreen::Delay(int ix, int iy, char szPrompt[])
{
	DrawString(szPrompt, ix,iy);  // prompt on status line
	while (_getch() != '\r');      // loop till <CR>
	DrawString(szPrompt, ix,iy,iBakColor, iBakColor);  // erase prompt
}

//*****************************************************************************
void CScreen::GotoXY(int iX, int iY)     // move cursor to (x,y)
{
	cursorPos.X = iX; cursorPos.Y = iY;
	SetConsoleCursorPosition(hConsole, cursorPos);
}

//*****************************************************************************
void CScreen::CursorOn(bool bCursor)     // turn cursor on/off
{
	curInfo.bVisible = bCursor;                   // set the cursor.
	SetConsoleCursorInfo(hConsole, &curInfo);
}

//*****************************************************************************
float CScreen::GetVal(int iX, int iY)
{   float val;
if (iX != -1 && iY != -1)
	GotoXY(iX, iY);
scanf_s("%f", &val);
return val;
}

//*****************************************************************************
void CScreen::GetStr(int iX, int iY, char szAns[], int iLen, char szFmt[], bool bEcho)
{   // gets a string of iLen chars specified by fmt
	// fmt = @ - alpha, # - numeric, ~ - alpha/numeric, | - any key
	char cLetter, *pTemp;
	int iCount = 0, iSize=iLen, iFmtSize=strlen(szFmt);
	bool bMem=false;

	if (iFmtSize!=iLen)             // build more/less fmt string
	{
		pTemp = new char[iLen+1];   // make fmt str to take anything
		strncpy(pTemp, szFmt, iLen);// copy original & add extra
		strncat(pTemp,"||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
			,(iLen-iFmtSize<0)?0:iLen-iFmtSize);
		pTemp[iLen]=0;              // terminate the fmt string
		szFmt=pTemp;                // use szFmt to traverse
		bMem=true;                  // flag to delete at bottom
	}

	GotoXY(iX,iY);
	for(int iX1=0; iX1<iSize; iX1++)printf("_");// space holders for answer
	while( iCount < iSize+1)                    // get string from KB
	{
		GotoXY(iX+iCount, iY); printf("%c",' ');// blank out for new char
		GotoXY(iX+iCount, iY);                  // backup
		if (szFmt[iCount]== '#')                // get a #
		{
			while(!InStr((cLetter=_getch()),"0123456789.")&&cLetter!='\b'
				&&cLetter!='\r')printf("\7");
			szAns[iCount] = cLetter;
		}
		else if (szFmt[iCount] == '@')          // get a letter
		{
			while(!InStr((cLetter=toupper(_getch())),"ABCDEFGHIJKLMNOPQRSTUVWXYZ ")
				&& cLetter!='\b'&&cLetter!='\r')printf("\7");
		}
		else if (szFmt[iCount] == '~')          // get a letter or #
		{
			while(!InStr((cLetter=toupper(_getch())),"0123456789.ABCDEFGHIJKLMNOPQRSTUVWXYZ ")
				&& cLetter!='\b'&&cLetter!='\r')printf("\7");
		}
		else if (szFmt[iCount] == '|')          // get any char
		{
			cLetter=_getch();
		}
		else if (szFmt[iCount] == 0)            // get a <CR>
		{
			while(((cLetter=toupper(_getch())) != '\r')
				&&cLetter!='\b')printf("\7");
		}
		else                                    // skip fmt letter
		{
			cLetter = szFmt[iCount];
		}

		if (cLetter == '\r')                    // if <CR> done
		{
			break;
		}
		if (cLetter == '\b')                    // if not backspace
		{  while(iCount >0 && !InStr(szFmt[--iCount],"#@")); // back up
		continue;
		}
		else
		{
			szAns[iCount++] = cLetter;           // put it in string
			if (bEcho)
				printf("%c", cLetter);
			else
				printf("%c",'*');
		}
	}
	szAns[iCount]=0;                            // terminate string
	if (bMem)
		delete [] pTemp;
	_flushall();
}

//******************************************************************************
bool CScreen::InStr(char s, char t[])
{  // returns true if s is in t
	bool bRtn=false;
	for(int iX=0; iX<(int)strlen(t); iX++)
	{
		if (t[iX] == s)
		{
			bRtn = true; break;
		}
	}
	return bRtn;
}

// *****************************************************************************
void CScreen::Box(int iULx, int iULy, int iLRx, int iLRy, int iBkColor)
{
	int iTempBk = iBakColor;   // save old color
	SetBkColor(iBkColor);      // change to new draw color
	for(int i=0; i<iLRy-iULy; i++)
		for(int c=0; c<iLRx-iULx; c++)
			DrawString(" ", iULx +c, iULy+i);
	SetBkColor(iTempBk);    // restore original Bk color
}