CC=gcc
CFLAGS=-Wall -Wextra -std=c99

TARGETS=image_editor

build: $(TARGETS)

image_editor: main.c
	$(CC) $(CFLAGS) main.c -o image_editor -lm

clean:
	rm -f $(TARGETS)

