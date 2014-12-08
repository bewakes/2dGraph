#ifndef LIST_H
#define LIST_H
#include "headers.h"
// ************************************
// The list can be used as stack also
// the operations are: pop(), append()
// ************************************

template<class T>
class List {
private:
	struct node {
		T info;
		struct node * next;
	};
	typedef struct node * NODEPTR;
	NODEPTR top;
	int total;
public:
	// methods
	List() { top = NULL; total = 0; }        // constructor
	bool isEmpty() { return top==NULL; }
	T pop();
	NODEPTR getTop() { return top;}
	void append(T x);
	T topVal();
	T getVal(int index);
	int totalCnt(){ return total;}
	void printList();
	void clearList();
}; // end of class declaration

#endif
