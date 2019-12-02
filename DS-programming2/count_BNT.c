#include <stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include <math.h>
#define numRow 5 /* power of 2 (2, 4, 8, 16, ...)가 아닌 값으로 설정 */ 
#define numCol 10 
int matrix[numRow][numCol] = { { 5,90,5,9,80,80,7,90,7,90 }, { 100,30,30,51,160,160,160,51,160,59 }, { 500,100,7000,100,900,600,100,100,650,100 } , { 1000,300,41,300,41,41,41,900,900,950 }, { 90,81,81,95,81,83,81,90,81,90 } };


typedef struct element* treePointer;

typedef struct element {
	int row; // 행의 인덱스
	int num; // 숫자
	int numcount; // 숫자 count
	treePointer left;
	treePointer right;

	treePointer winner; //승자 저장
}element;

//Binary Search tree
treePointer tree[numRow]; //행마다 BNT를 저장할 공간

treePointer modifiedSearch(treePointer tree, int i, int j) { //i는 행의 번호, j는 열의 번호
	treePointer temp = NULL; //tree의 위치 임시 기억

	while (tree) {
		if (matrix[i][j] == tree->num) //2차원 배열에 있는 값과 tree에 있는 숫자와 일치하면
		{
			tree->numcount += 1; //tree의 숫자의 count 변수를 증가
			return NULL;
		}
		if (matrix[i][j] < tree->num) //2차원 배열에 있는 값보다 tree에 있는 숫자가 더 크면
		{
			temp = tree;
			tree = tree->left; //왼쪽으로 이동
		}
		else  //2차원 배열에 있는 값보다 tree에 있는 숫자가 더 작으면
		{
			temp = tree;
			tree = tree->right; //오른쪽으로 이동
		}
	}
	return temp;
}


void insert(treePointer* node, int i, int j) { //i는 행의 번호, j는 열의 번호

	treePointer ptr, temp = modifiedSearch((*node), i, j);
	if (temp || !(*node)) {
		ptr = malloc(sizeof(*ptr));
		ptr->row = i;
		ptr->num = matrix[i][j];
		ptr->numcount = 1;
		ptr->left = ptr->right = NULL; //새로운 노드의 생성의 값을 초기화
		if (*node)
			if (matrix[i][j] < temp->num) temp->left = ptr; //새로운 노드 현재 위치의 왼쪽에 넣어줌
			else temp->right = ptr; //새로운 노드 현재 위치의 오른쪽에 넣어줌
		else
			*node = ptr; //첫 새로운 노드 생성
	}
}

//최대 count = 0 으로 초기화 후, 각 노드 traverse하면서 max값 변경 
void select_node(treePointer tree, int* count, treePointer* temp) {
	if (tree) {
		//재귀를 이용한 inorder로 노드를 traverse

		select_node(tree->left, count, temp);

			if (tree->numcount > (*count)) {
				(*count) = tree->numcount; //count값을 더 큰 값으로 초기화
				(*temp) = tree; //BST에서 최대 노드를 뽑아줌
			}
			else if (tree->numcount == (*count)) { //BST 내에서 count값이 최대값으로 일치하면
				if (tree->num > (*temp)->num) {  //숫자를 비교하여 더 큰 값으로
					(*temp) = tree; //BST에서 최대 노드를 뽑아줌
				}
			}
	
		select_node(tree->right, count, temp);

	}
}
            
void delete_node(treePointer* root, treePointer delete){
	treePointer parent, child, suc, suc_p, t;

	parent = NULL; // 시작이 되는 root 노드의 부모는 없다
	t = *root; //현재 노드를 가리키는 값

	//BST 안에서 구조체 배열에서 num은 유일한 값으로 key가 되기 때문에 일치하는 num을 탐색
	while (t != NULL && t->num != delete->num)
	{
		parent = t; //부모 노드에 t값을 준 다음
		t = (delete->num < parent->num) ? parent->left : parent->right; //값에 따라 t의 위치를 옮겨줌
	}

	// 1) 자식이 없을 경우
	if (t->left == NULL && t->right == NULL)
	{
		if (parent) // 부모 노드가 존재할 때
		{
			if (parent->left == t)
				parent->left = NULL;
			else
				parent->right = NULL;
		}
		else // 부모 노드가 존재하지 않을 때
			*root = NULL;
	}
	// 2) 한 개의 자식을 가지는 경우
	else if ((t->left == NULL) || (t->right == NULL))
	{
		child = (t->left != NULL) ? t->left : t->right;
		if (parent) //부모노드가 존재할 때
		{
			if (parent->left == t)
				parent->left = child;
			else
				parent->right = child;
		}
		else //부모노드가 존재하지 않을 때
			*root = child;
	}
	// 3) 2개의 자식이 있는 경우
	else
	{
		// 오른쪽의 subtree에서 가장 작은 값을 탐색
		suc_p = t;
		suc = t->right;
		while (suc->left != NULL)
		{
			suc_p = suc;
			suc = suc->left;
		}
		// 새로운 부모와 자식을 연결
		if (suc_p->left == suc)
			suc_p->left = suc->right; // 왼쪽의 끝에 도착했지만, 그 끝에 오른쪽 자식이 존재할 경우 대입.
		else
			suc_p->right = suc->right;
		t->num = suc->num; 
		t->numcount = suc->numcount;
		t->row = suc->row;
		t = suc; //삭제될 노드의 자리에 아까 탐색했던 최소값 대입
	}
	free(t);
}

treePointer lst_arr[numRow * 2];

void loser_tree(treePointer temp[numRow]) {

	int pre_num_first = pow(2, (int)(log2(numRow)) + 1); //b 이후의 인덱스인 노드는 미리 처리해주어야 한다. 

	//리프노드 삽입
	for (int i = 0; i < numRow; i++) {
		lst_arr[numRow + i] = temp[i];
		lst_arr[numRow + i]->winner = temp[i];
	}

	
	//미리 처리
	int pre_num = numRow * 2 - pre_num_first; //미리 처리할 노드의 개수
	int pre_compare_count = pre_num / 2; //미리 처리할 때 노드의 비교 횟수

	// 1) numcount 비교 2) num비교 3) row 비교 
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

	int curr_hnode_num; //현재 레벨의 노드의 개수
	int curr_compare_count; //현재 노드의 비교 횟수

	//미리 처리해준 값을 토대로 만들어진 full binary tree에 대한 값을 비교하여 루저트리 생성
	// 1) numcount 비교 2) num비교 3) row 비교 
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
	//루저트리 생성 완료

	//루저트리의 헤더는 승자가 됨
	if (lst_arr[0]->numcount >= 0) {
		printf("( %d, %d, %d )\n", lst_arr[0]->row, lst_arr[0]->num, lst_arr[0]->numcount);
	}
	//승자 출력


	int k = lst_arr[0]->row;
	delete_node(&tree[k], temp[k]); //루저트리의 헤더의 값과 일치하는 bst의 노드 삭제
	int count = 0;
	select_node(tree[k], &count, &temp[k]); //노드를 삭제한 bst에서 다시 최댓값 뽑기
	
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

	//bst에서 각각의 런에 최댓값 뽑기 첫번째 시도
	for (int i = 0; i < numRow; i++) {
		temp[i] = NULL;
		count = 0;
		select_node(tree[i], &count, &temp[i]);
		}
	
	//런이 끝날 때 까지 loser tree 반복
	do{
		loser_tree(temp);
	} while (temp);

}

