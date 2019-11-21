#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#include "phoneBookEntry.h"
#include "orderedLinkedList.h"

#include <iostream>

class phoneBook
{
public:
	//Constructor
	phoneBook() {};
	
	//Insert Functions
	void insert(const phoneBookEntry &entry);
	void insert(const std::string &name, const std::string &number);
	void insert(const std::string &name, const std::string &number, const std::string &email);
	
	//Mutator Functions
	bool erase(std::string name);
	bool find(std::string name);

	//Print Functions
	void print() const;
	void print(std::ostream &out) const;
	
	//Debug Function
	void debug(std::ostream &out) const;

	//Size Of Entries Function
	std::size_t size() const;

	//Iterators
	typedef orderedLinkedList<phoneBookEntry> phoneBookEntryList;
	typedef phoneBookEntryList::iterator iterator;
	typedef phoneBookEntryList::const_iterator const_iterator;

	iterator begin();
	iterator end();

private:
	//linkedList
	phoneBookEntryList entries;

	//Iterator To Point to Classes In Vector "entries"
	iterator vec;
};

#endif /* PHONEBOOK_H */