#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int IntCompare(int a, int b) {
	if (a > b)
		return 1;

	else if (a == b)
		return 0;

	return -1;
}