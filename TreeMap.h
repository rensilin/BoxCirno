#ifndef TREEMAP_H                                  //2017.1.3(2)
#define TREEMAP_H
#include "TreeSet.h"

template<class KEY,class VALUE>
class TreeMap{
public:
	class Entry;
	TreeSet<Entry>& getEntrySet();
    class iterator;
    int size();
    void push(const Entry &entry);
    void push(const KEY &key,const VALUE &value);
    bool del(const KEY &key);
    bool del(iterator it);
    iterator find(const KEY &key)const;
    iterator begin()const; 
    iterator end()const; 
    iterator rbegin()const; 
    iterator rend()const; 
    void clear();
private:
	TreeSet<Entry> entrySet;
};

template<class KEY,class VALUE>
void TreeMap<KEY,VALUE>::clear()
{
	entrySet.clear();
}

template<class KEY,class VALUE>
typename TreeMap<KEY,VALUE>::iterator TreeMap<KEY,VALUE>::begin()const
{
	return TreeMap<KEY,VALUE>::iterator(entrySet.begin());
}

template<class KEY,class VALUE>
typename TreeMap<KEY,VALUE>::iterator TreeMap<KEY,VALUE>::rbegin()const
{
	return iterator(entrySet.rbegin());
}

template<class KEY,class VALUE>
typename TreeMap<KEY,VALUE>::iterator TreeMap<KEY,VALUE>::end()const
{
	return iterator(entrySet.end());
}

template<class KEY,class VALUE>
typename TreeMap<KEY,VALUE>::iterator TreeMap<KEY,VALUE>::rend()const
{
	return iterator(entrySet.rend());
}

template<class KEY,class VALUE>
typename TreeMap<KEY,VALUE>::iterator TreeMap<KEY,VALUE>::find(const KEY &key)const
{
	return iterator(entrySet.find(Entry(key)));
}

template<class KEY,class VALUE>
bool TreeMap<KEY,VALUE>::del(iterator it)
{
	return entrySet.del(it.it);
}

template<class KEY,class VALUE>
bool TreeMap<KEY,VALUE>::del(const KEY &key)
{
	return entrySet.del(Entry(key));
}

template<class KEY,class VALUE>
void TreeMap<KEY,VALUE>::push(const KEY &key,const VALUE &value)
{
	push(Entry(key,value));
}

template<class KEY,class VALUE>
void TreeMap<KEY,VALUE>::push(const Entry &entry)
{
	entrySet.del(entry);
	entrySet.push(entry);
}

template<class KEY,class VALUE>
int TreeMap<KEY,VALUE>::size()
{
	return entrySet.size();
}

template<class KEY,class VALUE>
class TreeMap<KEY,VALUE>::iterator{
private:
	typename TreeSet<Entry>::iterator it;
	iterator(const typename TreeSet<Entry>::iterator &It):it(It)
	{
	}
public:
	iterator()
	{
	}
	friend TreeMap;
	const Entry& operator*()const
	{
		return *it;
	}
	const Entry* operator->()const
	{
		return &(*it);
	}
	friend bool operator==(const iterator &a,const iterator &b)
	{
		return a.it==b.it;
	}
	friend bool operator!=(const iterator &a,const iterator &b)
	{
		return a.it!=b.it;
	}
	iterator operator++()
	{
		++it;
		return *this;
	}
	iterator operator++(int)
	{
		iterator temp=*this;
		++it;
		return temp;
	}
	iterator operator--()
	{
		--it;
		return *this;
	}
	iterator operator--(int)
	{
		iterator temp=*this;
		--it;
		return temp;
	}
};

template<class KEY,class VALUE>
TreeSet<typename TreeMap<KEY,VALUE>::Entry>& TreeMap<KEY,VALUE>::getEntrySet()
{
	return entrySet;
}

template<class KEY,class VALUE>
class TreeMap<KEY,VALUE>::Entry{
public:
	KEY key;
	VALUE value;
	Entry()
	{
	}
	Entry(const KEY &Key):key(Key)
	{
	}
	Entry(const KEY &Key,const VALUE &Value):key(Key),value(Value)
	{
	}
	bool operator<(const Entry &x)const
	{
		return key<x.key;
	}
	bool operator==(const Entry &x)const
	{
		return key==x.key;
	}
};

#endif //#ifndef TREEMAP_H
