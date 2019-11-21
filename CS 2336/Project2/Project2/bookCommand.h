#ifndef BOOKCOMMAND_H
#define BOOKCOMMAND_H
#include "command.h"
#include <string>

class bookCommand : public command
{

public:
	//Constructor
	bookCommand(std::istream &in, std::ostream &out);

	//Destructor
	virtual ~bookCommand() {}

	//Abstract Function
	virtual void execute() = 0;

protected:
	std::istream &in;
	std::ostream &out;

	void display(const std::string str); //Display Inputted String
	std::string readIn(const std::string str); //Read In and Display Inputted String
};

#endif /* BOOKCOMMAND_H */