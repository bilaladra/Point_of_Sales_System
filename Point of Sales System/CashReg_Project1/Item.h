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
	char description[16];
	float price;
	char UPC[5];
	int QOH; //quantity on hand
	bool isDeleted;
};

#endif;