#ifndef ADDUPDATECOMMAND_H
#define ADDUPDATECOMMAND_H

#include "bookCommand.h"
#include "phoneBook.h"
#include "memoryLeak.h"

class addUpdateCommand : public bookCommand
{
private:
	phoneBook &book;

public:
	//Constructor
	addUpdateCommand(phoneBook &bookIn, std::istream &inStream, std::ostream &outStream);

	//Derived From Abstract Function
	void execute();
};

#endif /* ADDUPDATECOMMAND_H */