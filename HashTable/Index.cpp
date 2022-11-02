/*
* Sarah Asad
* Index.cpp
*/

#include "Index.h"
#include "HashFunction.h"

using namespace std;

//constructor
Index::Index() : table(10000) {}
//destructor 
Index::~Index() {}

//insert a element at the index, associate the integer value with element 
//int sotred in each node. string used to traverse 
void Index::Insert(shared_ptr<string>key , int val) {
	table.Insert(key, val);
}

//find an element in the index. return value 
int Index::Find(shared_ptr<string> key) {
	return table.Get(key);
}

//remove an element from the index 
void Index::Remove(shared_ptr<string> key) {
	table.Remove(key);
}

