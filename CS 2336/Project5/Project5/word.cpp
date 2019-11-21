#include "word.h"

word::word()
{
	nKey = "";
}

word::word(std::string theWord)
{
	vecKey.push_back(theWord);
	nKey = theWord;

	for (unsigned int i = 0; i < nKey.length(); i++)
	{
		nKey[i] = static_cast<char>(std::tolower(nKey[i]));
	}

	std::sort(nKey.begin(), nKey.end());
}

std::string word::words() const
{
	std::string words = "";

	for (unsigned int i = 0; i < vecKey.size(); i++) 
	{
		words += vecKey[i];
		if (i < vecKey.size() - 1) 
		{
			words += " "; //or use ", "
		}
	}

	return words;
}

std::string word::name() const
{
	return vecKey[0];
}

std::string word::key() const
{
	return nKey;
}

std::size_t word::size() const
{
	return vecKey.size();
}

bool word::operator==(const word & other)
{
	return nKey.compare(other.key()) == 0;
}

bool word::operator<(const word & other)
{
	return nKey.compare(other.key()) > 0;
}

bool word::operator>(const word & other)
{
	return nKey.compare(other.key()) < 0;
}

void word::operator<=(const word & other)
{
	vecKey.push_back(other.name());
}
