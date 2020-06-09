#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VERTICES 30
#define MIN2(x, y) ((x) < (y) ? (x) : (y))

typedef struct _node {
	int val;
	struct _node* link;
} node;

short int dfn[MAX_VERTICES];
short int low[MAX_VERTICES];
node** graph;
int num;

void init(FILE*);
void bicon(int, int);

int main() {
	FILE* fp = fopen("input.txt", "r");
	
	init(fp);
}

void init(FILE* fp) {
	int i, j, k;
	char tempStr[30], tempStr_2[10];
	node* temp, * New;

	if (!fscanf(fp, "%d", &num)) {
		printf("Wrong File\n");
		return -1;
	}

	// graph의 메모리 할당 & 초기화
	graph = (node**)calloc(num, sizeof(node*));
	for (i = 0; i < num; i++) {
		graph[i] = (node*)calloc(num, sizeof(node));
		graph[i]->val = i;
		graph[i]->link = NULL;
	}

	// 다음 줄로 넘어감
	while (fgetc(fp) != '\n')
		;

	// 그래프 구축
	for (i = 0; i < num; i++) {
		fgets(tempStr, 29, fp);
		temp = graph[i];
		
		// 줄의 첫 숫자는 무시
		j = 0;
		while (tempStr[j] != ' ' && tempStr[j] != '\n')
			j++;

		// 띄어쓰기를 기준으로, 숫자를 하나씩 graph[i]에 저장
		while (j < strlen(tempStr)) {
			k = 0;
			while (tempStr[j] != ' ' && tempStr[j] != '\n')
				tempStr_2[k++] = tempStr[j++];
			tempStr_2[k] = 0;

			if (k) {
				New = (node*)calloc(1, sizeof(node));
				temp->link = New, temp = temp->link;
				temp->val = atoi(tempStr_2), temp->link = NULL;
			}

			else
				j++;
		}
	}
}

void bicon(int u, int v) {
	// TO DO
}