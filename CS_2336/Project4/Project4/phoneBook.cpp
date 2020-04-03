#include <iostream>
#include <iomanip>

#include "phoneBook.h"

void phoneBook::insert(const phoneBookEntry &entry)
{
	//Call Find() Method To Check For If Name Exists
	if (find(entry.name()))
	{
		//Update Iterator Entry For New Entry
		*vec = entry;
	}
	else
	{
		//Create a New Entry In Vector
		entries.insert(phoneBookEntry(entry));
	}
}

void phoneBook::insert(const std::string &name, const std::string &number)
{
	//Call Find() Method To Check For If Name Exists
	if (find(name))
	{
		//Update Iterator Entry For New Number
		vec->phoneNumber(number);
	}
	else
	{
		//Create a New Entry In Vector For Name and Number
		entries.insert(phoneBookEntry(name, number));
	}
}

void phoneBook::insert(const std::string &name, const std::string &number, const std::string &email)
{
	//Call Find() Method To Check For If Name Exists
	if (find(name))
	{
		//Update Iterator Entry For New Number and Email
		vec->phoneNumber(number);
		vec->email(email);
	}
	else
	{
		//Create a New Entry In Vector For Name, Number, and Email
		entries.insert(phoneBookEntry(name, number, email));
	}
	
}

bool phoneBook::erase(std::string name)
{
	//Check if "name" exists in "entries" vector
	if (find(name))
	{
		//Name Was Found In Entries
		//Deletes Entry At Iterator "Vec" Location
		//.Erase() Method Automatically Moves All Elements Back To Fill Up The Gap
		entries.erase(vec);
		return true;
	}

	//"name" Does Not Exist In the Vector
	return false;
}

bool phoneBook::find(std::string name)
{
	//Using Iterator Function To Find Entry With Specified Name
	for (vec = begin(); vec != end(); vec++)
	{
		//Check Equality Between Input and Entry Names
		if (vec->name() == name)
		{
			//Both Names Are A Match
			return true;
		}
	}

	//Entry Name Did Not Match Input Name
	return false;
}

void phoneBook::print() const
{
	int nameSpace = 31; //"Name:" Column Width
	int phoneSpace = 16; //"Phone Number:" Column Width

	//Printing Out Titles For Each Column w/ SetW for Spaces
	std::cout << std::setw(nameSpace) << std::left << "Name:";
	std::cout << std::setw(phoneSpace) << std::left << "Phone Number:";
	std::cout << "E-Mail:" << std::endl;

	for (auto value : entries)
	{
		std::cout << std::setw(nameSpace) << std::left << value.name(); //Set Field Width, Then Prints Out Entry's Name

		std::cout << std::setw(phoneSpace) << std::left << value.phoneNumber(); //Set Field Width, Then Prints Out Entry's Phone Number

		std::cout << value.email() << std::endl; //Prints Out Entry's Email Address & Create a New Line
	}
}

void phoneBook::print(std::ostream &out) const
{
	int nameSpace = 31; //"Name:" Column Width
	int phoneSpace = 16; //"Phone Number:" Column Width

	//Printing Out Titles For Each Column w/ SetW for Spaces
	out << std::setw(nameSpace) << std::left << "Name:";
	out << std::setw(phoneSpace) << std::left << "Phone Number:";
	out << "E-Mail:" << std::endl;

	for (auto value : entries)
	{
		out << std::setw(nameSpace) << std::left << value.name(); //Set Field Width, Then Prints Out Entry's Name

		out << std::setw(phoneSpace) << std::left << value.phoneNumber(); //Set Field Width, Then Prints Out Entry's Phone Number

		out << value.email() << std::endl; //Prints Out Entry's Email Address & Create a New Line
	}
}

void phoneBook::debug(std::ostream & out) const
{
	//Print Out Unformmated Phone Book
	for (auto value : entries)
	{
		out << value.name() << value.phoneNumber() << value.email() << std::endl;
	}
}

std::size_t phoneBook::size() const
{
	//Return Size of Entries Vector
	return entries.size();
}

phoneBook::iterator phoneBook::begin()
{
	//Start Point of Entries Vector
	return entries.begin();
}

phoneBook::iterator phoneBook::end()
{
	//End Point of Entries Vector
	return entries.end();
}
