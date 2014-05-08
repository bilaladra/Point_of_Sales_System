// Cash Reg program
// File: Tape.h
// Authors: Bilal Adra and Stephen C. Renk      Date: 4/15/2014 edited 4/17/2014

// This class defines a receipt that comes out of a cash register, implemented as a Linked List,

#ifndef _Tape
#define _Tape

#include "Node.h"
#include "Item.h"
#include "CScreen.h"
#include <string>
#include <windows.h>

using namespace std;

class Tape
{
	Node * nHead, * nTail, *nPtr; // linked list pointers
	int itemCount;                       // # items in the list
	int posX, posY;         // (x, y) position of upper left corner of the Tape display
	int displaySize;         // number of Items displayed
	CScreen S;

	// ******* Helper methods ********
	Node * find(string upc); // find node nbr in list and return a pointer to it

public:
	Tape(int xPos, int yPos, int screenSize, CScreen &screen); // constructor - create an empty list
	Tape();
	~Tape(); // destroy the list
	bool addItem(Item * item); // add item to the head of the list
	bool addItem(string upc, string description, float price);
	bool voidItem(string upc); // remove item # nbr from the list
	void displayItems(); // display current Item & the previous displaySize-1 items
	// this will give the illusion of sccrolling - call after every addItem()
	void resetTape();
};

#endif;