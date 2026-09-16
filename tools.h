#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "file_utils.h"
#include "effects.h"
#include "utils.h"

void SELECT(int ***matrix, int m, int n,
			char *selected, int *x1, int *y1, int *x2, int *y2)
{
	if (!strcmp(selected, "ALL")) {
		if (matrix) {
			*x1 = 0;
			*y1 = 0;
			*x2 = m;
			*y2 = n;

			printf("Selected ALL\n");

		} else {
			printf("No image loaded\n");

		}

	} else {
		char *x1aux, *y1aux, *x2aux, *y2aux;
		y1aux = selected;
		x1aux = strtok(NULL, "\n ");
		y2aux = strtok(NULL, "\n ");
		x2aux = strtok(NULL, "\n ");

		int tmpx1, tmpx2, tmpy1, tmpy2;

		/*in the case that the selection corners are read as non-NULL
		pointers, they are stored as integers as the minimum/maximum of
		the read values*/
		if (y1aux && y2aux) {
			tmpy1 = min(atoi(y1aux), atoi(y2aux));
			tmpy2 = max(atoi(y1aux), atoi(y2aux));
		}

		if (x1aux && x2aux) {
			tmpx1 = min(atoi(x1aux), atoi(x2aux));
			tmpx2 = max(atoi(x1aux), atoi(x2aux));
		}

		if (!matrix) {
			printf("No image loaded\n");
		} else if (!x1aux || !y1aux || !x2aux || !y2aux) {
			printf("Invalid command\n");
		} else if (!(isnumber(x1aux) && isnumber(y1aux) &&
		isnumber(x2aux) && isnumber(y2aux))) {
			printf("Invalid command\n");
		} else if (tmpx1 == tmpx2 || tmpy1 == tmpy2 ||
		tmpx1 < 0  || tmpy1 < 0 || tmpx2 > m || tmpy2 > n) {
			printf("Invalid set of coordinates\n");

		} else {
			*x1 = tmpx1;
			*y1 = tmpy1;
			*x2 = tmpx2;
			*y2 = tmpy2;

			printf("Selected %d %d %d %d\n", *y1, *x1, *y2, *x2);

		}
	}
}

void ROTATE_SELECTION(int ****matrix, int x1, int y1,
					  int l, int angle, char format[3])
{
	int ***tmp;

	mallocation(&tmp, l, l, format);

	for (int i = 0; i < l; i++) {
		for (int j = 0; j < l; j++) {
			if (!strcmp(format, "P3") || !strcmp(format, "P6")) {
				tmp[i][j][0] = (*matrix)[i + x1][j + y1][0];
				tmp[i][j][1] = (*matrix)[i + x1][j + y1][1];
				tmp[i][j][2] = (*matrix)[i + x1][j + y1][2];
			} else {
				tmp[i][j][0] = (*matrix)[i + x1][j + y1][0];
			}
		}
	}

	/*90 degree rotation is implemented as a transposition
	and a flip applied simultaneously*/
	if (angle == 90) {
		for (int i = 0; i < l; i++) {
			for (int j = 0; j < l; j++) {
				if (!strcmp(format, "P3") || !strcmp(format, "P6")) {
					(*matrix)[i + x1][j + y1][0] = tmp[l - j - 1][i][0];
					(*matrix)[i + x1][j + y1][1] = tmp[l - j - 1][i][1];
					(*matrix)[i + x1][j + y1][2] = tmp[l - j - 1][i][2];
				} else {
					(*matrix)[i + x1][j + y1][0] = tmp[l - j - 1][i][0];
				}
			}
		}
	}

	/*180 degree rotation is implemented a flip*/
	if (angle == 180) {
		for (int i = 0; i < l; i++) {
			for (int j = 0; j < l; j++) {
				if (!strcmp(format, "P3") || !strcmp(format, "P6")) {
					(*matrix)[i + x1][j + y1][0] = tmp[l - i - 1][l - j - 1][0];
					(*matrix)[i + x1][j + y1][1] = tmp[l - i - 1][l - j - 1][1];
					(*matrix)[i + x1][j + y1][2] = tmp[l - i - 1][l - j - 1][2];
				} else {
					(*matrix)[i + x1][j + y1][0] = tmp[l - i - 1][l - j - 1][0];
				}
			}
		}
	}

	/*270 degree rotation, similarly to the 90 degree one
	is implemented as a transposition and a flip applied
	simultaneously*/
	if (angle == 270) {
		for (int i = 0; i < l; i++) {
			for (int j = 0; j < l; j++) {
				if (!strcmp(format, "P3") || !strcmp(format, "P6")) {
					(*matrix)[i + x1][j + y1][0] = tmp[j][l - i - 1][0];
					(*matrix)[i + x1][j + y1][1] = tmp[j][l - i - 1][1];
					(*matrix)[i + x1][j + y1][2] = tmp[j][l - i - 1][2];
				} else {
					(*matrix)[i + x1][j + y1][0] = tmp[j][l - i - 1][0];
				}
			}
		}
	}

	free_matrix(&tmp, l, l);
}

void ROTATE_ALL(int ****matrix, int *m, int *n,
				int *x2, int *y2, int angle, char format[3])
{
	/*the full rotation is implemented identically to the
	rotation on selected portions*/
	int maux = *m;
	int naux = *n;
	int ***tmp;

	mallocation(&tmp, *m, *n, format);

	for (int i = 0; i < (*m); i++) {
		for (int j = 0; j < (*n); j++) {
			if (!strcmp(format, "P3") || !strcmp(format, "P6")) {
				tmp[i][j][0] = (*matrix)[i][j][0];
				tmp[i][j][1] = (*matrix)[i][j][1];
				tmp[i][j][2] = (*matrix)[i][j][2];
			} else {
				tmp[i][j][0] = (*matrix)[i][j][0];
			}
		}
	}

	free_matrix(&(*matrix), *m, *n);

	if (angle == 180) {
		mallocation(&(*matrix), *m, *n, format);

		for (int i = 0; i < (*m); i++) {
			for (int j = 0; j < (*n); j++) {
				if (!strcmp(format, "P3") || !strcmp(format, "P6")) {
					(*matrix)[i][j][0] = tmp[(*m) - i - 1][(*n) - j - 1][0];
					(*matrix)[i][j][1] = tmp[(*m) - i - 1][(*n) - j - 1][1];
					(*matrix)[i][j][2] = tmp[(*m) - i - 1][(*n) - j - 1][2];
				} else {
					(*matrix)[i][j][0] = tmp[(*m) - i - 1][(*n) - j - 1][0];
				}
			}
		}

	} else {
		/*the rotation is split into two cases, depending on the necessary
		dimentions needed to allocate to the temporary matrix*/
		int aux = *m;
		*m = *n;
		*n = aux;
		mallocation(&(*matrix), *m, *n, format);

		if (angle == 90) {
			for (int i = 0; i < (*m); i++) {
				for (int j = 0; j < (*n); j++) {
					if (!strcmp(format, "P3") || !strcmp(format, "P6")) {
						(*matrix)[i][j][0] = tmp[(*n) - j - 1][i][0];
						(*matrix)[i][j][1] = tmp[(*n) - j - 1][i][1];
						(*matrix)[i][j][2] = tmp[(*n) - j - 1][i][2];
					} else {
						(*matrix)[i][j][0] = tmp[(*n) - j - 1][i][0];
					}
				}
			}
		} else {
			for (int i = 0; i < (*m); i++) {
				for (int j = 0; j < (*n); j++) {
					if (!strcmp(format, "P3") || !strcmp(format, "P6")) {
						(*matrix)[i][j][0] = tmp[j][(*m) - i - 1][0];
						(*matrix)[i][j][1] = tmp[j][(*m) - i - 1][1];
						(*matrix)[i][j][2] = tmp[j][(*m) - i - 1][2];
					} else {
						(*matrix)[i][j][0] = tmp[j][(*m) - i - 1][0];
					}
				}
			}
		}

		aux = *x2;
		*x2 = *y2;
		*y2 = aux;
	}

	free_matrix(&tmp, maux, naux);
}

void ROTATE(int ****matrix, int *m, int *n,
			int x1, int y1, int *x2, int *y2, char format[3])
{
	int angle = atoi(strtok(NULL, "\n "));
	int angleaux = angle;
	if (angle < 0) {
		angle = (-((-angle) % 360) + 360) % 360;
	}

	if (!*matrix) {
		printf("No image loaded\n");
	} else if (angle % 90 != 0) {
		printf("Unsupported rotation angle\n");
	} else if ((*x2 - x1 < *m || *y2 - y1 < *n) && (*x2 - x1) != (*y2 - y1)) {
		printf("The selection must be square\n");
	} else {
		if (x1 == 0 && y1 == 0 && *x2 == *m && *y2 == *n) {
			if (angle > 0 && angle < 360) {
				ROTATE_ALL(&(*matrix), &(*m),
						   &(*n), &(*x2), &(*y2), angle, format);
			}
		} else {
			if (angle > 0 && angle < 360) {
				ROTATE_SELECTION(&(*matrix), x1, y1, *x2 - x1, angle, format);
			}
		}
		printf("Rotated %d\n", angleaux);
	}
}

void CROP(int ****matrix, int *x1, int *y1,
		  int *x2, int *y2, int *m, int *n, char format[3])
{
	if (!(*matrix)) {
		printf("No image loaded\n");
		return;
	}
	int maux = *m;
	int naux = *n;

	*m = *x2 - *x1;
	*n = *y2 - *y1;

	int ***tmp;

	mallocation(&tmp, *m, *n, format);

	for (int i = 0; i < *m; i++) {
		for (int j = 0; j < *n; j++) {
			if (!strcmp(format, "P3") || !strcmp(format, "P6")) {
				tmp[i][j][0] = (*matrix)[i + *x1][j + *y1][0];
				tmp[i][j][1] = (*matrix)[i + *x1][j + *y1][1];
				tmp[i][j][2] = (*matrix)[i + *x1][j + *y1][2];
			} else {
				tmp[i][j][0] = (*matrix)[i + *x1][j + *y1][0];
			}
		}
	}

	free_matrix(&(*matrix), maux, naux);

	mallocation(&(*matrix), *m, *n, format);

	for (int i = 0; i < *m; i++) {
		for (int j = 0; j < *n; j++) {
			if (!strcmp(format, "P3") || !strcmp(format, "P6")) {
				(*matrix)[i][j][0] = tmp[i][j][0];
				(*matrix)[i][j][1] = tmp[i][j][1];
				(*matrix)[i][j][2] = tmp[i][j][2];
			} else {
				(*matrix)[i][j][0] = tmp[i][j][0];
			}
		}
	}

	free_matrix(&tmp, *m, *n);

	*x1 = 0;
	*y1 = 0;
	*x2 = *m;
	*y2 = *n;

	printf("Image cropped\n");

}
