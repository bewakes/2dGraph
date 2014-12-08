#include "expression.h"
#include "list.h"
#include "list.cpp"


double extractValue(char *s)
{
	double val;
	int num=0, dec=0;
	int tempcnt=0;
	bool flagdec = false;
	for(int i=0;s[i]!='\0'; i++) {
		if(s[i]=='.') { flagdec = true; continue; }
		if(!flagdec){
			num = num*10 + s[i] - '0';
		}
		else if(flagdec) { dec = dec*10 + s[i]- '0'; tempcnt+=1; }
	}
	val = num+ (double)dec/pow(10, tempcnt);
	return val;
}

/*************************************
	METHOD DEFINITIONS
*************************************/

/**************************************
	For the precedence of operators
**************************************/
int Expression::precedence(char a)
{
	char operators[] = {'-', '+', '*', '/', '^'}; // there are 5 general operators
	int precedeValue[] = {1,1,2,2,3};
	for (short int j=0; j<5; j++) {
		if(a==operators[j]) return precedeValue[j];
	}
}

/**************************************
	check whether digit or not
**************************************/
bool Expression::isDigit(char a)
{
	return (a>='0' and a<='9');
}

/**************************************
	check whether is operator or not
**************************************/
bool Expression::isOperator(char a)
{
	return (a=='-' or a=='+' or a=='*' or a=='/' or a=='^');
}

/**************************************
	check if string is function
**************************************/
bool Expression::isFunction(char *a)
{
	if(a[0]=='s' and a[1]=='i' and a[2]=='n' and a[3]=='h')return true; // if 'sinh' return true
	if(a[0]=='c' and a[1]=='o' and a[2]=='s' and a[3]=='h')return true; // if 'aosh' return true
	if(a[0]=='t' and a[1]=='a' and a[2]=='n' and a[3]=='h')return true; // if 'tanh' return true
	if(a[0]=='s' and a[1]=='i' and a[2]=='n' and a[3]!='h')return true; // if 'sin' return true
	if(a[0]=='c' and a[1]=='o' and a[2]=='s' and a[3]!='h')return true; // if 'aos' return true
	if(a[0]=='t' and a[1]=='a' and a[2]=='n' and a[3]!='h')return true; // if 'tan' return true
	if(a[0]=='l' and a[1]=='o' and a[2]=='g')return true; // if 'log' return true
	if(a[0]=='l' and a[1]=='n') return true;  // if 'ln' return true
	return false; // otherwise return false
}


bool Expression::isVariable(char a)
{
	return (a=='x' or a=='y' or a=='z');
}


bool Expression::isConstant(char a)
{
	return (a=='p' or a=='e');	// p for pi
}

Expression::Expression(char* s)
{
	strcpy(expression, s);
	expressionIndex = 0;
	tokenize();
	toPostfix();
}


/***************************************
	function precede(a, b):
	returns true if precedes
	returns false otherwise
***************************************/
bool Expression::precede(char a, char b)
{
	if(a=='^' and b=='^') return !(precedence(a)>precedence(b));
	return (precedence(a) >= precedence(b));
}



void Expression::tokenize()
{
	struct Token tempToken;
	char c;
	while((c=expression[expressionIndex])!='\0') { //isFunction(*expression);
		if(isDigit(c)) { // if first char is digit
			char temp[20];
			unsigned int count = 0, dotcount = 0;
			while(isDigit(c) or c=='.') {
                if(c=='.')dotcount++;
                if(dotcount>1) throw(parse); // error, more than 1 dots
				temp[count++] = c;
				c = expression[++expressionIndex];
			}
			temp[count] = '\0';
			tempToken.numVal = extractValue(temp);
			tempToken.type = VALUE;

			tokens.push_back(tempToken); // store the token
		}

		else if(isOperator(c)) { // symbol is an operator
            if(isOperator(expression[expressionIndex+1]))throw(parse); // error, two operators at same place
			if((expressionIndex==0 or expression[expressionIndex-1]=='(') and c=='-') c='#';
			char temp[2];
			temp[0]= c;
			temp[1] = '\0';

			strcpy(tempToken.str, temp);
			tempToken.type = OPERATOR;

			tokens.push_back(tempToken); // store the token

			c = expression[++expressionIndex];
		}

		else if(isVariable(c) or isConstant(c)) { // symbol is a variable or the constant
            char d = expression[expressionIndex+1];
            if(isVariable(d) or isConstant(d)) throw parse;
            if(c=='z')throw parse;
            char temp[2];
			temp[0]= c;
			temp[1] = '\0';

			strcpy(tempToken.str, temp);
			if(isVariable(c)) tempToken.type = VARIABLE;
			if(isConstant(c)) tempToken.type = CONSTANT;

			tokens.push_back(tempToken); // store the token

			c = expression[++expressionIndex];
		}

		else if (isFunction(expression+expressionIndex)) {
			char t2[3], t3[4], t4[5];
			int i = expressionIndex;
			char *tempc = expression;
			t2[0]=tempc[i]; t2[1]=tempc[i+1];t2[2]='\0';
			t3[0]=tempc[i];t3[1]=tempc[i+1];t3[2]=tempc[i+2];t3[3]='\0';
			t4[0]=tempc[i];t4[1]=tempc[i+1];t4[2]=tempc[i+2];t4[3]=tempc[i+3];t4[4]='\0';
			for(int x = 0;x<numFunctions; x++) {
				if(strcmp(t4, functions[x])==0) { strcpy(tempToken.str, functions[x]); expressionIndex+=4; break;}
				if(strcmp(t2, functions[x])==0) { strcpy(tempToken.str, functions[x]); expressionIndex+=2; break;}
				if(strcmp(t3, functions[x])==0) { strcpy(tempToken.str, functions[x]); expressionIndex+=3; break;}
			}
			tempToken.type = FUNCTION;

			tokens.push_back(tempToken); // store the token

			c = expression[expressionIndex];
		}

		else if (c=='(') {
			tempToken.type = LEFT_PARENTHESIS;
			char temp[2];
			temp[0] = c; temp[1]= '\0';

			strcpy(tempToken.str, temp);
			tokens.push_back(tempToken); // store the token

			expressionIndex++;
			c = expression[expressionIndex];
		}

		else if (c==')') {
			tempToken.type = RIGHT_PARENTHESIS;
			char temp[2];
			temp[0] = c; temp[1]= '\0';

			strcpy(tempToken.str, temp);
			tokens.push_back(tempToken); // store the token

			expressionIndex++;
			c = expression[expressionIndex];
		}

		else if (c==' ') { // ignore the spaces
			++expressionIndex;
			c = expression[expressionIndex];
		}

		else {
            throw (token);
			++expressionIndex;
			c= expression[expressionIndex];
		} // unrecognized token;
	}

}


void Expression::toPostfix()
{
	List<Token> operators;
    //if(tokens.size()<3) throw(parse);

	for(int x = 0;x<tokens.size();x++) {

		if(tokens[x].type==VALUE) { // if token is numeric value
            //if(x<tokens.size()-1 and tokens[x+1].type!=OPERATOR) throw parse;
			postfix.push_back(tokens[x]);

		}

		else if(tokens[x].type==OPERATOR) { // if the token is operator
            if(x==tokens.size()-1) throw(parse);
			if(operators.isEmpty())operators.append(tokens[x]);
			else if(strcmp(tokens[x].str, "#")==0){operators.append(tokens[x]);}
			else if(!operators.isEmpty() and strcmp(operators.topVal().str, "#")==0){
				postfix.push_back(operators.pop());
				operators.append(tokens[x]);
			}
			else if (!operators.isEmpty() and operators.topVal().type==FUNCTION) {
				postfix.push_back(operators.pop());
				if(operators.topVal().type==OPERATOR and precede(operators.topVal().str[0], tokens[x].str[0])){
					postfix.push_back(operators.pop());
				}
				operators.append(tokens[x]);
			}
			else {
				while(!operators.isEmpty() and operators.topVal().type==OPERATOR and precede(operators.topVal().str[0], tokens[x].str[0])) {
					postfix.push_back(operators.pop());
				}
				operators.append(tokens[x]);
			}
		}

		else if(tokens[x].type==LEFT_PARENTHESIS) operators.append(tokens[x]); // if token is left parenthesis

		else if(tokens[x].type==RIGHT_PARENTHESIS) { // if token is right parenthesis
			while(operators.topVal().type!=LEFT_PARENTHESIS) { // pop until left parenthesis occurs
				postfix.push_back(operators.pop());

                if(operators.isEmpty())throw(parenthesis); // error in parenthesis
			}
			operators.pop(); // popping the last '(' which is not needed in postfix

			if(!operators.isEmpty() and operators.topVal().type==FUNCTION) postfix.push_back(operators.pop());

		}

		else if(tokens[x].type==VARIABLE) { // if token is a variable
            if(tokens[x-1].type==VARIABLE or tokens[x+1].type==VARIABLE)throw(parse); // error, two variables at same place
           // if(x<tokens.size()-1 and tokens[x+1].type!=OPERATOR) throw parse;
			postfix.push_back(tokens[x]);
		}

		else if(tokens[x].type==CONSTANT) { // if token is a constant
            if(tokens[x-1].type==CONSTANT or tokens[x+1].type==CONSTANT)throw(parse); // error, two constants at same place
			postfix.push_back(tokens[x]);
		}

		else if(tokens[x].type==FUNCTION) { // if token is a function
			operators.append(tokens[x]);
		}

		else ;

	} // end of for

	while(!operators.isEmpty()) { //
        if(operators.topVal().type == LEFT_PARENTHESIS)throw(parenthesis); // error in parenthesis
		else postfix.push_back(operators.pop());
	}
	/*for(int x = 0; x<postfix.size(); x++) {
		if(postfix[x].type==VALUE) { cout << postfix[x].numVal<< " "; continue; }
		cout << postfix[x].str<< " ";
	}
	cout << endl; */
}

