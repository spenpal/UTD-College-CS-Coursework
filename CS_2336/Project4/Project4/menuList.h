#ifndef MENULIST_H_
#define MENULIST_H_

#include <string>
#include <ios>
#include <vector>
#include <string>

#include "command.h"

// this is the menu item. This is a menuItem.
// The menuItem contains the selection character,
// the menu text and the command to be executed
class menuItem
{
public:
	// the quitCommand class is a dummy command
	// it is only used when the default menuItem
	// constructor is used.
	class quitCommand : public command
	{
	public:
		quitCommand() {}
		virtual void execute() {}
	};
private:
	// the dummy quit command
	static quitCommand dummyCommand;
	// the menu item selection character
	char theSelectChar;
	// the menu item description
	std::string theDescription;
	// the command executed when the menu item is selected
	command &theCommand;
public:
	// constructors
	menuItem();
	// this is what your application will use. This is passed to the
	// menuList class's add member function.
	menuItem(char selectChar, const std::string &descriptionText, command &cmd);
	// get the menuItem selection character
	char select() const;
	// get the menuItem description
	std::string description() const;
	// execute the command
	void execute();
};

// the menuList class drives the menu. It uses a collection
// of menuItem objects to build the menu and execute commands
class menuList
{
private:
	// text for quit command
	const char QUIT = 'q';
	const std::string QUIT_TEXT = "exit the menu";
	// default menu heading. This is the text used
	// when the default menuList constructor is used
	const std::string TITLE_TEXT = "Application menu";
	// collection of meniItem objects
	std::vector<menuItem> itemList;
	// heading displayed before the menu (set by constructor)
	std::string menuText;
public:
	// use default application menu heading
	menuList();
	// specify your own menu heading
	menuList(const std::string &menuText);
	// destructor (note virtual in case we ever create
	// a subclass of menuList
	virtual ~menuList()
	{
	}
	// add an item to the list
	void add(menuItem &item);
	// start the menu process
	void start() const;
private:
	// display the menu
	void displayMenu() const;
	// read in and display the selection from the menu
	char getSelection() const;
};

#endif /* MENULIST_H_ */ 