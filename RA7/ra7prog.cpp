/* ra7prog.cpp
 * RA7
 * Jason Geoghegan
 * CS280-002
 * Professor Bassel Arafeh
*/

#include <iostream>
#include <string>
#include <stack>

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

int precedence_value(char op){
	int prec = 0;

	if (op == '*' || op == '/'){
		prec = 1;
	}
	else if (op == '+' || op == '-'){
		prec = 2;
	}
	return prec;
}

bool first_precedence_higher(char op1, char op2){
	bool op1higher = false;

	if (precedence_value(op1) <= precedence_value(op2)){ // >= should be fine
		op1higher = true;
	}
	return op1higher;
}

void infToPostfix(string instr){
	stack<char> charStack;
	string postfixexp = "";
	for (int i = 0; i < instr.length(); i++){
		char curchar = instr[i]; 
		if (isOperand(curchar)){
			postfixexp += curchar;
			postfixexp += ' ';
		}
		else if (isOperator(curchar)){
			while(!charStack.empty() && (charStack.top() != '(') 
				&& first_precedence_higher(charStack.top(), curchar)){
				postfixexp += charStack.top();
				postfixexp += ' ';
				charStack.pop();
			}
			charStack.push(curchar);
		}
		else if (curchar == '('){
			charStack.push(curchar);
		}
		else if (curchar == ')'){
			while (!charStack.empty() && charStack.top() != '('){
				postfixexp += charStack.top();
				postfixexp += ' ';
				charStack.pop();
			}
			charStack.pop();
		}
	}
	while(!charStack.empty()){
		postfixexp += charStack.top();
		postfixexp += ' ';
		charStack.pop();
	}
	cout << postfixexp << endl;
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