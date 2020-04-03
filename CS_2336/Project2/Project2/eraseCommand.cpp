#include <iostream>
#include "eraseCommand.h"

eraseCommand::eraseCommand(phoneBook & bookIn, std::istream & inStream, std::ostream & outStream) : bookCommand(inStream, outStream), book(bookIn) {}

void eraseCommand::execute()
{
	//Prompt For A Name to Erase and Return It
	std::string eraseName = readIn("Enter name to erase");

	//Erase Message
	std::string eraseMsg = "";
	
	//If Name Was Erased
	if (book.erase(eraseName))
	{
		//Name Was Erased From List
		//Update and Display Erase Message
		eraseMsg = "Phone book entry " + eraseName + " was erased";
		display(eraseMsg);
	}
	else
	{
		//Name Was Not Erased From List (Does Not Exist)
		//Update and Display Erase Message
		eraseMsg = "Phone book entry " + eraseName + " was not erased";
		display(eraseMsg);
	}
}
