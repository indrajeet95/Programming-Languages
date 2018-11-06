#include <iostream>
#include <string.h>
#include <vector>
#include "Examiner.h"
#include <fstream>
#include <stack>

using namespace std;

enum class state {
	right = 0, endExpression, endInput, incorrectBrace, unequalBrace, invalidChar, wrongName, expectBrace, extraDot
};

state tokenGen(string inp);
state parser();
state error = state::right;

int idx=0;

enum class tokentype {
	openbrace = 0, closebrace, dot, integer, identifier, space, endexp, endinput
};

string inputExp = "";
vector <pair<string,tokentype>> tokens;

void fullClear() {
	inputExp = "";
	tokens.clear();
	error = state::right;
	idx = 0;
}

class Analyser {
public:
    Analyser();
    ~Analyser();
    sExp * baseMake();
    sExp * baseMaker();
	string toSfromExp(sExp * e);
};

Analyser::Analyser() {
    cout<<"<Program Started>"<<endl;
    sExp::isATOM();
  	Examiner object;
    sExp * Alist = sExp::sATOM("NIL");
    sExp * pDList = sExp::sATOM("NIL");
    sExp ** Dlist = &pDList;
	state ret = state::right;
	while (true) {
		sExp *  expTree = NULL;
		string expString;
		fullClear();
		ret = parser();
        if(ret == state::endExpression || ret == state::endInput) {
			expTree = baseMake();
            expString = toSfromExp(expTree);
            cout << "Dot notation>" << expString << endl;
            expTree = object.eval(expTree,Alist,Dlist);
            expString = toSfromExp(expTree);
            cout << ">" << expString << endl;
            if(ret == state::endInput)
                break;
        }
        else {
            throw std::runtime_error("***error:: invalid input***");
			break;
		}
	}
    cout<<"<End of program>"<<endl;
}

Analyser::~Analyser() {
    sExp::delsATOM();
}

sExp* Analyser::baseMake() {
	if(tokens.size() == 0)
		return NULL;
	if(tokens[idx].second == tokentype::openbrace) {
		idx++;
		if(tokens[idx].second == tokentype::closebrace)
			return baseMaker();
        sExp * le = baseMake();
        sExp * re;
		if(tokens[idx].second == tokentype::dot) {
			idx++;
            re = baseMake();
			if(tokens[idx].second != tokentype::closebrace) {
                throw std::runtime_error("***error:: expected )" + tokens[idx].first );
                return NULL;
            }
			idx++;
        }
        else
            re = baseMaker();
        sExp * newE = new sExp(notATOM);
        newE->setLeft(le);
        newE->setRight(re);
        return  newE;
    }
	else if(tokens[idx].second == tokentype::identifier) {
		idx++;
        return sExp::sATOM(tokens[idx-1].first);
    }
	else if(tokens[idx].second == tokentype::integer) {
        sExp * newIntegerExpression = new sExp(intATOM);
        newIntegerExpression->setVal(stoi(tokens[idx].first));
		idx++;
        return newIntegerExpression;
    }
    else {
        throw std::runtime_error(">Error:: Invalid token in s-expression:"+tokens[idx].first);
		return NULL;
    }
}

sExp* Analyser::baseMaker() {
	if(tokens[idx].second==tokentype::closebrace) {
		idx++;
        return sExp::sATOM("NIL");
    }
	else if(tokens[idx].second == tokentype::space) {
		idx++;
        sExp * newE = new sExp(notATOM);
        newE->setLeft(baseMake());
        newE->setRight(baseMaker());
        return newE;
    }
    else {
        throw std::runtime_error("***error:: Invalid token in s-expression: "+tokens[idx].first + "***");
        return NULL;
    }
}

string Analyser::toSfromExp(sExp * e) {
    string expString;
    switch(e->getType()){
        case notATOM:
            expString += '(';
            expString += toSfromExp(e->getLeft());
            expString += '.';
            expString += toSfromExp(e->getRight());
            expString += ')';
            break;
        case intATOM:
            expString += to_string(e->getVal());
            break;
        case symATOM:
            expString += e->getName();
            break;
        default:
            break;

    }
    return expString;
}

state tokenGen(string S) {
	int counter = 0;
	string T;
	int lenS = S.length();
	for(int i = 0; i <= lenS;)
	{
		int lenT = i;
		if(isalpha(S[i])) {
			T = S[i];
			i++;
			while(isalpha(S[i]) || isdigit(S[i])) {
				T = T + S[i];
				i++;
			}
			if((i - lenT) > 10)
				return state::wrongName;
			else
				tokens.push_back(make_pair(T, tokentype::identifier));
		}
		else if(isdigit(S[i]) || S[i] == '-' || S[i] == '+') {
			T = S[i];
			i++;
			while(isdigit(S[i])) {
				T = T + S[i];
				i++;
			}
			if((T == "-") || (T == "+"))
				return state::wrongName;
			else if((i - lenT) > 6)
				return state::wrongName;
			else
				tokens.push_back(make_pair(T, tokentype::integer));
		}
		else if(S[i] == '(') {
			if((i != 0) && (S[i-1] == ')'))
				return state::incorrectBrace;
			i++;
			tokens.push_back(make_pair("(", tokentype::openbrace));
			counter++;
		}
		else if(S[i] == ')') {
			i++;
			if(tokens.size() != 0) {
				if(tokens.back().second == tokentype::space)
					tokens.pop_back();
			}
			tokens.push_back(make_pair(")", tokentype::closebrace));
			counter--;
		}
		else if(S[i] == '.') {
			if(counter <= 0)
				return state::extraDot;
			if ((tokens.back().second == tokentype::dot) || (tokens.back().second == tokentype::openbrace) || S[i+1] == ')')
				return state::extraDot;
			tokens.push_back(make_pair(".", tokentype::dot));
			i++;
		}
		else if(S[i] == ' ' || S[i] == '	') {
			if(T.size() > 0) {
				if ((tokens.back().second == tokentype::dot) || (tokens.back().second == tokentype::openbrace) || (tokens.back().second == tokentype::space) || S[i+1] == '.') {
					i++;
					continue;
				}
				tokens.push_back(make_pair(" ", tokentype::space));
			}
			i++;
		}
		else if(S[i] == 0)
			return state::right;
		else if(S[i] == '\t')
			i++;
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

void printError(state current) {
	if(current == state::incorrectBrace)
		cout<<"**error: order of parenthesis incorrect**" << endl;
	else if(current == state::unequalBrace)
		cout<<"**error: parenthesis matching unequal**" << endl;
	else if(current == state::invalidChar)
		cout<<"**error: unexpected special character**" << endl;
	else if(current == state::extraDot)
		cout<<"**error: Unexpected dot**" << endl;
	else if(current == state::wrongName)
		cout <<"**error: size of the token is beyond limit**" << endl;
	else if(current == state::expectBrace)
		cout<<"ERROR: invalid inputExp-expression: Missing Paranthesis" << endl;
}

void checkandtokenGen() {
	state ret = state::right;
	ret = checkP(inputExp);
	if(ret != state::right) {
		printError(ret);
		fullClear();
		return;
	}
	ret = tokenGen(inputExp);
	if(ret != state::right)
		printError(ret);
	return;
}

void addinput(string inp) {
	if(inp.substr( inp.length()-1 ) != "." && inp.substr( inp.length()-1 ) != "$" && inp.substr( inp.length()-1 ) != ")" && inp.substr( inp.length()-1 ) != "(")
		inp.append(" ");
	inputExp = inputExp + inp;
}

state parseString(string line) {
	unsigned int len;
	len = line.length();
	if(len == 0)
		return state::right;
	addinput(line);
	if(len == 1) {
		if(line == "$") {
			inputExp.pop_back();
			checkandtokenGen();
			return state::endExpression;
		}
	}
	else if(len == 2) {
		if(line == "$$") {
			inputExp.pop_back();
			inputExp.pop_back();
			checkandtokenGen();
			return state::endInput;
		}
		else if(line.substr( len-1 ) == "$") {
			inputExp.pop_back();
			checkandtokenGen();
			return state::endExpression;
		}
	}
	else {
		if(line.substr( len-2 ) == "$$") {
			inputExp.pop_back();
			inputExp.pop_back();
			checkandtokenGen();
			return state::endInput;
		}
		else if(line.substr( len-1 ) == "$") {
			inputExp.pop_back();
			checkandtokenGen();
			return state::endExpression;
		}
	}
	return state::right;
}

state parser() {
	string line;
	state ret;
	do {
		getline(cin,line);
		ret = parseString(line);
	}
	while((ret != state::endInput) && (ret != state::endExpression));
	return ret;
}

int main() {
    Analyser Analyser;
    return 0;
}
