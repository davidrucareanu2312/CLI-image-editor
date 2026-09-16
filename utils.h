#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "file_utils.h"
#include "tools.h"
#include "effects.h"

typedef struct {
	//structure used for storing the loaded images
	char format[3];
	int m, n;
	int range;
	int ***image;
	int x1, y1, x2, y2;
} img;

void mallocation(int ****matrix, int m, int n, char format[3])
{
	//function used to allocate memory to store an image
	(*matrix) = (int ***)malloc(m * sizeof(int **));

	for (int i = 0; i < m; i++) {
		(*matrix)[i] = (int **)malloc(n * sizeof(int *));
		for (int j = 0; j < n; j++) {
			if (!strcmp(format, "P3") || !strcmp(format, "P6")) {
				(*matrix)[i][j] = (int *)malloc(3 * sizeof(int));
			} else {
				(*matrix)[i][j] = (int *)malloc(sizeof(int));
			}
		}
	}

}

void free_matrix(int ****matrix, int m, int n)
{
	//function used to free memory of an stored image
	if (*matrix) {
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				free((*matrix)[i][j]);
			}
			free((*matrix)[i]);
		}
		free(*matrix);
	}
}

int min(int x, int y)
{
	if (x > y) {
		return y;
	}
	return x;
}

int max(int x, int y)
{
	if (x > y) {
		return x;
	}
	return y;
}

int ispowtwo(int x)
{
	if (x < 2) {
		return 0;
	}
	while (x % 2 == 0) {
		x = x / 2;
	}
	if (x == 1) {
		return 1;
	}
	return 0;
}

int isnumber(char *number)
{
	//utility function used for an edge case for the SELECT command
	if (number[0] == '-') {
		for (int i = 1; i < (int)strlen(number); i++) {
			if (!isdigit(number[i])) {
				return 0;
			}
		}
	} else {
		for (int i = 0; i < (int)strlen(number); i++) {
			if (!isdigit(number[i])) {
				return 0;
			}
		}
	}
	return 1;
}

int clamp(int x, int min, int max)
{
	//utility function used for the APPLY command
	if (x < min) {
		return min;
	}
	if (x > max) {
		return max;
	}
	return x;
}
