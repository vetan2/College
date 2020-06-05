#include <stdio.h>
#include <stdlib.h>

typedef struct _poly {
	float coef;
	int expon;
	struct _poly* link;
} poly;

int compare(int a, int b);
poly* makePoly(FILE* f);
void printPoly(poly* paraPoly);

int isZero(poly* paraPoly);
float coef(poly* paraPoly, int expon);
int leadExp(poly* paraPoly);
poly* Attach(poly* paraPoly, float coef, int expon);
poly* Remove(poly* paraPoly, int expon);

void main() {
	FILE* fA, * fB;
	fA = fopen("A.txt", "r");
	fB = fopen("B.txt", "r");

	poly* polyA = NULL;
	poly* polyB = NULL;
	poly* polySum = NULL;

	int sum;

	polyA = makePoly(fA);
	polyB = makePoly(fB);
	polySum = NULL;

	while (!isZero(polyA) && !isZero(polyB)) {
		switch (compare(leadExp(polyA), leadExp(polyB))) {
		case -1:
			polySum = Attach(polySum, coef(polyB, leadExp(polyB)), leadExp(polyB));
			polyB = Remove(polyB, leadExp(polyB));
			break;
		case 0:
			sum = coef(polyA, leadExp(polyA)) + coef(polyB, leadExp(polyB));
			if (sum)
				polySum = Attach(polySum, sum, leadExp(polyA));
			polyA = Remove(polyA, leadExp(polyA));
			polyB = Remove(polyB, leadExp(polyB));
			break;
		case 1:
			polySum = Attach(polySum, coef(polyA, leadExp(polyA)), leadExp(polyA));
			polyA = Remove(polyA, leadExp(polyA));
		}
	}

	if (isZero(polyA) && !isZero(polyB))
		while (!isZero(polyB)) {
			polySum = Attach(polySum, coef(polyB, leadExp(polyB)), leadExp(polyB));
			polyB = Remove(polyB, leadExp(polyB));
		}

	if(!isZero(polyA) && isZero(polyB))
		while (!isZero(polyA)) {
			polySum = Attach(polySum, coef(polyA, leadExp(polyA)), leadExp(polyA));
			polyA = Remove(polyA, leadExp(polyA));
		}

	printPoly(polySum);
	return;
}

poly* makePoly(FILE* f) {
	float coef;
	int expon;
	poly* newPoly = NULL;

	while (1) {
		if (fscanf(f, "%f", &coef) == EOF)
			break;
		fscanf(f, "%d", &expon);

		newPoly = Attach(newPoly, coef, expon);
	}
	
	return newPoly;
}

poly* Attach(poly* paraPoly, float coef, int expon) {
	poly* newPoly = (poly*)calloc(sizeof(poly), 1);
	newPoly->coef = coef;
	newPoly->expon = expon;
	newPoly->link = NULL;

	if (paraPoly == NULL)
		return newPoly;

	poly* topPoly = paraPoly;
	if (topPoly->expon < expon) {
		newPoly->link = topPoly;
		topPoly = newPoly;
		return topPoly;
	}

	poly* curPoly = paraPoly;
	while (1) {
		if (curPoly->expon == expon) {
			curPoly->coef = coef;
			free(newPoly);
			return topPoly;
		}

		if (curPoly->link == NULL)
			break;

		if (curPoly->link->expon < expon)
			break;

		curPoly = curPoly->link;
	}
	
	newPoly->link = curPoly->link;
	curPoly->link = newPoly;
	return topPoly;
}

int isZero(poly* paraPoly) {
	if (paraPoly == NULL)
		return 1;
	return 0;
}

int compare(int a, int b) {
	if (a > b)
		return 1;
	
	if (a == b)
		return 0;

	return -1;
}

int leadExp(poly* paraPoly) {
	poly* curPoly = paraPoly;
	int max = curPoly->expon;

	if (curPoly == NULL)
		return 0;

	while (curPoly->link != NULL) {
		curPoly = curPoly->link;
		if (curPoly->expon > max)
			max = curPoly->expon;
	}

	return max;
}

float coef(poly* paraPoly, int expon) {
	poly* curPoly = paraPoly;

	if (curPoly == NULL)
		return 0;

	if (curPoly->expon == expon)
		return curPoly->coef;

	float coef = 0;
	while (curPoly->link != NULL) {
		curPoly = curPoly->link;
		if (curPoly->expon == expon) {
			coef = curPoly->coef;
			break;
		}
	}

	return coef;
}

poly* Remove(poly* paraPoly, int expon) {
	poly* topPoly = paraPoly;

	if (topPoly == NULL)
		return NULL;

	if (topPoly->expon == expon) {
		if (topPoly->link == NULL)
			return NULL;

		topPoly = topPoly->link;
		return topPoly;
	}

	if (topPoly->link == NULL)
		return topPoly;

	poly* curPoly = topPoly->link;
	if (curPoly->expon == expon) {
		topPoly->link = curPoly->link;
		free(curPoly);
		return topPoly;
	}
	
	poly* prePoly = topPoly;
	while (curPoly->link != NULL) {
		prePoly = prePoly->link;
		curPoly = curPoly->link;
		if (curPoly->expon == expon) {
			prePoly->link = curPoly->link;
			free(curPoly);
			return topPoly;
		}
	}

	return topPoly;
}

void printPoly(poly* paraPoly) {
	poly* curPoly = paraPoly;
	while (curPoly != NULL) {
		printf("(%d,%d)", (int)curPoly->coef, curPoly->expon);
		if (curPoly->link != NULL)
			printf(", ");
		curPoly = curPoly->link;
	}
	printf("\n");
	return;
}