// Cash Reg program
// File: Inventory.h
// Authors: Bilal Adra, Eric Zuidema, Timothy Byukusenge     Date: 4/15/2014 edited 4/20/2014

// This structure defines an Inventory for a company

#ifndef _Inventory
#define _Inventory

#include <windows.h>
#include <string>
#include "Item.h"

using namespace std;

class Inventory
{
private:
	int findIndex(string UPC); //Internal item lookup -- may not be neccessary

public:
	Item items[10]; //Inventory will be a 10 element array for now
	Inventory(); //Constructor: Instantiate the inventory and add items
	//void addItem(Item item); //Add an item to the inventory -- REMOVED BECAUSE FIXED ARRAY
	//void removeItem(string UPC); //Remove an item from the inventory -- REMOVED BECAUSE FIXED ARRAY
	void adjustStock(string UPC, int newQty); //Update the stock quantity of an inventory item
	void changeDescription(string UPC, string newDescription); //Update the description of an inventory item
	void changePrice(string UPC, float newPrice); //Update the price of an inventory item
	Item * findItem(string UPC); //Retrieve item from the inventory reference
};

#endif;