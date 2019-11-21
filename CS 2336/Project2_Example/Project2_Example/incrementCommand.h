/*
 * incrementCommand.h
 *
 *  Created on: Sep 23, 2018
 *      Author: dgv130030
 */

#ifndef INCREMENTCOMMAND_H_
#define INCREMENTCOMMAND_H_

#include <iostream>
#include  "command.h"

class incrementCommand : public command
{
private:
	// this a reference to the int we will increment
	int &value;
public:
	// value is passed by reference to us on the constructor
	incrementCommand(int &value)
		: value(value)
	{
	}
	virtual ~incrementCommand()
	{
	}
	// override the execute member function from class command
	// we simply increment the value and display it on cout
	void execute() // the override ensures that we have the correct signature for execute()
	{
		value++;
		std::cout << "value: " << value << std::endl;
	}
};

#endif /* INCREMENTCOMMAND_H_ */