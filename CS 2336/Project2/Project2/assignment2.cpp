//Main Class

#include <iostream>
#include <string>

#include "phoneBook.h"
#include "menuList.h"
#include "bookCommand.h"
#include "addUpdateCommand.h"
#include "eraseCommand.h"
#include "printCommand.h"

int main()
{
	// build the menuList and phoneBook
	menuList menu("Phone book menu:");
	phoneBook book;

	// create the commands
	addUpdateCommand addUpdate(book, std::cin, std::cout);
	eraseCommand erase(book, std::cin, std::cout);
	printCommand print(book, std::cin, std::cout);

	//actual menu entries for the application
	menuItem addUpdateItem('a', "add/update", addUpdate);
	menuItem eraseItem('e', "erase", erase);
	menuItem printItem('p', "print", print);

	// add the menuItem values to the menulist
	menu.add(addUpdateItem);
	menu.add(eraseItem);
	menu.add(printItem);

	std::cout << "Starting the Phone Book" << std::endl;
	std::cout << std::endl;

	//Start the Program
	menu.start();

	return 0;
}