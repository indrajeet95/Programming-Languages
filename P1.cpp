#include <iostream>
#include <string.h>
#include <vector>
#include <stack>
using namespace std;

string inputExp = "";
vector <string> tokens;

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

state tokenGen(string S);
state error = state::right;

struct Node {
	string data;
	Node *left;
	Node *right;
};

Node* New(string temp) {
	Node* N = new Node;
	N->data = temp;
	N->left = NULL;
	N->right = NULL;
	return N;
}

Node* TreeBaseMake();
Node* TreeBase();

void display(Node* root) {
	if(root != NULL) {
		if(root->left == NULL && root->right == NULL)
			cout << root->data;
		else {
			cout<<"(";
			display(root->left);
			cout<<" . ";
			display(root->right);
			cout<<")";
		}
	}
}

int idx = 0;
Node* TreeBase() {
	Node *root = NULL;
	if(tokens[idx] == ")") {
		idx++;
		return New("NIL");
	}
	else if(tokens[idx] == " ") {
		idx++;
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
	if(tokens[idx] == "(") {
		idx++;
		if(tokens[idx] == ")") {
			idx++;
			return New("NIL");
		}
		Node *left = TreeBaseMake();
		Node *right = NULL; 
		if(tokens[idx]==".") {
			idx++;
			right = TreeBaseMake();
			if(tokens[idx] != ")") {
				error = state::incorrectParathensis;
				return NULL;
			}
			idx++;
		}
		else
			right = TreeBase();
		root = New(" ");
		root->left = left;
		root->right = right;
		return root;
	}
	else if((tokens[idx][0] == '-') || (tokens[idx][0]>='0' && tokens[idx][0]<='9')  || (tokens[idx][0]>='A' && tokens[idx][0]<='Z') || (tokens[idx][0]>='a' && tokens[idx][0]<='z')) {
		root = New(tokens[idx]);
		idx++;
		return root;
	}
	return root;
}

state tokenGen(string S) {
	int counter = 0;
	int i = 0;
	string T;
	int lenS = S.length();
	for(int i = 0;i<=lenS;)
	{
		int lenT = i;
		if(S[i] == 0)
			return state::right;
		else if(S[i] == '\t')
			i++;
		else if(isalpha(S[i])) {
			T = S[i];
			i++;
			while(isalpha(S[i]) || isdigit(S[i])) {
				T = T + S[i];
				i++;
			}
			tokens.push_back(T);
		}
		else if(isdigit(S[i]) || S[i] == '-') {
			T = S[i];
			i++;
			while(isdigit(S[i])) {
				T = T + S[i];
				i++;
			}
			tokens.push_back(T);
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
		else
			return state::invalidChar;
	}
	return state::right;
}

state checkP(string s) {
	stack<char> holder;
	char x;
	for(int i=0; i<s.length();i++)
	{
		if(s[i] == '(') {
			holder.push(s[i]);
			continue;
		}
		if(s.empty())
			return state::incorrectBrace;
		if(s[i] == ')')
		{
			x = holder.top();
			holder.pop();
		}
	}
	if(holder.size() == 0)
		return state::right;
	else
		return state::unequalBrace;
}

void printError(state flag) {
	if(flag == state::incorrectBrace)
		cout<<"ERROR: Incorrectly placed opening and closing paranthesis" << endl;
	else if(state::unequalBrace == flag)
		cout<<"ERROR: Unequal opening and closing paranthesis" << endl;
	else if(state::invalidChar == flag)
		cout<<"ERROR: Invalid character found in the input" << endl;
	else if(state::extraDot == flag)
		cout<<"ERROR: Unexpected dot found" << endl;
	else if(state::incorrectParathensis == flag)
		cout<<"ERROR: Missing Paranthesis" << endl;		
}

void clearTokens() {
	inputExp = "";
	tokens.clear();
}

void executeExp() {
	state ans = state::right;
	ans = checkP(inputExp);
	if(ans != state::right) {
		printError(ans);
		clearTokens();
		return;
	}
	ans = tokenGen(inputExp);
	if(ans == state::right) {
		idx = 0;
		error = state::right;
		Node *toproot;
		toproot = TreeBaseMake();
		if(error != state::right) {
			printError(error);
			clearTokens();
			return;
		}
		else {
			display(toproot);
			cout << endl;
			toproot = NULL;
		}
	}
	else {
		printError(ans);
		clearTokens();
		return;
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
	int len = line.length();
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