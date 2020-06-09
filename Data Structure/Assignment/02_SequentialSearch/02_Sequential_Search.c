#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int i;

void shuffle(int* list, int size) {
	srand(time(NULL));
	int temp, random;
	for (i = 0; i < size; i++) {
		random = rand() % (size - i) + i;
		temp = list[i];
		list[i] = list[random];
		list[random] = temp;
	}
}

void main() {
	int size, search, index = -1;
	int* list;

	clock_t timeStart, timeEnd;
	int timeSearch;

	scanf("%d", &size);

	list = (int*)calloc(sizeof(int), size);
	for (i = 0; i < size; i++)
		list[i] = i;
	shuffle(list, size);

	/*
	for (i = 0; i < size; i++)
		printf("%d ", list[i]);
	*/

	printf("Number to search: ");
	scanf("%d", &search);

	timeStart = clock();
	for (i = 0; i < size; i++)
		if (list[i] == search) {
			index = i;
			printf("Searched: %d\n", index);
			break;
		}

	if (index < 0)
		printf("fail\n");
	timeEnd = clock();

	timeSearch = (int)(timeEnd - timeStart);
	printf("Time: %d\n", timeSearch);
}