#ifndef PHONEBOOKENTRY_H
#define PHONEBOOKENTRY_H
#include <string>

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

	//Accessors
	std::string name() const;
	std::string phoneNumber() const;
	std::string email() const;

	//Mutators
	void phoneNumber(const std::string &newNumber);
	void email(const std::string &newEmail);
};

#endif /* PHONEBOOKENTRY_H */