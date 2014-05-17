// Cash Reg program
// File: Tape.cpp
// Authors: Bilal Adra & Stephen C Renk      Date: 4/15/2014 edited 4/18/2014

// This program provides all the control functions for a Tape that acts like a reciept and is implemented as a Linked List

#include "Item.h"
#include "Node.h"
#include "Tape.h"
#include "CScreen.h"
#include <windows.h>
#include <string>

using namespace std;

Tape::Tape(int xPos, int yPos, int screenSize, CScreen &screen)   // constructor - create an empty list
{
	posX = xPos;
	posY = yPos;
	displaySize = screenSize;
	S = screen;

	nHead = nTail = nPtr = NULL;
	itemCount = 0;
}

Tape::Tape()
{
	posX = posY = 0;
	displaySize = 10;
	nHead = nTail = nPtr = NULL;
	itemCount = 0;
}

Tape::~Tape()   // destructor
{
	Node *temp = nHead;
	nPtr = nHead;

	while(nPtr)
	{
		delete temp;
		temp = nPtr->nextPtr;
	}

	delete nPtr, nHead, nTail;
}

bool Tape::addItem(Item * item)
{
	bool rtn = false;  //really should predicate return value on the allocation working

	nPtr = new Node();   // create a node

	nPtr->itemPtr = item;   // create & link a dynamic Item
	itemCount++;

	// hook node on head of list
	if (!nHead)    // empty list
	{
		nHead = nTail = nPtr;
		rtn = true;
	}
	else
	{
		nHead->prevPtr = nPtr;
		nPtr->nextPtr = nHead;
		nHead = nPtr;
		rtn = true;
	}

	return rtn;
}

bool Tape::addItem(string upc, string description, float price) // add item to the tail of the list
{
	bool rtn = false;  //really should predicate return value on the allocation working

	nPtr = new Node();                                      // create a node

	if(nPtr != NULL)
	{
		//create an item and set its properties
		Item *itm = new Item;
		strcpy(itm->description, description.c_str());
		itm->price = price;
		strcpy(itm->UPC, upc.c_str());

		nPtr->itemPtr = itm;   // create & link a dynamic Item
		itemCount++;

		// hook node on head of list
		if (!nHead)    // empty list
		{
			nHead = nTail = nPtr;
			rtn = true;
		}
		else
		{
			nHead->prevPtr = nPtr;
			nPtr->nextPtr = nHead;
			nHead = nPtr;
			rtn = true;
		}
	}
	return rtn;
}

bool Tape::voidItem( string upc)  // remove item # nbr from the list
{
	bool rtn = false;
	Node *temp;

	temp = nPtr = find(upc); //finds Item and makes nPtr point to it
	if(nPtr)
	{
		if(nPtr == nHead) //if its the head set the new head
		{
			nHead = nPtr->nextPtr;
			nHead->prevPtr = NULL;

			rtn = true;
		}
		else if(nPtr == nTail)
		{
			nTail = nPtr->prevPtr;
			nTail->nextPtr = NULL;

			rtn = true;
		}
		else
		{
			//links previous Item to next Item
			nPtr->nextPtr->prevPtr = nPtr->prevPtr;
			nPtr->prevPtr->nextPtr = nPtr->nextPtr;

			rtn = true;
		}
	}
 
	delete temp;
	return rtn;
}

void Tape::displayItems()  // display the displaySize newest items
{
	S.Box(posX, posY, posX+25, posY+displaySize+3,OLIVE); // draw display area
	int  count = 0;
	nPtr = nHead;

	S.GotoXY(posX + 3, posY);
	S.DrawString((char *)"Items Purchased");

	while(nPtr && count++ < displaySize )
	{
		//sets string position
		S.GotoXY(posX+1,posY+count);

		//draws the string
		float price = nPtr->itemPtr->price;
		string tempDescr;
		strcpy(nPtr->itemPtr->description, tempDescr.c_str());
		S.DrawString((char *)(tempDescr + "-----" + to_string(price)).c_str());
		nPtr = nPtr->nextPtr;
	}
}

Node * Tape::find( string upc)  // Private method: find node UPC in list and return a pointer to it
{
	string temp;
	nPtr = nHead;
	while (nPtr)                   // look at each Item until found
	{
		strcpy(nPtr->itemPtr->UPC, temp.c_str());
		if (temp.compare(upc) == 0)   // if strings are equal
			return nPtr;           // found the node, return a pointer to it in list
		else
			nPtr = nPtr->nextPtr;  // move to next Item
	}
	return NULL;                   // not found
}

void Tape::resetTape()
{
	Node *temp = nHead;
	nPtr = nHead;

	while(nPtr)
	{
		delete temp;
		temp = nPtr->nextPtr;
	}

	nHead = nTail = nPtr = NULL;
	itemCount = 0;
}