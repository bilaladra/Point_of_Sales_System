// Cash Reg program
// File: Item.h
// Authors: Bilal Adra, Eric Zuidema, Timothy Byukusenge      Date: 4/15/2014 edited 4/17/2014

// This structure defines an Item for a company

#ifndef _Item
#define _Item

#include <windows.h>
#include <string>

using namespace std;

struct Item
{
public:
	string description;
	float price;
	string UPC;
	int QOH;//quantity on hand
};

#endif;