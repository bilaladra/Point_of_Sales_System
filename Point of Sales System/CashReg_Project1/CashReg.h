// Cash Reg program
// File: CashReg.h
// Authors: Bilal Adra, Eric Zuidema, Timothy Byukusenge      Date: 4/15/2014 edited 5/4/2014

// This structure defines a Cash Register for a company

#ifndef _CashReg
#define _CashReg

#include <windows.h>
#include <string>
#include "Inventory.h"
#include "Item.h"
#include "Tape.h"

using namespace std;

class CashReg
{
private:
	string input;
	float total;
	float registerTotal;
	float tax;
	Tape * tape;
	Inventory inventory;
	Item item;
	int posX;
	int posY;
	int length;
	int width;
	CScreen S;

public:
	CashReg();
	CashReg(int xPos, int yPos, int sizeX, int sizeY, CScreen &screen);
	CashReg(int xPos, int yPos, int sizeX, int sizeY, CScreen &screen, Inventory &inventory, float total);
	void  addItem(string upc);
	void  voidItem(string upc);
	void  adjustInventory(string UPC, int qtyToAdjustBy);
	float calcChange(float paid);
	void  updateRegCash(float paid);
	void  displayItems();
	void  displayTotal();
	void  newCustomer();
};

#endif;