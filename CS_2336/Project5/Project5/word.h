#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>

class word
{
	public:
		// Default constructor
		word();
		// Constructor w/ Word
		word(std::string theWord);
		// Returns the first word
		std::string name() const;
		// Returns all the words in the vector
		std::string words() const;
		// Returns the key for the word
		std::string key() const;
		// Checks for equality with other
		std::size_t size() const;
		//Size of vecKey
		bool operator==(const word &other);
		// Checks for less than other
		bool operator<(const word &other);
		// Checks for greater than other
		bool operator>(const word &other);
		// Pushes back other value
		void operator<=(const word &other);

	private:
		std::string nKey;
		std::vector<std::string> vecKey;
};


inline std::ostream& operator<<(std::ostream &out, const word outputWord)
{
	return out << "[" << outputWord.key() << ", {" << outputWord.words() << "}]" << std::endl;
}