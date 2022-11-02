/*
* Sarah Asad
* HashFuncion.h
*/

#include <memory>
#include <string>
#include <vector>

using namespace std;

class HashFunction {
public:
	//compute hash for arbitrarily long string 
	static int Hash(shared_ptr<string>);

private: 
	//converts string to ascii 
	static int ConvertToASCII(string word);
};
