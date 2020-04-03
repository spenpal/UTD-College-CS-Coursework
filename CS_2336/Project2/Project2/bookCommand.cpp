#include <iostream>
#include "bookCommand.h"

bookCommand::bookCommand(std::istream &inStream, std::ostream &outStream) : in(inStream), out(outStream) {}

void bookCommand::display(const std::string str)
{
	//Display "str" to "out" stream
	out << str << std::endl;
}

std::string bookCommand::readIn(const std::string str)
{
	//Input String
	std::string input = "";

	//Print Out "Str"
	display(str);

	//Read Input String
	std::getline(in, input);

	//Print New Line
	//out << std::endl;

	//Return String to Caller Function
	return input;
}

