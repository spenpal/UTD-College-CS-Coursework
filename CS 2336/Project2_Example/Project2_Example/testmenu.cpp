/*
 * project2.cpp
 *
 * The main function calls two other functions. testPhoneBook and testList
 *
 * The testList function tests the DoubleLinkedList class.
 *
 * The testPhoneBook function tests the PhoneBook class which also uses
 * the PhoneBookEntry class and the OrderedDoubleLinkedList class.
 *
 */

#include <iostream>
#include <string>

#include "incrementCommand.h"
#include "decrementCommand.h"
#include "menuList.h"

int main()
{
	// create the int variable. This will be updated by the commands below
	int value = 0;

	// create the commands
	incrementCommand increment(value);
	decrementCommand decrement(value);

	// build the menuList and menuItems
	menuList menu("Simple menu:");
	// these are the actual menu entries for the application
	menuItem incrementItem('i', "increment", increment); // increment menu command
	menuItem decrementItem('d', "decrement", decrement); // decrement menu command
	// note that the quit command is added to the menu automatically

	// add the menuItem values to the menulist
	menu.add(incrementItem);
	menu.add(decrementItem);

	std::cout << "Starting the Simple Menu Program" << std::endl;
	std::cout << std::endl;

	// run the menu
	menu.start();

	return 0;
}
