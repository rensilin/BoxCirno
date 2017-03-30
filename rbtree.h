#ifndef RBTREE_H                                      //17.1.5(–ﬁ∏¥size_of_tree≥¨¥Û–ÕBUG)
#define RBTREE_H
#undef BLACK
#undef RED
#define BLACK 0
#define RED   1
template<class _TN>
class rbtree{
private:
    int size_of_tree;
    struct rbnode;
    rbnode *root;
    rbnode *nil;
    void right_rotate(rbnode *x);
    void left_rotate(rbnode *x);
    void transplant(rbnode *x,rbnode *y);
    void push_fixup(rbnode *x);
    void del_fixup(rbnode *x);
    void del_node(rbnode *x);
    rbnode* maxnode(rbnode *x)const;
    rbnode* minnode(rbnode *x)const;
public:
    class iterator;
    rbtree()
    {
    	root=nil=new rbnode(BLACK);
        size_of_tree=0;
    }
    ~rbtree();
    int size();
    void clear();
    void push(const _TN &data);
    bool del(const _TN &data);/*del one data in this tree*/
    bool del(rbtree<_TN>::iterator it);
    iterator find(const _TN &data)const;
    iterator lower_bound(const _TN &data)const;
    iterator upper_bound(const _TN &data)const;
    int count(const _TN &data)const;
    const _TN& maximum()const;
    const _TN& minimum()const;
    iterator begin()const; 
    iterator end()const; 
    iterator rbegin()const; 
    iterator rend()const; 
};

template<class _TN>
void rbtree<_TN>::clear()
{
	size_of_tree=0;
	rbnode *i=nil->next,*j;
	while(i!=nil)
	{
		j=i->next;
		delete i;
		i=j;
	}
	root=nil;
}

template<class _TN>
rbtree<_TN>::~rbtree()
{
	clear();
	delete nil;
}

template<class _TN>
struct rbtree<_TN>::rbnode{
    _TN data;
    int color;
    rbnode *left;
    rbnode *right;
    rbnode *p;
    rbnode *pre;
    rbnode *next;
    ~rbnode()
    {
    	pre->next=next;
    	next->pre=pre;
    }
    rbnode()
    {
    }
    rbnode(int Color)
    {
		color=Color;
		pre=next=this;
    }
    rbnode(const _TN &rb_data,int rb_color,rbnode *rb_left,rbnode *rb_right,rbnode *rb_p)
    {
        data=rb_data;
        color=rb_color;
        left=rb_left;
        right=rb_right;
        p=rb_p;
    }
};

template<class _TN>
class rbtree<_TN>::iterator{
private:
	rbnode *node;
	iterator(rbnode *Node):node(Node)
	{
	}
public:
	iterator()
	{
	}
	friend rbtree;
	const _TN& operator*()const
	{
		return node->data;
	}
	const _TN* operator->()const
	{
		return &(node->data);
	}
	friend bool operator==(const iterator &a,const iterator &b)
	{
		return a.node==b.node;
	}
	friend bool operator!=(const iterator &a,const iterator &b)
	{
		return a.node!=b.node;
	}
	iterator operator++()
	{
		node=node->next;
		return *this;
	}
	iterator operator++(int)
	{
		iterator temp=*this;
		node=node->next;
		return temp;
	}
	iterator operator--()
	{
		node=node->pre;
		return *this;
	}
	iterator operator--(int)
	{
		iterator temp=*this;
		node=node->pre;
		return temp;
	}
};

template<class _TN>
int rbtree<_TN>::size()
{
    return size_of_tree;
}

template<class _TN>
void rbtree<_TN>::del_fixup(rbnode *x)
{
	while(x!=root&&x->color==BLACK)
	{
	    rbnode *w;/*brother*/
		if(x==x->p->left)
		{
			w=x->p->right;
			if(w->color==RED)
			{
				w->color=BLACK;
				x->p->color=RED;
				left_rotate(x->p);
				w=x->p->right;
			}
			if(w->left->color==BLACK&&w->right->color==BLACK)
			{
				w->color=RED;
				x=x->p;
		    }
		    else
		    {
		    	if(w->left->color==RED)
		    	{
		    		w->left->color=BLACK;
		    		w->color=RED;
		    		right_rotate(w);
		    		w=w->p;
		    	}
		    	w->right->color=BLACK;
		    	w->color=x->p->color;
		    	x->p->color=BLACK;
		    	left_rotate(x->p);
		    	return;
		    }
		}
		else
		{
			w=x->p->left;
			if(w->color==RED)
			{
				w->color=BLACK;
				x->p->color=RED;
				right_rotate(x->p);
				w=x->p->left;
			}
			if(w->left->color==BLACK&&w->right->color==BLACK)
			{
				w->color=RED;
				x=x->p;
		    }
		    else
		    {
		    	if(w->right->color==RED)
		    	{
		    		w->right->color=BLACK;
		    		w->color=RED;
		    		left_rotate(w);
		    		w=w->p;
		    	}
		    	w->left->color=BLACK;
		    	w->color=x->p->color;
		    	x->p->color=BLACK;
		    	right_rotate(x->p);
		    	return;
		    }
		}
	}
	x->color=BLACK;
}

template<class _TN>
void rbtree<_TN>::del_node(rbnode *x)
{
	size_of_tree--;
	int original_color=x->color;
	rbnode *z;
	if(x->right==nil)
	{
		transplant(x,x->left);
		x->left->p=x->p;
		z=x->left;
		delete x;
		x=z;
	}
	else if(x->left==nil)
	{
		transplant(x,x->right);
		x->right->p=x->p;
		z=x->right;
		delete x;
		x=z;
	}
	else
	{
		rbnode *y=x;
		z=minnode(y->right);
		x=z->right;
		original_color=z->color;
		transplant(z,x);
		x->p=z->p;
		transplant(y,z);
		z->color=y->color;
		z->p=y->p;
		z->left=y->left;
		z->right=y->right;
		z->left->p=z;
		z->right->p=z;
		delete y;
	}
	if(original_color==BLACK)del_fixup(x);
}

template<class _TN>
typename rbtree<_TN>::rbnode* rbtree<_TN>::maxnode(rbnode *x)const
{
    while(x->right!=nil)x=x->right;
    return x;
}

template<class _TN>
typename rbtree<_TN>::rbnode* rbtree<_TN>::minnode(rbnode *x)const
{
    while(x->left!=nil)x=x->left;
    return x;
}

template<class _TN>
void rbtree<_TN>::transplant(rbnode *x,rbnode *y)
{
	if(x==root)
	{
	    root=y;
	}
	else if(x==x->p->right)x->p->right=y;
	else x->p->left=y;
}

template<class _TN>
bool rbtree<_TN>::del(const _TN &data)
{
	rbnode *i=root;
	while(i!=nil)
	{
		if(data==i->data)
		{
			del_node(i);
		    return true;
		}
		if(data<(i->data))i=i->left;
		else i=i->right;
	}
	return false;
}

template<class _TN>
bool rbtree<_TN>::del(rbtree<_TN>::iterator it)
{
	if(it.node==nil)return false;
	del_node(it.node);
	return true;
}

template<class _TN>
typename rbtree<_TN>::iterator rbtree<_TN>::find(const _TN &data)const
{
	rbnode *i=root;
	while(i!=nil)
	{
		if(data==i->data)return iterator(i);
		if(data<i->data)i=i->left;
		else i=i->right;
	}
	return iterator(nil);
}

template<class _TN>
typename rbtree<_TN>::iterator rbtree<_TN>::lower_bound(const _TN &data)const
{
	rbnode *i=root;
	while(i!=nil)
	{
		if(i->data<data)i=i->right;
		else
		{
			while(i->pre!=nil&&!(i->pre->data<data))i=i->pre;
			return iterator(i);
		}
	}
	return iterator(nil);
}

template<class _TN>
typename rbtree<_TN>::iterator rbtree<_TN>::upper_bound(const _TN &data)const
{
	rbnode *i=root;
	while(i!=nil)
	{
		if(i->data<=data)i=i->right;
		else
		{
			while(i->pre!=nil&&data<i->pre->data)i=i->pre;
			return iterator(i);
		}
	}
	return iterator(nil);
}

template<class _TN>
typename rbtree<_TN>::iterator rbtree<_TN>::begin()const
{
	return iterator(nil->next);
}

template<class _TN>
typename rbtree<_TN>::iterator rbtree<_TN>::end()const
{
	return iterator(nil);
}

template<class _TN>
typename rbtree<_TN>::iterator rbtree<_TN>::rbegin()const
{
	return iterator(nil->pre);
}

template<class _TN>
typename rbtree<_TN>::iterator rbtree<_TN>::rend()const
{
	return iterator(nil);
}

template<class _TN>
void rbtree<_TN>::push_fixup(rbnode *x)
{
    while(x->p->color==RED)
    {
        rbnode *w;/*uncle*/
		if(x->p==x->p->p->left)
		{
			w=x->p->p->right;
			if(w->color==RED)
			{
				x->p->color=w->color=BLACK;
				x=x->p->p;
				x->color=RED;
			}
			else
			{
				if(x==x->p->right)
				{
					x=x->p;
					left_rotate(x);
				}
				x->p->color=BLACK;
				x->p->p->color=RED;
				right_rotate(x->p->p);
			}
		}
		else
		{
			w=x->p->p->left;
			if(w->color==RED)
			{
				x->p->color=w->color=BLACK;
				x=x->p->p;
				x->color=RED;
			}
			else
			{
				if(x==x->p->left)
				{
					x=x->p;
					right_rotate(x);
				}
				x->p->color=BLACK;
				x->p->p->color=RED;
				left_rotate(x->p->p);
			}
		}
    }
    root->color=BLACK;
}

template<class _TN>
void rbtree<_TN>::push(const _TN &data)
{
    size_of_tree++;
    if(root==nil)
	{
	    root=new rbnode(data,BLACK,nil,nil,nil);
	    nil->pre=nil->next=root;
	    root->pre=root->next=nil;
	}
    else
    {
        rbnode *x=root,*y;
        do{
            y=x;
            if(data<(x->data))x=x->left;
            else x=x->right;
        }while(x!=nil);
        x=new rbnode(data,RED,nil,nil,y);
        if(data<(y->data))
		{
		    y->left=x;
		    x->pre=y->pre;
		    x->next=y;
		}
        else
		{
		    y->right=x;
		    x->next=y->next;
		    x->pre=y;
		}
		x->pre->next=x->next->pre=x;
        push_fixup(x);
    }
}

template<class _TN>
const _TN& rbtree<_TN>::maximum()const
{
    return nil->next->data;
}

template<class _TN>
const _TN& rbtree<_TN>::minimum()const
{
    return nil->pre->data;
}

template<class _TN>
void rbtree<_TN>::right_rotate(rbnode *x)
{
	rbnode *y=x->left;
	y->p=x->p;
	if(x==root)root=y;
	else if(x==x->p->left)x->p->left=y;
	else x->p->right=y;
	x->left=y->right;
	if(y->right!=nil)y->right->p=x;
	y->right=x;
	x->p=y;
}

template<class _TN>
void rbtree<_TN>::left_rotate(rbnode *x)
{
	rbnode *y=x->right;
	y->p=x->p;
	if(x==root)root=y;
	else if(x==x->p->left)x->p->left=y;
	else x->p->right=y;
	x->right=y->left;
	if(y->left!=nil)y->left->p=x;
	y->left=x;
	x->p=y;
}

#undef BLACK
#undef RED
#endif //#ifndef RBTREE_H

