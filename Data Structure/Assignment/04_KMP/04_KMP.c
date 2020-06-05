#include <stdio.h>
#include <string.h>

#define maxStrSize 100
#define maxPatSize 100

int failure[maxPatSize];
char str[maxStrSize];
char pat[maxPatSize];

int Match() {
	int i = 0, j = 0;
	
	while (i < strlen(str) && j < strlen(pat)) {
		if (str[i] == pat[j])
			i++, j++;

		else if (j == 0)
			i++;

		else
			j = failure[j - 1] + 1;
	}

	return ((j == strlen(pat)) ? (i - strlen(pat)) : -1);
}

void fail() {
	int i, j;

	failure[0] = -1;
	
	for (j = 1; j < strlen(pat); j++) {
		i = failure[j - 1];
		while (pat[j] != pat[i + 1] && i >= 0)
			i = failure[i];

		if (pat[j] == pat[i + 1])
			failure[j] = i + 1;

		else failure[j] = -1;
	}
}

int main() {
	FILE* fileInput = fopen("input.txt", "r");

	fgets(str, 100, fileInput);
	str[strlen(str) - 1] = '\0';
	
	fgets(pat, 100, fileInput);

	fail();
	printf("%d", Match());

	fclose(fileInput);
}