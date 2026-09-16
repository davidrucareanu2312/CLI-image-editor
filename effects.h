#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "file_utils.h"
#include "tools.h"
#include "utils.h"

void HISTOGRAM(int ***matrix, int m, int n, char format[3])
{
	char *xaux, *yaux, *tmp;
	xaux = strtok(NULL, "\n ");
	yaux = strtok(NULL, "\n ");
	tmp = strtok(NULL, "\n ");
	if (!matrix) {
		printf("No image loaded\n");
		return;
	} else if (!xaux || !yaux || tmp) {
		printf("Invalid command\n");
		return;
	} else if (!strcmp(format, "P3") || !strcmp(format, "P6")) {
		printf("Black and white image needed\n");
		return;
	}

	int x = atoi(xaux);
	int y = atoi(yaux);

	int freq[256] = {0};
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			++freq[(matrix)[i][j][0]];
		}
	}

	int max = 0;
	for (int i = 0; i < y; i++) {
		int tmp = 0;
		for (int j = i * 256 / y; j < (i + 1) * 256 / y; j++) {
			tmp = tmp + freq[j];
		}
		if (tmp > max) {
			max = tmp;
		}
	}

	for (int i = 0; i < y; i++) {
		int tmp = 0;
		for (int j = i * 256 / y; j < (i + 1) * 256 / y; j++) {
			tmp = tmp + freq[j];
		}
		tmp = tmp * x / max;
		char t = '\t';
		char c = '|';
		printf("%d%c%c%c", tmp, t, c, t);
		for (int j = 0; j < tmp; j++) {
			printf("*");
		}
		printf("\n");

	}

}

void EQUALIZE(int ****matrix, int m, int n, char format[3])
{
	if (!(*matrix)) {
		printf("No image loaded\n");
		return;
	}
	if (!strcmp(format, "P3") || !strcmp(format, "P6")) {
		printf("Black and white image needed\n");
		return;
	}

	int area = m * n;

	int freq[256] = {0};
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			++freq[(*matrix)[i][j][0]];
		}
	}

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			int a = (*matrix)[i][j][0];
			int sum = 0;
			for (int k = 0; k <= a; k++) {
				sum = sum + freq[k];
			}
			(*matrix)[i][j][0] = round(255 * sum / (double)area);
		}
	}

	printf("Equalize done\n");

}

void APPLY_EFFECT(int ****matrix, int x1, int y1, int x2,
				  int y2, int m, int n, int range, int effect)
{
	int h = x2 - x1, l = y2 - y1;
	int ***tmp;

	mallocation(&tmp, h, l, "P3");

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < l; j++) {
			tmp[i][j][0] = 0;
			tmp[i][j][1] = 0;
			tmp[i][j][2] = 0;
		}
	}

	//the kernels for all the effects are predefined
	int ker[4][3][3] = {{{-1, -1, -1}, {-1, 8, -1}, {-1, -1, -1}},
						{{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}},
						{{1, 1, 1}, {1, 1, 1}, {1, 1, 1}},
						{{1, 2, 1}, {2, 4, 2}, {1, 2, 1}}};
	int v[4] = {1, 1, 9, 16};

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < l; j++) {
			for (int p = -1; p < 2; p++) {
				for (int q = -1; q < 2; q++) {
					if (i + p + x1 >= 0 && j + q + y1 >= 0 &&
						i + p + x1 < m && j + q + y1 < n) {
						tmp[i][j][0] += ker[effect][p + 1][q + 1] *
						(*matrix)[i + p + x1][j + q + y1][0];
						tmp[i][j][1] += ker[effect][p + 1][q + 1] *
						(*matrix)[i + p + x1][j + q + y1][1];
						tmp[i][j][2] += ker[effect][p + 1][q + 1] *
						(*matrix)[i + p + x1][j + q + y1][2];
					}
				}
			}
		}
	}

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < l; j++) {
			if (i + x1 > 0 && j + y1 > 0 && i + x1 < m - 1 && j + y1 < n - 1) {
				//a check is made to make sure the applied changes are in bounds
				/*the clamp function is applied in case of and effect that might
				result in an out of bounds pixel value*/
				(*matrix)[i + x1][j + y1][0] =
				clamp(tmp[i][j][0] / v[effect], 0, range);
				(*matrix)[i + x1][j + y1][1] =
				clamp(tmp[i][j][1] / v[effect], 0, range);
				(*matrix)[i + x1][j + y1][2] =
				clamp(tmp[i][j][2] / v[effect], 0, range);
			}
		}
	}

	free_matrix(&tmp, h, l);
}

void APPLY(int ****matrix, int x1, int y1, int x2, int y2, int m, int n,
		   int range, char format[3])
{
	char *parameter;
	parameter = strtok(NULL, "\n ");

	if (!*matrix) {
		printf("No image loaded\n");
	} else if (!parameter) {
		printf("Invalid command\n");
	} else if (!strcmp(parameter, "EDGE") || !strcmp(parameter, "SHARPEN") ||
			   !strcmp(parameter, "BLUR") ||
			   !strcmp(parameter, "GAUSSIAN_BLUR")) {
		if (!(!strcmp(format, "P3") || !strcmp(format, "P6"))) {
			printf("Easy, Charlie Chaplin\n");
		} else {
			/*the functions are called with the corresponding parameter
			to be able to use the necessary kernel for the effect*/
			if (!strcmp(parameter, "EDGE")) {
				APPLY_EFFECT(&(*matrix), x1, y1, x2, y2, m, n, range, 0);
			}
			if (!strcmp(parameter, "SHARPEN")) {
				APPLY_EFFECT(&(*matrix), x1, y1, x2, y2, m, n, range, 1);
			}
			if (!strcmp(parameter, "BLUR")) {
				APPLY_EFFECT(&(*matrix), x1, y1, x2, y2, m, n, range, 2);
			}
			if (!strcmp(parameter, "GAUSSIAN_BLUR")) {
				APPLY_EFFECT(&(*matrix), x1, y1, x2, y2, m, n, range, 3);
			}
			printf("APPLY %s done\n", parameter);
		}
	} else {
		printf("APPLY parameter invalid\n");
	}
}
