/*
 * stack.h
 *
 *  The stack class is a stack data structor for command pointers. The entries that make up the start are of 
 *  type stackEntry. 
 *
 *  Created on: Oct 1, 2018
 *      Author: dgv130030
 */

#ifndef STACK_H_
#define STACK_H_

class command;

class stack;

class stackEntry
{
	friend class stack;

	private:

	protected:

};

class stack
{
	private:

	public:
	
	     stack(const stack &from) = delete;
        stack& operator=(const stack &from) = delete; 

};

#endif /* STACK_H_ */
