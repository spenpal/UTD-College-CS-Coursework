/*
 * linkedList.h
 *
 * Implementation of a double linked list.
 *
 * We have the ability to get the first and last entries and navigate
 * through the entries in the linked list.
 *
 * There are actually three classes here:
 *
 * listNode<DataType>
 *
 * listNodeIterator<DataType>

 * and
 *
 * linkedList<DataType>
 *
 * The listNodeIterator is the iterator for the new linked list class.
 *
 */
#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include "memoryLeak.h"

#include <iostream>
#include <iomanip>
#include <cstddef>
#include <iterator>

 // forward declaration of the template class linkedList
template<class DataType>
class linkedList;
// forward declaration of iterator
template<class DataType>
class linkedListIterator;

// listNode class
template<class DataType>
class listNode
{
	// make DoubleLinkedList<DataType> a friend class
	friend class linkedList<DataType>;
	friend class linkedListIterator<DataType>;
private:
	// contains the actual data
	DataType dataType;
	// pointer to the next item in the list or nullptr 
	// if at the end
	listNode<DataType>* pNext;
	// pointer to the previous item in the list or nullptr 
	// if at the start
	listNode<DataType>* pPrevious;
public:
	// default constructor
	listNode();
	// constructor with data
	listNode(const DataType &newItem);
protected:
	// get the next node
	listNode* next() const
	{
		return pNext;
	}

	// get the previous node
	listNode* previous() const
	{
		return pPrevious;
	}

	// return the data stored in the node as a const
	const DataType& data() const
	{
		return dataType;
	}
	// return the data stored in the node
	DataType& data()
	{
		return dataType;
	}
	// update the next node
	void next(listNode *nextNode)
	{
		pNext = nextNode;
	}
	// update the previous node
	void previous(listNode *previousNode)
	{
		pPrevious = previousNode;
	}
};

// linkedList class
template<class DataType>
class linkedList
{
public:
	// the linkedListIterator is defined as a nested class inside linkedList.
	//
	// most of this code has been provided for you. 
	// the exceptions are the operator--() operator--(int) member fuctions and
	// and the operator== and operator!= member functions. 
	class linkedListIterator
	{
		typedef DataType 								value_type;
		typedef DataType& 								reference;
		typedef const DataType& 						const_reference;
		typedef DataType* 								pointer;
		typedef const DataType* 						const_pointer;
		typedef std::bidirectional_iterator_tag			iterator_category;
		typedef std::ptrdiff_t 							difference_type;
		typedef std::size_t								size_type;
		typedef linkedListIterator						iterator;
		typedef const linkedListIterator	  			const_iterator;

		friend class linkedList<DataType>;
	
	public:
		linkedListIterator() {}

		linkedListIterator(listNode<DataType> *current)
			: current(current)
		{
		}

		virtual ~linkedListIterator()
		{
		}

		reference operator*()
		{
			return current->data();
		}

		const_reference operator*() const
		{
			return current->data();
		}

		pointer operator->()
		{
			return &(current->data());
		}

		const_pointer operator->() const
		{
			return &(current->data());
		}

		iterator operator++()
		{
			if (current != nullptr)
			{
				current = current->next();
			}
			return *this;
		}

		iterator operator++(int)
		{
			iterator it = *this;
			if (current != nullptr)
			{
				current = current->next();
			}
			return it;
		}

		iterator operator--()
		{
			if (current != nullptr)
			{
				current = current->previous();
			}
			return *this;
		}

		iterator operator--(int)
		{
			iterator it = *this;
			if (current != nullptr)
			{
				current = current->previous();
			}
			return it;
		}

		bool operator==(const_iterator &other) const
		{
			return this->current == other.current;
		}

		bool operator!=(const_iterator &other) const
		{
			return this->current != other.current;
		}

	private:
		listNode<DataType> *current;
	};
	typedef DataType 								value_type;
	typedef DataType& 								reference;
	typedef const DataType& 						const_reference;
	typedef DataType* 								pointer;
	typedef const DataType* 						const_pointer;
	typedef std::bidirectional_iterator_tag			iterator_category;
	typedef std::ptrdiff_t 							difference_type;
	typedef std::size_t								size_type;
	typedef linkedListIterator 						iterator;
	typedef const linkedListIterator	  			const_iterator;

private:
	// your instance data goes here. Think like the pointers to the first and last nodes 
	//  as well as the count of nodes
	listNode<DataType> *firstNode;
	listNode<DataType> *lastNode;
	std::size_t countOfNodes;

public:
	// default constructor
	linkedList()
	{
		countOfNodes = 0;
		firstNode = nullptr;
		lastNode = nullptr;
	}
	// copy constructor
	linkedList(const linkedList &other);
	// assignment operator
	const linkedList &operator=(const linkedList &other)
	{
		linkedList list(other);
		return list;
	}
	// destructor
	virtual ~linkedList();
	// return the number of nodes in the list
	std::size_t size() const
	{
		return countOfNodes;
	}
	// return true if the list is empty
	bool empty() const
	{
		return size() == 0;
	}
	// display the contents of the list to std::cout
	void print() const
	{
		print(std::cout);
	}
	// display the contents of the list to the ostream&
	void print(std::ostream &out) const
	{
		if (!empty())
		{
			for (auto iter = begin(); iter != end(); iter++)
			{
				out << *iter << std::endl;
			}
		}
		else
		{
			out << "List is empty." << std::endl;
		}
	}

	// iterators
	iterator begin()
	{
		return iterator(firstNode);
	}

	const_iterator begin() const
	{
		return iterator(firstNode);
	}

	iterator end()
	{
		return iterator(nullptr);
	}

	const_iterator end() const
	{
		return iterator(nullptr);
	}

	// dump the contends in debug format to passed in 
	// ostream - usage to cout would be:
	//   list.debug(std::cout);
	void debug(std::ostream &out) const;
public:
	// add an item to the front of the list
	virtual void push_front(const DataType &newItem);
	// add an item to the back of the list
	virtual void push_back(const DataType &newItem);
	// remove an item from the front of the list
	virtual void pop_front();
	// remove an item from the back of the list
	virtual void pop_back();
	// insert newItem before the node specified by the iterator
	virtual void insert_before(iterator iter, const DataType &newItem);
	// insert newItem after the node specified by the iterator
	virtual void insert_after(iterator iter, const DataType &newItem);
	// find the node and return the iterator for that element.
	// Return end() if not found
	virtual iterator find(const DataType &existingItem);
	// remove the node equal to currentItem
	virtual bool erase(const DataType &currentItem);
	// remove the node by address existingNode
	virtual iterator erase(iterator iter);
	// return the iterator for the last node
	virtual iterator back()
	{
		return linkedListIterator(lastNode);
	}
	virtual const_iterator back() const
	{
		return linkedListIterator(lastNode);
	}
protected:
private:
	// any helper functions you need should eithee be protected or private
	virtual void eraseNodes();
};

// the constructors for the listNode class are included here
// default constructor
template <class DataType>
listNode<DataType>::listNode()
	: dataType(), pNext(nullptr), pPrevious(nullptr)
{
}

// constructor with data
template <class DataType>
listNode<DataType>::listNode(const DataType &newItem)
	: dataType(newItem), pNext(nullptr), pPrevious(nullptr)
{
}

template<class DataType>
inline linkedList<DataType>::linkedList(const linkedList & other)
{
	if (!empty())
	{
		eraseNodes();
	}

	for (auto iter = other.begin(); iter != other.end(); iter++)
	{
		push_front(*iter);
	}
}

template<class DataType>
inline linkedList<DataType>::~linkedList()
{
	//std::cout << "Calling Destructor" << std::endl;
	eraseNodes();
	//std::cout << "Destructor Completed" << std::endl;
}

// note the code for the debug() function is included
// display a debug version of the list
template<class DataType>
void linkedList<DataType>::debug(std::ostream &out) const
{
	print(out);
}

// I have included a few function headers and bodies here where the syntax is odd
// Your implementation of non-inlined member funcctions needs to go there. 
// default constructor


// this is one where the C++ stynax gets very odd. Since we are returning back an interator we
// need to qualify it with the class it is in (linkedList<DataType>::iterator and we 
// have ot include the typename so the C++ comiler knows this is a typename and not some other
// template class usage. 
// find the node and return the address to the node. Return
// nullptr if not found
// other member functions go here.
template<class DataType>
inline void linkedList<DataType>::push_front(const DataType &newItem)
{
	countOfNodes++;

	listNode<DataType> *insert = new listNode<DataType>(newItem);

	if (firstNode == nullptr)
	{
		insert->next(nullptr);
		firstNode = lastNode = insert;
	}
	else
	{
		insert->next(firstNode);
		firstNode->previous(insert);
		firstNode = insert;
	}
}

template<class DataType>
inline void linkedList<DataType>::push_back(const DataType &newItem)
{
	countOfNodes++;

	listNode<DataType> *insert = new listNode<DataType>(newItem);

	if (lastNode == nullptr)
	{
		insert->previous(nullptr);
		firstNode = lastNode = insert;
	}
	else
	{
		lastNode->next(insert);
		insert->previous(lastNode);
		lastNode = insert;
	}
}

template<class DataType>
inline void linkedList<DataType>::pop_front()
{
	if (firstNode != nullptr)
	{
		listNode<DataType> *temp = firstNode;

		if (temp->next() == nullptr)
		{
			//delete temp;
			firstNode = lastNode = nullptr;
		}
		else
		{
			firstNode = firstNode->next();
			firstNode->previous(nullptr);
		}

		delete temp;
		countOfNodes--;

		if (empty())
		{
			firstNode = nullptr;
			lastNode = nullptr;
		}
	}
}

template<class DataType>
inline void linkedList<DataType>::pop_back()
{
	if (lastNode != nullptr)
	{
		listNode<DataType> *temp = lastNode;

		if (temp->previous() == nullptr)
		{
			//delete temp;
			firstNode = lastNode = nullptr;
		}
		else
		{
			lastNode = lastNode->previous();
			lastNode->next(nullptr);
		}

		delete temp;
		countOfNodes--;

		if (empty())
		{
			firstNode = nullptr;
			lastNode = nullptr;
		}
	}
}

template<class DataType>
inline void linkedList<DataType>::insert_before(iterator iter, const DataType &newItem)
{
	countOfNodes++;
	listNode<DataType> *insert = new listNode<DataType>(newItem);

	if (firstNode == nullptr)
	{
		firstNode = lastNode = insert;
	}
	else
	{
		if (iter == end())
		{
			delete insert;
			push_front(newItem);
		}
		else
		{
			for (auto it = begin(); it != end(); it++)
			{
				if (it == iter)
				{
					insert->previous((it.current)->previous());

					insert->next(it.current);

					if (it.current->previous() != nullptr)
					{
						(it.current)->previous()->next(insert);
					}
					else
					{
						firstNode = insert;
					}

					(it.current)->previous(insert);
					//break;
				}
			}

			//delete insert;
		}
	}
}

template<class DataType>
inline void linkedList<DataType>::insert_after(iterator iter, const DataType &newItem)
{
	countOfNodes++;
	listNode<DataType> *insert = new listNode<DataType>(newItem);

	if (lastNode == nullptr)
	{
		firstNode = lastNode = insert;
	}
	else
	{
		if (iter == nullptr)
		{
			delete insert;
			push_back(newItem);
		}
		else
		{
			for (auto it = begin(); it != end(); it++)
			{
				if (it == iter)
				{
					insert->previous(it.current);

					insert->next(it.current->next());

					if (it.current->next() != nullptr)
					{
						it.current->next()->previous(insert);
					}
					else
					{
						lastNode = insert;
					}

					it.current->next(insert);
					//break;
				}
			}

			//delete insert;
		}
	}
}

template<class DataType>
typename linkedList<DataType>::iterator linkedList<DataType>::find(const DataType &existingItem)
{
	for (auto iter = begin(); iter != end(); iter++)
	{
		if (*iter == existingItem)
		{
			return iter;
		}
	}	

	return nullptr;
}

// remove the node by iterator
template<class DataType>
typename linkedList<DataType>::iterator  linkedList<DataType>::erase(linkedList<DataType>::iterator iter)
{
	if (iter != end())
	{
		listNode<DataType> *erase = iter.current;	

		if (erase->previous() == nullptr && erase->next() == nullptr)
		{
			firstNode = lastNode = nullptr;
			//iter++;

			delete erase;
			countOfNodes--;
		}
		else
		{
			if (erase->previous() == nullptr)
			{
				pop_front();
			}
			else if (erase->next() == nullptr)
			{
				pop_back();
			}
			else
			{
				erase->next()->previous(erase->previous());
				erase->previous()->next(erase->next());

				delete erase;
				countOfNodes--;
			}
		}

		return iter;
	}

	return nullptr;
}

// remove the node equal to currentItem
template<class DataType>
bool linkedList<DataType>::erase(const DataType &currentItem)
{
	auto iter = erase(find(currentItem));

	if (iter != nullptr)
	{
		return true;
	}

	return false;
}

template<class DataType>
inline void linkedList<DataType>::eraseNodes()
{
	iterator iter = begin();
	while (!empty())
	{
		erase(iter);
		iter = begin();
		//print();
	}
}
// all of your member function need to be before the #endif
#endif // DOUBLELINKEDLIST_H_