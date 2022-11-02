//Sarah Asad 
//Assignment 4 

/*
* Sarah Asad
* Index.h
*/

#include <memory>
#include <string>
#include "HashTable.h"

using namespace std;


class Index {
	HashTable table;

public:
	//constructor
	Index();
	//destructor 
	~Index();
	//insert a element at the index, associate the integer value with element 
	//int sotred in each node. string used to traverse 
	void Insert(shared_ptr<string>, int);
	//find an element in the index. return value 
	int Find(shared_ptr<string>);
	//remove an element from the index 
	void Remove(shared_ptr<string>);
};


