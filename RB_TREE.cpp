#include<iostream>
using namespace std;
typedef struct 
{
	Node* left;
	Node *right;
	Node *p;
	int key;
	char color;
} Node;
class RBTree
{
private:
	Node * T;
	Node *nil;
public:
	Node * Tree_MIN(Node *x)
	{
		while (x->left != NULL)
		{
			x = x->left;
		}
		return x;
	}
	Node * Tree_MAX(Node *x)
	{
		while (x->right != NULL)
		{
			x = x->right;
		}
		return x;
	}
	void INORED_TREE_WALK(Node* x)
	{
		INORED_TREE_WALK(x->left);
		cout << x->key << "  ";
		INORED_TREE_WALK(x->right);
	}
	Node* Tree_Search(Node* x, int k)
	{
		while (x != NULL && k != x->key)
		{
			if (k < x->key)
				x = x->left;
			else
				x = x->right;
		}
		return x;
	}
	Node * Tree_SUCCESSOR(Node *x)
	{
		if (x->right != NULL)
			return Tree_SUCCESSOR(x->right);
		Node *y = x->p;
		while (y != NULL && y->right == x)
		{
			x = x->p;
			y = x->p;
		}
		return y;
	}
	void Left_Rotate(Node *& T,Node *&x)
	{
		Node * y = x->right;
		x->right = y->left;
		if(y->left!=nil)//如果右孩子的左孩子不为哨兵节点的情况
			y->left->p = x;
		x->p = y;
		y->left = x;
		y->p = x->p;
		if (x->p == nil)//x为根节点的情况
			T = y;
		else if (x == x->p->left)
		{
			x->p->left = y;
		}
		else
		{
			x->p->left = y;
		}
	}
	void Right_Rotate(Node *& T, Node *&x)
	{
		Node *y = x->left;
		if(y->right!=nil)//如果左孩子的右孩子不为哨兵节点的情况
			y->right->p = x;
		x->left = y->right;
		x->p = y;
		y->right = x;
		if (x->p == nil)
			T = y;
		else if (x == x->p->left)
			y = x->p->left;
		else
			y = x->p->right;
	}
	void RE_Insert(Node * &T, Node *&x)
	{
		Node *m = T;
		Node *n = nil;
		while (m != nil)
		{
			n = m;
			if (x->key > n->key)
				m = n->right;
			else
				m = n->left;
		}
		x->p = n;
		if (n == nil)
			T = x;
		else if (x->key > n->key)
			x = n->right;
		else
			x = n->left;
		x->color = 'R';
		x->left = nil;
		x->right = nil;
		RB_Insert_Fixup(T, x);
	}
	void RB_Insert_Fixup(Node *&T, Node *&x)
	{
		while (x->p->color = 'R')
		{
			if (x->p == x->p->p->left)
			{
				Node *y = x->p->p->right;
				if (y->color == 'R')
				{
					x->p->color = 'B';
					x->p->p->left->color = 'B';
					x->p->p->color = 'R';
					x = x->p->p;
				}
				else if (x == x->p->right)
				{
					x = x->p;
					Left_Rotate(T, x);
				}
				x->p->color = 'B';
				x->p->p->color = 'R';
				Right_Rotate(T, x->p->p);
			}
			else
			{
				Node *y = x->p->p->left;
				if (y->color == 'R')
				{
					x->p->color = 'B';
					x->p->p->left->color = 'B';
					x->p->p->color = 'R';
					x = x->p->p;
				}
				else if (x == x->p->left)
				{
					x = x->p;
					Right_Rotate(T, x);
				}
				x->p->color = 'B';
				x->p->p->color = 'R';
				Left_Rotate(T, x->p->p);
			}
			T->color = 'B';
		}
	}
	void RB_Transplant(Node *&T, Node *&u, Node *&v)
	{
		if (u->p == nil)
			T = v;
		else if (u == u->p->left)
			u->p->left = v;
		else if (u == u->p->right)
			u->p->right = v;
		v->p = u->p;
	}
	void RB_Delete(Node *&T, Node *&x)
	{
		Node *y = x;
		y->color = x->color;
		if (x->left == nil)
		{
			Node *m = x->right;
			RB_Transplant(T, x, m);
		}
		else if (x->right == nil)
		{
			Node *m = x->left;
			RB_Transplant(T, x, m);
		}
		else
		{
			Node *n = Tree_MIN(x->right);
			y->color = n->color;
			Node *m = n->right;
			if (n->p == x)//出现这种情况的时候，是一个直线的情况，else中的句子会有问题，所以要改变一点
				m->p = n;
			else
			{
				RB_Transplant(T, n, n->right);
				n->right = x->right;
				x->right->p = n;
			}
			RB_Transplant(T, x, n);
			n->left = x->left;
			x->left->p = x;
			n->color = x->color;
			if (y->color = 'B')
				RB_Delete_Fixup(T, m);
		}
	}
	void RB_Delete_Fixup(Node*&T, Node*&x)
	{
		while (x!=T&&x->color=='B')
		{
			if (x == x->p->left)
			{
				Node* w = x->p->right;
				if (w->color == 'R')
				{
					w->color = 'B';
					x->p->color = 'R';
					Left_Rotate(T, x->p);
					w = x->p->right;
				}
				if (w->left->color == 'B'&&w->right->color == 'B')
				{
					w->color = 'R';
					x = x->p;
				}
				else if (w->right->color == 'B')
				{
					w->left->color = 'B';
					w->color = 'R';
					Right_Rotate(T, w);
					w = x->p->right;
				}
				w->color = 'R';
				x->p->color = 'B';
				Left_Rotate(T, x->p);
				w->right->color = 'B';
				x = T;
			}
			else
			{
				Node* w = x->p->left;
				if (w->color == 'R')
				{
					w->color = 'B';
					x->p->color = 'R';
					Right_Rotate(T, x->p);
					w = x->p->left;
				}
				if (w->right->color == 'B'&&w->left->color == 'B')
				{
					w->color = 'R';
					x = x->p;
				}
				else if (w->left->color == 'B')
				{
					w->right->color = 'B';
					w->color = 'R';
					Left_Rotate(T, w);
					w = x->p->left;
				}
				w->color = 'R';
				x->p->color = 'B';
				Right_Rotate(T, x->p);
				w->left->color = 'B';
				x = T;
			}
		}
		x->color = 'B';
	}
};
