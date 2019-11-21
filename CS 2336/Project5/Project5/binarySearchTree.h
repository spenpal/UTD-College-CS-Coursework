#pragma once

#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>

// forward declaration of the template class binarySearchTree
template<class DataType>
class binarySearchTree;

// treeNode class
template <class DataType>
class treeNode
{
	friend class binarySearchTree<DataType>;

	private:
		DataType key;
		treeNode<DataType> *left;
		treeNode<DataType> *right;

	public:
		//Default constructor
		treeNode() 
		{
			left = nullptr;
			right = nullptr;
		}

		//Copy Constructor
		treeNode(const DataType &dataItem, treeNode *leftPtr, treeNode *rightPtr) 
		{
			key = dataItem;
			left = leftPtr;
			right = rightPtr;
		}

};

// binarySearchTree class
template <class DataType>
class binarySearchTree
{
	private:
		//Variables
		std::size_t numOfNodes;
		treeNode<DataType> *root;

		//Helper Functions
		void copyTree(treeNode<DataType> *&headNode, treeNode<DataType> *otherNode); //Copies Tree From OtherNode Into This Tree

		void inorder(treeNode <DataType>* p, void(*itemFound)(const DataType& foundItem)) const; //Prints Out Tree In Inorder Traversal
		void preorder(treeNode <DataType>* p, void(*itemFound)(const DataType& foundItem)) const; //Prints Out Tree In Preorder Traversal
		void postorder(treeNode <DataType>* p, void(*itemFound)(const DataType& foundItem)) const; //Prints Out Tree In Postorder Traversal

		void destroy(treeNode<DataType>* &p); //Deletes Everything In "P" Tree
		void deleteFromTree(treeNode<DataType>* &p); //Deletes "P" Node In The Tree

		void customPrint(treeNode<DataType>* p, std::ostream &out) const;

		//Other Variables
		treeNode<DataType> *varNode = nullptr;

	public:
		//Constructor
		binarySearchTree();

		//Copy Constructor
		binarySearchTree(const binarySearchTree &other);

		//Assignment Operator
		const binarySearchTree& operator=(const binarySearchTree &other)
		{
			//Avoid Self-Copy
			if (this != &other)
			{
				destroy(root);

				//Check if otherTree is empty
				if (other.root == nullptr)
				{
					root = nullptr;
					numOfNodes = 0;
				}
				//Finally, copy otherTree into this Tree
				else
				{
					copyTree(root, other.root);
					numOfNodes = other.numOfNodes;
				}
			}
		}

		//Destructor
		virtual ~binarySearchTree();

		// The empty member function will return true if the binary tree is empty.
		bool empty() const;

		// Returns the number of items in the binary tree.
		std::size_t size() const;

		// Display the contents of the binary tree in order by key (inorder traversal).
		void print() const;

		// print to ostream
		void print(std::ostream &out) const;

		//Display the contents of the binary tree for debugging purposes.
		void debug(std::ostream &out) const;

		// The first parameter passed to the find member function is the item we are looking for. 
		// The second parameter is the address of a function that will be called if the searchItem is found in the binary tree collection. 
		// The found item is passed to the function. If the item is found, the function is called, and the find member function will return true. 
		// If the item is not found, the function is not called, and find returns false.
		bool find(const DataType &searchItem, void(*foundNode)(const DataType&));

		// The erase member function will remove the specified item from the binary tree (if it finds the entry in the tree).
		// The erase member function will return true if the item was found and removed and false if it was not found.
		bool erase(const DataType &deleteItem);

		// Insert the item into the binary tree. 
		// If the item already exists the duplicateItemFound function will be called. This is the function passed as the 2nd parameter to the insert function.
		void insert(const DataType &newItem);

		// Insert the item into the binary tree. If the entry already exists it should be replaced by the new one.
		void insert(const DataType &newItem, void(*duplicateItemFound)(DataType &existingItem, const DataType &newItem));

		// The traverse function will do an "inorder" traversal of the binary tree. 
		// For every item found it will call the itemFound function and pass it a const reference to the found item.
		void traverse(void(*itemFound)(const DataType& foundItem)) const;

		// traverse preorder
		void traversePreOrder(void(*itemFound)(const DataType& foundItem)) const;

		// traverse postorder
		void traversePostOrder(void(*itemFound)(const DataType& foundItem)) const;

		//Return treeNode "varNode" for key values
		DataType loc() const
		{
			return varNode->key;
		}
};

/*
	STATIC FUNCTIONS
*/

// processFound class?
template <class DataType>
static void processFound(const DataType &item) 
{
	//Useless Static Function
}

// Get a modifiable version of the found item (existingItem) and a const reference to the new item.
template <class DataType>
static void update(DataType &existingItem, const DataType &newItem) 
{
	existingItem <= newItem;
}

// Change existingItem into newItem.
template <class DataType>
static void replace(DataType &existingItem, const DataType &newItem) 
{
	existingItem = newItem;
}

// Print out the item
template <class DataType>
static void itemFound(const DataType &item)
{
	std::cout << item << " ";
}

// printItemFound class outputs the item with custom ostream
template <class DataType>
static void customItemFound(const DataType &item, std::ostream &out) 
{
	out << item << " ";
}

/*
	MAIN FUNCTIONS
*/


template<class DataType>
binarySearchTree<DataType>::binarySearchTree()
{
	root = nullptr;
	numOfNodes = 0;
}

template<class DataType>
binarySearchTree<DataType>::binarySearchTree(const binarySearchTree &other)
{
	//Clearing Current Tree
	destroy(root);

	//Check if "other" tree is empty, otherwise do a deep copy
	if (other.empty())
	{
		root = nullptr;
		numOfNodes = 0;
	}
	else
	{
		//Helper Function
		copyTree(root, other.root);
		numOfNodes = other.numOfNodes;
	}
}

template<class DataType>
binarySearchTree<DataType>::~binarySearchTree()
{
	destroy(root);
}

template<class DataType>
bool binarySearchTree<DataType>::empty() const
{
	return root == nullptr;
}

template<class DataType>
std::size_t binarySearchTree<DataType>::size() const
{
	return numOfNodes;
}

template<class DataType>
void binarySearchTree<DataType>::print() const
{
	print(std::cout);
}

template<class DataType>
void binarySearchTree<DataType>::print(std::ostream &out) const
{
	customPrint(root, out);
}

template<class DataType>
void binarySearchTree<DataType>::debug(std::ostream &out) const
{
	// Prints the top item
	if (root != nullptr)
	{
		out << root->key;

	}
}

template<class DataType>
bool binarySearchTree<DataType>::find(const DataType &searchItem, void(*foundNode)(const DataType &))
{
	//Make temp's address equal to root's address
	treeNode<DataType> *temp = nullptr;
	bool found = false;

	//Check if tree is empty
	if (empty())
	{
		std::cout << "Tree is empty" << std::endl;
	}
	else
	{
		temp = root;

		//Running a loop to find "searchItem". If "temp != nullptr", then proceed to find "searchItem"
		while (temp != nullptr && !found)
		{
			//Check if temp variable is equal to searchItem and if so, return true
			if (temp->key == searchItem)
			{
				foundNode(searchItem);
				found = true;
			}
			//If searchItem is less than var key, then search left side of the node
			else if (temp->key > searchItem)
			{
				temp = temp->left;
			}
			//else search right side of the node
			else
			{
				temp = temp->right;
			}
		}
	}

	//Get the location and key info from find function
	varNode = temp;

	return found;
}

template<class DataType>
bool binarySearchTree<DataType>::erase(const DataType & deleteItem)
{
	treeNode<DataType> *temp; //Pointer to traverse the tree
	treeNode<DataType> *trailTemp; //Pointer behind temp
	bool found = false;

	//Check if tree is empty
	if (empty())
	{
		std::cout << "Nothing to delete from an empty tree" << std::endl;
	}
	else
	{
		temp = root;
		trailTemp = root;

		//Find if deleteItem is in the tree
		while (temp != nullptr && !found)
		{
			if (temp->key == deleteItem)
			{
				found = true;
			}
			else
			{
				trailTemp = temp;

				if (temp->key > deleteItem)
				{
					temp = temp->left;
				}
				else
				{
					temp = temp->right;
				}
			}
		}

		//Check if temp found the deleteItem
		if (temp == nullptr)
		{
			std::cout << "Item does not exist in the tree" << std::endl;
		}
		//Do operations if deleteItem is found in the tree
		else if (found)
		{
			if (temp == root)
			{
				deleteFromTree(root);
			}
			else if(trailTemp->key > deleteItem)
			{
				deleteFromTree(trailTemp->left);
			}
			else
			{
				deleteFromTree(trailTemp->right);
			}
		}
		//Else statement if deleteItem is not in tree
		else
		{
			std::cout << "Item does not exist in the tree" << std::endl;
		}
	}

	return found;
}

template<class DataType>
void binarySearchTree<DataType>::insert(const DataType & newItem)
{
	//Using other insert function and plugging in another update function when duplicate is found
	insert(newItem, replace);
}

template<class DataType>
void binarySearchTree<DataType>::insert(const DataType &newItem, void(*duplicateItemFound)(DataType &existingItem, const DataType &newItem))
{
	treeNode<DataType> *temp; //Pointer to traverse the tree
	treeNode<DataType> *trailTemp = nullptr; //Pointer behind temp
	treeNode<DataType> *insertNode = new treeNode<DataType>(newItem, nullptr, nullptr);

	// Check if tree is empty
	if (empty())
	{
		// If empty, set root as insertNode
		root = insertNode;
	}
	else 
	{
		temp = root;
		
		while (temp != nullptr)
		{
			trailTemp = temp;

			if (temp->key == newItem)
			{
				duplicateItemFound(temp->key, newItem);
				return;
			}
			else if(temp->key > newItem)
			{
				temp = temp->left;
			}
			else
			{
				temp = temp->right;
			}
		}

		if(trailTemp->key > newItem)
		{
			trailTemp->left = insertNode;
		}
		else
		{
			trailTemp->right = insertNode;
		}
	}

	//Increase numOfNodes
	numOfNodes++;
}

template<class DataType>
void binarySearchTree<DataType>::traverse(void(*itemFound)(const DataType &foundItem)) const
{
	//Using helper function to inorder traversal through the tree
	inorder(root, itemFound);
}

template<class DataType>
inline void binarySearchTree<DataType>::traversePreOrder(void(*itemFound)(const DataType &foundItem)) const
{
	preorder(root, itemFound);
}

template<class DataType>
inline void binarySearchTree<DataType>::traversePostOrder(void(*itemFound)(const DataType &foundItem)) const
{
	postorder(root, itemFound);
}


/*
	HELPER FUNCTIONS
*/

template<class DataType>
void binarySearchTree<DataType>::copyTree(treeNode<DataType> *&headNode, treeNode<DataType> *otherNode)
{
	//Check if "otherNode" is nullptr and if it is, set "headNode" to nullptr, as well | Otherwise, proceed deep copying
	if (otherNode == nullptr)
	{
		headNode = nullptr;
	}
	else
	{
		//Create new treeNode
		headNode = new treeNode<DataType>;
		//Assign same key to headNode as otherNode key
		headNode->key = otherNode->key;
		//Copy for left and right nodes of the tree
		copyTree(headNode->left, otherNode->left);
		copyTree(headNode->right, otherNode->right);
	}
}

template<class DataType>
inline void binarySearchTree<DataType>::inorder(treeNode <DataType>* p, void(*itemFound)(const DataType &foundItem)) const
{
	//If root isn't a nullptr, then print out elements in inorder traversal form
	if (p != nullptr)
	{
		inorder(p->left, itemFound);
		//Run Static Function To Print Out Key
		itemFound(p->key);
		inorder(p->right, itemFound);
	}
}

template<class DataType>
inline void binarySearchTree<DataType>::preorder(treeNode<DataType>* p, void(*itemFound)(const DataType &foundItem)) const
{
	//If root isn't a nullptr, then print out elements in preorder traversal form
	if (p != nullptr)
	{
		//Run Static Function To Print Out Key
		itemFound(p->key);
		preorder(p->left, itemFound);
		preorder(p->right, itemFound);
	}
}

template<class DataType>
inline void binarySearchTree<DataType>::postorder(treeNode<DataType>* p, void(*itemFound)(const DataType &foundItem)) const
{
	//If root isn't a nullptr, then print out elements in postorder traversal form
	if (p != nullptr)
	{
		postorder(p->left, itemFound);
		postorder(p->right, itemFound);
		//Run Static Function To Print Out Key
		itemFound(p->key);
	}
}

template<class DataType>
void binarySearchTree<DataType>::destroy(treeNode<DataType>* &p)
{
	//Destroy All Nodes in the Tree Using Postorder Traversal
	if (p != nullptr)
	{
		destroy(p->left);
		destroy(p->right);
		delete p;
		p = nullptr;
	}
}

template<class DataType>
void binarySearchTree<DataType>::deleteFromTree(treeNode<DataType>* &p)
{
	treeNode<DataType> *temp; //Pointer to traverse the tree
	treeNode<DataType> *trailTemp; //Pointer behind temp
	treeNode<DataType> *deleteNode; //pointer to delete the node

	if (p == nullptr)
	{
		std::cout << "The deleteNode does not exist." << std::endl;
	}
	else if(p->left == nullptr && p->right == nullptr)
	{
		deleteNode = p;
		p = nullptr;
		delete deleteNode;
		numOfNodes--;
	}
	else if (p->left == nullptr)
	{
		deleteNode = p;
		p = deleteNode->right;
		delete deleteNode;
		numOfNodes--;
	}
	else if (p->right == nullptr)
	{
		deleteNode = p;
		p = deleteNode->left;
		delete deleteNode;
		numOfNodes--;
	}
	else
	{
		temp = p->left;
		trailTemp = nullptr;

		while (temp->right != nullptr)
		{
			trailTemp = temp;
			temp = temp->right;
		}

		p->key = temp->key;

		if (trailTemp == nullptr)
		{
			p->left = temp->left;
		}
		else
		{
			trailTemp->right = temp->left;
		}

		delete temp;
		numOfNodes--;
	}
}

template<class DataType>
void binarySearchTree<DataType>::customPrint(treeNode<DataType>* p, std::ostream & out) const
{
	//If root isn't a nullptr, then print out elements in inorder traversal form
	if (p != nullptr)
	{
		customPrint(p->left, out);
		out << p->key << std::endl;
		customPrint(p->right, out);
	}
}
