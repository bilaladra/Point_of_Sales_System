// Cash Reg program
// File: Inventory.h
// Authors: Bilal Adra, Eric Zuidema, Timothy Byukusenge     Date: 4/15/2014 edited 4/20/2014

// This structure defines an Inventory for a company

#ifndef _Inventory
#define _Inventory

#include <windows.h>
#include <string>
#include "Item.h"
#include "RFile.h"
#include <stack>

using namespace std;

class Inventory
{
private:
	void buildStack(); // Builds stack of deleted records

public:
	Inventory(); //Constructor: Instantiate the inventory and add items
	~Inventory();
	RFile *f; // The random access file controller
	bool addItem(Item item); //Add an item to the inventory -- REMOVED BECAUSE FIXED ARRAY
	bool removeItem(string UPC); //Remove an item from the inventory -- REMOVED BECAUSE FIXED ARRAY
	bool adjustStock(string UPC, int newQty); //Update the stock quantity of an inventory item
	bool changeDescription(string UPC, string newDescription); //Update the description of an inventory item
	bool changePrice(string UPC, float newPrice); //Update the price of an inventory item
	bool findItem(string UPC, Item* pFoundItem); //Retrieve item from the inventory reference
	bool findItem(int rec, Item* pFoundItem); //Retrieve item from the inventory reference
	stack <int> deleted; // Stack of deleted record references
	int numRecords; // Reflects the true number of items (*RFile count() used for iteration)
};

#endif;