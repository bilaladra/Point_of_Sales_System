// Cash Reg program
// File: Node.h
// Authors: Bilal Adra & Stephen C. Renk     Date: 4/15/2014 edited 4/17/2014

// This structure defines an Node for the Tape

#ifndef _Node
#define _Node

#include "Item.h"
#include <windows.h>

using namespace std;

class Node // local class - data element for linked list
{               // Node is used in a doubly linked list (forward & backward)
public:
	Item * itemPtr; // pointer to an Item
	Node * nextPtr; // pointer to the next Node
	Node * prevPtr; // pointer to previous Node

	Node() // Constructor: create unconnecting node
	{ itemPtr = NULL; nextPtr = prevPtr = NULL;}
};

#endif;