#include "CashReg.h"
#include "CScreen.h"
#include "Inventory.h"
#include "RFile.h"

#pragma once
class Store
{
private:
	Inventory *inv; //inventory of the store
	CashReg *reg;   //cash register
	CScreen *screen;//display screen
	float registerTotal; //total money in the register
	RFile *errorLogFile;  //variable to manipulate an error log file

	void goToRegister();   //starts register
	void goToInventory();  //goes to inventory
	void goToErrorLog();   //goes to the error log
	void displayInstructions( int boxX, int boxY, int length, int width, int color, int storeArea); //function to display instructions dependign on the area of the store
	void openStore(); //opens the store dialog and has the user choose what they want to do

public:
	Store(void);
	~Store(void);
};

