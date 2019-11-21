/*
 * decrementCommand.h
 *
 *  Created on: Sep 23, 2018
 *      Author: dgv130030
 */

#ifndef DECREMENTCOMMAND_H_
#define DECREMENTCOMMAND_H_

#include <iostream>
#include "command.h"

class decrementCommand : public command
{
private:
	// this a reference to the int we will increment
	int &value;
public:
	// value is passed by reference to us on the constructor
	decrementCommand(int &value)
		: value(value)
	{
	}
	virtual ~decrementCommand()
	{
	}
	// override the execute member function from class command
	// we simply decrement the value and display it on cout
	void execute()// the override ensures that we have the correct signature for execute()
	{
		value--;
		std::cout << "value: " << value << std::endl;
	}
};

#endif /* DECREMENTCOMMAND_H_ */