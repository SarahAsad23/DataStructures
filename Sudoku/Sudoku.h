/*
* Sudoku.h
* Sarah Asad
* CSS 342 - Stiber
* Program 3 - Sudoku Puzzle
* 5/20/21
*
* this program starts by creating a sqaure corresponding to the sqaure class which is nested within the puzzle class. the puzzle class creates a 2D array of sqaures which represnt the Sudoku puzzle. the puzzle values 
* are then read by an overloaded istream operator and filled into their corresponding squares. the sqaures are then flagged as either "fixed" meaning that they are values between 1 and 9 or they are variable meaning 
* that they are to be solved. there is then a recursive solve() method wwhich solves the puzzle. the puzzle is then displayed by an overloaded ostream operator. there are also a variety of other methods to set values, 
* get values, find the size, the number of variable sqaures, etc.
* 
* This is the header file for the Sudoku.cpp class which declares all the methods and member variables used in the Sudoku.cpp file.
*/

#pragma once
#include <iostream>

using namespace std;



class Puzzle {
	class Square {
		int value_; //holds the data for the square 
		bool fixed_; //indicates whether the value is fixed or variable 

	public:
		/*
		* constructor for the sqaure class. initializes the value for the sqaure which is by default set to -1.
		* and sets the values to fixed/true if they are between 1 and 9. otherwise false which indicates that
		* the value is variable/to be solved.
		*/
		Square(bool isFixed, int value = -1);
		/*
		* parameters: int which is the value to be set
		* returns: void - nothing
		* takes a value and sets it to the sqaure. if it is fixed then throws an exception
		* that the square cannot be assigned a value.
		*/
		void setValue(int data);
		/*
		* parameters: none
		* returns: int which is the value of the sqaure.
		*/
		int getValue() const;
		/*
		* Parameters: none 
		* Returns: bool which indicates if the value is fixed or not. true - fixed and false - variable
		*/
		bool isFixed() const {
			return fixed_;
		}
		/*
		* parameters: none 
		* Returns: bool. true if value is 1 - 9 and false otherwise
		*/
		bool isValueAssigned() const {
			return value_ >= 1 && value_ <= 9;
		}
	};

	enum {SIZE = 9}; //size for a whole puzzle 
	enum {BLOCKSIZE = 3}; //size for a 3x3 block

	Square* squares_[SIZE][SIZE]; //2D array which represents the whole puzzle and holds the individual squares 
	int size_; //used by the size method to compute variable squares before solving 

	/*
	* parameters: ints - row and column index & an int value to be set
	* returns: bool. true if the value can be set at that location and false if it cannot be set there.
	* this method checks that the row, column, and 3x3 block do not contain the same value as the value trying to be set.
	*
	*/
	bool canSetValue(int row, int col, int value);
	/*
	* Parameters: ints - row and colum index & a value to be set.
	* returns: bool. true if successsfully set and false otherwise.
	* this method sets a given int value at a certain index. it checks the given index to
	* make sure it is not fixed. it also calls a the canSetValue() which ensures that
	* the same row, column, and block do not already contain the value.
	*/
	bool set(int row, int column, int value);
	/*
	* parameters: a row and column int value.
	* returns: bool. true if puzzle was successfully solved and false otherwise.
	* uses recursive backtracking to iterate through the possible values 1 through 9 and find the correct spot to insert them
	* by making sure the the row, column, and 3x3 block do not already contain that value. keep moving to the next row and column
	* and check values that can be inserted.
	*/
	bool solve(int row, int col);
	
public:

	/*
	* constructor for the puzzle class. iterates through the size of the 2D array and creates new squares.
	* also keeps track of th initial amohunt of variable sqares through an int variable calles size_.
	*/
	Puzzle();
	/*
	* destructor for the puzzle class. iterates through and nulls out all the sqaures on the
	* board and then deletes them.
	*/
	~Puzzle();
	/*
	* helper method for the recursive solve function. it starts to solve the puzzle at (0 , 0)
	* which is the first row and column and recursively works from there. if the puzzle is not
	* solveable then trows an exception.
	*/
	void solve();
	/*
	* Parameters: ints - row and colum index & a value to be set.
	* returns: void - nothing
	* this method sets a given int value at a certain index. it makes sure the value is not fixed.
	*/
	void initialize(int row, int col, int value);

	/*
	* Parameters: int values representing the row and column index.
	* Returns: int which the the value at the index
	* this method take a row and coumn index and returns the value at that index. if the index exceeds
	* the bounds of the board then an exception is thrown.
	*/
	int get(int row, int col);
	//returns the number of variable sqaures in the puzzle 
	int size() const {
		return size_;
	}

	/*
	* Parameters: none
	* Returns: int whoch is the number of variable sqaures
	* this method iterates through the board and returns the number of sqaures that can be solved for
	*/
	int numEmpty();
	/*
	* parameters: istream& which takes the string of digits
	* returns: istream& which is an overloaded input operator.
	* this method reads the string sequence of numbers and seperates them. it then iterates through the board
	* and adds them in.
	*/
	friend istream& operator>>(istream& in, string input);
	/*
	* Parameters: ostream& which is used to print the const Puzzle&
	* Returns: ostream& which is an overloaded output operator
	* this method prints out the values of the puzzle. every row and clumn is lines up and 3x3 blocks are
	* seperated using - and | and + characters.
	*/
	friend ostream& operator<<(ostream& os, const Puzzle& game);
};


