#ifndef _RB_TREE_H_
#define _RB_TREE_H_
#include<stdlib.h>
#include<iostream>

using namespace std;

typedef enum
{
	RED,
	BLACK
}ColorType;

typedef struct RB_TREE
{
	int key;
	struct RB_TREE *left;
	struct RB_TREE *right;
	struct RB_TREE *p;
	ColorType color;
}RB_TREE;

typedef struct RBT_Root
{
	RB_TREE* root;
	RB_TREE* nil;
}RBT_Root;

void rbTree_left_rotate(RBT_Root* T, RB_TREE* x);
void rbTree_right_rotate(RBT_Root* T, RB_TREE* x);
void rb_Tree_insert(RBT_Root* *T, int k);
void rb_Tree_delete(RBT_Root* *T, int k);
void RB_Delete_Fixup(RBT_Root* *T, RB_TREE* x);
RB_TREE* rbt_findMin(RBT_Root* T, RB_TREE* t);
void RB_Insert_Fixup(RBT_Root* T, RB_TREE* x);
void rb_Tree_transplant(RBT_Root* T, RB_TREE* u, RB_TREE* v);

RBT_Root* rbTree_init()
{
	// 初始化红黑树
	RBT_Root* T;
	T = (RBT_Root*)malloc(sizeof(RBT_Root));
	T->nil = (RB_TREE*)malloc(sizeof(sizeof(RB_TREE)));
	T->nil->color = BLACK;
	T->nil->p = NULL;
	T->nil->left = NULL;
	T->nil->right = NULL;
	T->root = T->nil;
	return T;
}

/*******************************************
插入操作分为3步：
1、将红黑树当二叉查找树，找到其插入位置；
2、初始化插入结点，将新结点的颜色设置为红色；
3、通过调用调整函数，将二叉查找树重新调整为红黑树
*********************************************/
void rb_Tree_insert(RBT_Root* *T, int k)
{
	//1、找到其要插入的位置。
	//从根结点开始，通过不断的同新结点的值进行比较，最终找到要插入的位置
	RB_TREE *x, *p; // x为当前结点，p为当前结点父结点
	x = (*T)->root;
	p = x;
	while (x != (*T)->nil)
	{
		p = x;
		if (k < x->key)
		{
			x = x->left;
		}
		else if (k > x->key)
		{
			x = x->right;
		}
		else
		{
			cout << "结点已存在" << endl;
			return;
		}
	}

	// 初始化结点，将新结点的颜色设置为红色
	x = (RB_TREE*)malloc(sizeof(RB_TREE));
	x->key = k;
	x->color = RED;
	x->p = p;
	x->left = (*T)->nil;
	x->right = (*T)->nil;

	// 对新插入的结点，建立与其父结点之间的联系
	if ((*T)->root == (*T)->nil)
	{
		(*T)->root = x;
	}
	else if (k < p->key)
	{
		p->left = x;
	}
	else
	{
		p->right = x;
	}

	// 对二叉树进行调整
	RB_Insert_Fixup((*T), x);

}



void rb_Tree_delete(RBT_Root* *T, int k)
{
	// 删除一个元素
	if ((*T)->root == NULL)
	{
		return;
	}
	// 找到要被删除的结点
	RB_TREE* toDel = (*T)->root;
	RB_TREE* x = NULL;
	while (toDel != (*T)->nil && toDel->key != k)
	{
		if (k < toDel->key)
		{
			toDel = toDel->left;
		}
		else if (k > toDel->key)
		{
			toDel = toDel->right;
		}
	}

	if (toDel == (*T)->nil)
	{
		cout << k << "不存在" << endl;
	}

	// 存在，如果有两个孩子，找右子树中最小结点代替，删除最小结点
	if (toDel->left != (*T)->nil && toDel->right != (*T)->nil)
	{
		RB_TREE* alt = rbt_findMin((*T), toDel->right);
		toDel->key = alt->key;
		k = alt->key;
		toDel = alt;
	}

	// 如果只有一个孩子，直接用孩子结点替换该结点
	if (toDel->left == (*T)->nil)
	{
		x = toDel->right;
		rb_Tree_transplant((*T), toDel, toDel->right);
	}
	else if (toDel->right == (*T)->nil)
	{
		x = toDel->left;
		rb_Tree_transplant((*T), toDel, toDel->left);
	}

	//在删除该结点之前，需判断此结点的颜色：如果是红色，直接删除，不会破坏红黑树；
	//若是黑色，删除后会破坏红黑树的第 5 条性质，需要对树做调整。
	if (toDel->color == BLACK)
	{
		RB_Delete_Fixup(T, x);
	}

	free(toDel);
}


void RB_Delete_Fixup(RBT_Root* *T, RB_TREE* x)
{
	while (x != (*T)->root && x->color == BLACK)
	{
		if (x == x->p->left)
		{
			RB_TREE* w = x->p->right;
			// 第一种情况，兄弟结点是红色
			if (w->color == RED)
			{
				w->color = BLACK;
				w->p->color = RED;
				rbTree_left_rotate((*T), x->p);
				w = x->p->right;
			}
			//第2种情况：兄弟是黑色的，并且兄弟的两个儿子都是黑色的。
			if (w->left->color == BLACK && w->right->color == BLACK)
			{
				w->color = RED;
				x = x->p;
			}
			//第3种情况
			if (w->left->color == RED && w->right->color == BLACK)
			{
				w->left->color = BLACK;
				w->color = RED;
				rbTree_right_rotate((*T), w);
				w = x->p->right;
			}
			//第4种情况
			if (w->right->color == RED)
			{
				w->color = x->p->color;
				x->p->color = BLACK;
				w->right->color = BLACK;
				rbTree_left_rotate((*T), x->p);
				x = (*T)->root;
			}
		}
		else
		{
			RB_TREE* w = x->p->left;
			//第 1 种情况
			if (w->color == RED)
			{
				w->color = BLACK;
				x->p->color = RED;
				rbTree_right_rotate((*T), x->p);
				w = x->p->left;
			}
			//第 2 种情况
			if (w->left->color == BLACK && w->right->color == BLACK)
			{
				w->color = RED;
				x = x->p;
			}
			//第 3 种情况
			if (w->left->color == BLACK && w->right->color == RED)
			{
				w->color = RED;
				w->right->color = BLACK;
				w = x->p->left;
			}
			//第 4 种情况
			if (w->right->color == BLACK)
			{
				w->color = w->p->color;
				x->p->color = BLACK;
				w->left->color = BLACK;
				rbTree_right_rotate((*T), x->p);
				x = (*T)->root;
			}
		}
	}
	x->color = BLACK;//最终将根结点的颜色设为黑色
}

// 找到最小元素
RB_TREE* rbt_findMin(RBT_Root* T, RB_TREE* t)
{
	if (t == T->nil)
	{
		return T->nil;
	}
	while (t->left != T->nil)
	{
		t = t->left;
	}
	return t;
}

void rb_Tree_transplant(RBT_Root* T, RB_TREE* u, RB_TREE* v)
{
	// 交换u v元素
	if (u->p == T->nil)
	{
		T->root = v;
	}
	else if (u == u->p->left)
	{
		u->p->left = v;
	}
	else
	{
		u->p->right = v;
	}
	v->p = u->p;
}

// 重新调整红黑树
void RB_Insert_Fixup(RBT_Root* T, RB_TREE* x)
{
	// 首先判断其父结点颜色为红色时才需要调整；为黑色时直接插入即可，不需要调整
	while (x->p->color == RED)
	{
		// 涉及叔叔结点，需分开讨论，确定父结点是祖父结点的左孩子还是右孩子
		if (x->p == x->p->p->left)
		{
			RB_TREE* y = x->p->p->right; // 找到其叔叔结点
			/************************
			如果叔叔结点为红色，此为第一种情况：
			处理办法：父结点颜色改为黑色，叔叔结点亚瑟改为黑色，
			祖父结点改为红色，将祖父结点赋值为当前结点，继续判断
			*************************/
			if (y->color == RED)
			{
				x->p->color = BLACK;
				y->color = BLACK;
				x->p->p->color = RED;
				x = x->p->p;
			}
			else
			{
				/************************************
				如果叔叔结点为黑色，徐芬两种情况处理：
				1、当前结点是父结点的右孩子，
				2、当前结点为父结点的左孩子。
				*************************************/
				if (x == x->p->right)
				{
					// 将父结点作为当前结点进行左旋操作
					x = x->p;
					rbTree_left_rotate(T, x);
				}
				else
				{
					// 当前结点为父结点左孩子，将父结点改为黑色，
					// 祖父结点改为红色，从祖父结点进行右旋处理
					x->p->color = BLACK;
					x->p->p->color = BLACK;
					rbTree_right_rotate(T, x->p->p);
				}
			}
		}
		else
		{
			// 父结点为祖父结点的右孩子
			RB_TREE* y = x->p->p->left;
			if (y->color == RED)
			{
				x->p->color = BLACK;
				y->color = BLACK;
				x->p->p->color = RED;
				x = x->p->p;
			}
			else
			{
				if (x == x->p->left)
				{
					x = x->p;
					rbTree_right_rotate(T, x);
				}
				else
				{
					x->p->color = BLACK;
					x->p->p->color = RED;
					rbTree_left_rotate(T, x->p->p);
				}
			}
		}
	}
	T->root->color = BLACK;
}


//T表示为树根，x 表示需要进行左旋的子树的根结点
void rbTree_left_rotate(RBT_Root* T, RB_TREE* x)
{
	RB_TREE* y = x->right;
	x->right = y->left;
	if (x->right != T->nil)
	{
		// 重新设置父结点
		x->right->p = x;
	}
	y->p = x->p;
	/***************************************
	重新设置 y 的双亲结点同 y 的连接，分为 2 种情况：
	1、原 x 结点本身就是整棵树的根结点，此时只需要将 T 指针指向 y；
	2、根据 y 中关键字同其父结点关键字的值的大小，判断 y 是父结点的左孩子还是右孩子
	****************************************/
	if (y->p == T->nil)
	{
		T->root = y;
	}
	else if (y->key < y->p->key)
	{
		y->p->left = y;
	}
	else
	{
		y->p->right = y;
	}
	y->left = x;
	x->p = y;
}

// 右旋
void rbTree_right_rotate(RBT_Root* T, RB_TREE* x)
{
	RB_TREE* y = x->left;
	x->left = y->right;
	if (x->left != T->nil)
	{
		x->left->p = x;
	}
	y->p = x->p;
	if (y->p == T->nil)
	{
		T->root = y;
	}
	else if (y->key < y->p->key)
	{
		y->p->left = y;
	}
	else
	{
		y->p->right = y;
	}
	y->right = x;
	x->p = y;
}

void rbTree_prePrint(RBT_Root* T, RB_TREE* t) 
{
	if (T->nil == t) 
	{
		return;
	}
	if (t->color == RED) 
	{
		cout << t->key << "R" << " ";
	}
	else 
	{
		cout << t->key << "B" << " ";
	}
	rbTree_prePrint(T, t->left);
	rbTree_prePrint(T, t->right);
}
void rbTree_inPrint(RBT_Root* T, RB_TREE* t)
{
	if (T->nil == t)
	{
		return;
	}
	rbTree_inPrint(T, t->left);
	if (t->color == RED) 
	{
		cout << t->key << "R" << " ";
	}
	else
	{
		cout << t->key << "B" << " ";
	}
	rbTree_inPrint(T, t->right);
}

//输出红黑树的前序遍历和中序遍历的结果
void rbTree_print(RBT_Root* T) 
{
	cout << "前序遍历 ：";
	rbTree_prePrint(T, T->root);
	cout << endl;
	cout << "中序遍历 ：";
	rbTree_inPrint(T, T->root);
	cout << endl;
}
#endif // !_RB_TREE_H_

