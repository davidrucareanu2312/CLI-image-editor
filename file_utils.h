#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "tools.h"
#include "effects.h"
#include "utils.h"

void comment(FILE *in)
{
	/*a call of this function will skip every commented
	line that comes afterwards*/
	fseek(in, 1, SEEK_CUR);

	char line[100];
	fpos_t poz;

	fgetpos(in, &poz);
	fgets(line, 100, in);

	/*it reads lines and stores the last position beforehand
	until it finds a line that is not commented*/
	while (line[0] == '#') {
		fgetpos(in, &poz);
		fgets(line, 100, in);
	}

	fsetpos(in, &poz);

}

void LOAD_TXT(int ****matrix, int *m, int *n,
			  int *range, char format[3], FILE *in)
{
	if (!strcmp(format, "P1")) {
		fscanf(in, "%d%d", n, m);

		mallocation(&(*matrix), *m, *n, "P1");
		for (int i = 0; i < *m; i++) {
			for (int j = 0; j < *n; j++) {
				fscanf(in, "%d", &(*matrix)[i][j][0]);
			}
		}

	} else {
		fscanf(in, "%d%d%d", n, m, range);

		if (!strcmp(format, "P2")) {
			mallocation(&(*matrix), *m, *n, "P2");
			for (int i = 0; i < *m; i++) {
				for (int j = 0; j < *n; j++) {
					fscanf(in, "%d", &(*matrix)[i][j][0]);
				}
			}
		}

		if (!strcmp(format, "P3")) {
			mallocation(&(*matrix), *m, *n, "P3");
			for (int i = 0; i < *m; i++) {
				for (int j = 0; j < *n; j++) {
					fscanf(in, "%d%d%d", &(*matrix)[i][j][0],
						   &(*matrix)[i][j][1], &(*matrix)[i][j][2]);
				}
			}
		}
	}

	fclose(in);
}

void LOAD_BIN(int ****matrix, int *m, int *n,
			  int *range, char format[3], FILE *in)
{
	/*in binary files, the values are stored as a single
	byte, so they are read as a single byte variable type*/
	if (!strcmp(format, "P4")) {
		fscanf(in, "%d%d", n, m);

		fseek(in, 1, SEEK_CUR);
		mallocation(&(*matrix), *m, *n, "P4");
		for (int i = 0; i < *m; i++) {
			for (int j = 0; j < *n; j++) {
				unsigned char tmp;
				fread(&tmp, sizeof(unsigned char), 1, in);
				(*matrix)[i][j][0] = tmp;
			}
		}
	} else {
		fscanf(in, "%d%d%d", n, m, range);

		if (!strcmp(format, "P5")) {
			fseek(in, 1, SEEK_CUR);
			mallocation(&(*matrix), *m, *n, "P5");
			for (int i = 0; i < *m; i++) {
				for (int j = 0; j < *n; j++) {
					unsigned char tmp;
					fread(&tmp, sizeof(unsigned char), 1, in);
					(*matrix)[i][j][0] = tmp;
				}
			}
		}

		if (!strcmp(format, "P6")) {
			fseek(in, 1, SEEK_CUR);
			mallocation(&(*matrix), *m, *n, "P6");
			for (int i = 0; i < *m; i++) {
				for (int j = 0; j < *n; j++) {
					unsigned char tmp[3];
					fread(tmp, sizeof(unsigned char), 3, in);
					(*matrix)[i][j][0] = tmp[0];
					(*matrix)[i][j][1] = tmp[1];
					(*matrix)[i][j][2] = tmp[2];
				}
			}
		}
	}

	fclose(in);
}

void LOAD(char format[3], int *m, int *n, int *range,
		  int ****matrix, int *x1, int *y1, int *x2, int *y2, char *filename)
{
	free_matrix(&(*matrix), *m, *n);

	FILE *in = fopen(filename, "rt");
	if (!in) {
		printf("Failed to load %s\n", filename);
		*matrix = NULL;
		return;
	}

	/*the format is read firstly to be able to call the
	corresponding load function*/
	fscanf(in, "%s", format);

	comment(in);

	if (!strcmp(format, "P1") || !strcmp(format, "P2") ||
		!strcmp(format, "P3")) {
		LOAD_TXT(&(*matrix), &(*m), &(*n), &(*range), format, in);
	} else {
		LOAD_BIN(&(*matrix), &(*m), &(*n), &(*range), format, in);
	}

	printf("Loaded %s\n", filename);

	*x1 = 0;
	*y1 = 0;
	*x2 = *m;
	*y2 = *n;

}

void SAVE_TXT(img matrix, char *filename)
{
	FILE *out = fopen(filename, "wt");

	if (!strcmp(matrix.format, "P1") || !strcmp(matrix.format, "P4")) {
		fprintf(out, "P1\n");
		fprintf(out, "%d %d\n", matrix.n, matrix.m);
		for (int i = 0; i < matrix.m; i++) {
			for (int j = 0; j < matrix.n; j++) {
				fprintf(out, "%d ", matrix.image[i][j][0]);
			}
			fprintf(out, ("\n"));
		}

	} else if (!strcmp(matrix.format, "P2") || !strcmp(matrix.format, "P5")) {
		fprintf(out, "P2\n");
		fprintf(out, "%d %d\n%d\n", matrix.n, matrix.m, matrix.range);
		for (int i = 0; i < matrix.m; i++) {
			for (int j = 0; j < matrix.n; j++) {
				fprintf(out, "%d ", matrix.image[i][j][0]);
			}
			fprintf(out, ("\n"));
		}

	} else {
		fprintf(out, "P3\n");
		fprintf(out, "%d %d\n%d\n", matrix.n, matrix.m, matrix.range);
		for (int i = 0; i < matrix.m; i++) {
			for (int j = 0; j < matrix.n; j++) {
				fprintf(out, "%d %d %d ", matrix.image[i][j][0],
						matrix.image[i][j][1], matrix.image[i][j][2]);
			}
			fprintf(out, "\n");
		}
	}

	fclose(out);
}

void SAVE_BIN(img matrix, char *filename)
{
	FILE *out = fopen(filename, "wt");

	/*similarly to the loading of binary files, as the values
	have to be stored as only one byte, a temporary variable
	is used to write into the file*/
	if (!strcmp(matrix.format, "P1") || !strcmp(matrix.format, "P4")) {
		fprintf(out, "P4\n");
		fprintf(out, "%d %d\n", matrix.n, matrix.m);
		for (int i = 0; i < matrix.m; i++) {
			for (int j = 0; j < matrix.n; j++) {
				unsigned char tmp;
				tmp = matrix.image[i][j][0];
				fwrite(&tmp, sizeof(unsigned char), 1, out);
			}

		}
	} else if (!strcmp(matrix.format, "P2") || !strcmp(matrix.format, "P5")) {
		fprintf(out, "P5\n");
		fprintf(out, "%d %d\n%d\n", matrix.n, matrix.m, matrix.range);
		for (int i = 0; i < matrix.m; i++) {
			for (int j = 0; j < matrix.n; j++) {
				unsigned char tmp;
				tmp = matrix.image[i][j][0];
				fwrite(&tmp, sizeof(unsigned char), 1, out);
			}

		}
	} else {
		fprintf(out, "P6\n");
		fprintf(out, "%d %d\n%d\n", matrix.n, matrix.m, matrix.range);
		for (int i = 0; i < matrix.m; i++) {
			for (int j = 0; j < matrix.n; j++) {
				unsigned char tmp;
				for (int q = 0; q < 3; q++) {
					tmp = matrix.image[i][j][q];
					fwrite(&tmp, sizeof(unsigned char), 1, out);
				}
			}

		}
	}

	fclose(out);
}

void SAVE(img matrix)
{
	char *ascii = NULL, *filename;
	filename = strtok(NULL, "\n ");
	ascii = strtok(NULL, "\n ");

	if (!matrix.image) {
		printf("No image loaded\n");
	} else {
		FILE *out = fopen(filename, "wt");

		if (!out) {
			printf("No image loaded\n");
			return;
		}

		fclose(out);

		/*depending on whether the last string was able to get
		initialized, the corresponding save function is called*/
		if (ascii) {
			SAVE_TXT(matrix, filename);
		} else {
			SAVE_BIN(matrix, filename);
		}
		printf("Saved %s\n", filename);
	}
}
