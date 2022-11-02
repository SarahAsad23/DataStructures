/*
* Calculator.cpp
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
*/

#include "Calculator.h"
#include <sstream>
using namespace std;


//-----------------------------------------------------------------------------
// ITokStream
//-----------------------------------------------------------------------------

/*
* parameters: istream& - refernce to an input stream
* Constructor for the ItokStream class which sets the member variable is_
* to the passed in input stream.
*/
ITokStream::ITokStream(istream& inputStream) : is_(inputStream), dataOffset_(0) {
}

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
ITokStream& ITokStream::operator>>(Token& t) {
	/*
		What should be the behavior?

		Every single time you call this method, it should return the next token from the input stream

		What is your end condition?
		There are two end conditions:
			1. End of the expression: '\n'.
			2. End of the program: '.'.

		How should we read the inputs?
		cin is not full line compatible as in it returns the data as soon as it find a space.
		1 + 3	=>	"1"
		we cant use cin >> value - wont work because we need the entire line.
		I need to use cin.getline() to read the entire line at a time. So when i'm done processing
		the line i know one of the end condition is met - which is "end of expression".

		Secondly, if the cin.getline() returns "." - then end the program.

		Now how to implement this.

		Remember: ts >> token; // returns the next token regardless of it was one cin.getline() or 50 cin.getlines().
		Secondly, when cin.getline() does have tokens, you need to parse the first token and keep the data around so
		that the next call will process the same cin.getline() data previously gotten and return the next token.

		while (I have no previous data or the previous data has been fully consmed) {
			if my internal latestInput is empty thne i should call cin.getline() to get the next line.
		}

		When the above loop completes, I have valid non-empty data available.

		then parse the next token and keep track of how much of the data has been processed.


		"1" => 1
	*/

	if (data_.length() <= dataOffset_) {// All prior data has been consumed.

		// All existing data has been consumed so let's reset the input.
		dataOffset_ = 0;
		data_ = "";

		char input[MaxDataSize];
		while (data_.length() <= dataOffset_) {
			cout << "> ";
			is_.getline(input, MaxDataSize);
			data_ = input;
			data_ += "\n";
		}
	}

	t.value_ = "";
	while (dataOffset_ < data_.length()) {
		char ch = data_[dataOffset_];
		if (ch == '\n') {
			t.type_ = TokType::expression_end;
			t.value_ = ch;
			dataOffset_++;
			break;
		}
		else if (ch == '.') {
			t.type_ = TokType::end;
			t.value_ = ch;
			dataOffset_++;
			break;
		}
		else if (ch == '#') {
			t.type_ = TokType::display;
			t.value_ = ch;
			dataOffset_++;
			break;
		}
		else if (isspace(ch)) {
			dataOffset_++;
		}
		else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^') {
			t.type_ = TokType::operatorr;
			t.value_ = ch;
			dataOffset_++;
			break;
		}
		else if (ch == '(') {
			t.type_ = TokType::lparen;
			t.value_ = ch;
			dataOffset_++;
			break;
		}
		else if (ch == ')') {
			t.type_ = TokType::rparen;
			t.value_ = ch;
			dataOffset_++;
			break;
		}
		else if (ch == ':') {
			if (dataOffset_ + 1 >= data_.length()) {
				throw "invalid input specified";
			}
			if (data_[dataOffset_ + 1] != '=') {
				throw "invalid";
			}

			t.type_ = TokType::equals;
			t.value_ += data_[dataOffset_];
			t.value_ += data_[dataOffset_ + 1];
			dataOffset_ += 2;
			break;
		}
		else if (isdigit(ch)) {
			t.type_ = TokType::number;
			while (dataOffset_ < data_.length() && isdigit(data_[dataOffset_])) {
				t.value_ += data_[dataOffset_];
				dataOffset_++;
			}

			break;
		}
		else if (isalpha(ch)) {
			t.type_ = TokType::variable;
			t.value_ = ch;
			dataOffset_++;
			break;
		}
	}

	cout << "Parsed Token: Type: " << (int)t.type_ << "\tValue: " << t.value_ << "\n";

	return *this;
}

//-----------------------------------------------------------------------------
// Token
//-----------------------------------------------------------------------------

/*
* Parameters: TokTpye - the type of token and string which is the value of the token
* constructor for the enum token class which just sets the passed in type and value to the
* corresponding type_ and value_ memeber variables.
*/
Token::Token(TokType t, string v) {
	type_ = t;
	value_ = v;
}

//-----------------------------------------------------------------------------
// AST
//-----------------------------------------------------------------------------

/*
* Parameters: a vector of tokens
* the is ths constructor for the AST class which takes in a vector of tokens
* that are arrangesd in postfix. This vector is used alongside a stack to
* build the binary expression tree.
*/
AST::AST(vector<Token>& infix) {
	// The input is in infix notation. Convert it to postfix.
	vector<Token> postfix = toPostfix(infix);

	//stack used to store variables and numbers
	stack <shared_ptr<Node>> stack;
	//traverse through the postfix vector
	for (int i = 0; i < postfix.size(); i++) {
		//if the type is a variable or number
		if (postfix[i].type_ == TokType::variable || postfix[i].type_ == TokType::number) {
			//create new token and push it into stack
			root_ = make_shared<Node>(postfix[i]);

			stack.push(root_);
		}
		else {
			//if it is an operator 
			root_ = make_shared<Node>(postfix[i]);
			//pop 2 nodes from stack and store then in new nodes
			shared_ptr<Node> node1(stack.top());
			stack.pop();
			shared_ptr<Node> node2(stack.top());
			stack.pop();

			//set the left and right children 
			root_->right_ = node1;
			root_->left_ = node2;
			//push it back into the stack 
			stack.push(root_);
		}
	}
	//the remaning root is left and added to the stack. 
	root_ = stack.top();
	stack.pop();
}



AST::AST(const AST& rhs) {
	if (this != &rhs) {
		if (rhs.root_ != nullptr) {
			root_ = make_shared<Node>(rhs.root_->tok_);
			copyNode(root_, rhs.root_);
		}
	}
}

AST& AST::operator=(const AST& rhs) {
	if (this != &rhs) {
		if (rhs.root_ != nullptr) {
			if (root_ != nullptr) {
				while (root_->left_ != nullptr) {
					root_->left_ = nullptr;
				}
				while (root_->right_ != nullptr) {
					root_->right_ = nullptr;
				}
				root_ = nullptr;
			}
			root_ = make_shared<Node>(rhs.root_->tok_);
			copyNode(root_, rhs.root_);
		}
	}
	return *this;
}

AST::~AST() {
	if (root_ != nullptr) {
		while (root_->left_ != nullptr ) {
			root_->left_ = nullptr;
		}
		while (root_->right_ != nullptr) {
			root_->right_ = nullptr;
		}
		root_ = nullptr; 
	}
}

void AST::toInfix(shared_ptr<Node> node, string& output) const
{
	if (node == nullptr) {
		return;
	}

	toInfix(node->left_, output);
	output += node->tok_.value_;
	toInfix(node->right_, output);
}

void AST::copyNode(shared_ptr<Node> mySide, shared_ptr<Node> otherSide)
{
	if (otherSide == nullptr) {
		return;
	}

	if (otherSide->left_ != nullptr) {
		mySide->left_ = make_shared<Node>(otherSide->left_->tok_);
	}

	if (otherSide->right_ != nullptr) {
		mySide->right_ = make_shared<Node>(otherSide->right_->tok_);
	}

	copyNode(mySide->left_, otherSide->left_);
	copyNode(mySide->right_, otherSide->right_);
}

/*
* Parameters: a Token to test for values
* returns: int which corresponds to precedence
* this method is used by the toPostFix method to figure out the precedence of
* various operations. ^ has highest precedence followed by * and / which is
* followed by + and -
*/
int AST::precedence(Token t) {
	//power is highest 
	if (t.value_ == "^") {
		return 3;
	}
	//* and / are next 
	else if (t.value_ == "*" || t.value_ == "/") {
		return 2;
	}
	//+ and - are last 
	else if (t.value_ == "+" || t.value_ == "-") {
		return 1;
	}
	//default value 
	return -1;
}

/*
* Parameters: a vector of tokens in infix
* Returns: a vector of tokens in postifx
* this method takes a vector in infix and checks the type of the token to create
* a vector in postfix. it uses a stack to push and pop values until the postfix
* expression is acheived.
*/
vector<Token> AST::toPostfix(vector<Token> infix) {
	vector<Token> postfix;

	for (int i = 0; i < infix.size(); i++) {
		cout << "Infix[" << i << "] " << (int)infix[i].type_ << "\t" << infix[i].value_ << "\n";
	}

	//stack used to push and pop values 
	stack <Token> stack;
	//iterate through the infix vector 
	for (int i = 0; i < infix.size(); i++) {
		//if type is a variable or number then directly push into postfix vector
		if (infix[i].type_ == TokType::variable || infix[i].type_ == TokType::number) {
			postfix.push_back(infix[i]);
		}
		//if type is left paren then push into stack 
		else if (infix[i].type_ == TokType::lparen) {
			stack.push(infix[i]);
		}
		//if type is right paren 
		else if (infix[i].type_ == TokType::rparen) {
			//while the stack is not empty and it does not hit a left paren
			while ((!stack.empty()) && (stack.top().type_ != TokType::lparen)) {
				postfix.push_back(stack.top());
				stack.pop();
			}
			stack.pop();
		}
		//if type is operator 
		else if (infix.at(i).type_ == TokType::operatorr) {
			//and first element in stack has greater precedence then the current token 
			while ((!stack.empty()) && precedence(infix.at(i)) < precedence(stack.top())) {
				Token t = stack.top();
				stack.pop();
				postfix.push_back(t);
			}
			stack.push(infix.at(i));
		}
	}
	while (!stack.empty()) {
		Token t = stack.top();
		stack.pop();
		postfix.push_back(t);
	}

	for (int i = 0; i < postfix.size(); i++) {
		cout << "Postfix[" << i << "] " << (int)postfix[i].type_ << "\t" << postfix[i].value_ << "\n";
	}

	return postfix;
}

AST AST::simplify(map<string, AST>& variables) {
	AST result = *this;

	result.replace_variables(variables);
	bool simplified = true;
	while (simplified) {
		simplified = false;
		result.simplify(result.root_, simplified);
	}

	return result;
}

void AST::simplify(shared_ptr<Node> node, bool& simplified)
{
	if (node == nullptr) {
		return;
	}
	else if (node->tok_.type_ == TokType::operatorr) {
		if (node->left_ != nullptr && node->right_ != nullptr &&
			node->left_->tok_.type_ == TokType::number && node->right_->tok_.type_ == TokType::number) {

			int result = 0;
			switch (node->tok_.value_[0]) {
			case '+':
				result = atoi(node->left_->tok_.value_.c_str()) + atoi(node->right_->tok_.value_.c_str());
				break;
			case '-':
				result = atoi(node->left_->tok_.value_.c_str()) - atoi(node->right_->tok_.value_.c_str());
				break;
			case '*':
				result = atoi(node->left_->tok_.value_.c_str()) * atoi(node->right_->tok_.value_.c_str());
				break;
			case '/':
				result = atoi(node->left_->tok_.value_.c_str()) / atoi(node->right_->tok_.value_.c_str());
				break;
			default:
				throw "not supported";
			}

			node->tok_.type_ = TokType::number;
			node->tok_.value_ = to_string(result);
			node->left_ = nullptr;
			node->right_ = nullptr;
			simplified = true;
		}
	}

	simplify(node->left_, simplified);
	simplify(node->right_, simplified);
}

void AST::display() const
{
	display(root_);
	cout << "\n";
}

void AST::display(shared_ptr<Node> node) const
{
	if (node == nullptr) {
		return;
	}

	cout << "[" << node->tok_.value_ << " ";
	display(node->left_);
	display(node->right_);
	cout << "]";
}

void AST::replace_variables(map<string, AST>& variables)
{
	replace_variables(root_, variables);
	cout << "After Variable Replacement: ";
	display();
}

void AST::replace_variables(shared_ptr<Node> node, map<string, AST>& variables)
{
	if (node == nullptr) {
		return;
	}

	replace_variables(node->left_, variables);
	replace_variables(node->right_, variables);

	if (node->tok_.type_ == TokType::variable) {
		auto iterator = variables.find(node->tok_.value_);
		if (iterator != variables.end()) {
			AST variableAST = iterator->second;

			node->tok_ = variableAST.root_->tok_;
			copyNode(node, variableAST.root_);
		}
	}
}

string AST::toInfix() const {
	string result; 
	toInfix(root_, result);
	return result;
}

//-----------------------------------------------------------------------------
// Evaluator
//-----------------------------------------------------------------------------

void Evaluator::split_expression(vector<Token>& expression, string& variable, vector<Token>& rhs)
{
	int equalIndex = -1;
	for (int i = 0; i < expression.size(); i++) {
		if (expression[i].type_ == TokType::equals) {
			equalIndex = i;
			break;
		}
	}

	if (equalIndex > -1) {
		// There is an equal size. We need to make sure there is a single variable before the equal sign.
		if (equalIndex != 1 || expression[0].type_ != TokType::variable) {
			// Invalid expression. There isnt a variable before the equal sign.
			throw "Invalid expression";
		}

		variable = expression[0].value_;
	}

	for (int i = (equalIndex == -1 ? 0 : 2); i < expression.size(); i++) {
		rhs.push_back(expression[i]);
	}
}

void Evaluator::display_variables()
{
	for (auto iterator = variables_.begin(); iterator != variables_.end(); iterator++) {
		cout << "Variable: " << iterator->first << "\n";
		cout << "Expression: ";
		iterator->second.display();
	}
}

void Evaluator::process_expression(vector<Token>& expression)
{
	cout << "Will process expression: Tokens so far:" << expression.size() << "\n";

	if (expression.size() == 1 && expression[0].type_ == TokType::display) {
		display_variables();
	}
	else {
		vector<Token> rhs;
		string variable;
		split_expression(expression, variable, rhs);

		cout << "Variable: " << variable << "\n";
		for (int i = 0; i < rhs.size(); i++) {
			cout << "RHS: Type: " << (int)rhs[i].type_ << "\tValue: " << rhs[i].value_ << "\n";
		}

		AST ast(rhs);
		ast.display();

		if (!variable.empty()) {
			cout << "Storing the expression for variable " << variable << "\n";
			variables_.insert_or_assign(variable, ast);
		}
		else
		{
			AST result = ast.simplify(variables_);
			cout << "Result: ";
			result.display();

			cout << "Infix: " << result.toInfix() << "\n";
		}
	}
}


void Evaluator::run() {
	Token t;
	ITokStream tsin(cin);
	vector <Token> expression;

	while (true) {
		tsin >> t;
		if (t.type_ == TokType::end) {
			break;
		}
		else if (t.type_ == TokType::expression_end) {
			if (expression.size() > 0) {
				process_expression(expression);
			}

			expression.clear();
		}
		else {
			expression.push_back(t);
		}
	}


	cout << "Good bye";
}


//-----------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------

void main() {
	Evaluator eval;
	eval.run();
}






