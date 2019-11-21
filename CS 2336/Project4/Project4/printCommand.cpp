#include <iostream>
#include "printCommand.h"

//Constructor
printCommand::printCommand(phoneBook & bookIn, std::istream & inStream, std::ostream & outStream) : bookCommand(inStream, outStream), book(bookIn) {}

void printCommand::execute()
{
	//Print Out Directory
	book.print(out);
}
