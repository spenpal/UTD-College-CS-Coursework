#ifndef ERASECOMMAND_H
#define ERASECOMMAND_H
#include "bookCommand.h"
#include "phoneBook.h"

class eraseCommand : public bookCommand
{
private:
	phoneBook &book;

public:
	//Constructor
	eraseCommand(phoneBook &bookIn, std::istream &inStream, std::ostream &outStream);

	//Derived From Abstract Class
	void execute();
};

#endif /* ERASECOMMAND_H */