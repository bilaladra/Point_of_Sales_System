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
	f.FOpen("inventory.dat", sizeof(Item)); // Opens the inventory file
	buildStack(); // Build the deleted records stack
}


// Adds an item to the inventory
void Inventory::addItem(Item item)
{
	if(!deleted.empty()) // Check if stack is empty
	{
		int replace = deleted.pop();
		f.update(replace, &item); // Swap deleted value with new item
	}
	else
	{
		f.write(&item); // write the record at the end
	}
}

void Inventory::removeItem(string UPC)
{
	fileSize = f.count(); // get number of allocated records in the file
	Item temp; // temp store for item being read

	// iterate through the records
	for(int i = 0; i < fileSize; i++)
	{
		if(f.read(i, &temp) && !temp.UPC.compare(UPC)) // item with correct UPC found
		{
			temp.isDeleted = True; // set deleted flag on the item - *I added this to the struct
			f.update(i, &temp); // overwrite the current entry (isDeleted = false)
			break; // record found -> break out of loop
		}
	}
}

// Updates the stock of an item in the inventory
void Inventory::adjustStock(string UPC, int newQty)
{
	fileSize = f.count(); // get number of allocated records in the file
	Item temp; // temp store for item being read

	// iterate through the records
	for(int i = 0; i < fileSize; i++)
	{
		if(f.read(i, &temp) && !temp.UPC.compare(UPC)) // item with correct UPC found
		{
			temp.QOH = newQty; // Update the quantity
			f.update(i, &temp);
			break; // record found -> break out of loop
		}
	}
}

// Updates the description of an item in the inventory
void Inventory::changeDescription(string UPC, string newDescription)
{
	fileSize = f.count(); // get number of allocated records in the file
	Item temp; // temp store for item being read

	// iterate through the records
	for(int i = 0; i < fileSize; i++)
	{
		if(f.read(i, &temp) && !temp.UPC.compare(UPC)) // item with correct UPC found
		{
			temp.description = newDescription; // Update the description
			f.update(i, &temp);
			break; // record found -> break out of loop
		}
	}
}

// Updates the price of an item in the inventory
void Inventory::changePrice(string UPC, float newPrice)
{
	fileSize = f.count(); // get number of allocated records in the file
	Item temp; // temp store for item being read

	// iterate through the records
	for(int i = 0; i < fileSize; i++)
	{
		if(f.read(i, &temp) && !temp.UPC.compare(UPC)) // item with correct UPC found
		{
			temp.price = newPrice; // Update the price
			f.update(i, &temp);
			break; // record found -> break out of loop
		}
	}
}

// Find an item in the inventory (I didn't modify this)
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

// Builds the stack containing references to deleted records
void Inventory::buildStack()
{
	fileSize = f.count(); // Get number of allocated records in the file
	Item temp; // Temp store for item being read

	// Iterate through the records
	for(int i = 0; i < fileSize; i++)
	{
		if(f.read(i, &temp) && temp.isDeleted) // Deleted item found
		{
			deleted.push(i); //Push the record number onto stack			
		}
	}
}