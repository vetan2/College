#include "poly.h"

poly* makePoly(FILE* f);
void printPoly(poly* p);

int main() {
	FILE* fA, * fB;
	fA = fopen("A.txt", "r");
	fB = fopen("B.txt", "r");

	poly* polyA = makePoly(fA);
	poly* polyB = makePoly(fB);

	printPoly(Add(polyA, polyB));
	printPoly(Mult(polyA, polyB));

	fclose(fA), fclose(fB);
}

poly* makePoly(FILE* f) {
	float coef;
	int expon;
	poly* _new = Zero();

	while (1) {
		if (fscanf(f, "%f", &coef) == EOF)
			break;
		fscanf(f, "%d", &expon);

		_new = Attach(_new, coef, expon);
	}

	return _new;
}

void printPoly(poly* p) {
	poly* cur = p;
	while (cur) {
		printf("(%.2f,%d)", cur->coef, cur->expon);
		if (cur->link != NULL)
			printf(", ");
		cur = cur->link;
	}
	printf("\n");
	return;
}