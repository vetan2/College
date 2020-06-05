#include <stdio.h>
#include <stdlib.h>

int** maze, ** mark;
int size_row, size_col, exit_row, exit_col;
int top, found, i, j;

typedef struct {
	int vert;
	int horiz;
} moveStruct;
moveStruct move[8] = { { 0, 1 } ,{ 1, 1 }, { 1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 }, { -1, 0 }, { -1, 1 } };

typedef struct _element{
	int row;
	int col;
	int dir;
} element;
element position;
element* stack;

element pop() {
	top--;

	if(top >= 0)
		return stack[top];

	element new = { 1, 1, 0 };
	return new;
}

void push(element para) {
	top++;

	stack[top] = para;
}

void path() {
	int row, col, nextRow, nextCol, dir;

	found = 0;
	mark[1][1] = 1, top = 0;
	stack[0].row = 1, stack[0].col = 1, stack[0].dir = 0;

	while (top > -1 && !found) {
		position = pop();
		row = position.row, col = position.col, dir = position.dir;

		while (dir < 8 && !found) {
			nextRow = row + move[dir].vert;
			nextCol = col + move[dir].horiz;
			
			if (nextRow == exit_row && nextCol == exit_col)
				found = 1;

			else if (!maze[nextRow][nextCol] && !mark[nextRow][nextCol]) {
				mark[nextRow][nextCol] = 1;
				if (stack[top].row == row && stack[top].col == col)
					stack[top].dir = dir;

				else {
					position.row = row, position.col = col, position.dir = dir;
					push(position);
				}
				
				row = nextRow, col = nextCol, dir = 0;
			}

			else
				++dir;
		}
	}

	if (found) {
		printf("The path is : \n");
		printf("row col \n");
		for (i = 0; i <= top; i++)
			printf("%2d%5d\n", stack[i].row, stack[i].col);
		printf("%2d%5d\n", row, col);
		printf("%2d%5d\n", exit_row, exit_col);
	}

	else
		printf("-1\n");

	return;
}

int main() {
	FILE* file = fopen("input.txt", "r");

	fscanf(file, "%d %d\n", &size_row, &size_col);
	exit_row = size_row, exit_col = size_col;
	stack = calloc(size_row * size_col, sizeof(element));



	size_row += 2, size_col += 2;

	maze = (int**)calloc(size_row, sizeof(int*));
	for (i = 0; i < size_row; i++)
		maze[i] = (int*)calloc(size_col, sizeof(int));

	for (i = 0; i < size_row; i++) {
		if (i == 0 || i == size_row - 1)
			for (j = 0; j < size_col; j++)
				maze[i][j] = 1;

		else
			maze[i][0] = 1, maze[i][size_col - 1] = 1;
	}

	for (i = 1; i < size_row - 1; i++)
		for (j = 1; j < size_col - 1; j++)
			fscanf(file, "%d", &(maze[i][j]));
		


	mark = (int**)calloc(size_row, sizeof(int*));
	for (i = 0; i < size_row; i++)
		mark[i] = (int*)calloc(size_col, sizeof(int));

	path();
}