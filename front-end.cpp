#include <iostream>
#include <string.h>
#include <vector>
using namespace std;

enum class state {
	right = 0,
	status,
	incorrectBrace,
	unequalBrace,
	invalidChar,
	incorrectParathensis,
	incorrectName,
	extraDot
};

state createtokens(string S);
state error = state::right;

int tokenIndex = 0;

struct Node {
	string data;
	Node *left;
	Node *right;
};

Node* TreeBaseMake();
Node* TreeBase();

Node* New(string temp) {
	Node* N = new Node;
	N->data = temp;
	N->left = NULL;
	N->right = NULL;
	return N;
}

string inputExp = "";
vector <string> tokens;

void printoutput(Node* root) {
	if(root != NULL) {
		if(root->left == NULL && root->right == NULL)
			cout << root->data;
		else {
			cout<<"(";
			printoutput(root->left);
			cout<<" . ";
			printoutput(root->right);
			cout<<")";
		}
	}
}

Node* TreeBase() {
	Node *root = NULL;
	if(tokens[tokenIndex] == ")") {
		tokenIndex++;
		return New("NIL");
	}
	else if(tokens[tokenIndex]==" ") {
		tokenIndex++;
		root = New(" ");
		root->left = TreeBaseMake();
		root->right = TreeBase();
		return root;
	}
	else
		return NULL;
}

Node* TreeBaseMake() {
	Node *root = NULL;
	if(tokens[tokenIndex]=="(") {
		tokenIndex++;
		if(tokens[tokenIndex]==")") {
			tokenIndex++;
			return New("NIL");
		}
		Node *left = TreeBaseMake();
		Node *right = NULL; 
		if(tokens[tokenIndex]==".") {
			tokenIndex++;
			right = TreeBaseMake();
			if(tokens[tokenIndex]!=")") {
				error = state::incorrectParathensis;
				return NULL;
			}
			tokenIndex++;
		}
		else
			right = TreeBase();
		root = New(" ");
		root->left = left;
		root->right = right;
		return root;
	}
	else if((tokens[tokenIndex][0]==45) || (tokens[tokenIndex][0]>='0' && tokens[tokenIndex][0]<='9')  || (tokens[tokenIndex][0]>='A' && tokens[tokenIndex][0]<='Z') || (tokens[tokenIndex][0]>='a' && tokens[tokenIndex][0]<='z')) {
		root = New(tokens[tokenIndex]);
		tokenIndex++;
		return root;
	}
	return root;
}

//create array with populated tokens. Check all the conditions for validness.
state createtokens(string S) { //isalpha
	int counter = 0;
	int i = 0;
	string T;
	int lenS = S.length();
	for(int i = 0;i<=lenS;)
	{
		int lenT = i;
		if(isalpha(S[i])) {
			T = S[i];
			i++;
			while(isalpha(S[i]) || isdigit(S[i])) {
				T = T + S[i];
				i++;
			}
//			if((i - lenT) > 10)
//				return state::incorrectName;
//			else
			tokens.push_back(T);
		}
		else if(isdigit(S[i]) || S[i] == '-') {
			T = S[i];
			i++;
			while(isdigit(S[i])) {
				T = T + S[i];
				i++;
			}
			if(T == "-")
				return state::incorrectName;
			else
				tokens.push_back(T);
//			if((i - lenT) <= 10)
//				tokens.push_back(T);
//			else if((i - lenT) <= 10)
//				tokens.push_back(T);
//			else
//				return state::incorrectName;
		}
		else if(S[i] == '(') {
			if(S[i-1] == ')')
				return state::incorrectBrace;
			i++;
			tokens.push_back("(");
			counter++;
		}
		else if(S[i] == ')') {
			i++;
			tokens.push_back(")");
			counter--;
		}
		else if(S[i] == '.') {
			if(counter <= 0)
				return state::incorrectParathensis;
			if ((tokens.back() == ".") || (tokens.back() == "(") || S[i+1] == ')') {
				return state::extraDot;
			}
			tokens.push_back(".");
			i++;
		}
		else if(S[i] == ' ') {
			if(T.size() > 0) {
				if ((tokens.back() == ".") || (tokens.back() == "(") || (tokens.back() == " ") || S[i+1] == '.') {
					i++;
					continue;
				}
				tokens.push_back(" ");
			}
			i++;
		}
		else if(S[i] == 0)
			return state::right;
		else if(S[i]=='\t')
			i++;
		else
			return state::invalidChar;
	}
	return state::right;
}

state checkparenthesis(string s) { //use stack for counting braces 
	int i=0, countOpen=0;
	while(i<s.length()) {
		if(s[i] == 40) {
			countOpen++;
			i++;
		}
		else if(s[i]==41) {
			countOpen--;
			if(countOpen < 0)
				return state::incorrectBrace;
			i++;
		}
		else
			i++;
	}
	if(countOpen == 0)
		return state::right;
	else
		return state::unequalBrace;
}

void printError(state flag) { //if else instead of switch case
	switch(flag) {
		case state::incorrectBrace:
			cout<<"ERROR: Incorrectly placed opening and closing paranthesis" << endl;
			break;
		case state::unequalBrace:
			cout<<"ERROR: Unequal opening and closing paranthesis" << endl;
			break;
		case state::invalidChar:
			cout<<"ERROR: Invalid character found in the input" << endl;
			break;
		case state::incorrectName:
			cout<<"ERROR: Name of an atom or a value is incompliant" << endl;
			break;
		case state::extraDot:
			cout<<"ERROR: Unexpected dot found" << endl;
			break;
		case state::incorrectParathensis:
			cout<<"ERROR: Missing Paranthesis" << endl;
			break;
		default:
			cout << "NO IDEAA!!!" << endl;
	}
}

void clearTokens() {
	inputExp = "";
	tokens.clear();
}

void executeExp() {
	state ans = state::right;
	ans = checkparenthesis(inputExp);
	if(ans != state::right) {
		printError(ans);
		clearTokens();
		return;
	}
	ans = createtokens(inputExp);
	if(ans != state::right) {
		printError(ans);
		clearTokens();
		return;
	}
	else {
		tokenIndex = 0;
		error = state::right;
		Node *toproot;
		toproot = TreeBaseMake();
		if(error != state::right) {
			printError(error);
			clearTokens();
			return;
		}
		else {
			printoutput(toproot);
			cout << endl;
			toproot = NULL;
		}
	}
	clearTokens();
	return;
}

void addInput(string s) {
	if(s.back() != '.' && s.back() != '$')
		s.append(" ");
	inputExp = inputExp + s;
}

state parseString(string line) {
	unsigned int len = line.length();
	addInput(line);
	if(line == "$") {
			inputExp.pop_back();
			executeExp();
			return state::right;
	}
	else if(line == "$$") {
			inputExp.pop_back();
			inputExp.pop_back();
			executeExp();
			cout << "Bye!\n";
			return state::status;
	}
	else
		return state::right;
}

int main() {
	string L;
	state ans;
	do {
		getline(cin,L);
		ans = parseString(L);
	}
	while(ans != state::status);
	return 0;
}