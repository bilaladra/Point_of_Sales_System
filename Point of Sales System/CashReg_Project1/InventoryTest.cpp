// Cash Reg program
// File: InventoryTest.cpp
// Authors: Bilal Adra, Eric Zuidema, Timothy Byukusenge     Date: 4/18/2014 edited 4/20/2014

// This class tests the Node class

#include <Windows.h>
#include <conio.h>
#include "Tape.h"
#include "CScreen.h"
#include <iostream>
#include "Inventory.h"

using namespace std;
/*
void main() // using main() to test the inventory object
{
Inventory inventory; // create an inventory

// Assert that the items were created
if(sizeof(inventory.items) > 0)
{
cout << "PASSED: Items were created\n";
}

// Assert that the items were populated using random sampling
if(inventory.items[0].description == "Watch" && inventory.items[1].price == 100 && inventory.items[4].UPC == "44444")
{
cout << "PASSED: Items were populated\n";
}

// Assert that an item's description can be changed
string description = "new description";
inventory.changeDescription("11111", description);

if(inventory.items[1].description == description)
{
cout << "PASSED: The description was updated\n";
}

// Assert that an item's price can be changed
float price = 5.75;
inventory.changePrice("11111", price);

if(inventory.items[1].price == price)
{
cout << "PASSED: The price was updated\n";
}

// Assert that an item's quantity can be changed
int quantity = 12;
inventory.adjustStock("11111", quantity);

if(inventory.items[1].QOH == quantity)
{
cout << "PASSED: The quantity was updated\n";
}

// Assert that an item can be looked up
Item i = *inventory.findItem("00000");
if(i.description == "Watch")
{
cout << "PASSED: an item can be referenced\n";
}

_getch();
}*/