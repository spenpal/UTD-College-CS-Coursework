#ifndef PRINTCOMMAND_H
#define PRINTCOMMAND_H
#include "bookCommand.h"
#include "phoneBook.h"

class printCommand : public bookCommand
{
private:
	phoneBook &book;

public:
	//Constructor
	printCommand(phoneBook &bookIn, std::istream &inStream, std::ostream &outStream);

	//Derived From Abstract Class
	void execute();
};

#endif /* PRINTCOMMAND_H */