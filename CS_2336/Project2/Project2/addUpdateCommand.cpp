#include <iostream>
#include "addUpdateCommand.h"

addUpdateCommand::addUpdateCommand(phoneBook &bookIn, std::istream &inStream, std::ostream &outStream) : bookCommand(inStream, outStream), book(bookIn) {}

void addUpdateCommand::execute()
{
	//Prompt For Name and Return That Name
	std::string nName = readIn("Enter name to add/update");

	//Prompt For Number and Return That Number
	std::string nNumber = readIn("Enter phone number");

	//Prompt For Email and Return That Email
	std::string nEmail = readIn("Enter e-mail address");

	//Add Or Update Message
	std::string entryMsg = "";

	//Check If Entry Exists In Vector
	if (book.find(nName))
	{
		//Update The Entry
		book.insert(nName, nNumber, nEmail);

		//Print Out Update Message
		entryMsg = "Updating phone book entry for " + nName;
		display(entryMsg);	
	}
	else
	{
		//Add Message
		entryMsg = "Adding phone book entry for " + nName;

		//Check if an Email is Present For the Entry
		if (nEmail == "")
		{
			//Add Entry
			book.insert(nName, nNumber);

			//Print Out Add Message
			display(entryMsg);
		}
		else
		{
			//Add Entry
			book.insert(nName, nNumber, nEmail);

			//Print Out Add Message
			display(entryMsg);
		}
	}

}


