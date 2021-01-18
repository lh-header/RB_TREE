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
	// ��ʼ�������
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
���������Ϊ3����
1�����������������������ҵ������λ�ã�
2����ʼ�������㣬���½�����ɫ����Ϊ��ɫ��
3��ͨ�����õ�����������������������µ���Ϊ�����
*********************************************/
void rb_Tree_insert(RBT_Root* *T, int k)
{
	//1���ҵ���Ҫ�����λ�á�
	//�Ӹ���㿪ʼ��ͨ�����ϵ�ͬ�½���ֵ���бȽϣ������ҵ�Ҫ�����λ��
	RB_TREE *x, *p; // xΪ��ǰ��㣬pΪ��ǰ��㸸���
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
			cout << "����Ѵ���" << endl;
			return;
		}
	}

	// ��ʼ����㣬���½�����ɫ����Ϊ��ɫ
	x = (RB_TREE*)malloc(sizeof(RB_TREE));
	x->key = k;
	x->color = RED;
	x->p = p;
	x->left = (*T)->nil;
	x->right = (*T)->nil;

	// ���²���Ľ�㣬�������丸���֮�����ϵ
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

	// �Զ��������е���
	RB_Insert_Fixup((*T), x);

}



void rb_Tree_delete(RBT_Root* *T, int k)
{
	// ɾ��һ��Ԫ��
	if ((*T)->root == NULL)
	{
		return;
	}
	// �ҵ�Ҫ��ɾ���Ľ��
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
		cout << k << "������" << endl;
	}

	// ���ڣ�������������ӣ�������������С�����棬ɾ����С���
	if (toDel->left != (*T)->nil && toDel->right != (*T)->nil)
	{
		RB_TREE* alt = rbt_findMin((*T), toDel->right);
		toDel->key = alt->key;
		k = alt->key;
		toDel = alt;
	}

	// ���ֻ��һ�����ӣ�ֱ���ú��ӽ���滻�ý��
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

	//��ɾ���ý��֮ǰ�����жϴ˽�����ɫ������Ǻ�ɫ��ֱ��ɾ���������ƻ��������
	//���Ǻ�ɫ��ɾ������ƻ�������ĵ� 5 �����ʣ���Ҫ������������
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
			// ��һ��������ֵܽ���Ǻ�ɫ
			if (w->color == RED)
			{
				w->color = BLACK;
				w->p->color = RED;
				rbTree_left_rotate((*T), x->p);
				w = x->p->right;
			}
			//��2��������ֵ��Ǻ�ɫ�ģ������ֵܵ��������Ӷ��Ǻ�ɫ�ġ�
			if (w->left->color == BLACK && w->right->color == BLACK)
			{
				w->color = RED;
				x = x->p;
			}
			//��3�����
			if (w->left->color == RED && w->right->color == BLACK)
			{
				w->left->color = BLACK;
				w->color = RED;
				rbTree_right_rotate((*T), w);
				w = x->p->right;
			}
			//��4�����
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
			//�� 1 �����
			if (w->color == RED)
			{
				w->color = BLACK;
				x->p->color = RED;
				rbTree_right_rotate((*T), x->p);
				w = x->p->left;
			}
			//�� 2 �����
			if (w->left->color == BLACK && w->right->color == BLACK)
			{
				w->color = RED;
				x = x->p;
			}
			//�� 3 �����
			if (w->left->color == BLACK && w->right->color == RED)
			{
				w->color = RED;
				w->right->color = BLACK;
				w = x->p->left;
			}
			//�� 4 �����
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
	x->color = BLACK;//���ս���������ɫ��Ϊ��ɫ
}

// �ҵ���СԪ��
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
	// ����u vԪ��
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

// ���µ��������
void RB_Insert_Fixup(RBT_Root* T, RB_TREE* x)
{
	// �����ж��丸�����ɫΪ��ɫʱ����Ҫ������Ϊ��ɫʱֱ�Ӳ��뼴�ɣ�����Ҫ����
	while (x->p->color == RED)
	{
		// �漰�����㣬��ֿ����ۣ�ȷ����������游�������ӻ����Һ���
		if (x->p == x->p->p->left)
		{
			RB_TREE* y = x->p->p->right; // �ҵ���������
			/************************
			���������Ϊ��ɫ����Ϊ��һ�������
			����취���������ɫ��Ϊ��ɫ����������ɪ��Ϊ��ɫ��
			�游����Ϊ��ɫ�����游��㸳ֵΪ��ǰ��㣬�����ж�
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
				���������Ϊ��ɫ����������������
				1����ǰ����Ǹ������Һ��ӣ�
				2����ǰ���Ϊ���������ӡ�
				*************************************/
				if (x == x->p->right)
				{
					// ���������Ϊ��ǰ��������������
					x = x->p;
					rbTree_left_rotate(T, x);
				}
				else
				{
					// ��ǰ���Ϊ��������ӣ���������Ϊ��ɫ��
					// �游����Ϊ��ɫ�����游��������������
					x->p->color = BLACK;
					x->p->p->color = BLACK;
					rbTree_right_rotate(T, x->p->p);
				}
			}
		}
		else
		{
			// �����Ϊ�游�����Һ���
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


//T��ʾΪ������x ��ʾ��Ҫ���������������ĸ����
void rbTree_left_rotate(RBT_Root* T, RB_TREE* x)
{
	RB_TREE* y = x->right;
	x->right = y->left;
	if (x->right != T->nil)
	{
		// �������ø����
		x->right->p = x;
	}
	y->p = x->p;
	/***************************************
	�������� y ��˫�׽��ͬ y �����ӣ���Ϊ 2 �������
	1��ԭ x ��㱾������������ĸ���㣬��ʱֻ��Ҫ�� T ָ��ָ�� y��
	2������ y �йؼ���ͬ�丸���ؼ��ֵ�ֵ�Ĵ�С���ж� y �Ǹ��������ӻ����Һ���
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

// ����
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

//����������ǰ���������������Ľ��
void rbTree_print(RBT_Root* T) 
{
	cout << "ǰ����� ��";
	rbTree_prePrint(T, T->root);
	cout << endl;
	cout << "������� ��";
	rbTree_inPrint(T, T->root);
	cout << endl;
}
#endif // !_RB_TREE_H_

