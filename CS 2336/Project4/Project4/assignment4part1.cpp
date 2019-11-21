//#include <iostream>
//#include <string>
//#include "linkedList.h"
//
//int main()
//{
//	//// test push_front followed by pop_front
//	//std::cout << "Create list" << std::endl;
//	//linkedList<int> list;
//
//	//std::cout << "Print empty list" << std::endl;
//	//list.debug(std::cout);
//
//	//std::cout << "push_front(12)" << std::endl;
//	//list.push_front(12);
//
//	//std::cout << "Print list" << std::endl;
//	//list.debug(std::cout);
//
//	//std::cout << "push_front(13)" << std::endl;
//	//list.push_back(13);
//
//	//std::cout << "Print list" << std::endl;
//	//list.debug(std::cout);
//
//	//std::cout << "push_front(14)" << std::endl;
//	//list.push_front(14);
//
//	//std::cout << "Print list" << std::endl;
//	//list.debug(std::cout);
//
//	//std::cout << "pop_front" << std::endl;
//	//list.pop_front();
//
//	//std::cout << "Print list" << std::endl;
//	//list.debug(std::cout);
//
//	//std::cout << "pop_front" << std::endl;
//	//list.pop_front();
//
//	//std::cout << "Print list" << std::endl;
//	//list.debug(std::cout);
//
//	//std::cout << "pop_front" << std::endl;
//	//list.pop_front();
//
//	//std::cout << "Print list" << std::endl;
//	//list.debug(std::cout);
//	
//	std::cout << "Create list" << std::endl;
//	linkedList<int> list;
//
//	std::cout << "Print empty list" << std::endl;
//	list.print();
//
//	std::cout << "insert_after(12)" << std::endl;
//	list.insert_after(nullptr, 12);
//
//	std::cout << "Print list" << std::endl;
//	list.print();
//	
//	std::cout << "insert_after(12, 13)" << std::endl;
//	list.insert_after(nullptr, 13);
//
//	std::cout << "Print list" << std::endl;
//	list.print();
//
//	std::cout << "Erase 12" << std::endl;
//	std::cout << list.erase(12) << std::endl;
//
//	std::cout << "Print list" << std::endl;
//	list.print();
//
//	std::cout << "insert_after(12, 14)" << std::endl;
//	list.insert_after(list.find(12), 14);
//
//	std::cout << "Print list" << std::endl;
//	list.print();
//
//	std::cout << "insert_before(12, 15)" << std::endl;
//	list.insert_before(list.find(12), 15);
//
//	std::cout << "Print list" << std::endl;
//	list.print();
//
//	std::cout << "insert_before(12, 16)" << std::endl;
//	list.insert_before(list.find(12), 16);
//
//	std::cout << "Print list" << std::endl;
//	list.print();
//
//	std::cout << "Erase 12" << std::endl;
//	list.erase(12);
//
//	std::cout << "Print list" << std::endl;
//	list.print();
//
//	std::cout << "pop_front" << std::endl;
//	list.pop_front();
//
//	std::cout << "Print list" << std::endl;
//	list.print();
//
//	std::cout << "pop_back" << std::endl;
//	list.pop_back();
//
//	std::cout << "Print list" << std::endl;
//	list.print();
//	
//
//	// Pause system
//	system("pause");
//
//	return 0;
//}