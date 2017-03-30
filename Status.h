#ifndef STATUS_H
#define STATUS_H
#include "Point.h"
#include "TreeSet.h"
#include "Vector.h"
#include "Game.h"
#include "KM.h"

#define INF 0x3f3f3f3f

class Status
{
public:
	Vector<Point> *aim;
    int g;
    int h;
    int pre;
    TreeSet<Point> box;
    Point man;
    Status()
    {
    }
    Status(const Status &o)
    {
    	g=o.g;
    	h=o.h;
    	man=o.man;
    	pre=o.pre;
    	aim=o.aim;
    	for(TreeSet<Point>::iterator i=o.box.begin();i!=o.box.end();i++)box.push(*i);
    }
    void solveH()
    {
    	KM km(aim->getLength());
		TreeSet<Point>::iterator it;
		int i;
		h=INF;
    	for(it=box.begin(),i=0;it!=box.end();it++,i++)
    	{
    		h=min(h,man-*it-1);
    		for(int j=0;j<aim->getLength();j++)
    		    km.add_edge(i,j,-(*it-((*aim)[j])));
    	}
    	h+=-km.solve();
//    	h=min(1,h);
    }
    void operator=(const Status &o)
    {
    	g=o.g;
    	h=o.h;
    	man=o.man;
    	pre=o.pre;
    	aim=o.aim;
    	box.clear();
    	for(TreeSet<Point>::iterator i=o.box.begin();i!=o.box.end();i++)box.push(*i);
    }
    bool operator<(const Status &o)const
    {
    	if(h==-1)
    	{
    		if(man!=o.man)return man<o.man;
    		TreeSet<Point>::iterator i=box.begin(),j=o.box.begin();
    		while(i!=box.end())
    		{
    		    if(*i!=*j)return *i<*j;
    		    i++;j++;
    		}
    		return false;
    	}
    	return g+h<o.g+o.h;
    }
	bool operator==(const Status &o)const
    {
    	if(h==-1)
    	{
		    if(man!=o.man)return false;
    		TreeSet<Point>::iterator i=box.begin(),j=o.box.begin();
    		while(i!=box.end())
    		{
    		    if(*i!=*j)return false;
    		    i++;j++;
    		}
    		return true;
    	}
    	return g+h==o.g+o.h;
    }
};

#endif // STATUS_H
