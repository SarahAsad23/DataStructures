/*
* Sarah Asad
* HashFunction.cpp
*/

#include "HashFunction.h"
using namespace std;

const int SIZE = 10000; 

//compute hash for a string 
int HashFunction::Hash(shared_ptr<string> key) {
	//index where the key will be inserted into the table 
	int index; 
	//dereference pointer
	string word = *key;
	//convert word to ASCII 
	int ascii = ConvertToASCII(word);
	//take ASCII value and mod it with the size
	//this value will map to the corresponding index where key/value will be inserted. 
	index = (ascii % SIZE); 
	return index; 
}

//converts string to ascii
int HashFunction::ConvertToASCII(string word) {
	int x = 0;
	for (size_t i = 0; i < word.length(); i++) {
		x = (x << 8) ^ word.at(i);
	}
	return x;
}