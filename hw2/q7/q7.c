#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv) {
	int n = atoi(argv[1]);
	long page_size = getpagesize();
	char *buf = malloc(n * page_size);
	memset(buf, 0, n * page_size);

	printf("Page size: %ld bytes\n", page_size);
	printf("Allocated %d pages (%ld bytes)\n", n, n * page_size);

	free(buf);
	return 0;
}
