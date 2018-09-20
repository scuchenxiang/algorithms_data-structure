#include<iostream>
using namespace std;
typedef struct 
{
	Node* left;
	Node *right;
	Node *p;
	int key;
} Node;
class BStree
{
private:
    Node*T;
public:
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
	void Tree_Insert(Node *&T, Node *x)//T为树的根结点
	{
		Node *m = T;
		Node *n = NULL;
		while (m != NULL)
		{
			n = m;
			if (x->key < m->key)
				m = m->left;
			else
				m = m->right;
		}
		x->p = n;
		if (m == NULL)
			T = x;
		else if (x->key < n->key)
			n->left = x;
		else
			n->right = x;
	}
	void TRANSPLANT(Node *&T, Node *&v, Node *&u)
	{
		if (u->p == NULL)//防止出现u是根结点的情况
			T = v;
		else if (u = (u->p)->left)
			v = (u->p)->left;
		else if (u = (u->p)->left)
			v = (u->p)->left;
		if (v != NULL)//防止出现v是NULL的情况
			v->p = u->p;
	}
	void Tree_Delete(Node *&T,Node *&x)
	{
		if (x->left == NULL)//第一种没有左子树
			TRANSPLANT(T, x->right, x);
		else if (x->right == NULL && x->left != NULL)//只有左子树
			TRANSPLANT(T, x->left, x);
		else//左右子树都有
		{
			Node *y = Tree_MIN(x->right);
			if (y != x->right)
			{
				TRANSPLANT(T, y->right, y);
				y->right = x->right;
				x->right->p = y;
			}
			TRANSPLANT(T, y, x);
			x->left->p = y;
			y->left = x->left;
		}
	}
	
};
 
