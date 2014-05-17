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

enum area{STOREOPENED, INVENTORY, INREGISTER, INCUSTOMER, ERRORLOG, INLOG, EDITINVENTORY};

Store::Store(void)
{
	screen = new CScreen();
	inv = new Inventory();
	registerTotal = 100.00f;
	errorLogFile = new RFile((char *)"log.arc", sizeof(Error));

	if(errorLogFile->stat() != 0) //if file wasnt opened
	{
		fstream tempFile("log.arc", ios::out);//create a file because RFile wont for some reason
		errorLogFile->Fopen((char *)"log.arc", sizeof(Error));
	}

	openStore(); //starts the application
}

Store::~Store(void)
{
	delete reg;
	delete inv;
	delete errorLogFile;
	delete screen;
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
	{
		screen->Box(boxX, boxY, length, width, color);
		screen->GotoXY(boxX + 20, boxY + 4 );
		screen->DrawString((char*)"Instructions");
		screen->GotoXY(boxX + 20, boxY + 6);
		screen->DrawString((char*)"Enter A to add a new item");
		screen->GotoXY(boxX + 20, boxY + 8);
		screen->DrawString((char*)"Enter D to delete an item");
		screen->GotoXY(boxX + 20, boxY + 10);
		screen->DrawString((char*)"Enter T to edit an item");
		screen->GotoXY(boxX + 20, boxY + 12);
		screen->DrawString((char*)"Enter F to find an item");
		screen->GotoXY(boxX + 20, boxY + 14);
		screen->DrawString((char*)"Enter B to browse the inventory");
		screen->GotoXY(boxX + 20, boxY + 16);
		screen->DrawString((char*)"Enter E to Exit");
	}
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
	else if(storeArea == EDITINVENTORY)
	{
		screen->Box(boxX, boxY, length, width, color);
		screen->GotoXY(boxX + 20, boxY + 8 );
		screen->DrawString((char*)"Instructions");
		screen->GotoXY(boxX + 20, boxY + 10);
		screen->DrawString((char*)"Enter D to edit the Description");
		screen->GotoXY(boxX + 20, boxY + 12);
		screen->DrawString((char*)"Enter P to edit the Price");
		screen->GotoXY(boxX + 20, boxY + 14);
		screen->DrawString((char*)"Enter Q to edit the Quantity on hand");
		screen->GotoXY(boxX + 20, boxY + 16);
		screen->DrawString((char*)"Enter E to Exit");
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

	bool inCustomer = false;
	bool UPCEntered = false;
	bool exit	   = false;
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
	string fmt   = "@";
	bool exit = false;
	char * input = new char();

	do
	{
		screen->GotoXY(33, 4);
		screen->GetStr(33, 4, input, 1, (char *)fmt.c_str());
		if((*input == 'a' || *input == 'A'))
		{
			Item temp;
			memset( &temp, 0x00, sizeof(Item) ); //clear memory for item
			temp.isDeleted = false;

			//prompt for description
			fmt   = "@@@@@@@@@@@@@@@";
			screen->GotoXY(10, 18);
			screen->DrawString((char*)"Enter the Description ");
			screen->GetStr(10, 20, input, 15, (char *)fmt.c_str());
			strcpy(temp.description, input);
			screen->Box(10, 18, 80, 25, GOLD);

			//prompt for UPC
			fmt   = "####";
			screen->GotoXY(10, 18);
			screen->DrawString((char*)"Enter the UPC ");
			screen->GetStr(10, 20, input, 4, (char *)fmt.c_str());
			strcpy(temp.UPC, input);
			screen->Box(10, 18, 80, 25, GOLD);

			//prompt for price
			fmt   = "##########";
			screen->GotoXY(10, 18);
			screen->DrawString((char*)"Enter the price ");
			screen->GetStr(10, 20, input, 10, (char *)fmt.c_str());
			temp.price = (float)atof(input);
			screen->Box(10, 18, 80, 25, GOLD);

			//prompt for quantity
			fmt   = "#####";
			screen->GotoXY(10, 18);
			screen->DrawString((char*)"Enter the quantity on hand ");
			screen->GetStr(10, 20, input, 10, (char *)fmt.c_str());
			temp.QOH = atoi(input);
			screen->Box(10, 18, 80, 25, GOLD);

			if(!inv->addItem(temp)) //adds item to inventory and writes an error if it fails
			{  
				Error tmp;
				strcpy(tmp.error, "123 Item not added");
				errorLogFile->write(&tmp);
			}
		}
		else if((*input == 'd' || *input == 'D'))
		{
			//prompt for UPC
			fmt   = "####";
			screen->GotoXY(10, 18);
			screen->DrawString((char*)"Enter the UPC ");
			screen->GetStr(10, 20, input, 4, (char *)fmt.c_str());
			
			if(!inv->removeItem(input)) //removes from inventory and writes an error if it fails
			{  
				Error tmp;
				strcpy(tmp.error, "123 Item not removed");
				errorLogFile->write(&tmp);
			}
			displayInstructions(0,0,80,30, GOLD, INVENTORY);
		}
		else if((*input == 't' || *input == 'T'))
		{
			displayInstructions(0,0,80,30, FUSCIA, EDITINVENTORY);

			bool exitEdit = false;
			char * inputInEdit = new char();
			char * upc = new char();

			//prompt for UPC
			fmt   = "####";
			screen->GotoXY(10, 18);
			screen->DrawString((char*)"Enter the UPC ");
			screen->GetStr(10, 20, upc, 4, (char *)fmt.c_str());
			screen->Box(10, 18, 80, 25, FUSCIA);

			do
			{
				screen->GotoXY(33, 8);
				screen->GetStr(33, 8, inputInEdit, 1, (char *)"@");
				if(*inputInEdit == 'p' || *inputInEdit == 'P' ) //shows next three entries in the log
				{
					fmt   = "##########";
					screen->GotoXY(10, 18);
					screen->DrawString((char*)"Enter the price ");
					screen->GetStr(10, 20, input, 10, (char *)fmt.c_str());
					
					if(!inv->changePrice(upc, (float)atof(input)))//changes price, writes to log if fails
					{  
						Error tmp;
						strcpy(tmp.error, ("123 price not updated " + (string)upc).c_str());
						errorLogFile->write(&tmp);
					}

					screen->Box(10, 18, 80, 25, FUSCIA);
				}
				else if(*inputInEdit == 'd' || *inputInEdit == 'D') //exits this dialog
				{
					//prompt for description
					fmt   = "@@@@@@@@@@@@@@@";
					screen->GotoXY(10, 18);
					screen->DrawString((char*)"Enter the Description ");
					screen->GetStr(10, 20, input, 15, (char *)fmt.c_str());
					
					if(!inv->changeDescription(upc, input))//changes description, writes to log if fails
					{  
						Error tmp;
						strcpy(tmp.error, ("123 description not updated " + (string)upc).c_str());
						errorLogFile->write(&tmp);
					}

					screen->Box(10, 18, 80, 25, FUSCIA);
				}
				else if(*inputInEdit == 'q' || *inputInEdit == 'Q') //exits this dialog
				{
					//prompt for quantity
					fmt   = "#####";
					screen->GotoXY(10, 18);
					screen->DrawString((char*)"Enter the quantity on hand ");
					screen->GetStr(10, 20, input, 10, (char *)fmt.c_str());
					if(!inv->adjustStock(upc, atoi(input)))//changes stock, writes to log if fails
					{  
						Error tmp;
						strcpy(tmp.error, ("123 quantity not updated " + (string)upc).c_str());
						errorLogFile->write(&tmp);
					}
					screen->Box(10, 18, 80, 25, FUSCIA);
				}
				else if(*inputInEdit == 'e' || *inputInEdit == 'E') //exits this dialog
				{ exitEdit = true;  }
				else
				{ cout << '\a';  }
			}while(!exitEdit);

			displayInstructions(0,0,80,30, GOLD, INVENTORY); //back to main inventory list
		}
		else if((*input == 'f' || *input == 'F'))
		{
			//prompt for UPC
			fmt   = "####";
			screen->GotoXY(10, 18);
			screen->DrawString((char*)"Enter the UPC ");
			screen->GetStr(10, 20, input, 4, (char *)fmt.c_str());
			
			Item *temp = new Item;

			if(inv->findItem(input, temp)) //if item is found print its contents
			{
				screen->GotoXY(10, 22);
				screen->DrawString((char*)temp->UPC);
				screen->GotoXY(10, 24);
				screen->DrawString((char*)temp->description);
				screen->GotoXY(10, 26);
				screen->DrawString((char*)to_string(temp->price).c_str());
				screen->GotoXY(10, 28);
				screen->DrawString((char*)to_string(temp->QOH).c_str());
			}
			else  //write error if not found
			{  
				Error tmp;
				strcpy(tmp.error, ("123 item not found " + (string)input).c_str());
				errorLogFile->write(&tmp);
			}
			delete temp;
			screen->GetStr(10, 19, input, 1, (char *)fmt.c_str());
			displayInstructions(0,0,80,30, GOLD, INVENTORY);
		}
		else if((*input == 'b' || *input == 'B'))
		{
			Item *temp = new Item;
			int ctr = 0;

			while(inv->findItem(ctr, temp)) //while it finds an item print its content
			{
				screen->GotoXY(10, 20);
				screen->DrawString((char*)temp->UPC);
				screen->GotoXY(10, 22);
				screen->DrawString((char*)temp->description);
				screen->GotoXY(10, 24);
				screen->DrawString((char*)to_string(temp->price).c_str());
				screen->GotoXY(10, 26);
				screen->DrawString((char*)to_string(temp->QOH).c_str());

				screen->GetStr(10, 19, input, 1, (char *)fmt.c_str());
				ctr++;
			}
			delete temp;
			displayInstructions(0,0,80,30, GOLD, INVENTORY);
		}
		else if((*input == 'e' || *input == 'E'))
		{   exit = true;   }
		else
		{   cout << '\a';  }

		fmt   = "@";
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
		screen->GetStr(33, 8, input, 1, (char *)fmt.c_str()); //gets input
		if((*input == 'n' || *input == 'N')) //start a new log
		{
			screen->GotoXY(20, 20);
			screen->DrawString((char *)"Please enter a filename to start a new log",20 , 20);
			//prompts user for a filename
			fmt = "@@@@@@@@@@@@";
			screen->GotoXY(20, 22);
			screen->GetStr(20, 22, input, 12, (char *)fmt.c_str());

			filename = (string)input + ".arc";
			fstream tempFile(filename, ios::out); //create a file because RFile wont for some reason

			//opens the file
			errorLogFile->Fclose();
			errorLogFile->Fopen((char *)filename.c_str(), sizeof(Error));
		}
		else if((*input == 'd' || *input == 'D'))//display the contents of the log
		{
			displayInstructions(0,0,80,30, GREEN, INLOG); //display instructions inside log
			bool exitLog = false;
			char * inputInLog = new char();
			int logrec = 0;

			do
			{
				screen->GotoXY(33, 8);
				screen->GetStr(33, 8, inputInLog, 1, (char *)"@");
				if(*inputInLog == 'n' || *inputInLog == 'N' ) //shows next three entries in the log
				{
					Error *error = new Error();
					for(logrec; logrec < 3; logrec++)
					{
						if(errorLogFile->read(logrec, error)) //prints error if it exists in the file
						{
							screen->DrawString((char *)error, 10, 20 + logrec*2);
						}
						else
						{  screen->DrawString((char *)"There are no more errors", 10, 20 + logrec*2);  }
					}
					delete error;
				}
				else if(*inputInLog == 'e' || *inputInLog == 'E') //exits this dialog
				{ exitLog = true;  }
				else
				{ cout << '\a';  }
			}while(!exitLog);
		}
		else if((*input == 's' || *input == 'S')) //save the log
		{
			screen->GotoXY(20, 20);
			screen->DrawString((char *)"Please enter a filename to save the log",20 , 20 );

			fmt = "@@@@@@@@@@@@";
			screen->GotoXY(20, 22);
			screen->GetStr(20, 22, input, 12, (char *)fmt.c_str());//prompts user for a filename

			filename = (string)input + ".arc";
			fstream tempFile(filename, ios::out);//creates a file because RFile wont for some reason

			//create a temporary RFile instance to copy entries to
			RFile newFile((char *)filename.c_str(), sizeof(Error));

			Error *temp = new Error();
			int ctr = 0;

			while(errorLogFile->read(ctr, temp))//copies to the new file until there are no more entries
			{
				newFile.write(temp);
				ctr++;
			}
			delete temp;
		}
		else if((*input == 'c' || *input == 'C'))//clears teh file
		{
			errorLogFile->Fclose();

			remove((char *)filename.c_str()); //deltes the file

			fstream tempFile(filename, ios::out);//creates a file because RFile wont for some reason
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