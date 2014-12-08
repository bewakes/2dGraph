#include "list.h"

// ************************************
// methods definitions for our List class
// ************************************
struct Token;
template <class T> T List<T> :: pop()
{
	if(isEmpty()) {
        throw(parse);
    }
	T x;
	NODEPTR p;
	p = top;
	x = top->info;
    top = top->next;
    total--;
    delete p;
	return x;
}

template <class T> T List<T> ::getVal(int index) {
	if(index>=total or index<0) {
		cout << "Index out of range!!! returning 0 !!\n";
		//return 0;
	}
	int c=0;
	NODEPTR p;
	p=top;
	while(p!=NULL) {
		if(c==(total-1-index)) return p->info;
		c++;
		p=p->next;
	}
}

template <class T> void List<T> :: append(T x)
{
	NODEPTR p;
	p = new node;
	if(isEmpty()) p->next = NULL;
	else p->next = top;
	top = p;
	top->info = x;
	total++;
	return;
}


template <class T> T List<T> :: topVal()
{
	return top->info;
}

template <class T> void List<T>::printList()
{
	NODEPTR p;
	p = top;
	while(p!=NULL) {
		cout << p->info << " ";
		p = p->next;
	}
	cout << endl;
	return;
}

template <class T> void List<T>::clearList()
{
	NODEPTR p;
	p=top;
	while(total!=0) {
		pop();
	}
}

