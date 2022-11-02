//-----------------------------------------------------------------------------
// bintree.cpp
// memebr function definitions for BinTree class 
// Author: Sarah Asad 
// ----------------------------------------------------------------------------
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

#include "bintree.h"

using namespace std;

//-----------------------------------------------------------------------------
//constructor 
BinTree::BinTree() {
    root = nullptr;
}

//-----------------------------------------------------------------------------
//destructor
BinTree::~BinTree() {
    makeEmpty();
}

//-----------------------------------------------------------------------------
//copy constructor
BinTree::BinTree(const BinTree& tree) {
    root = copyHelper(tree.root);
}

//-----------------------------------------------------------------------------
//copyHelper 
//used by copy constructor
BinTree::Node* BinTree::copyHelper(const Node* node) const {
    if (node == nullptr) {
        return nullptr;
    }
    Node* newNode = new Node;
    NodeData* newData = new NodeData;
    *newData = *node->data;
    newNode->data = newData;
    newNode->left = copyHelper(node->left);
    newNode->right = copyHelper(node->right);

    return newNode;

}

//-----------------------------------------------------------------------------
//insert
//inserts a node inot the tree at correct position
bool BinTree::insert(NodeData* node) {
    //create new node
    Node* newNode = new Node;
    //set newNodes data 
    NodeData* newData = new NodeData;
    *newData = *node;
    newNode->data = newData;
    //null out left and right
    newNode->left = nullptr;
    newNode->right = nullptr;
    //if there is no root make the newNode the root 
    if (root == nullptr)
    {
        root = newNode;
    }
    else
    {
        Node* curNode = root;
        bool notAssigned = true;

        while (notAssigned)
        {
            if (*node == *curNode->data)
            {
                delete newNode;
                delete newData;
                newNode = nullptr;
                newData = nullptr;
                return false;
            }
            if (*node < *curNode->data)
            {
                if (curNode->left == nullptr)
                {
                    curNode->left = newNode;
                    notAssigned = false;
                }
                else
                {
                    curNode = curNode->left;
                }
            }
            else
            {
                if (curNode->right == nullptr)
                {
                    curNode->right = newNode;
                    notAssigned = false;
                }
                else
                {
                    curNode = curNode->right;
                }
            }
        }


    }
    delete node;
    node = nullptr;
    return true;
}

//-----------------------------------------------------------------------------
//isEmpty
//returns true if tree is empty and false otherwise
bool BinTree::isEmpty() const {
    //if there is no root - tree is empty
    if (root == nullptr) {
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
//deleteNode
//used by makeEmpty to remove and delete nodes recursively from whole tree 
void BinTree::deleteNode(Node*& node) {
    if (node != nullptr) {
        deleteNode(node->left);
        deleteNode(node->right);
        delete node->data;
        node->data = nullptr;
        delete node;
        node = nullptr;
    }
}

//-----------------------------------------------------------------------------
//makeEmpty
//calls delteNode to empty whole tree 
void BinTree::makeEmpty() {
    deleteNode(root);
}

//-----------------------------------------------------------------------------
//retrieve
//searched through tree for given node and if foun, returns true.
//false otherwise. 
bool BinTree::retrieve(const NodeData& one, NodeData*& two) const {
    if (isEmpty()) {
        return false;
    }
    Node* current = root;
    while (current != nullptr) {
        if (*current->data == one) {
            two = current->data;
            return true;
        }
        else if (*current->data < one) {
            current = current->right;
        }
        else {
            current = current->left;
        }
    }
    delete current;
    current = nullptr;
    return false;
}

//-----------------------------------------------------------------------------
//getSibling
//returns true if parents contains sibling and if ti si found. 
//false if the parents does not have any siblings. 
bool BinTree::getSibling(const NodeData& one, NodeData& two) const {
    if (isEmpty()) {
        return false;
    }

    Node* parent = findParent(root, one);
    if (parent == nullptr) {
        return false;
    }

    if (parent->left != nullptr && *(parent->left->data) == one) {
        if (parent->right != nullptr) {
            two = *(parent->right->data);
            return true;
        }

        return false;
    }

    if (parent->right != nullptr && *(parent->right->data) == one) {
        if (parent->left != nullptr) {
            two = *(parent->left->data);
            return true;
        }
    }

    return false;
}

//-----------------------------------------------------------------------------
//getParent
//returns true of parent of child is successfully found. false otherwise. 
bool BinTree::getParent(const NodeData& one, NodeData& two) const {
    if (isEmpty()) {
        return false;
    }

    Node* parent = findParent(root, one);
    if (parent == nullptr) {
        return false;
    }

    two = *parent->data;
    return true;
}

//-----------------------------------------------------------------------------
//findParent 
//helper method for getParent. 
BinTree::Node* BinTree::findParent(Node* currentParent, 
    const NodeData& node)const{
    if (currentParent == nullptr) {
        return nullptr;
    }
    else if (*currentParent->data == node) {
        // The parent contains the node, so likely this is the root.
        return nullptr;
    }

    if((currentParent->left != nullptr && node == *(currentParent->left->data)) 
        ||(currentParent->right != nullptr && node == 
            *(currentParent->right->data))) {
        return currentParent;
    }

    Node* newParent = findParent(currentParent->left, node);
    if (newParent != nullptr) {
        return newParent;
    }

    return findParent(currentParent->right, node);
}

//-----------------------------------------------------------------------------
//bsTreeToArray
void BinTree::bstreeToArray(NodeData* arr[]) {
    int count = 0;
    arrayHelper(arr, root, count);
    makeEmpty();
}

//-----------------------------------------------------------------------------
//arrayHelper
//used by method bstreetoArray to construct array 
void BinTree::arrayHelper(NodeData* ndArr[], Node* node, int& count) const
{
    if (node == NULL)
    {
        return;
    }
    NodeData* newData = new NodeData;
    *newData = *node->data;
    arrayHelper(ndArr, node->left, count);
    ndArr[count] = newData;
    count++;
    arrayHelper(ndArr, node->right, count);
}

//-----------------------------------------------------------------------------
//treeBuilder 
//used by arrayToBstree to build tree from sorted array 
BinTree::Node* BinTree::treeBuilder(NodeData* arr[], int low, int high)
{
    if (low > high) {
        return NULL;
    }
    Node* newNode = new Node;
    NodeData* newData = new NodeData;
    int index = (low + high) / 2;
    *newData = *arr[index];
    delete arr[index];
    arr[index] = NULL;
    newNode->data = newData;
    int leftIndex = 0;
    int rightIndex = 0;
    newNode->left = treeBuilder(arr, low, index - 1);
    newNode->right = treeBuilder(arr, index + 1, high);

    return newNode;
}

//-----------------------------------------------------------------------------
//arrayToBstree
void BinTree::arrayToBSTree(NodeData* arr[]) {
    int count = 0;
    while (arr[count] != NULL) {
        count++;
    }

    int index = count - 1;
    root = treeBuilder(arr, 0, index);
}

//-----------------------------------------------------------------------------
//operator=
BinTree& BinTree::operator=(const BinTree& tree) {
    //already equal 
    if (*this == tree) {
        return *this;
    }
    //if root is not null, then empty tree
    else if (root != NULL) {
        makeEmpty();
    }
    //copt each node to the right hand side. 
    root = copyHelper(tree.root);
    return *this;
}

//-----------------------------------------------------------------------------
//operator==
bool BinTree::operator==(const BinTree& tree) const {
    //both are emoty and return true 
    if (this->isEmpty() && tree.isEmpty()) {
        return true;
    }
    if (!this->isEmpty() && tree.isEmpty()
        || this->isEmpty() && !tree.isEmpty()) {
        return false;
    }
    return equalityHelper(root, tree.root);
}

//-----------------------------------------------------------------------------
//equalityHelper
//used by operator==
bool BinTree::equalityHelper(Node* currentNode, Node* node) const
{
    if (currentNode == nullptr && node == nullptr)
    {
        return true;
    }
    if (currentNode == nullptr && node != nullptr 
        || currentNode != nullptr && node == nullptr)
    {
        return false;
    }

    if (*currentNode->data != *node->data)
    {
        return false;
    }
    if (currentNode != nullptr && node != nullptr)
    {
        return equalityHelper(currentNode->left, node->left)
            && equalityHelper(currentNode->right, node->right);
    }

    return true;
}

//-----------------------------------------------------------------------------
//operator!=
bool BinTree::operator!=(const BinTree& tree) const {
    return 	!(*this == tree);
}

//-----------------------------------------------------------------------------
// displaySideways 
// Displays a binary tree as though you are viewing it from the side.
// Turn head 90 degrees counterclockwise (to the left) to see tree structure.
// Hard coded displaying to standard output.
void BinTree::displaySideways() const {
    sidewaysHelper(root, 0);
}

//-----------------------------------------------------------------------------
//sidewaysHelper
//helper method used to diplay binary tree
void BinTree::sidewaysHelper(Node* current, int level) const {
    if (current != nullptr) {
        level++;
        sidewaysHelper(current->right, level);

        // indent for readability, same number of spaces per depth level 
        for (int i = level; i >= 0; i--) {
            cout << "      ";
        }

        cout << *current->data << endl;        // display information of object
        sidewaysHelper(current->left, level);
    }
}

//-----------------------------------------------------------------------------
//operator<<
ostream& operator<<(ostream& os, const BinTree& tree) {
    tree.outputHelper(os, tree.root);
    os << endl;
    return os;
}

//-----------------------------------------------------------------------------
//outputhelper
void BinTree::outputHelper(ostream& os, Node* node) const {
    //nothing to display 
    if (node == nullptr) {
        return;
    }
    //traverse left and right nodes 
    outputHelper(os, node->left);
    os << *node->data << "";
    outputHelper(os, node->right);
}