#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_TERMS 100
typedef struct treeNode* treePtr;
typedef struct treeNode {
	treePtr lchild;
	int key;
	treePtr rchild;
} treeNode;

void inorder(treePtr);
void preorder(treePtr);
void insert(treePtr*, int);
treePtr modifiedSearch(treePtr, int);
treePtr modifiedSearchForDelete(treePtr, int);
int getTreeSize(treePtr);
void delete(treePtr*, int);
void rangeSearch(treePtr, int, int);

int main() {
	int i, n, m, A[MAX_TERMS], D[MAX_TERMS];
	treePtr tree = NULL;

	// 파일 오픈
	FILE* fp = fopen("input.txt", "r");
	fscanf(fp, "%d", &n);	// 숫자의 개수를 n에 저장
	for (i = 0; i < n; i++)
		fscanf(fp, "%d", A + i);	// n개의 숫자를 배열 A에 저장

	for (i = 0; i < n; i++)
		insert(&tree, A[i]);	// 순서대로 binary search tree에 저장

	inorder(tree), printf("\n");
	preorder(tree), printf("\n");


	fscanf(fp, "%d", &m);	// 숫자의 개수를 d에 저장
	for (i = 0; i < m; i++)
		fscanf(fp, "%d", D + i);	// d개의 숫자를 배열 D에 저장

	for (i = 0; i < m; i++) {
		delete(&tree, D[i]);
		inorder(tree);
		printf("\n");
	}


	fclose(fp);
}

void insert(treePtr* tree, int n) {
	treePtr new;
	treePtr parent = modifiedSearch(*tree, n);

	// 삽입할 숫자 n이 트리에 없거나, *tree가 NULL일 때에만 삽입을 실행한다.
	if (parent || !(*tree)) {
		new = (treePtr)calloc(1, sizeof(treeNode));
		new->key = n;
		new->lchild = new->rchild = NULL;

		// *tree가 NULL이 아니라면 parent의 알맞은 child에 삽입
		if (*tree) {
			if (n < parent->key)
				parent->lchild = new;

			else
				parent->rchild = new;
		}

		// *tree가 NULL이라면, 이제 *tree는 new를 가리킴
		else
			*tree = new;
	}
}

/*
	트리가 비어 있거나, 트리에서 숫자 n을 찾았다면 NULL을 반환
	트리에서 숫자 n을 못 찾았다면 마지막으로 방문한 노드를 반환
*/
treePtr modifiedSearch(treePtr tree, int n) {

	// 트리가 비어있지 않으면 탐색을 실행
	while (tree) {

		// 트리에서 숫자 n을 찾았다면 NULL을 반환
		if (n == tree->key)
			return NULL;

		if (n < tree->key) {
			/*
				 n이 현재 노드의 data보다 작은데
				현재 노드의 lchild가 없으면 현재 트리의 주소를 반환
			*/
			if (!tree->lchild)
				return tree;

			else
				tree = tree->lchild;
		}


		else {
			/*
				 n이 현재 노드의 data보다 큰데
				현재 노드의 rchild가 없으면 현재 트리의 주소를 반환
			*/
			if (!tree->rchild)
				return tree;

			else
				tree = tree->rchild;
		}
	}

	// 트리가 비어있으면 NULL을 반환
	return NULL;
}

/*
	트리가 비어 있거나, 트리에서 숫자 n을 못 찾았다면 NULL을 반환
	숫자 n을 찾았다면 찾은 노드를 반환
*/
treePtr modifiedSearchForDelete(treePtr tree, int n) {
	// 트리가 비어있지 않으면 탐색을 실행
	while (tree) {

		// 트리에서 숫자 n을 찾았다면 NULL을 반환
		if (n == tree->key)
			return tree;

		if (n < tree->key) {
			/*
				 n이 현재 노드의 data보다 작은데
				현재 노드의 lchild가 없으면 현재 트리의 주소를 반환
			*/
			if (!tree->lchild)
				return tree;

			else
				tree = tree->lchild;
		}


		else {
			/*
				 n이 현재 노드의 data보다 큰데
				현재 노드의 rchild가 없으면 현재 트리의 주소를 반환
			*/
			if (!tree->rchild)
				return tree;

			else
				tree = tree->rchild;
		}
	}

	// 트리가 비어있으면 NULL을 반환
	return NULL;
}

void inorder(treePtr tree) {
	if (tree) {
		inorder(tree->lchild);
		printf("%-3d", tree->key);
		inorder(tree->rchild);
	}
}

void preorder(treePtr tree) {
	if (tree) {
		printf("%-3d", tree->key);
		preorder(tree->lchild);
		preorder(tree->rchild);
	}
}

void delete(treePtr* tree, int d) {

	treePtr toDel = modifiedSearchForDelete(*tree, d), sub;
	treePtr* temp;

	if (!toDel)
		return;

	if (toDel == *tree) {
		if (!toDel->lchild && !toDel->rchild) {
			*tree = NULL;
			free(toDel);
			return;
		}

		if (getTreeSize(toDel->lchild) > getTreeSize(toDel->rchild))
			sub = toDel->lchild;

		else if (getTreeSize(toDel->lchild) == getTreeSize(toDel->rchild)) {
			srand(time(NULL));
			if (rand() % 2)
				sub = toDel->rchild;

			else
				sub = toDel->lchild;
		}

		else
			sub = toDel->rchild;

		if (sub == toDel->lchild) {
			temp = &(toDel->lchild);
			while ((*temp)->rchild)
				temp = &((*temp)->rchild);

			toDel->key = (*temp)->key;
			delete(temp, (*temp)->key);
		}

		else {
			temp = &(toDel->lchild);
			while ((*temp)->rchild)
				temp = &((*temp)->rchild);

			toDel->key = (*temp)->key;
			delete(temp, (*temp)->key);
		}
	}

	else
		delete(&toDel, toDel->key);
}

int getTreeSize(treePtr tree) {
	int size = 0;

	if (tree) {
		size += getTreeSize(tree->lchild);
		size++;
		size += getTreeSize(tree->rchild);
	}

	return size;
}