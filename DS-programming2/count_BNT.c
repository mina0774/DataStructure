#include <stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include <math.h>
#define numRow 5 /* power of 2 (2, 4, 8, 16, ...)�� �ƴ� ������ ���� */ 
#define numCol 10 
int matrix[numRow][numCol] = { { 5,90,5,9,80,80,7,90,7,90 }, { 100,30,30,51,160,160,160,51,160,59 }, { 500,100,7000,100,900,600,100,100,650,100 } , { 1000,300,41,300,41,41,41,900,900,950 }, { 90,81,81,95,81,83,81,90,81,90 } };


typedef struct element* treePointer;

typedef struct element {
	int row; // ���� �ε���
	int num; // ����
	int numcount; // ���� count
	treePointer left;
	treePointer right;

	treePointer winner; //���� ����
}element;

//Binary Search tree
treePointer tree[numRow]; //�ึ�� BNT�� ������ ����

treePointer modifiedSearch(treePointer tree, int i, int j) { //i�� ���� ��ȣ, j�� ���� ��ȣ
	treePointer temp = NULL; //tree�� ��ġ �ӽ� ���

	while (tree) {
		if (matrix[i][j] == tree->num) //2���� �迭�� �ִ� ���� tree�� �ִ� ���ڿ� ��ġ�ϸ�
		{
			tree->numcount += 1; //tree�� ������ count ������ ����
			return NULL;
		}
		if (matrix[i][j] < tree->num) //2���� �迭�� �ִ� ������ tree�� �ִ� ���ڰ� �� ũ��
		{
			temp = tree;
			tree = tree->left; //�������� �̵�
		}
		else  //2���� �迭�� �ִ� ������ tree�� �ִ� ���ڰ� �� ������
		{
			temp = tree;
			tree = tree->right; //���������� �̵�
		}
	}
	return temp;
}


void insert(treePointer* node, int i, int j) { //i�� ���� ��ȣ, j�� ���� ��ȣ

	treePointer ptr, temp = modifiedSearch((*node), i, j);
	if (temp || !(*node)) {
		ptr = malloc(sizeof(*ptr));
		ptr->row = i;
		ptr->num = matrix[i][j];
		ptr->numcount = 1;
		ptr->left = ptr->right = NULL; //���ο� ����� ������ ���� �ʱ�ȭ
		if (*node)
			if (matrix[i][j] < temp->num) temp->left = ptr; //���ο� ��� ���� ��ġ�� ���ʿ� �־���
			else temp->right = ptr; //���ο� ��� ���� ��ġ�� �����ʿ� �־���
		else
			*node = ptr; //ù ���ο� ��� ����
	}
}

//�ִ� count = 0 ���� �ʱ�ȭ ��, �� ��� traverse�ϸ鼭 max�� ���� 
void select_node(treePointer tree, int* count, treePointer* temp) {
	if (tree) {
		//��͸� �̿��� inorder�� ��带 traverse

		select_node(tree->left, count, temp);

			if (tree->numcount > (*count)) {
				(*count) = tree->numcount; //count���� �� ū ������ �ʱ�ȭ
				(*temp) = tree; //BST���� �ִ� ��带 �̾���
			}
			else if (tree->numcount == (*count)) { //BST ������ count���� �ִ밪���� ��ġ�ϸ�
				if (tree->num > (*temp)->num) {  //���ڸ� ���Ͽ� �� ū ������
					(*temp) = tree; //BST���� �ִ� ��带 �̾���
				}
			}
	
		select_node(tree->right, count, temp);

	}
}
            
void delete_node(treePointer* root, treePointer delete){
	treePointer parent, child, suc, suc_p, t;

	parent = NULL; // ������ �Ǵ� root ����� �θ�� ����
	t = *root; //���� ��带 ����Ű�� ��

	//BST �ȿ��� ����ü �迭���� num�� ������ ������ key�� �Ǳ� ������ ��ġ�ϴ� num�� Ž��
	while (t != NULL && t->num != delete->num)
	{
		parent = t; //�θ� ��忡 t���� �� ����
		t = (delete->num < parent->num) ? parent->left : parent->right; //���� ���� t�� ��ġ�� �Ű���
	}

	// 1) �ڽ��� ���� ���
	if (t->left == NULL && t->right == NULL)
	{
		if (parent) // �θ� ��尡 ������ ��
		{
			if (parent->left == t)
				parent->left = NULL;
			else
				parent->right = NULL;
		}
		else // �θ� ��尡 �������� ���� ��
			*root = NULL;
	}
	// 2) �� ���� �ڽ��� ������ ���
	else if ((t->left == NULL) || (t->right == NULL))
	{
		child = (t->left != NULL) ? t->left : t->right;
		if (parent) //�θ��尡 ������ ��
		{
			if (parent->left == t)
				parent->left = child;
			else
				parent->right = child;
		}
		else //�θ��尡 �������� ���� ��
			*root = child;
	}
	// 3) 2���� �ڽ��� �ִ� ���
	else
	{
		// �������� subtree���� ���� ���� ���� Ž��
		suc_p = t;
		suc = t->right;
		while (suc->left != NULL)
		{
			suc_p = suc;
			suc = suc->left;
		}
		// ���ο� �θ�� �ڽ��� ����
		if (suc_p->left == suc)
			suc_p->left = suc->right; // ������ ���� ����������, �� ���� ������ �ڽ��� ������ ��� ����.
		else
			suc_p->right = suc->right;
		t->num = suc->num; 
		t->numcount = suc->numcount;
		t->row = suc->row;
		t = suc; //������ ����� �ڸ��� �Ʊ� Ž���ߴ� �ּҰ� ����
	}
	free(t);
}

treePointer lst_arr[numRow * 2];

void loser_tree(treePointer temp[numRow]) {

	int pre_num_first = pow(2, (int)(log2(numRow)) + 1); //b ������ �ε����� ���� �̸� ó�����־�� �Ѵ�. 

	//������� ����
	for (int i = 0; i < numRow; i++) {
		lst_arr[numRow + i] = temp[i];
		lst_arr[numRow + i]->winner = temp[i];
	}

	
	//�̸� ó��
	int pre_num = numRow * 2 - pre_num_first; //�̸� ó���� ����� ����
	int pre_compare_count = pre_num / 2; //�̸� ó���� �� ����� �� Ƚ��

	// 1) numcount �� 2) num�� 3) row �� 
	for (int i = 0; i < pre_compare_count; i++) {
		if (lst_arr[pre_num_first + 2 * i]->numcount > lst_arr[pre_num_first + 2 * i + 1]->numcount){
			lst_arr[(pre_num_first + 2 * i ) / 2] = lst_arr[pre_num_first + 2 * i + 1];
			lst_arr[(pre_num_first + 2 * i ) / 2]->winner = lst_arr[pre_num_first + 2 * i];
		}
		else if (lst_arr[pre_num_first + 2 * i]->numcount < lst_arr[pre_num_first + 2 * i + 1]->numcount) {
			lst_arr[(pre_num_first + 2 * i ) / 2] = lst_arr[pre_num_first + 2 * i ];  
			lst_arr[(pre_num_first + 2 * i ) / 2]->winner = lst_arr[pre_num_first + 2 * i+1];
		}
		else { //tie breaking
			if (lst_arr[pre_num_first + 2 * i]->num > lst_arr[pre_num_first + 2 * i+1]->num) {
				lst_arr[(pre_num_first + 2 * i ) / 2] = lst_arr[pre_num_first + 2 * i + 1];
				lst_arr[(pre_num_first + 2 * i ) / 2]->winner = lst_arr[pre_num_first + 2 * i];
			}
			else if (lst_arr[pre_num_first + 2 * i]->num < lst_arr[pre_num_first + 2 * i+1]->num) {
				lst_arr[(pre_num_first + 2 * i) / 2] = lst_arr[pre_num_first + 2 * i];
				lst_arr[(pre_num_first + 2 * i) / 2]->winner = lst_arr[pre_num_first + 2 * i + 1];
			}
			else {
				if (lst_arr[pre_num_first + 2 * i]->row > lst_arr[pre_num_first + 2 * i+1]->row) {
					lst_arr[(pre_num_first + 2 * i ) / 2] = lst_arr[pre_num_first + 2 * i + 1];
					lst_arr[(pre_num_first + 2 * i ) / 2]->winner = lst_arr[pre_num_first + 2 * i];
				}
				else {
					lst_arr[(pre_num_first + 2 * i) / 2] = lst_arr[pre_num_first + 2 * i];
					lst_arr[(pre_num_first + 2 * i) / 2]->winner = lst_arr[pre_num_first + 2 * i + 1];
				}
			}
		}
	}

	
	int full_bnt_h = (int)log2(numRow) + 1;

	int curr_hnode_num; //���� ������ ����� ����
	int curr_compare_count; //���� ����� �� Ƚ��

	//�̸� ó������ ���� ���� ������� full binary tree�� ���� ���� ���Ͽ� ����Ʈ�� ����
	// 1) numcount �� 2) num�� 3) row �� 
	for (int i = 0; i < full_bnt_h; i++) {
		curr_hnode_num = (int)pow(2,  (int)full_bnt_h-i-1); 
		curr_compare_count = curr_hnode_num / 2;
		int a = pre_num_first / (int)(pow(2, i + 1));
		for (int j = 0; j < curr_compare_count ; j++) {
			if (lst_arr[a + 2 * j]->winner->numcount > lst_arr[a + 2 * j + 1]->winner->numcount) {
				lst_arr[(a + 2 * j ) / 2] = lst_arr[a + 2 * j + 1]->winner;
				lst_arr[(a + 2 * j ) / 2]->winner = lst_arr[ a + 2 * j]->winner;
			}
			else if (lst_arr[a + 2 * j]->winner->numcount < lst_arr[a + 2 * j + 1]->winner->numcount)
			{
				lst_arr[(a + 2 * j ) / 2] = lst_arr[a + 2 * j ]->winner;
				lst_arr[(a + 2 * j ) / 2]->winner = lst_arr[a + 2 * j+1]->winner;
			}
			else { //tie breaking
				if (lst_arr[a + 2 * j]->winner->num > lst_arr[a + 2 * j + 1]->winner->num) {
					lst_arr[(a + 2 * j ) / 2] = lst_arr[a + 2 * j + 1]->winner;
					lst_arr[(a+ 2 * j ) / 2]->winner = lst_arr[a + 2 * j]->winner;
				}
				else if (lst_arr[a + 2 * j]->winner->num < lst_arr[a + 2 * j + 1]->winner->num)
				{
					lst_arr[(a + 2 * j) / 2] = lst_arr[a + 2 * j]->winner;
					lst_arr[(a + 2 * j) / 2]->winner = lst_arr[a + 2 * j + 1]->winner;
				}
				else {
					if (lst_arr[a + 2 * j]->winner->row > lst_arr[a + 2 * j + 1]->winner->row) {
						lst_arr[(a + 2 * j ) / 2] = lst_arr[a + 2 * j + 1]->winner;
						lst_arr[(a + 2 * j ) / 2]->winner = lst_arr[a + 2 * j]->winner;
					}
					else if (lst_arr[a + 2 * j]->winner->row < lst_arr[a + 2 * j + 1]->winner->row)
					{
						lst_arr[(a + 2 * j) / 2] = lst_arr[a + 2 * j]->winner;
						lst_arr[(a + 2 * j) / 2]->winner = lst_arr[a + 2 * j + 1]->winner;
					}
				}
			}
		}
	}
	lst_arr[0] = lst_arr[1]->winner;
	//����Ʈ�� ���� �Ϸ�

	//����Ʈ���� ����� ���ڰ� ��
	if (lst_arr[0]->numcount >= 0) {
		printf("( %d, %d, %d )\n", lst_arr[0]->row, lst_arr[0]->num, lst_arr[0]->numcount);
	}
	//���� ���


	int k = lst_arr[0]->row;
	delete_node(&tree[k], temp[k]); //����Ʈ���� ����� ���� ��ġ�ϴ� bst�� ��� ����
	int count = 0;
	select_node(tree[k], &count, &temp[k]); //��带 ������ bst���� �ٽ� �ִ� �̱�
	
}


treePointer temp[numRow];

int main() {
	//treePointer temp[numRow];
	int count;
	for (int i = 0; i < numRow; i++) {
		for (int j = 0; j < numCol; j++) {
			insert(&tree[i], i, j);
		}
	}

	//bst���� ������ ���� �ִ� �̱� ù��° �õ�
	for (int i = 0; i < numRow; i++) {
		temp[i] = NULL;
		count = 0;
		select_node(tree[i], &count, &temp[i]);
		}
	
	//���� ���� �� ���� loser tree �ݺ�
	do{
		loser_tree(temp);
	} while (temp);

}

