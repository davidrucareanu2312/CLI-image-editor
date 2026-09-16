#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "file_utils.h"
#include "tools.h"
#include "effects.h"
#include "utils.h"

int main(void)
{
	int read = 1;
	char line[100], *COMMAND;
	img matrix;
	matrix.image = NULL;

	while (read) {
		//lines are read untill one containing EXIT is found
		fgets(line, 100, stdin);

		/*the first part of the line, the command is separated,
		to be able to call the corresponding function*/
		COMMAND = strtok(line, "\n ");
		int com = 0;

		if (!strcmp(COMMAND, "LOAD")) {
			char *filename = strtok(NULL, "\n ");
			LOAD(matrix.format, &matrix.m,
				 &matrix.n, &matrix.range, &matrix.image,
				 &matrix.x1, &matrix.y1, &matrix.x2, &matrix.y2, filename);
			com = 1;
		}

		if (!strcmp(COMMAND, "SELECT")) {
			char *selected = strtok(NULL, "\n ");
			SELECT(matrix.image, matrix.m, matrix.n, selected, &matrix.x1,
				   &matrix.y1, &matrix.x2, &matrix.y2);
			com = 1;
		}

		if (!strcmp(COMMAND, "HISTOGRAM")) {
			HISTOGRAM(matrix.image, matrix.m, matrix.n, matrix.format);
			com = 1;
		}

		if (!strcmp(COMMAND, "EQUALIZE")) {
			EQUALIZE(&matrix.image, matrix.m, matrix.n, matrix.format);
			com = 1;
		}

		if (!strcmp(COMMAND, "ROTATE")) {
			ROTATE(&matrix.image, &matrix.m, &matrix.n, matrix.x1, matrix.y1,
				   &matrix.x2, &matrix.y2, matrix.format);
			com = 1;
		}

		if (!strcmp(COMMAND, "CROP")) {
			CROP(&matrix.image, &matrix.x1, &matrix.y1, &matrix.x2, &matrix.y2,
				 &matrix.m, &matrix.n, matrix.format);
			com = 1;

		}

		if (!strcmp(COMMAND, "APPLY")) {
			APPLY(&matrix.image, matrix.x1, matrix.y1, matrix.x2, matrix.y2,
				  matrix.m, matrix.n, matrix.range, matrix.format);
			com = 1;
		}

		if (!strcmp(COMMAND, "SAVE")) {
			SAVE(matrix);
			com = 1;
		}

		if (!strcmp(COMMAND, "EXIT")) {
			if (!matrix.image) {
				printf("No image loaded\n");
			}
			read = 0;
			com = 1;
		}

		if (com == 0) {
			printf("Invalid command\n");
		}
	}

	free_matrix(&matrix.image, matrix.m, matrix.n);
	return 0;
}
