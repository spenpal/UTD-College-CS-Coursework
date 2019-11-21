#include <iostream>
#include "phoneBookEntry.h"

//Default Constructor
phoneBookEntry::phoneBookEntry()
{
	personName = "";
	personNumber = "";
	emailAddy = "";
}

//Set Name and Number
phoneBookEntry::phoneBookEntry(const std::string &name, const std::string &number)
{
	personName = name;
	personNumber = number;
	emailAddy = "";
}

//Set Name, Number, and Email
phoneBookEntry::phoneBookEntry(const std::string &name, const std::string &number, const std::string &email)
{
	personName = name;
	personNumber = number;
	emailAddy = email;
}

//Copy Information From Class [phoneBookEntry "from"]
phoneBookEntry::phoneBookEntry(const phoneBookEntry &from)
{
	personName = from.personName;
	personNumber = from.personNumber;
	emailAddy = from.emailAddy;
}

//Return Name
std::string phoneBookEntry::name() const
{
	return personName;
}

//Return Phone Number
std::string phoneBookEntry::phoneNumber() const
{
	return personNumber;
}

//Return Email Address
std::string phoneBookEntry::email() const
{
	return emailAddy;
}

//Update Phone Number
void phoneBookEntry::phoneNumber(const std::string &newNumber)
{
	personNumber = newNumber;
}

//Update Email
void phoneBookEntry::email(const std::string &newEmail)
{
	emailAddy = newEmail;
}

