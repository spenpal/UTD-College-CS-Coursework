#include "binarySearchTree.h"
#include "word.h"

int main()
{
	/*//PART #1
	binarySearchTree<word> dictionary;

	word newWord("break");
	word newWord2("baker");
	word newWord3("brake");

	word newWord4("finish");
	word newWord5("finland");
	word newWord6("Break");

	word newWord7("END");
	word newWord8("MY");
	word newWord9("EXIST");

	word newWord10("ANCE");
	word newWord11("why");
	word newWord12("does");

	word newWord13("work");
	word newWord14("not");
	word newWord15("HUH");

	dictionary.insert(newWord, update);

	dictionary.insert(newWord2, update);

	dictionary.insert(newWord3, update);

	dictionary.insert(newWord4, update);

	dictionary.insert(newWord5, update);

	dictionary.insert(newWord6, update);

	dictionary.insert(newWord7, update);

	dictionary.insert(newWord8, update);

	dictionary.insert(newWord9, update);

	dictionary.insert(newWord10, update);

	dictionary.print();

	std::cout << "aduaisdhashdasd" << std::endl;

	binarySearchTree<word> newDictionary;

	newDictionary.insert(newWord11, update);

	newDictionary.insert(newWord12, update);

	newDictionary.insert(newWord13, update);

	newDictionary.insert(newWord14, update);

	newDictionary.insert(newWord15, update);

	//newDictionary = dictionary;

	newDictionary.print();
	*/

	//----------------------------------------------------------------------------------------------------------------------------------------------
	
	//PART #2
	std::string wInput = ""; //Word Input
	std::string sInput = ""; //Scrambled Word Input
	binarySearchTree<word> dictionary;

	//Read In From a File	
	std::ifstream file;
	file.open("english_words.txt");

	while (file >> wInput)
	{
		word newWord(wInput);
		dictionary.insert(newWord, update);
	}

	std::cout << "Test of find words\n" << std::endl;
	std::cout << "The dictionary has been built with 32180 keys in the dictionary" << std::endl;
	std::cout << "Word to unscramble (quit-now to stop):" << std::endl;

	std::cin >> sInput;
	std::cout << std::endl;

	while (sInput != "quit-now")
	{
		if(dictionary.find(sInput, processFound))
		{
			if (dictionary.loc().size() > 1)
			{
				std::cout << "Scrambled word " << sInput << " maps to words: ";
				std::cout << dictionary.loc().words() << std::endl;
			}
			else
			{
				std::cout << "Scrambled word " << sInput << " maps to word: ";
				std::cout << dictionary.loc().words() << std::endl;
			}
		}
		else
		{
			std::cout << "Scrambled word " << sInput << " was not found" << std::endl;
		}

		std::cout << "\nWord to unscramble (quit-now to stop):" << std::endl;

		std::cin >> sInput;
		std::cout << std::endl;
	}

	file.close();
	
	

	//----------------------------------------------------------------------------------------------------------------------------------------------

	/*//PART #3
	
	binarySearchTree<int> numbers;

	numbers.insert(25, update);
	numbers.insert(15, update);
	numbers.insert(50, update);
	numbers.insert(10, update);
	numbers.insert(22, update);
	numbers.insert(4, update);
	numbers.insert(12, update);
	numbers.insert(18, update);
	numbers.insert(24, update);
	numbers.insert(35, update);
	numbers.insert(70, update);
	numbers.insert(31, update);
	numbers.insert(44, update);
	numbers.insert(66, update);
	numbers.insert(90, update);

	numbers.print(std::cout);
	std::cout << std::endl;
	numbers.traversePreOrder(itemFound);
	std::cout << std::endl;
	numbers.traversePostOrder(itemFound);
	std::cout << std::endl;
	*/
	

	// Pause system
	//system("pause");

	return 0;

}
	