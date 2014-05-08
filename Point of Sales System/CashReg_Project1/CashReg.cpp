// Cash Reg program
// File: CashReg.h
// Authors: Bilal Adra, Eric Zuidema, Timothy Byukusenge      Date: 4/15/2014 edited 4/17/2014

// This structure defines a Cash Register for a company

#include <windows.h>
#include <string>
#include "Inventory.h"
#include "Item.h"
#include "Tape.h"
#include "CashReg.h"

using namespace std;

const float taxRate = 0.07;

//Default Constructor
CashReg::CashReg()
{
	posX = 25;
	posY = 0;
	length = 60;
	width = 13;
	S; // New not needed here, object created in header file.
	total = registerTotal = tax = 0.00f;	
	
	tape = new Tape(0,0,width + 9, S); // Create the tape

	displayTotal();
	displayItems();
}

CashReg::CashReg(int xPos, int yPos, int sizeX, int sizeY, CScreen &screen)
{
	posX = xPos;
	posY = yPos;
	length = sizeX;
	width = sizeY;
	S = screen;
	total = registerTotal = tax = 0.00f;

	tape = new Tape(0,0,sizeY + 9, S);

	displayTotal();
	displayItems();
}

CashReg::CashReg(int xPos, int yPos, int sizeX, int sizeY, CScreen &screen, Inventory &inv, float total)
{
	posX = xPos;
	posY = yPos;
	length = sizeX;
	width = sizeY;
	S = screen;
	this->total = tax = 0.00f;
	registerTotal = total;
	inventory = inv;

	tape = new Tape(0,0,sizeY + 9, S);

	displayTotal();
	displayItems();
}

void CashReg::displayItems()
{	tape->displayItems(); }

void CashReg::displayTotal()
{
	S.Box(posX, posY, length + posX, width, GREEN);

	S.GotoXY(posX + 2, posY);
	S.DrawString((char *)("Sub-Total: " + to_string(total)).c_str());

	S.GotoXY(posX+2, posY+2);
	S.DrawString((char *)("Tax:       " + to_string(tax)).c_str());

	S.GotoXY(posX+2, posY+4);
	S.DrawString((char *)("Total:     " + to_string(total + tax)).c_str());
}

void CashReg::adjustInventory(string UPC, int qtyToAdjustBy)
{
	inventory.adjustStock(UPC, qtyToAdjustBy);
}

void CashReg::addItem(string UPC)
{
	Item * lookup = inventory.findItem(UPC);
	if(lookup != NULL)
	{
		tape->addItem(lookup); // Add the item to the tape.
		total += lookup->price; // Add item price to the total
		tax = total * taxRate; //calculate tax

		adjustInventory(UPC, -1);

		displayTotal(); // Print the total.
		displayItems(); // Reprint the tape.
	}
	
	/*
	else
	{
		// Error: Item not in the inventory.
	}
	*/
}

void CashReg::voidItem(string UPC)
{
	if(tape->voidItem(UPC)) // Remove item from the tape.
	{
		Item * lookup = inventory.findItem(UPC); // Look-up the item for price info.
		total -= lookup->price; // Subract item price from total

		adjustInventory(UPC, 1);

		displayTotal(); // Print the total.
		displayItems(); // Reprint the tape.
	}

	/*
	else
	{
		// Error: Item could not be removed.
	}
	*/
}

float  CashReg::calcChange(float paid)
{
	total += tax;
	float change;
	if(paid >= total)
	{  
		change = paid - total; 
		
		S.GotoXY(posX+2, posY+6);
		S.DrawString((char *)("Change:     " + to_string(change)).c_str());

		return change;
	}

	S.GotoXY(posX+2, posY+6);
	S.DrawString((char *)("The Customer did not pay enough"));
	return -1;
}
void CashReg::updateRegCash(float paid)
{
	registerTotal += paid;
	S.GotoXY(posX+2, posY+8);
	S.DrawString((char *)("The Register now holds:  " + to_string(registerTotal)).c_str());
}

void CashReg::newCustomer()
{
	total = tax = 0;	
	
	tape = new Tape(0,0,width + 9, S); // Create the tape
	
	displayTotal();
	displayItems();
}