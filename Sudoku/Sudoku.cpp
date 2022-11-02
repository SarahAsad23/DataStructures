/*
* Sudoku.cpp
* Sarah Asad
* CSS 342 - Stiber
* Program 3 - Sudoku puzzle 
* 5/20/21
* 
* this program starts by creating a sqaure corresponding to the sqaure class which is nested within the puzzle class. the puzzle class creates a 2D array of sqaures which represnt the Sudoku puzzle. the puzzle values 
* are then read by an overloaded istream operator and filled into their corresponding squares. the sqaures are then flagged as either "fixed" meaning that they are values between 1 and 9 or they are variable meaning 
* that they are to be solved. there is then a recursive solve() method wwhich solves the puzzle. the puzzle is then displayed by an overloaded ostream operator. there are also a variety of other methods to set values, 
* get values, find the size, the number of variable sqaures, etc.  
*/

#include "Sudoku.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------------------
// Puzzle::Square
//---------------------------------------------------------------------------------------------------------------------

/*
* constructor for the sqaure class. initializes the value for the sqaure which is by default set to -1.
* and sets the values to fixed/true if they are between 1 and 9. otherwise false which indicates that
* the value is variable/to be solved.
*/
Puzzle::Square::Square(bool isFixed, int value) {
	//if value between 1 and 9. 
	if (isFixed && (value < 1 || value > 9)) {
		throw "Invalid value specified for fixed square";
	}

	fixed_ = isFixed;
	value_ = value;
}

/*
* parameters: none
* returns: int which is the value of the sqaure.
*/
int Puzzle::Square::getValue() const {
	return value_;
}

/*
* parameters: int which is the value to be set
* returns: void - nothing
* takes a value and sets it to the sqaure. if it is fixed then throws an exception
* that the square cannot be assigned a value.
*/
void Puzzle::Square::setValue(int value) {
	if (isFixed()) {
		throw "Fixed square cannot be assigned a value";
	}

	value_ = value;
}

//---------------------------------------------------------------------------------------------------------------------
// Puzzle
//---------------------------------------------------------------------------------------------------------------------

/*
* constructor for the puzzle class. iterates through the size of the 2D array and creates new squares.
* also keeps track of th initial amohunt of variable sqares through an int variable calles size_.
*/
Puzzle::Puzzle() {
	size_ = 0; //holds the amount of variable squares 
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			squares_[i][j] = nullptr;
		}
	}
}

/*
* destructor for the puzzle class. iterates through and nulls out all the sqaures on the
* board and then deletes them.
*/
Puzzle::~Puzzle() {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (squares_[i][j] != nullptr) {
				delete squares_[i][j];
				squares_[i][j] = nullptr;
			}
		}
	}
}

/*
* helper method for the recursive solve function. it starts to solve the puzzle at (0 , 0)
* which is the first row and column and recursively works from there. if the puzzle is not
* solveable then trows an exception.
*/
void Puzzle::solve() {
	if (!solve(0, 0)) {
		throw "Puzzle is not solveable";
	}
}

/*
* parameters: a row and column int value.
* returns: bool. true if puzzle was successfully solved and false otherwise.
* uses recursive backtracking to iterate through the possible values 1 through 9 and find the correct spot to insert them
* by making sure the the row, column, and 3x3 block do not already contain that value. keep moving to the next row and column
* and check values that can be inserted.
*/
bool Puzzle::solve(int row, int col) {
	//if the value of the surrent square is not fixed 
	if (!squares_[row][col]->isFixed()) {
		//used to iterate through all the possible values to be added (1 - 9)
		for (int proposedValue = 1; proposedValue <= 9; proposedValue++) {
			//uses helper method which checks if value can be added
			if (canSetValue(row, col, proposedValue)) {
				//if true then set the value at row and column index. 
				set(row, col, proposedValue);
				//if tis the last row and column the return true. 
				if (row == SIZE - 1 && col == SIZE - 1) {
					return true;
				}
				//if the column is not the last one 
				else if (col < SIZE - 1) {
					//and value can be inserted 
					if (solve(row, col + 1)) {
						return true;
					}
				}
				else {
					if (solve(row + 1, 0)) {
						return true;
					}
				}
				//set the value 
				set(row, col, -1);
			}
		}
	}
	else {
		// The cell contains fixed value so move to the next square if there are more. If not, then the puzzle has been solved.
		if (row == SIZE - 1 && col == SIZE - 1) {
			return true;
		}
		else if (col < SIZE - 1) {
			if (solve(row, col + 1)) {
				return true;
			}
		}
		else {
			if (solve(row + 1, 0)) {
				return true;
			}
		}
	}

	return false;
}

/*
* Parameters: ints - row and colum index & a value to be set.
* returns: bool. true if successsfully set and false otherwise.
* this method sets a given int value at a certain index. it checks the given index to
* make sure it is not fixed. it also calls a the canSetValue() which ensures that
* the same row, column, and block do not already contain the value.
*/
bool Puzzle::set(int row, int col, int value) {
	//if the sqaure is already fixed then throw an exception
	if (squares_[row][col]->isFixed()) {
		throw "Value is fixed and cannot be changed";
	}
	//if row, column, an d 3x3 contain value, do not set value. 
	if (!canSetValue(row, col, value)) {
		return false;
	}
	//otherwise set value and return true. 
	squares_[row][col]->setValue(value);
	return true;
}

/*
* Parameters: ints - row and colum index & a value to be set.
* returns: void - nothing
* this method sets a given int value at a certain index. it makes sure the value is not fixed.
*/
void Puzzle::initialize(int row, int col, int value) {
	squares_[row][col] = new Square(value > 0, value);
}

/*
* parameters: ints - row and column index & an int value to be set
* returns: bool. true if the value can be set at that location and false if it cannot be set there.
* this method checks that the row, column, and 3x3 block do not contain the same value as the value trying to be set.
*
*/
bool Puzzle::canSetValue(int row, int col, int value) {

	//The vlaue must be unique in the row.
	for (int i = 0; i < SIZE; i++) {
		if (i != col && squares_[row][i]->isValueAssigned() && squares_[row][i]->getValue() == value) {
			return false;
		}
	}

	//The value must be unique in the column.
	for (int i = 0; i < SIZE; i++) {
		if (i != row && squares_[i][col]->isValueAssigned() && squares_[i][col]->getValue() == value) {
			return false;
		}
	}

	// The value must be unique in the 3x3 block.
	int startRow = (row / 3) * BLOCKSIZE;
	int startCol = (col / 3) * BLOCKSIZE;

	for (int i = startRow; i < startRow + BLOCKSIZE; i++) {
		for (int j = startCol; j < startCol + BLOCKSIZE; j++) {
			if (i != row && j != col && squares_[i][j]->isValueAssigned() && squares_[i][j]->getValue() == value) {
				return false;
			}
		}
	}

	return true;
}

/*
* Parameters: int values representing the row and column index.
* Returns: int which the the value at the index
* this method take a row and coumn index and returns the value at that index. if the index exceeds
* the bounds of the board then an exception is thrown.
*/
int Puzzle::get(int row, int col) {
	//if index out of bounds throw exception 
	if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
		throw "index is out of bounds";
	}
	//return value 
	return squares_[row][col]->getValue();
}

/*
* Parameters: none
* Returns: int whoch is the number of variable sqaures
* this method iterates through the board and returns the number of sqaures that can be solved for
*/
int Puzzle::numEmpty() {
	int count = 0; //holds variable squares 
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			//if the sqaure is fixed and cannot be assigned increment the count variable. 
			if (!squares_[i][j]->isFixed() && !squares_[i][j]->isValueAssigned()) {
				count++;
			}
		}
	}

	return count;
}

/*
* parameters: istream& which takes the string of digits
* returns: istream& which is an overloaded input operator.
* this method reads the string sequence of numbers and seperates them. it then iterates through the board
* and adds them in.
*/
istream& operator>>(istream& in, Puzzle& puzzle) {
	string input;

	in >> input;
	int index = 0;
	int count = 0;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (index < input.length() && count <= 81) {
				int value = atoi(input.substr(index++, 1).c_str());
				puzzle.initialize(i, j, value);
				count++;
			}
			else {
				puzzle.initialize(i, j, 0);
			}
		}
	}
	return in;
}

/*
* Parameters: ostream& which is used to print the const Puzzle&
* Returns: ostream& which is an overloaded output operator
* this method prints out the values of the puzzle. every row and clumn is lines up and 3x3 blocks are
* seperated using - and | and + characters.
*/
ostream& operator<<(ostream& os, const Puzzle& game) {
	int count = 0; //used to help decide where to put the - and | and +
	for (int i = 0; i < game.SIZE; i++) {
		for (int j = 0; j < game.SIZE; j++) {
			count++; //increment the count for each element printed
			//print the value
			os << game.squares_[i][j]->getValue() << " ";
			//if the couts mod is 0 then place a | symbol there - ignores the last number in evrey row 
			if (count % 3 == 0 && count % 9 != 0) {
				os << "|";
			}
		}
		//if the count is at specific numbers then a dividing line should be printed. 
		if (count == 27 || count == 54){
			os << "\n------+------+------";
		}
		os << "\n";
	}

	return os;
}

//main method used for testing 
int main()
{
	Puzzle puzzle;
	cout << "Enter the input for the puzzle: ";
	cin >> puzzle;
	cout << "Before Solving" << "\n";
	cout << puzzle;
	cout << "\n";
	cout << "After Solving" << "\n";
	puzzle.solve();
	cout << puzzle;
}