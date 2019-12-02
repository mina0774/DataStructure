#include <stdio.h>
#include <stdlib.h>

typedef struct listNode* listPointer;
typedef struct listNode {
	int data;
	listPointer link;
}listNode;

void createNode(listPointer* r);
void Rmove(listPointer *l, listPointer *r,int m);
void Lmove(listPointer *l, listPointer *r,int m);
void printList(listPointer r,listPointer l);


int main() {
	int num=0; //�̵��� �ʿ��� ��� ��
	listPointer left = NULL, right = NULL;
	int select_num=0;
	while (1) {
		printf("**********�޴�**********\n");
		printf("1) ����Ʈ �߰�\n");
		printf("2) ���������� �̵�\n");
		printf("3) �������� �̵�\n");
		printf("4) ����Ʈ ���\n");
		printf("5) ���α׷� ����\n");
		printf("�޴��� �����ϼ���: \n");
		scanf_s("%d", &select_num,sizeof(select_num));

		switch (select_num)
		{
		case 1:
			createNode(&right);
			break;
		case 2:
			printf("���������� �� ĭ �̵��ϰڽ��ϱ�? :");
			scanf_s("%d", &num,sizeof(num));
			Rmove(&left, &right, num);
			break;
		case 3:
			printf("�������� �� ĭ �̵��ϰڽ��ϱ�? :");
			scanf_s("%d", &num,sizeof(num));
			Lmove(&left, &right, num);
			break;
		case 4:
			printList(right,left);
			break;
		case 5:
			exit(0);
		}
	}
}


void createNode(listPointer* r) {
	int num; //��� ������ �� �Է¹޴� ���� 
	int listnum; //�߰��� ����Ʈ�� ����
	printf("�� ���� ����Ʈ�� �߰��ϰڽ��ϱ�?: ");
	scanf_s("%d", &listnum,sizeof(listnum));
	for (int i = 0; i < listnum; i++) {
		listPointer node = (listPointer)malloc(sizeof(listNode));
		printf("����Ʈ�� �����͸� �Է��ϼ���: ");
		scanf_s("%d", &num,sizeof(num));

		node->data = num;
		node->link = *r; //��带 ����Ʈ�� �����ʿ� �־��� ��,
		*r = node; //����Ʈ�� ����Ű�� *r �����͸� ����Ʈ�� �� ������ �Ű��ش�
	}
} 

void Rmove(listPointer* l, listPointer* r, int n) {
	listPointer temp = NULL; //������ �̵� �� ������ ��ġ�� ��Ÿ���� ������
	for (; n > 0 && *r != NULL; n--) {
		temp = (*r)->link; //temp�� ���� ��ġ�� ������ ��带 ����
		(*r)->link = *l; //*r�� link�� *l�� ��ġ�� �־��� 
		*l = *r; //*l�� ��ġ�� *r�� �־���
		*r = temp; //*r�� ��ġ�� *r ������ ����Ʈ�� �־���
	}
}

void Lmove(listPointer * l, listPointer * r, int n) {
		listPointer temp = NULL;
		for (; n > 0 && *l != NULL; n--) {
			temp = (*l)->link; //temp�� ���� ��ġ�� ���� ��带 ����
			(*l)->link = *r; //*l�� link�� *r�� ��ġ�� �־���
			*r = *l; //*r�� ��ġ�� *l�� �־���
			*l = temp;//*l�� ��ġ�� *l ������ ����Ʈ�� �־���
		}
}

void printList(listPointer r,listPointer l) {
	printf("������ ����Ʈ: ");
	for (; r; r = r->link) {
		printf("%4d", r->data);
	}
	printf("\n");
	printf("���� ����Ʈ: ");
	for (;l; l = l->link) {
		printf("%4d", l->data);
	}
	printf("\n");
}