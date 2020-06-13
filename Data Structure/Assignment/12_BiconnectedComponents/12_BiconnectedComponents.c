#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VERTICES 30
#define MIN2(x, y) ((x) < (y) ? (x) : (y))

typedef struct _node {
	int val;
	struct _node* link;
} node;

typedef struct _edgeNode {
	int val1, val2;
	struct _edgeNode* link;
} edgeNode;

short int dfn[MAX_VERTICES];
short int low[MAX_VERTICES];
node** graph;
edgeNode* top = NULL;
edgeNode* bot = NULL;
int num;

void init(FILE*);
void bicon(int, int);
void add(int, int);
void delete(int*, int*);

int main() {
	FILE* fp = fopen("input.txt", "r");
	
	init(fp);
	bicon(3, -1);
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

	// dfn, low, num의 초기화
	for (i = 0; i < num; i++)
		dfn[i] = low[i] = -1;
	num = 0;
}

void bicon(int u, int v) {
	// TO DO
	node* ptr;
	int w, x, y;

	// u에 대한 dfn, low 초기화
	dfn[u] = low[u] = num++;

	// ptr이 가장 아래일 때까지
	for (ptr = graph[u]; ptr; ptr = ptr->link) {
		w = ptr->val;

		// u보다 높이 올라가면 스택에 저장
		if (v != w && dfn[w] < dfn[u])
			add(u, w);

		// w가 방문하지 않은 노드라면
		if (dfn[w] < 0) {

			// w 이하의 노드들에 대해 bicon 해줌
			bicon(w, u);

			low[u] = MIN2(low[u], low[w]);
			if (low[w] >= dfn[u]) {
				do {
					delete(&x, &y);
					printf("<%d, %d> ", x, y);
				} while (!((x == u) && (y == w)));

				printf("\n");
			}
		}

		else if (w != v)low[u] = MIN2(low[u], dfn[w]);
	}
}

// 스택에 edge <u, v>를 삽입
void add(int u, int v) {
	edgeNode* new = (edgeNode*)calloc(1, sizeof(edgeNode));
	new->val1 = u, new->val2 = v, new->link = NULL;

	if (!top)
		top = bot = new;

	else {
		top->link = new;
		top = top->link;
	}
}

// 스택의 top을 삭제, *x와 *y는 삭제된 edge의 두 vertex를 가리킴
void delete(int* x, int* y) {
	edgeNode* toDel = top;
	edgeNode* temp = bot;

	if (!top)
		return;

	*x = toDel->val1, * y = toDel->val2;

	if (temp == toDel)
		top = bot = NULL;

	else {
		while (temp->link != toDel)
			temp = temp->link;
		temp->link = NULL, top = temp;
	}

	free(toDel);
}