#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <cstring>
#include <iostream>
using namespace std;

class mathError{};

class parseError{
public:
    char s[100];
public:
    parseError() { strcpy(s,"input Error!!\n"); }
    virtual char * handle(){return s;}
};

class tokenError: public parseError {
public:
    char s[100];
    tokenError() { strcpy(s, "Unrecognized token!!\n"); }
    char * handle() { return s; }
};

class parenthesisError: public parseError {
public:
    char s[100];
     parenthesisError() { strcpy(s, "Parenthesis Error!!\n"); }
    char* handle() { return s; }
};

 parseError *const parse = new parseError;
 parseError *const token = new tokenError;
 parseError  *const parenthesis = new parenthesisError;


#endif
