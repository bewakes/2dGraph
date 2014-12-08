#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "headers.h"


const int VALUE = 1;
const int OPERATOR = 2;
const int VARIABLE = 3;
const int CONSTANT = 4;
const int FUNCTION = 5;
const int LEFT_PARENTHESIS = 6;
const int RIGHT_PARENTHESIS = 7;

const char functions[8][5] = {"sinh", "cosh", "tanh", "log", "ln", "sin", "cos", "tan"}; 
const unsigned int numFunctions = 8;

/************************************************
   the Expression class receives the input string
   and converts it into the postfix format and then
   evaluates it.
   It contains the methods for operator precedence,
   and has methods to check whether the stirng or
   character is recognized token or not.
************************************************/

struct Token {
		union{
			char str[20];
			float numVal;
		};
		unsigned int type;
	};

class Expression {

private:
	unsigned int expressionIndex;
	char expression[100];
	vector<Token> tokens;

public:
	vector<Token> postfix;
	Expression(char *s);
	//void initialize(char *s);
	void tokenize();
	int precedence(char a);
	bool isDigit(char a);
	bool isOperator(char a);
	bool isFunction(char *a);
	bool isVariable(char a);
	bool isConstant(char a);
	bool precede(char a, char b); // returns true if precedes else false
	void toPostfix();
};

double extractValue(char*);

#endif
