#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define TRUE 1
#define FALSE 0
#define numRow 10
#define numCol 10
#define DST_ROW 5 //목적지 좌표 row 마음대로 바꿀 수 있음
#define DST_COL 6 //목적지 좌표 col
#define stack_size 100

typedef struct {
	int row;  int col; int dir_vector; //direction vector에 대한 설명은 아래 참조
} element;

typedef struct {
	short int vert; short int horiz;
} offsets;

void setup_maze();
void set_move();
void path();
int init_dir_vector(int row, int col); //함수 정의에서 설명, 8 방향을 정찰
int get_next_dir(int dir_vector); //함수 정의에서 설명, 다음 방향 정해줌
void push(element position);
element pop();

int top = -1;
offsets move[8];
short int maze[numRow + 2][numCol + 2];
element stack[stack_size];


void main() {
	setup_maze();
	set_move();
	path();
}

void setup_maze() {
	int i, j;
	short int maze0[numRow][numCol] = {
	{ 0,0,1,0,1,1,1,0,1,0 },
	{ 1,0,0,1,1,1,0,1,0,1 },
	{ 1,1,0,1,1,0,1,0,1,1 },
	{ 0,0,1,0,1,1,1,0,0,0 },
	{ 0,1,1,0,1,0,1,0,1,0 },
	{ 1,0,1,1,1,1,0,0,1,0 },
	{ 1,1,0,1,0,1,0,0,1,0 },
	{ 1,0,0,0,1,0,1,0,0,0 },
	{ 0,1,0,1,1,1,0,1,1,0 },
	{ 1,0,0,1,1,1,0,0,0,0 }
	};

	//목적지 좌표로 입구 (1,1)은 불허
	if (DST_ROW == 1 && DST_COL == 1) {
		printf("\nThe coordinate of destination should be different from that of entrance (1,1).");
		exit(0);
	}
	//목적지는 block되어 있지 않도록 한다.
	if (maze0[DST_ROW - 1][DST_COL - 1] == 1) {
		printf("\nmaze[DST_ROW][DST_COL] should be 0.");
		exit(0);
	}

	for (i = 0; i < numCol + 2; i++) maze[0][i] = 1;
	for (i = 0; i < numCol + 2; i++) maze[numRow + 1][i] = 1;
	for (i = 0; i < numRow + 2; i++) maze[i][0] = 1;
	for (i = 0; i < numRow + 2; i++) maze[i][numCol + 1] = 1;
	for (i = 1; i <= numRow; i++)
		for (j = 1; j <= numCol; j++) maze[i][j] = maze0[i - 1][j - 1];
}

void set_move() {
	move[0].vert = -1; move[0].horiz = 0; //북
	move[1].vert = -1; move[1].horiz = 1; //북동
	move[2].vert = 0; move[2].horiz = 1; //동
	move[3].vert = 1; move[3].horiz = 1; //남동
	move[4].vert = 1; move[4].horiz = 0; //남
	move[5].vert = 1; move[5].horiz = -1; //남서
	move[6].vert = 0; move[6].horiz = -1; //서
	move[7].vert = -1; move[7].horiz = -1; //북서
}

void push(element position) {
	stack[++top] = position;
}

element pop() {
	element position;

	position = stack[top--];
	return position;
}

void path() {
	int i, row, col, nextRow, nextCol, dir, dir_vector, found = FALSE;
	element position; 

	row = 1; col = 1;
	maze[1][1] = -1; //-1은 이미 지나간 자리라고 생각하자.
	dir_vector = init_dir_vector(1, 1); //8개의 방향 정찰해서 갈 수 있는 방향을 찾기
	if (dir_vector == -1) found = TRUE; //(1,1)의 바로 이웃에 목적지가 있음 
	else {
		top = 0;
		stack[0].row = 1; stack[0].col = 1; stack[0].dir_vector = dir_vector; 
	}
	while (top > -1 && !found) {
		position = pop();

		row = position.row;
		col = position.col;
		dir_vector = position.dir_vector;
		dir = get_next_dir(init_dir_vector(row,col)); //먼저 현재 위치에서 갈 수 있는 방향을 다 정찰한 후, 랜덤으로 어디로 갈지 알려줘야하므로 두 개의 함수를 사용한다.
		
		while (dir < 8 && !found) {
			nextRow = row + move[dir].vert;
			nextCol = col + move[dir].horiz;

			if (nextRow == DST_ROW && nextCol == DST_COL) //만약 경로가 마지막 목적지이면 found에 참값
			{
				found = TRUE;
			}
			else {
				maze[nextRow][nextCol] = -1; //이미 지나간 자리
				position.row = row; position.col = col;
				position.dir_vector = get_next_dir(init_dir_vector(row, col)); //가야할 위치 -1로 바꾸고 가요
				push(position); //스택에 넣어줌
				row = nextRow; col = nextCol; //현재 위치를 이동할 위치로 옮김
				dir_vector = init_dir_vector(row, col); //이동할 수 있는 경로 설정
				dir = get_next_dir(dir_vector); //갈 수 있는 방향을 랜덤으로 정함
			}
			}
	}
	if (found) {
		printf("The path is:\n");
		printf("row  col\n");
		for (i = 0; i <= top; i++) printf("%2d%5d\n", stack[i].row, stack[i].col);
		printf("%2d%5d\n", row, col);
		printf("%2d%5d\n", DST_ROW, DST_COL);
	}
	else printf("The maze does not have a path\n");
}

int init_dir_vector(int row, int col) {
	// input: 위치 좌표
	// output: -1 또는 0 ~ 255(=2^8-1) 범위의 정수값
	//
	// 위치 (row, col)의 8 방향 이웃 중 목적지가 있으면 -1을 return (하여 경로 찾기를 종료하게 한다), 
	// 없으면 0 ~ 255(=2^8-1) 범위의 정수값으로 표현된 direction vector를 초기화하여 return
	//
	// direction vector: 8-bit string으로 그 크기에 해당하는 정수값으로 표현된다
	// least significant bit부터 0번, 1번, ..., 7번 bit라고 부르자
	// i번 bit는 방향 i에 대응 (i=0,...,7). 예: 0번 bit는 north, 1번 bit는 north-east, ...
	// i번 bit=1: 방향 i는 경로 찾기에서 이미 try했다는 것을 표시
	// i번 bit=0: 방향 i로는 아직 가보지 않았다는 것을 표시
	// 위치 (row, col)의 direction vector 초기화: 위치 (row, col)의 8 방향 각각에 대해, block되어 갈 수 없거나
	// 이미 갔던 방향이면 direction vector의 해당 bit을 1로, 그렇지 않으면 0으로 set.
	// 초기화 수행 방법 (아래 코드): 처음에 8 bits를 모두 1로 set한 후, 각 방향 별로 체크하여 갈 수 있으면 해당 bit을 0으로 reset

	int dir, nextRow, nextCol, dir_vector;

	dir_vector = ((int)pow(2.0, 8.0)) - 1; // dir_vector = 10진수로 255, 이진수로 11111111 (8 bit 모두 1)
	for (dir = 0; dir < 8; dir++) {
		nextRow = row + move[dir].vert;
		nextCol = col + move[dir].horiz;
		if (nextRow == DST_ROW && nextCol == DST_COL) return -1; // to report that a path is found
		if (!maze[nextRow][nextCol]) dir_vector -= (int)pow(2.0, (double)dir);
	}
	return dir_vector;
}

int get_next_dir(int dir_vector) {
	// input: direction vector (0 ~ 255(=2^8-1) 범위의 정수값)
	// output: 방향값 0..7 또는 8
	//
	// direction vector를 표현한 정수값을 받아서 8 bits 각각의 1 또는 0 값을 
	// 크기 8인 배열 (아래 코드의 d[8] 배열)에 옮긴 후
	// 0인 bit 중 하나를 random하게 선택하여 그 bit에 해당되는 방향값 (0..7)을 return
	// 만약 0인 bit가 하나도 없으면 8을 return (하여 backtrack을 수행하게 한다)

	int dir, d[8], count, k;

	for (dir = 0; dir < 8; dir++) {
		d[dir] = dir_vector % 2;
		dir_vector /= 2;
	}

	// count = direction vector에서 0 bit의 갯수
	count = 0;
	for (dir = 0; dir < 8; dir++) if (d[dir] == 0) count++;

	if (count == 0) return 8;

	// count개의 0 bit 중 하나를 random하게 선택하여 해당 방향값 (0..7)을 return
	k = rand() % count + 1;
	for (dir = 0; dir < 8; dir++) {
		if (d[dir] == 0) k--;
		if (k == 0) return dir;
	}
}