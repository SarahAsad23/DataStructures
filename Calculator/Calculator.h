/*
* Calculator.h
* Sarah Asad
* CSS 342 - Stiber
* Program 4 - Symbolic Calculator
* 6/4/21
*
* This Program uses a tokenizer to read a string of input from cin (in infix).
* it then creates tokens with their corresponding type and value. the tokens
* are inserted into a vector and then are converted into a postifx expression.
* this postfix expression vector is used in the AST constructor to construct
* a binary expression tree. a simplify method is then used to simplify binary
* tree and uses a map to store the variable as the key and its corresponding
* expression as the value. the simplified expression is then converted back
* into infix and is printed to the console.
*
* This is the header file for the Calculator.cpp class which declares all the 
* methods and member variables used in the Calculator.cpp file.
*/


#pragma once

#include <string>
#include <vector>
#include <stack>
#include <istream>
#include <iostream>
#include <map>

using namespace std; 

/*
* enum class which holds all the types of tokens
* unknown: used in the default constructor 
* operator: '+ - * / ^'
* variable: 'x y z a b c' etc.
* number: any posotive int
* lparen: '(' 
* rparen: ')' 
* equals: ':='
* display: '#' (used to display list of variables and their corresponding expressions)
* expresssion_end: '\n' (tokenizer has reached end of expreesion and can stop reading)
* end: '.' program terminates once a single '.' is entered  
*/
enum class TokType {unknown, operatorr, variable, number, lparen, rparen, equals, display, expression_end, end};
struct Token {
	//default constructor which sets the token type to unknown 
	Token(){
		type_ = TokType::unknown;
	}
	//constructor which sets the type and value to the passed in t and v
	Token(TokType t, string v);
	TokType type_;
	string value_;
};

class ITokStream {
	//istream 
	istream& is_;\
	//string of data which is read
	string data_;
	//index used to increment to the next char
	int dataOffset_;

	//max size of input 
	enum { MaxDataSize = 100 };

public:
	/*
	* parameters: istream& - refernce to an input stream
	* Constructor for the ItokStream class which sets the member variable is_
	* to the passed in input stream.
	*/
	ITokStream(istream& inputStream);
	/*
	* Parameters: Token& - refernece to a token
	* Returns: ITokStream& - overload to cin
	* overloaded istream operator which takes a string of input from the user and
	* reads each char seperatley as tokens. it then assigns the type (operator, number,
	* left paren, right paren, variable, equals, end) and also assigns its value. it then
	* breaks the input into a variable and expression if ther is an assignment op (:=).
	* the expression is stored in a vector of tokens and the variable is saved as a simple
	* token.
	*/
	ITokStream& operator>>(Token& t);
	explicit operator bool() const {
		return (bool)is_;
	}
};

class AST {
public:
	/*
	* Parameters: a vector of tokens
	* the is ths constructor for the AST class which takes in a vector of tokens
	* that are arrangesd in postfix. This vector is used alongside a stack to
	* build the binary expression tree.
	*/
	AST(vector<Token>& infix);
	~AST();

	string toInfix() const;
	AST simplify(map<string, AST>& variables);

	//copy constructor 
	AST(const AST& rhs);

	//copy assignment operator
	AST& operator=(const AST& rhs);

	//equals operator overload
	bool operator==(AST rhs) const; 

	//does not equal operator overload
	bool operator!=(AST rhs) const;

	void display() const;

private:
	struct Node {
		//sets tok_ to passed in Token t
		Node(Token t) {
			tok_ = t;
		}
		//representa the token 
		Token tok_;
		//left child node of parent 
		shared_ptr<Node> left_;
		//right child node of parent
		shared_ptr<Node> right_;
	};
	//root node
	shared_ptr<Node> root_;

	void toInfix(shared_ptr<Node> node, string& output) const;

	void replace_variables(map<string, AST>& variables);
	void replace_variables(shared_ptr<Node> node, map<string, AST>& variables);

	void copyNode(shared_ptr<Node> mySide, shared_ptr<Node> otherSide);
	int precedence(Token t);
	vector<Token> toPostfix(vector<Token> infix);
	void simplify(shared_ptr<Node> node, bool& simplified);
	void display(shared_ptr<Node> node) const;
};

class Evaluator
{
	//map to hold variables and their corresponding expression trees
	map<string, AST> variables_;

public:
	void run();

private:
	void display_variables();
	void process_expression(vector<Token>& expression);
	void split_expression(vector<Token>& expression, string& variable, vector<Token>& rhs);

};