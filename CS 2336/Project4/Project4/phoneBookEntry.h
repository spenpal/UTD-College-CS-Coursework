#ifndef PHONEBOOKENTRY_H
#define PHONEBOOKENTRY_H
#include <string>
#include <iostream>

class phoneBookEntry
{
private: 
	std::string personName;
	std::string personNumber;
	std::string emailAddy;

public:
	//Constructors
	phoneBookEntry();
	phoneBookEntry(const std::string &name, const std::string &number);
	phoneBookEntry(const std::string &name, const std::string &number, const std::string &email);
	phoneBookEntry(const phoneBookEntry &from); //Copy Constructor

	//Comparsion Operators
	bool operator==(const phoneBookEntry &entry) const;
	bool operator!=(const phoneBookEntry &entry) const;
	bool operator<(const phoneBookEntry &entry) const;
	bool operator<=(const phoneBookEntry &entry) const;
	bool operator>(const phoneBookEntry &entry) const;
	bool operator>=(const phoneBookEntry &entry) const;

	//Accessors
	std::string name() const;
	std::string phoneNumber() const;
	std::string email() const;

	//Mutators
	void phoneNumber(const std::string &newNumber);
	void email(const std::string &newEmail);
};

std::ostream& operator<<(std::ostream &out, const phoneBookEntry &entry);

#endif /* PHONEBOOKENTRY_H */