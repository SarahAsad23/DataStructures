/*
* Sarah Asad
* HashTable.h
*/

#include <memory>
#include <string>
#include <vector>
#include <list>

using namespace std;

const int SIZE = 10000;

class Entry {
public:
	Entry(shared_ptr<string> k, int v);
	void setKey(shared_ptr<string> k);
	shared_ptr<string> getKey();
	void setValue(int v);
	int getValue();
private:
	shared_ptr<string> key;
	int value;
};

class HashTable {
public: 
	//constructor - indicates how large to make initial table
	HashTable(int size);
	//insert an element into table 
    void Insert(shared_ptr<string> key, int val);
	//is an element present in the table 
	bool Contains(shared_ptr<string> key);
	//find an element in the index 
	int Get(shared_ptr<string> key);
	//remove a entry referenced by string
	void Remove(shared_ptr<string> key);
private: 
	vector<Entry> *table;
};


