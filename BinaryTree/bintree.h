//-----------------------------------------------------------------------------
// bintree.h
// simple BinTree class
// Author: Sarah Asad 
//-----------------------------------------------------------------------------
// BinTree Class:
// - displays output of full binary tree 
// - allows for comparison of 2 trees 
// - allows for assignmnet of 2 trees 
// - allows for insertion of nodes into the tree
// - allows to clear a whole tree and find if a tree is empty or not 
// - able to retrieve paretns and children given specific nodes 
//-----------------------------------------------------------------------------
// Implementation and Assumptions: 
// - an empty tree is made usring constructon time 
// - tree elements are inititalized to nullptr 
// - tree is filled with data from seperate text file
// - arrray used for tree construction has a default size of 100 
//-----------------------------------------------------------------------------

#include <iostream>
#include "nodedata.h"


using namespace std;

class BinTree {
	//overloaded output operator
	friend ostream& operator<<(ostream&, const BinTree&);

public:
	BinTree();                                    //constructor
	BinTree(const BinTree&);                      //copy constructor
	~BinTree();                                   //destructor
	bool isEmpty() const;                         //T - tree empty, F otherwise
	void makeEmpty();                             //destructor, calls MakeEmpty

	BinTree& operator=(const BinTree&);           //assignment operator
	bool operator==(const BinTree&) const;        //equality - 2 trees equal
	bool operator!=(const BinTree&) const;        //inequality - not equal
	bool insert(NodeData*);                       //insert node into tree
	void displaySideways() const;                 //display tree sideways

	//True if node found, False otherwise 
	bool retrieve(const NodeData&, NodeData*&) const;
	//finds sibling of given node. True if exist, false otherwise
	bool getSibling(const NodeData&, NodeData&) const;
	//find parent of given node. True if found, false otherwise 
	bool getParent(const NodeData&, NodeData&) const;
	//fills array with NodeData
	void bstreeToArray(NodeData* []);
	//builds binary tree with sorted NodeData array
	void arrayToBSTree(NodeData* []);

private:
	struct Node {
		NodeData* data;                           //pointer to data object
		Node* left;                               //left subtree pointer
		Node* right;                              //right subtree pointer

		Node() {
			data = nullptr;
			left = nullptr;
			right = nullptr;
		}
	};

	Node* root;                                   //root of the tree

	//helper methods 

	//helps print the tree
	void sidewaysHelper(Node* node, int data) const;
	//helps output the tree contents 
	void outputHelper(ostream&, Node*) const;
	//helps makeEmpty delete all nodes
	void deleteNode(Node*& node);
	//used by copy constructor to copy nodes 
	Node* copyHelper(const Node*) const;
	//used by operator==
	bool equalityHelper(Node*, Node*) const;
	//helps copy tree elements into array 
	void arrayHelper(NodeData* [], Node*, int&) const;
	//takes sorted array and builds tree
	Node* treeBuilder(NodeData* [], int, int);
	//used by getparent to find parent of a given node
	Node* findParent(Node* currentParent, const NodeData& node) const;
};