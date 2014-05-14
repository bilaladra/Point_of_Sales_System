// Cash Reg program
// File: Store.cpp
// Authors: Bilal Adra, Eric Zuidema     Date: 5/4/2014 edited 5/4/2014

#include "Store.h"
#include <Windows.h>
#include <conio.h>
#include "CashReg.h"
#include "CScreen.h"
#include "Error.h"
#include <iostream>
#include <string>

using namespace std;

enum area{STOREOPENED, INVENTORY, INREGISTER, INCUSTOMER, ERRORLOG, INLOG};

Store::Store(void)
{
	screen = new CScreen();
	inv = new Inventory();
	registerTotal = 100.00f;
	errorLogFile = new RFile((char *)"log.arc", sizeof(Error));

	if(errorLogFile->stat() != 0)
	{
		fstream tempFile("log.arc", ios::trunc | ios::out);//create a file because RFile wont for some reason
	}

	openStore();
}


Store::~Store(void)
{
}

void Store::displayInstructions( int boxX, int boxY, int length, int width, int color, int storeArea)
{
	if(storeArea == INCUSTOMER)
	{
		screen->Box(boxX, boxY, length, width, color);
		screen->GotoXY(boxX + 10, boxY + 1);
		screen->DrawString((char*)"Instructions");
		screen->GotoXY(boxX + 2, boxY + 3);
		screen->DrawString((char*)"Enter M to enter a UPC code");
		screen->GotoXY(boxX + 2, boxY + 5);
		screen->DrawString((char*)"Enter V to void an item");
		screen->GotoXY(boxX + 2, boxY + 7);
		screen->DrawString((char*)"Enter T to total the order and close out the customer");
	}
	else if(storeArea == INREGISTER)
	{
		screen->Box(boxX, boxY, length, width, color);
		screen->GotoXY(boxX + 10, boxY + 1);
		screen->DrawString((char*)"Instructions");
		screen->GotoXY(boxX + 2, boxY + 3);
		screen->DrawString((char*)"Enter N to Start a new Customer");
		screen->GotoXY(boxX + 2, boxY + 5);
		screen->DrawString((char*)"Enter E to Exit");
	}
	else if(storeArea == INVENTORY)
	{}
	else if(storeArea == STOREOPENED)
	{
		screen->Box(boxX, boxY, length, width, color);
		screen->GotoXY(boxX + 20, boxY + 8 );
		screen->DrawString((char*)"Instructions");
		screen->GotoXY(boxX + 20, boxY + 10);
		screen->DrawString((char*)"Enter I to access Inventory");
		screen->GotoXY(boxX + 20, boxY + 12);
		screen->DrawString((char*)"Enter C to access Cash Register");
		screen->GotoXY(boxX + 20, boxY + 14);
		screen->DrawString((char*)"Enter L to access Error Log");
		screen->GotoXY(boxX + 20, boxY + 16);
		screen->DrawString((char*)"Enter E to Exit");
	}
	else if(storeArea == ERRORLOG)
	{
		screen->Box(boxX, boxY, length, width, color);
		screen->GotoXY(boxX + 20, boxY + 8 );
		screen->DrawString((char*)"Instructions");
		screen->GotoXY(boxX + 20, boxY + 10);
		screen->DrawString((char*)"Enter D to display the contents of the log");
		screen->GotoXY(boxX + 20, boxY + 12);
		screen->DrawString((char*)"Enter S to save the current log");
		screen->GotoXY(boxX + 20, boxY + 14);
		screen->DrawString((char*)"Enter C to clear the log");
		screen->GotoXY(boxX + 20, boxY + 16);
		screen->DrawString((char*)"Enter N to start new log");
		screen->GotoXY(boxX + 20, boxY + 18);
		screen->DrawString((char*)"Enter E to exit");
	}
	else if(storeArea == INLOG)
	{
		screen->Box(boxX, boxY, length, width, color);
		screen->GotoXY(boxX + 20, boxY + 8 );
		screen->DrawString((char*)"Instructions");
		screen->GotoXY(boxX + 20, boxY + 10);
		screen->DrawString((char*)"Enter N to display the next three errors");
		screen->GotoXY(boxX + 20, boxY + 12);
		screen->DrawString((char*)"Enter E to exit");
		screen->Box(boxX, boxY + 16, length, width, RED);
	}
}

void Store::openStore()
{
	//prompt to go to Inventory or Register
	string fmt   = "@";
	bool exit = false;
	char * input = new char();

	displayInstructions(0,0,80,30,BLUE, STOREOPENED);

	do
	{
		screen->GotoXY(33, 8);
		screen->GetStr(33, 8, input, 1, (char *)fmt.c_str());
		if((*input == 'i' || *input == 'I'))
		{	goToInventory();  }
		else if((*input == 'c' || *input == 'C'))
		{	goToRegister();   }
		else if((*input == 'l' || *input == 'L'))
		{	goToErrorLog();   }
		else if((*input == 'e' || *input == 'E'))
		{   exit = true;   }
		else
		{   cout << '\a';  }
	}while(!exit);
}

void Store::goToRegister()
{
	reg = new CashReg(25,0,60,13, *screen, *inv, 100 );

	displayInstructions(25, 13, 80, 24, MBLUE, INREGISTER);

	char * input = new char();
	string fmt   = "@";

	boolean inCustomer = false;
	boolean UPCEntered = false;
	boolean exit	   = false;
	do
	{
		fmt   = "@";
		screen->GotoXY(35 + 13, 14);
		screen->GetStr(35 + 13, 14, input, 1, (char *)fmt.c_str());
		if((*input == 'n' || *input == 'N') && !inCustomer)
		{
			reg->newCustomer();
		
			//display instructions
			inCustomer = true;

			reg->displayTotal();
			reg->displayItems();
			displayInstructions(25, 13, 80, 24, MBLUE, INCUSTOMER);
		}
		else if((*input == 'm' || *input == 'M') && inCustomer)
		{
			//gets upc
			fmt = "~~~~~"; //sets format string
			screen->GotoXY(27, 22);
			screen->DrawString((char*)"Enter a UPC: ");//prompts for upc
			screen->GetStr(27 + 13, 22, input, 5, (char *)fmt.c_str());
			string upc = input;

			//gets amount
			fmt = "##";  //sets format string
			screen->GotoXY(27, 22);
			screen->DrawString((char*)"Enter the amount: "); //prompts for upc
			screen->GetStr(27 + 18, 22, input, 2, (char *)fmt.c_str());
			int amt = atoi(input);

			//enters the amount of items to the register
			for(int i = 0; i < amt; i++)
			{
				reg->addItem(upc);
			}

			//display instructions
			displayInstructions(25, 13, 80, 24, MBLUE, INCUSTOMER);
			reg->displayItems();
		}
		else if((*input == 'v' || *input == 'V') && inCustomer)
		{
			//gets upc
			fmt = "~~~~~"; //sets format string
			screen->GotoXY(27, 22);
			screen->DrawString((char*)"Enter a UPC: ");//prompts for upc
			screen->GetStr(27 + 13, 22, input, 5, (char *)fmt.c_str());
			string upc = input;

			reg->voidItem(input);

			//display instructions
			displayInstructions( 25, 13, 80, 24, MBLUE, INCUSTOMER);
			reg->displayItems();
		}
		else if((*input == 't' || *input == 'T') && inCustomer)
		{
			fmt   = "#######|##";
			screen->GotoXY(27, 22);
			screen->DrawString((char*)"Enter price paid: "); //prompts for money
			screen->GetStr(27 + 18, 22, input, 10, (char *)fmt.c_str());
			float paid = (float)atof(input);
			float change = reg->calcChange(paid);

			while(change == -1)
			{
				screen->GetStr(27 + 18, 22, input, 10, (char *)fmt.c_str());
				paid = (float)atof(input);
				change = reg->calcChange(paid);
			}

			reg->updateRegCash(paid - change);
			inCustomer = false;

			//display instructions
			displayInstructions(25, 13, 80, 24, MBLUE, INREGISTER);
		}
		else if((*input == 'e' || *input == 'E') && !inCustomer)
		{ exit = true; }
		else
		{ cout <<"\a"; }
	}while(!exit);

	displayInstructions(0,0,80,30,BLUE, STOREOPENED);//display instructions for store
}

void Store::goToInventory()
{
	displayInstructions(0,0,80,30, GOLD, INVENTORY);
	boolean exit = false;
	do
	{



	}while(!exit);

	displayInstructions(0,0,80,30,BLUE, STOREOPENED);//display instructions for store
}

void Store::goToErrorLog()
{
	displayInstructions(0,0,80,30, AQUA, ERRORLOG);
	
	string fmt   = "@";
	bool exit = false;
	char * input = new char();
	string filename = "log.arc";

	do
	{
		screen->GotoXY(33, 8);
		screen->GetStr(33, 8, input, 1, (char *)fmt.c_str());
		if((*input == 'n' || *input == 'N'))
		{
			screen->GotoXY(20, 20);
			screen->DrawString((char *)"Please enter a filename to start a new log",20 , 20);

			fmt = "@@@@@@@@@@@@";
			screen->GotoXY(20, 22);
			screen->GetStr(20, 22, input, 12, (char *)fmt.c_str());

			fstream tempFile(filename, ios::trunc | ios::out); //create a file because RFile wont for some reason
			
			filename = (string)input + ".arc";

			errorLogFile->Fclose();
			errorLogFile->Fopen((char *)filename.c_str(), sizeof(Error));
		}
		else if((*input == 'd' || *input == 'D'))
		{
			displayInstructions(0,0,80,30, GREEN, INLOG);
			bool exitLog = false;
			char * inputInLog = new char();
			int logrec = 0;

			do
			{
				screen->GotoXY(33, 8);
				screen->GetStr(33, 8, inputInLog, 1, (char *)"@");
				if(*inputInLog == 'n' || *inputInLog == 'N' )
				{
					Error *error = new Error();
					for(logrec; logrec < 3; logrec++)
					{
						if(errorLogFile->read(logrec, error))
						{
							screen->DrawString((char *)error, 10, 20 + logrec*2);
						}
						else
						{  screen->DrawString((char *)"There are no more errors", 10, 20 + logrec*2);  }
					}
				}
				else if(*inputInLog == 'e' || *inputInLog == 'E')
				{ exitLog = true;  }
				else
				{ cout << '\a';  }
			}while(!exitLog);
		}
		else if((*input == 's' || *input == 'S'))
		{
			screen->GotoXY(20, 20);
			screen->DrawString((char *)"Please enter a filename to save the log",20 , 20 );

			fmt = "@@@@@@@@@@@@";
			screen->GotoXY(20, 22);
			screen->GetStr(20, 22, input, 12, (char *)fmt.c_str());

			filename = (string)input + ".arc";

			fstream tempFile(filename, ios::trunc | ios::out);//create a file because RFile wont for some reason
			
			RFile newFile((char *)filename.c_str(), sizeof(Error));
			
			int fileSize = errorLogFile->count();
			Error *temp = new Error();
			int ctr = 0;

			while(errorLogFile->read(ctr, temp))
			{
				newFile.write(temp);
				ctr++;
			}
		}
		else if((*input == 'c' || *input == 'C'))
		{
			errorLogFile->Fclose();
			
			remove((char *)filename.c_str());

			errorLogFile->Fopen((char *)filename.c_str(), sizeof(Error));
		}
		else if((*input == 'e' || *input == 'E'))
		{ exit = true; }
		else
		{ cout <<"\a"; }
		
		displayInstructions(0,0,80,30, AQUA, ERRORLOG);//display instructions for log
	}while(!exit);

	displayInstructions(0,0,80,30,BLUE, STOREOPENED);//display instructions for store
}