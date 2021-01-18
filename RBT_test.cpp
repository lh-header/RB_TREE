#include"rb_tree.h"

int main()
{
	RBT_Root* T = rbTree_init();
	for (int i = 1; i < 10; i++)
	{
		rb_Tree_insert(&T, i);
	}
	rbTree_print(T);
	cout << endl;
	rb_Tree_delete(&T, 5);
	rbTree_print(T);
	system("pause");
	return 0;
}