/* ra7prog.cpp
 * RA7
 * Jason Geoghegan
 * CS280-002
 * Professor Bassel Arafeh
*/

#include <iostream>
#include <string>
#include <stack>
#include <vector>

using namespace std;

bool isOperator(char ch){
	if (ch == '+' || ch == '-' || ch == '*' || ch == '/'){
		return true;
	}
	else{
		return false;
	}
}

bool isOperand(char ch){
	if (isalnum(ch)){
		return true;
	}
	else{
		return false;
	}
}

string stack_to_postfix(stack<char> charStack){
	string postfixExpr;
	if (!charStack.empty()){
		for (int i = 0; i < charStack.size(); i++){
			if (charStack.top() == ')'){
				charStack.pop();
				break;
			}
			else{
				postfixExpr += charStack.top();
				charStack.pop();
			}
		}
	}
	return postfixExpr;
}

void infToPostfix(string instr){
	string postfixExpr;
	int len = instr.length();
	stack <char> charStack;

	for (int i = 0; i < len; i++){
		char curchar = instr[i];
		if (curchar == '('){
			charStack.push(curchar);
		}
		else if (isalpha(curchar)){
			postfixExpr += curchar;
		}
		else if (curchar == ')'){
			postfixExpr += stack_to_postfix(charStack);
		}
	}
	postfixExpr += stack_to_postfix(charStack);
	cout << postfixExpr << endl;
	return;
}

int main ( ) 
{
	string instr;
	cout<<"Please enter an infix notation expression using single lowercase characters:" << endl;
	getline(cin, instr);
	infToPostfix(instr);
	
	return 0;	
}