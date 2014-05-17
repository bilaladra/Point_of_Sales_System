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
	f = new RFile("inventory.dat", sizeof(Item)); // Opens the inventory file

	if(f->stat() != 0)
	{
		fstream tempFile("inventory.dat", ios::out);//create a file because RFile wont for some reason
		f->Fopen("inventory.dat", sizeof(Item));
	}

	buildStack(); // Build the deleted records stack
}

Inventory::~Inventory()
{
	f->Fclose();
	delete f;
}

// Adds an item to the inventory
bool Inventory::addItem(Item item)
{
	bool rtn = false;

	if(!deleted.empty()) // Check if stack is empty
	{
		int replace = deleted.top(); // Set to item at top of stack
		if(f->update(replace, &item)); // Swap deleted value with new item
		{
			deleted.pop(); // Remove item from stack
			rtn = true;
		}
	}
	else
	{
		if(f->write(&item)); // write the record at the end
		{  rtn = true;  }
	}

	numRecords++;
	return rtn;
}

bool Inventory::removeItem(string UPC)
{
	bool rtn = false;
	int fileSize = f->count(); // get number of allocated records in the file
	Item temp; // temp store for item being read

	// iterate through the records
	for(int i = 0; i < fileSize; i++)
	{
		if(f->read(i, &temp) && !((string)temp.UPC).compare(UPC) && !temp.isDeleted) // item with correct UPC found
		{
			temp.isDeleted = true; // set deleted flag on the item - *I added this to the struct
			f->update(i, &temp); // overwrite the current entry (isDeleted = false)
			deleted.push(i);  //add recordnum to stack of deleted records
			numRecords--;
			rtn = true;
			break; // record found -> break out of loop
		}
	}
	return rtn;
}

// Updates the stock of an item in the inventory
bool Inventory::adjustStock(string UPC, int newQty)
{
	bool rtn = false;
	int fileSize = f->count(); // get number of allocated records in the file
	Item temp; // temp store for item being read

	// iterate through the records
	for(int i = 0; i < fileSize; i++)
	{
		if(f->read(i, &temp) && !((string)temp.UPC).compare(UPC) && !temp.isDeleted) // item with correct UPC found
		{
			temp.QOH += newQty; // Update the quantity
			f->update(i, &temp);
			rtn = true;
			break; // record found -> break out of loop
		}
	}
	return rtn;
}

// Updates the description of an item in the inventory
bool Inventory::changeDescription(string UPC, string newDescription)
{
	bool rtn = false;
	int fileSize = f->count(); // get number of allocated records in the file
	Item temp; // temp store for item being read

	// iterate through the records
	for(int i = 0; i < fileSize; i++)
	{
		if(f->read(i, &temp) && !((string)temp.UPC).compare(UPC) && !temp.isDeleted) // item with correct UPC found  //UPC comparison not working
		{
			strcpy(temp.description, newDescription.c_str()); // Update the description
			f->update(i, &temp);
			rtn = true;
			break; // record found -> break out of loop
		}
	}
	return rtn;
}

// Updates the price of an item in the inventory
bool Inventory::changePrice(string UPC, float newPrice)
{
	bool rtn = false;
	int fileSize = f->count(); // get number of allocated records in the file
	Item temp; // temp store for item being read

	// iterate through the records
	for(int i = 0; i < fileSize; i++)
	{
		if(f->read(i, &temp) && !((string)temp.UPC).compare(UPC) && !temp.isDeleted) // item with correct UPC found
		{
			temp.price = newPrice; // Update the price
			f->update(i, &temp);
			rtn = true;
			break; // record found -> break out of loop
		}
	}
	return rtn;
}

// Find an item in the inventory
bool Inventory::findItem(string UPC, Item* pFoundItem )
{
	int fileSize = f->count(); // get number of allocated records in the file
	// iterate through the records
	for(int i = 0; i < fileSize; i++)
	{
		if( f->read(i, pFoundItem) && !strcmp((char *)pFoundItem->UPC, (char *)UPC.c_str()) && !pFoundItem->isDeleted) // item with correct UPC found
		{
			return true; // record found -> break out of loop
		}
	}
	return false; //return null
}

bool Inventory::findItem(int rec, Item* pFoundItem)
{
	if(f->read(rec, pFoundItem))
	{
		return true;
	}
	return false;
}

// Builds the stack containing references to deleted records
// Also initializes the numRecords property
void Inventory::buildStack()
{
	int fileSize = f->count(); // Get number of allocated records in the file
	numRecords = fileSize; // Set the number of records
	Item temp; // Temp store for item being read

	// Iterate through the records
	for(int i = 0; i < fileSize; i++)
	{
		if(f->read(i, &temp) && temp.isDeleted) // Deleted item found
		{
			deleted.push(i); // Push the record number onto stack
			numRecords--; // Adjust the record account for deleted records
		}
	}
}