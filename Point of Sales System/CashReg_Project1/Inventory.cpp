// Cash Reg program
// File: Inventory.cpp
// Authors: Bilal Adra, Eric Zuidema, Timothy Byukusenge     Date: 4/18/2014 edited 4/20/2014

// This program provides all the control functions for an Inventory

#include "Item.h"
#include "Node.h"
#include <iostream>
#include "Inventory.h"
#include "Tape.h"
#include "CScreen.h"
#include <windows.h>
#include <string>

using namespace std;

Inventory::Inventory() //Constructor
{
	// Populate inventory items with hard-coded values

	items[0].description = "Watch";
	items[0].price = 40.00;
	items[0].QOH = 10;
	items[0].UPC = "00000";

	items[1].description = "Ring";
	items[1].price = 100.00;
	items[1].QOH = 10;
	items[1].UPC = "11111";

	items[2].description = "Toothbrush";
	items[2].price = 5.50;
	items[2].QOH = 10;
	items[2].UPC = "22222";

	items[3].description = "Milk";
	items[3].price = 2.75;
	items[3].QOH = 10;
	items[3].UPC = "33333";

	items[4].description = "Soda";
	items[4].price = 0.75;
	items[4].QOH = 10;
	items[4].UPC = "44444";

	items[5].description = "Sandwich";
	items[5].price = 3.45;
	items[5].QOH = 10;
	items[5].UPC = "55555";

	items[6].description = "Gum";
	items[6].price = 0.25;
	items[6].QOH = 10;
	items[6].UPC = "66666";

	items[7].description = "Pen";
	items[7].price = 1.00;
	items[7].QOH = 10;
	items[7].UPC = "77777";

	items[8].description = "Lotto";
	items[8].price = 2.75;
	items[8].QOH = 10;
	items[8].UPC = "88888";

	items[9].description = "Magazine";
	items[9].price = 10.00;
	items[9].QOH = 10;
	items[9].UPC = "99999";
}

// Updates the stock of an item in the inventory
void Inventory::adjustStock(string UPC, int newQty)
{
	// Search for the item in the inventory
	int index = findIndex(UPC);

	// Item was found in the inventory
	if(index > -1)
	{
		items[index].QOH = newQty;
	}
	else
	{
		//cout << "Error: item not found in inventory";
	}
}

// Updates the description of an item in the inventory
void Inventory::changeDescription(string UPC, string newDescription)
{
	// Search for the item in the inventory
	int index = findIndex(UPC);

	// Item was found in the inventory
	if(index > -1)
	{
		items[index].description = newDescription;
	}
	else
	{
		//cout << "Error: item not found in inventory";
	}
}

// Updates the price of an item in the inventory
void Inventory::changePrice(string UPC, float newPrice)
{
	// Search for the item in the inventory
	int index = findIndex(UPC);

	// Item was found in the inventory
	if(index > -1)
	{
		items[index].price = newPrice;
	}
	else
	{
		//cout << "Error: item not found in inventory";
	}
}

// Returns the index of the item in the inventory
int Inventory::findIndex(string UPC)
{
	int index = -1;
	for(int i = 0; i < sizeof(items)/sizeof(Item); i++)
	{
		if(items[i].UPC == UPC)
		{
			index = i; // If found return it's index
			break;
		}
	}

	return index;
}

// This method may be able to replace the one above -- let's look at this
Item * Inventory::findItem(string UPC)
{
	for(int i = 0; i < sizeof(items)/sizeof(Item); i++)
	{
		if(items[i].UPC == UPC)
		{
			return &items[i];//item found
		}
	}
	return NULL; //item wasnt found
}