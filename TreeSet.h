#ifndef TREESET_H                                  //2017.1.3
#define TREESET_H
#include "rbtree.h"

template<class TN>
class TreeSet:public rbtree<TN>{
public:
	void push(const TN &data);
};

template<class TN>
void TreeSet<TN>::push(const TN &data)
{
	if(this->find(data)==this->end())rbtree<TN>::push(data);
}

#endif //#ifndef TREESET_H
