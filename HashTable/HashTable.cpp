/*
* Sarah Asad
* HashTable.cpp
*/

#include "HashTable.h"
#include "HashFunction.h"


using namespace std; 

//--------------------------Entry----------------------------

Entry::Entry(shared_ptr<string> k, int v) {
	key = k;
	value = v;
}
void Entry::setValue(int v) {
	value = v;
}
void Entry::setKey(shared_ptr<string> k) {
	key = k;
}
shared_ptr<string> Entry::getKey() {
	return key;
}
int Entry::getValue() {
	return value;
}

//--------------------------HashTable---------------------------

//constructor - indicates how large to make initial table
HashTable::HashTable(int size) {
	size = SIZE;
	table = new vector<Entry>[SIZE];
}

//insert an element into table 
void HashTable::Insert(shared_ptr<string> key, int val) {
	//get index 
	int index = HashFunction::Hash(key);

	vector<Entry> ::iterator i;
	for (i = table[index].begin(); i != table[index].end(); i++) {
		//keys are the same so replace the value 
		if (i->getKey()->compare(key->c_str()) == 0) {
			table[index].erase(i);
			break;
		}
	}

	//add entry into the corresponding index of the vector
	table[index].push_back(Entry(key, val));
}


//is an element present in the table 
bool HashTable::Contains(shared_ptr<string> key) {
	//get index to search at 
	int index = HashFunction::Hash(key);
	//traverse vector 
	vector<Entry> ::iterator i;
	for (i = table[index].begin(); i != table[index].end(); i++) {
		//keys match - return true
		if (i->getKey()->compare(key->c_str()) == 0) {
			return true;
		}
		return false;
	}
}

//find an element in the index 
//given key(string), return value(int). 
int HashTable::Get(shared_ptr<string> key) {
	//get index
	int index = HashFunction::Hash(key);
	vector<Entry>::iterator i;
	for (i = table[index].begin(); i != table[index].end(); i++) {
		//if keys equal then return value 
		if (i->getKey()->compare(key->c_str()) == 0) {
			return i->getValue();
		}
	}
	//key not found 
	return -1;
}

//remove a entry referenced by string
void HashTable::Remove(shared_ptr<string> key) {
	//get index 
	int index = HashFunction::Hash(key);
	vector<Entry> :: iterator i; 
	for (i = table[index].begin(); i != table[index].end(); i++) {
		//if key found - erase 
		if (i->getKey()->compare(key->c_str()) == 0) {
			table[index].erase(i);
			break;
		}
	}
}



