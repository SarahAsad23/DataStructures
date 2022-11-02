/*
* SkipList.h
* Sarah Asad
* Student #: 2150439
* CSS 342 - Stiber
* Program 2 - STL compliant SkipList
* 5/1/21
*
* This DkipList program is templated and allows any primitive type to be added into the SkipList. It creates a dynamically allocated heads_ and tails_ 
* array which are used as entry points to the internal storage of the SkipList. the internal part of the SkipList is made up of SKipListNodes that have 
* the ability of holding variouds data types corresponding to the levels of the heads_ and tails_ array. this program has the capibilities of inserting 
* a new item into the SkipList, searching the SkipList to see if it contains a certain item, and erasing/removing an item from the SkipList. it also has 
* the capibilites to clean an entire SkipList, return the size of the SkipList and check if a SkipList is empty. 
*/

#include "SkipList.h"

//sets data_ to parameter data and sets all pointers to nullptr
template <typename T>
SkipList<T>::SkipListNode::SkipListNode(T data) {
	data_ = data;
	next_ = nullptr;
	prev_ = nullptr;
	upLevel_ = nullptr;
	downLevel_ = nullptr;
}

//allocates heads_ and tails_ arrays to maxlevel and sets all elements of heads_ and tails_ array to nullptr
template<typename T>
SkipList<T>::SkipList(int maxLevel) {
	maxLevel_ = maxLevel;

	heads_ = new SkipListNode * [maxLevel];
	tails_ = new SkipListNode * [maxLevel];

	for (int i = 0; i < maxLevel_; i++) {
		heads_[i] = tails_[i] = nullptr;
	}
}

//destructor which deletes the head and tails arrrays and sets all pointers to nullptr 
template <typename T>
SkipList<T>::~SkipList()
{
	for (int i = 0; i < maxLevel_; i += 1)
	{
		if (heads_[i] != nullptr)
		{
			SkipListNode* node = heads_[i];
			while (node != nullptr)
			{
				SkipListNode* nodeToBeDeleted = node;
				node = node->next_;
				delete nodeToBeDeleted;
			}
		}
	}

	delete[] heads_;
	delete[] tails_;
}

/*
* parameters: an initialized SkipList
* initializer list constructor - takes a SkipList and inserts all the values
*/
template <typename T>
SkipList<T>::SkipList(initializer_list<T> other) {
	//sets max levels and creates new head and tail arrays 
	maxLevel_ = 3;
	heads_ = new SkipListNode * [maxLevel_];
	tails_ = new SkipListNode * [maxLevel_];

	//nulls out all pointers of heads and tails 
	for (int i = 0; i < maxLevel_; i++) {
		heads_[i] = tails_[i] = nullptr;
	}

	//inserts all the vlaues 
	for (T value : other) {
		insert(value);
	}
}

/*
* parameters - SkipList& on the right hand side
* copy constructor takes a passed in skiplist and copies/inserts all the elements in level 0 of that SkipList into another skiplist object.
*/
template <typename T>
SkipList<T>::SkipList(const SkipList<T>& other) {

	// Initlaize this class to have the same levels as the other
	if (this != &other) {

		maxLevel_ = other.maxLevel_;

		//creates heads and tails array and sets the number of level 
		heads_ = new SkipListNode * [maxLevel_];
		tails_ = new SkipListNode * [maxLevel_];

		// nulls out head and tail nodes 
		for (int i = 0; i < maxLevel_; i++) {
			heads_[i] = tails_[i] = nullptr;
		}

		//loops through the other SkipList using the iterator and inserts each value into the new SkipLsit
		for (SkipList<T>::iterator i = other.begin(); i != other.end(); i++) {
			insert(*i);
		}
	}
}

/*
* parameters: an initialized skipList& representing the right hand side
* returns: a Skiplist& representing the left hand side
* copy assignmnet operator - takes the lhs SkipList and assigns it to the values in level 0 of the rhs SkipList
*/
template<typename T>
SkipList<T>& SkipList<T>::operator=(const SkipList<T>& rhs) {
	//if lhs doesnt already equal the rhs 
	if (this != &rhs) {
		//clear the lhs and delete the head and tail arrays 
		this->clear();
		delete[] this->heads_;
		delete[] this->tails_;

		// Initlaize this class to have the same levels as the rhs
		maxLevel_ = rhs.maxLevel_;

		//creates heads and tails array and sets the number of level 
		heads_ = new SkipListNode * [maxLevel_];
		tails_ = new SkipListNode * [maxLevel_];

		// nulls out head and tail nodes 
		for (int i = 0; i < maxLevel_; i++) {
			heads_[i] = tails_[i] = nullptr;
		}

		//loops through the other SkipList using the iterator and inserts each value into this
		for (SkipList<T>::iterator i = rhs.begin(); i != rhs.end(); i++) {
			insert(*i);
		}
		return *this;
	}

	return *this;
}

/*
* Parameter - SkipList&& - rhs SkipLsit
* move constructor - takes all emelements from level 0 of the rhs SkipList and moves it to level 0 of the lhs SKipList
*/
template <typename T>
SkipList<T>::SkipList(SkipList&& other) {
	//set lhs max levels of heads and tails arrays for the lhs based on levels in rhs 
	maxLevel_ = other.maxLevel_;
	heads_ = other.heads_;
	tails_ = other.tails_;

	//null out head ans tail arrays for the rhs 
	other.maxLevel_ = 0;
	other.heads_ = nullptr;
	other.tails_ = nullptr;
}

/*
* parameters: SkipList&& which respresents the rhs SkipList
* returns: SkipList& which is the lhs SkipList
* move assignment operator - which clears the lhs, deletes the heads and tails of the lhs. and sets the new values corresponding to the rhs SkipList
* in the end nulls out the rhs
*/
template<typename T>
SkipList<T>& SkipList<T>::operator=(SkipList<T>&& other) {
	//if the lhs does not alread equal the rhs 
	if (this != &other) {
		//clear the lhs and delete the head and tail arrays 
		this->clear();
		delete[] heads_;
		delete[] tails_;

		//set the maxlevels of the lhs SkipList using the max levels of the rhs
		maxLevel_ = other.maxLevel_;
		heads_ = other.heads_;
		tails_ = other.tails_;

		//set the rhs's heads and tails arrays to nullptr
		other.maxLevel_ = 0;
		other.heads_ = nullptr;
		other.tails_ = nullptr;
	}

	return *this;
}

/*
* parameter: SkipList which represents the SkipList on the right hand side
* returns: bool. true if equal - false otherwise
* compares 2 SkipLists at level 0 and returns true if they are the same and flase otherwise
*/
template <typename T>
bool SkipList<T>::operator==(const SkipList& rhs) const {

	if (this->size() != rhs.size()) {
		return false;
	}
	for (SkipList<T>::iterator i = begin(), j = rhs.begin(); i != end() && j != rhs.end(); i++, j++) {
		if (*i != *j) {
			return false;
		}
	}

	return true;
}

/*
* parameter: SkipList which represents the SkipList on the right hand side
* returns: bool. false if equal - true otherwise
* compares 2 SkipLists at level 0 and returns false if they are the same and true otherwise
*/
template <typename T>
bool SkipList<T>::operator!=(const SkipList& rhs) const {
	if (this->size() != rhs.size()) {
		return true;
	}
	for (SkipList<T>::iterator i = rhs.begin(); i != rhs.end(); i++) {
		for (SkipList<T>::iterator j = begin(); j != end(); j++) {
			if (*i != *j) {
				return true;
			}
		}
	}
	return false;
}

/*
* parameters: none
* reutrns: int - the number of elements in level 0
* iterates through level 0 of the SkipList and counts the number of elements in that level.
*/
template <typename T>
int SkipList<T>::size() const {
	int count = 0; //count of the number of elements - initially set to 0
	for (SkipList<T>::iterator i = begin(); i != end(); i++) {
		count++;
	}
	return count;
}

/*
* parameters: none
* returns: bool - true if empty and false if not.
* checks the head node at level 0. if the node is null that means the list is empty and to return true. else, return false.
*/
template <typename T>
bool SkipList<T>::empty() const {
	if (heads_[0] == nullptr)
	{
		return true;
	}
	return false;
}

/*
* takes no parameters and has no return value
* clears a SKiplist of al its nodes and sets all the nodes to nullptr
*/
template <typename T>
void SkipList<T>::clear() {

	//starting at the maxlevel in the SkipList 
	for (int currentLevel = 0; currentLevel < maxLevel_; currentLevel++) {
		//current node is the head at the current level 
		SkipListNode* currentNode = heads_[currentLevel];
		//while the current node does not equal nullptr
		while (currentNode != nullptr) {
			//create another node which holds the next node and deletes the previous node
			SkipListNode* nodeToDelete = currentNode;
			currentNode = currentNode->next_;
			delete nodeToDelete;
		}

		//heads and tails arrays are set to nullptr
		heads_[currentLevel] = nullptr;
		tails_[currentLevel] = nullptr;
	}
}

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
template <typename T>
ostream& operator<<(ostream& os, const SkipList<T>& list) {
	for (int currentLevel = list.maxLevel_ - 1; currentLevel >= 0; currentLevel--) {
		os << "Level: " << currentLevel << " -- ";
		if (list.heads_[currentLevel] == nullptr) {
			os << "empty";
		}
		else {
			auto node = list.heads_[currentLevel];
			while (node != nullptr) {
				os << node->data_ << " ";
				node = node->next_;
			}
		}

		os << endl;
	}
	return os;
}

/*
* no parameters
* returns: bool
*
* uses a random generator to "flip a coin" returns true 50% of the time and false 50% of the time.
* this method is called by the insert method to determine if a SkipListNode being inserted should be added to
* consecutive higher levels.
*/
template <typename T>
bool SkipList<T>::alsoHigher() const {
	static default_random_engine generator;
	static bernoulli_distribution coin(0.5);
	return coin(generator);
}

/*
* Parameters: generic data type which is the new value being added to the skipList
* Returns: bool: true if successfully added and false if duplicate exists which is predetermined by the findInsertionPoint method
*
* this method makes use of the findInsertionPoint by checking for duplicaes and insetion point method. it also calls the addBefore and
* addAfter method which is determined by the prevOrNext node which compares values and calls either of these methods depending on if
* the new item should be inserted before or after that node. this method also calls the alsoHigher method which determines if a node added
* at level 0 should be also added to consecutive higher levels.
*/
template<typename T>
bool SkipList<T>::insert(const T item) {
	vector<SkipListNode*> parents; //holds location of possible insertion points
	//if duplicate then return false 
	if (!findInsertionPoint(item, parents)) {
		return false;
	}

	SkipListNode* lowerLevelNode = nullptr; //lower level node 
	for (size_t i = 0; i < parents.size(); i++) {
		SkipListNode* prevOrNext = parents[i];

		SkipListNode* newNode = new SkipListNode(item);
		newNode->downLevel_ = lowerLevelNode;
		//if there is a lower level node then add an upLevel_ ppointer 
		if (lowerLevelNode != nullptr) {
			lowerLevelNode->upLevel_ = newNode;
		}

		if (prevOrNext == nullptr) {
			// There is no node at this level so connect it to head and tail.
			heads_[i] = tails_[i] = newNode;
		}
		else if (prevOrNext->data_ < item) {
			//add the newnode after the prevOrNext node if the item is less than the value of 
			//the prevOrNext node
			addAfter(i, newNode, prevOrNext);
		}
		else if (prevOrNext->data_ > item) {
			//add the newnode before the prevOrNext node if the item is greater than the value of 
			//the prevOrNext node
			addBefore(i, newNode, prevOrNext);
		}
		//once inserted into level 0, "filps a coin" to see if the node will be added to higher levels. 
		//node is added to higher levels as long as this method returns true
		if (!alsoHigher()) {
			break;
		}
		lowerLevelNode = newNode;
	}
	return true;
}

/*
* Parameter: generic data type
* Returns: bool
*
* takes an data type and removes nodes that contain that value from the SkipList. traverses through looking for item in all levels, rearranges pointers,
* and deallocates memory associated with the nodes. returns true if successfully removed. returns false if list is empty or if node does
* not exist in the SkipList
*/
template <typename T>
bool SkipList<T>::erase(const T item) {
	int currentLevel = maxLevel_ - 1; //find the highest level 
	SkipListNode* node = nullptr;
	//while the current level is greater than or equal to 0, find the highest level non-null head pointer and set it to 
	//the node variable 
	while (currentLevel >= 0) {
		if (heads_[currentLevel] != nullptr) {
			node = heads_[currentLevel];
			break;
		}

		currentLevel--;
	}

	if (node == nullptr) {
		//means all head pointer are null and returns false
		return false;
	}

	while (node != nullptr) {
		if (node->data_ == item) {

			//start from the top most node and move downwards. At each level. remove 
			//the node from the linked list and delete it. After deletion, move to the node 
			//in the lower level until there are no more levels.
			while (node != nullptr) {
				SkipListNode* downNode = node->downLevel_;
				SkipListNode* beforeNode = node->prev_;
				SkipListNode* afterNode = node->next_;

				// If this is the first node in the linked list, then update the head as well.
				if (beforeNode == nullptr) {
					heads_[currentLevel] = afterNode;
				}
				//if this is the last node in the linked list, then update the tail as well
				if (afterNode == nullptr) {
					tails_[currentLevel] = beforeNode;
				}
				//if the item beign erased has a null node to the left of it then set the node to afterNode
				if (beforeNode != nullptr) {
					beforeNode->next_ = afterNode;
				}
				//if the item being erased has a null node to the right of it then set the node to the beforeNode
				if (afterNode != nullptr) {
					afterNode->prev_ = beforeNode;
				}
				//delete the node and move down a level
				delete node;
				node = downNode;
				currentLevel--;
			}

			return true;
		}
		else if (node->next_ != nullptr && node->next_->data_ <= item) {
			node = node->next_;
		}
		else if (node->prev_ != nullptr && node->prev_->data_ >= item) {
			node = node->prev_;
		}
		else {
			node = node->downLevel_;
			currentLevel--;
		}
	}
	//if list is not empty and doesnt contain the item being erased, return false 
	return false;
}

/*
* parameter: generic data type - the item being searched for
* Return value: bool
*
* given an primitive type to search for, traverses through the list starting at the highest level non-null head pointer makes its way down levels
* until it either find the node it is searching for. in that case it returns true. if the list is empty or the value doesnt exist in the list,
* reutrns false.
*/
template<typename T>
bool SkipList<T>::contains(const T item) const {
	//find the hignest level non-null head pointer
	int currentLevel = maxLevel_ - 1;
	SkipListNode* node = nullptr;
	//while the level is greater than 0, set the starting node to the highest non-null head pointer
	while (currentLevel >= 0) {
		if (heads_[currentLevel] != nullptr) {
			node = heads_[currentLevel];
		}
		//move down levels until a non null head pointer is found 
		currentLevel--;
	}
	//in this case the list is empty so return false
	if (node == nullptr) {
		return false;
	}
	else {
		while (node != nullptr) {
			//if the node is equal to the item being searched for, return true 
			if (node->data_ == item) {
				return true;
			}
			//if the node is less than the item being searched for, move to the next node 
			else if (node->next_ != nullptr && node->next_->data_ <= item) {
				node = node->next_;
			}
			//if the next node is greater than the node being searched for, move to the previous node
			else if (node->prev_ != nullptr && node->prev_->data_ >= item) {
				node = node->prev_;
			}
			else {
				//move down a level 
				node = node->downLevel_;
				currentLevel--;
			}
		}
		//if list is not empty and doesnt contain the item being searched for, return false 
		return false;
	}
}

/*
/*
* Parameters: generic data type which is the node being added, and a vector which holds the data for the path the node took to get to level 0
* Returns: bool, true if successfully found a spot. false if there is a duplicate
*
* this method is a helper method for the insert method and does all the searching work. traverses through the list and finds the correct spot
* to insert the new item being added.
*/
template <typename T>
bool SkipList<T>::findInsertionPoint(T newData, std::vector<SkipList<T>::SkipListNode*>& parents)
{
	// Find the first non-null head pointer.
	SkipListNode* node = nullptr;
	int currentLevel = maxLevel_ - 1;

	parents.resize(maxLevel_);
	//find the highest level non-null head pointer and set it to node 
	while (currentLevel >= 0) {
		if (heads_[currentLevel] != nullptr) {
			node = heads_[currentLevel];
			break;
		}

		parents[currentLevel] = nullptr;
		currentLevel--;
	}

	while (node != nullptr) {
		if (node->data_ == newData) {
			// Duplicate value already exists.
			return false;
		}
		//if nodes next value is less than the item being added, moves to the next node in the same level 
		else if (node->next_ != nullptr && node->next_->data_ <= newData) {
			node = node->next_;
		}
		//if node previous value is greater than the item being added, moves to the previous node
		else if (node->prev_ != nullptr && node->prev_->data_ >= newData) {
			node = node->prev_;
		}
		else {
			//moves the node down a level 
			parents[currentLevel] = node;
			node = node->downLevel_;
			currentLevel--;
		}
	}

	return true;
}

/*
* no reurn value (void)
* Parameters: generic data type which is the current level node is being added to , SkipListNode* which is the new node being added, and
* another SkipListNode* which is the node the new node is inserted before.
*
* takes a new node to be inserted and adds it before the next node. checks to see which level the work is being done on and manipulates the
* pointers to add the new node in. takes care of the case where it is the first node in the level and modifies the head in that case.
*/
template<typename T>
void SkipList<T>::addBefore(int currentLevel, SkipList::SkipListNode* newNode, SkipListNode* nextNode)
{
	SkipListNode* n1 = nextNode->prev_;
	SkipListNode* n2 = nextNode;
	if (n1 == nullptr) {
		// This is the first node in the list so the head must be modified as well
		heads_[currentLevel] = newNode;

		n2->prev_ = newNode;
		newNode->next_ = n2;
	}
	else {
		//take the nodes before and after the new node and rearrange the pointers
		n1->next_ = newNode;
		newNode->prev_ = n1;

		n2->prev_ = newNode;
		newNode->next_ = n2;
	}
}

/*
* no reurn value (void)
* Parameters: generic data type which is the current level node is being added to , SkipListNode* which is the new node being added, and
* another SkipListNode* which is the node the new node is inserted after.
*
* takes a new node to be inserted and adds it after the before node. checks to see which level the work is being done on and manipulates the
* pointers to add the new node in. takes care of the case where it is the last node in the level and modifies the tail in that case.
*/
template <typename T>
void SkipList<T>::addAfter(int currentLevel, SkipList::SkipListNode* newNode, SkipList::SkipListNode* beforeNode)
{
	SkipListNode* n1 = beforeNode;
	SkipListNode* n2 = beforeNode->next_;

	if (n2 == nullptr) {
		// This is the last node in the list so tail has to be modified as well
		tails_[currentLevel] = newNode;

		n1->next_ = newNode;
		newNode->prev_ = n1;
	}
	else {
		//take the nodes before and after the new node and rearrange the pointers 
		n1->next_ = newNode;
		newNode->prev_ = n1;

		newNode->next_ = n2;
		n2->prev_ = newNode;
	}
}
