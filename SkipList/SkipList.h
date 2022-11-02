/*
* SkipList.h
* Sarah Asad
* Student #: 2150439
* CSS 342 - Stiber
* Program 2 - STL compliant SkipList
* 5/1/21
*
* This is the header file for the SkipList.cpp class which declares all the methods and member variables used in the SkipList.cpp file.
*/

#pragma once
#include <random>
#include <vector>
#include <string>
#include <ostream>
#include <iostream>
#include <iterator>
#include <cstddef>

using namespace std;

//template for the SkipList class so any generic type T can be used in SkipList
template<typename T>
class SkipList {

	struct SkipListNode {
		//sets data_ to parameter data and sets all pointers to nullptr
		explicit SkipListNode(T data); 
		T data_; //the data that each node holds 
		SkipListNode* next_; //next pointer on the same level
		SkipListNode* prev_; //previous pointer on the same level 
		SkipListNode* upLevel_; //pointer to upper node 
		SkipListNode* downLevel_; //pointer to lower node
	};

	//the number of levels in the SkipList decided at construction time 
	int maxLevel_;
	SkipListNode** heads_; //array of head nodes
	SkipListNode** tails_; //array of tail nodes 

	/*
	* Parameters: generic data type which is the node being added, and a vector which holds the data for the path the node took to get to level 0
	* Returns: bool, true if successfully found a spot. false if there is a duplicate
	*
	* this method is a helper method for the insert method and does all the searching work. traverses through the list and finds the correct spot
	* to insert the new item being added.
	*/
	bool findInsertionPoint(T newData, std::vector<SkipList<T>::SkipListNode*>& parents);
	/*
	* no reurn value (void)
	* Parameters: generic data type which is the current level node is being added to , SkipListNode* which is the new node being added, and
	* another SkipListNode* which is the node the new node is inserted before.
	*
	* takes a new node to be inserted and adds it before the next node. checks to see which level the work is being done on and manipulates the
	* pointers to add the new node in. takes care of the case where it is the first node in the level and modifies the head in that case.
	*/
	void addBefore(int currentLevel, SkipListNode* newNode, SkipListNode* nextNode);
	/*
	* no reurn value (void)
	* Parameters: generic data type which is the current level node is being added to , SkipListNode* which is the new node being added, and
	* another SkipListNode* which is the node the new node is inserted after.
	*
	* takes a new node to be inserted and adds it after the before node. checks to see which level the work is being done on and manipulates the
	* pointers to add the new node in. takes care of the case where it is the last node in the level and modifies the tail in that case.
	*/
	void addAfter(int currentLevel, SkipListNode* newNode, SkipListNode* beforeNode);
	/*
	* no parameters
	* returns: bool
	*
	* uses a random generator to "flip a coin" returns true 50% of the time and false 50% of the time.
	* this method is called by the insert method to determine if a SkipListNode being inserted should be added to
	* consecutive higher levels.
	*/
	bool alsoHigher() const;

public:

	/*
	* parameters - SkipList& on the right hand side
	* copy constructor takes a passed in skiplist and copies/inserts all the elements in level 0 of that SkipList into another skiplist object.
	*/
	SkipList(const SkipList& other);  
	/*
	* parameters: an initialized skipList& representing the right hand side
	* returns: a Skiplist& representing the left hand side
	* copy assignmnet operator - takes the lhs SkipList and assigns it to the values in level 0 of the rhs SkipList
	*/
	SkipList& operator=(const SkipList& rhs);
	/*
	* Parameter - SkipList&& - rhs SkipLsit
	* move constructor - takes all emelements from level 0 of the rhs SkipList and moves it to level 0 of the lhs SKipList
	*/
	SkipList(SkipList&& other); 
	/*
	* parameters: SkipList&& which respresents the rhs SkipList
	* returns: SkipList& which is the lhs SkipList
	* move assignment operator - which clears the lhs, deletes the heads and tails of the lhs. and sets the new values corresponding to the rhs SkipList
	* in the end nulls out the rhs
	*/
	SkipList& operator=(SkipList&& other);

	/*
	* parameters: an initialized SkipList
	* initializer list constructor - takes a SkipList and inserts all the values
	*/
	SkipList(initializer_list<T> other);

	/*
	* parameter: SkipList which represents the SkipList on the right hand side
	* returns: bool. true if equal - false otherwise
	* compares 2 SkipLists at level 0 and returns true if they are the same and flase otherwise
	*/
	bool operator==(const SkipList& rhs) const;
	/*
	* parameter: SkipList which represents the SkipList on the right hand side
	* returns: bool. false if equal - true otherwise
	* compares 2 SkipLists at level 0 and returns false if they are the same and true otherwise
	*/
	bool operator != (const SkipList& rhs) const;

	//allocates heads_ and tails_ arrays to maxlevel and sets all elements of heads_ and tails_ array to nullptr
	explicit SkipList(int maxLevels = 1);
	//destructor which deletes the head and tails arrrays and sets all pointers to nullptr 
	virtual ~SkipList<T>();

	/*
	* Parameters: generic data type which is the new value being added to the skipList
	* Returns: bool: true if successfully added and false if duplicate exists which is predetermined by the findInsertionPoint method
	*
	* this method makes use of the findInsertionPoint by checking for duplicaes and insetion point method. it also calls the addBefore and
	* addAfter method which is determined by the prevOrNext node which compares values and calls either of these methods depending on if
	* the new item should be inserted before or after that node. this method also calls the alsoHigher method which determines if a node added
	* at level 0 should be also added to consecutive higher levels.
	*/
	bool insert(const T item);
	/*
	* Parameter: generic data type
	* Returns: bool
	*
	* takes an data type and removes nodes that contain that value from the SkipList. traverses through looking for item in all levels, rearranges pointers, 
	* and deallocates memory associated with the nodes. returns true if successfully removed. returns false if list is empty or if node does 
	* not exist in the SkipList
	*/
	bool erase(const T item);
	/*
	* parameter: generic data type - the item being searched for
	* Return value: bool
	*
	* given an primitive type to search for, traverses through the list starting at the highest level non-null head pointer makes its way down levels
	* until it either find the node it is searching for. in that case it returns true. if the list is empty or the value doesnt exist in the list,
	* reutrns false.
	*/
	bool contains(const T item) const;

	/*
	* parameters: none
	* returns: bool - true if empty and false if not.
	* checks the head node at level 0. if the node is null that means the list is empty and to return true. else, return false.
	*/
	bool empty() const;
	/*
	* takes no parameters and has no return value
	* clears a SKiplist of al its nodes and sets all the nodes to nullptr
	*/
	void clear();
	/*
	* parameters: none
	* reutrns: int - the number of elements in level 0
	* iterates through level 0 of the SkipList and counts the number of elements in that level.
	*/
	int size() const;

	/*
	* Parameters: ostream& which is used for printing and SkipList& which gives acces to all the skiplist memebrs
	* Reutrns: ostream& which is used to print to the console
	*
	* prints the contents of the Skiplist in the form:
	*
	* Level: 4 -- empty
	* Level: 3 -- 93
	* Level: 2 -- 35, 93
	* Level: 1 -- 3, 23, 35, 67, 93
	* Level: 0 -- 3, 7, 23, 35, 67, 68, 72, 87, 93
	*
	* Prints "empty" if the level does not contain any nodes.
	*/
	template<typename U>
	friend ostream& operator<<(ostream& os, const SkipList<U>& list);

	class iterator 	{
		//represents the current node
		const SkipListNode* currentNode;

	public:
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = const T*;
		using reference = const T&;

		//finds the starting noode at level 0
		iterator(const SkipListNode* startingNode) {
			currentNode = startingNode;
		}
		//finds the node to the right of the starting node
		iterator(const iterator& rhs) {
			currentNode = rhs.currentNode;
		}
		//overloaded equal operator 
		bool operator==(const iterator& rhs) const {
			return (rhs.currentNode == currentNode);
		}
		//overloaded does not equal operator 
		bool operator!=(const iterator& rhs) const {
			return rhs.currentNode != currentNode;
		}
		//overloaded pointer operator 
		const T& operator*() {

			return currentNode->data_;
		}

		//overloaded increment operator - prefix
		iterator& operator++() {

			currentNode = currentNode->next_;
			return *this;
		}
		//overloaded increment operator - postfix 
		iterator operator++(int) {
			iterator hold = *this;
			currentNode = currentNode->next_;
			return iterator(hold);
		}
	};
	//returns an iterator pointing to the first node in level 0 of the SkipList 
		iterator begin() const {
		return iterator(heads_[0]);
	}
	//returns an iterator pointing to one after the last node in level 0 of the SkipList 
	//points to nullptr
	iterator end() const {
		SkipListNode* node = nullptr;
		return iterator(node);
	}
};

#include "SkipList.cpp"