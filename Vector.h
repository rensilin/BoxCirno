#ifndef VECTOR_H                                          //2017.1.4
#define VECTOR_H

template<class TN>
class Vector{
private:
	TN *data;
	int length;
	int maxL;
	void resize(int L)
	{
		maxL=L;
		TN *temp=new TN[maxL];
		for(int i=0;i<length;i++)temp[i]=data[i];
        delete[]data;
		data=temp;
	}
public:
	
	class iterator{
	private:
		TN *i;
		iterator(TN *I):i(I)
		{
		}
	public:
		friend Vector;
		iterator()
		{
		}
		TN& operator*()
		{
			return *i;
		}
		TN* operator->()
		{
			return i;
		}
		bool operator==(const iterator &x)const
		{
			return i==x.i;
		}
		bool operator!=(const iterator &x)const
		{
			return i!=x.i;
		}
		iterator operator++()
		{
			i++;
			return *this;
		}
		iterator operator++(int)
		{
			iterator temp=*this;
			i++;
			return temp;
		}
		iterator operator--()
		{
			i--;
			return *this;
		}
		iterator operator--(int)
		{
			iterator temp=*this;
			i--;
			return temp;
		}
	};
	
	Vector(int i=1)
	{
		length=0;
		maxL=i<<1;
		data=new TN[maxL];
	}
	Vector(const Vector<TN> &x)
	{
	    length=x.length;
	    maxL=length<<1;
		data=new TN[maxL];
		for(int i=0;i<length;i++)data[i]=x.data[i];
	}

    void operator=(const Vector<TN> &x)
    {
        if(this==&x)return;
        delete[]data;
        length=x.length;
        maxL=length<<1;
        data=new TN[maxL];
        for(int i=0;i<length;i++)data[i]=x.data[i];
    }

	Vector(TN Data[],int Length)
	{
		length=Length;
	    maxL=length<<1;
		data=new TN[maxL];
		for(int i=0;i<length;i++)data[i]=Data[i];
	}
	~Vector()
	{
		delete[] data;
	}
	
	void push(const TN &x)
	{
		data[length++]=x;
		if(length>(maxL>>1))resize(maxL<<1);
	}
	
	void pop()
	{
		length--;
		if(length<(maxL>>2))resize(maxL>>1);
	}
	
	TN& operator[](int i)
	{
		if(i>=length)
		{
		    if(i>=(maxL>>1))resize(i<<1);
		    length=i+1;
		}
		return data[i];
	}
	
	int getLength()const
	{
		return length;
	}
	
	iterator begin()const
	{
		return iterator(data);
	}
	
	iterator end()const
	{
		return iterator(data+length);
	}
	
};

#endif //#ifndef VECTOR_H
