#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>

#define HUGE_PAGE_SIZE (2UL * 1024 * 1024)

int main(int argc, char** argv) {
	clock_t start, end;

	int num_pages = atoi(argv[1]);
	int page_size = getpagesize();
	int use_huge = (argc >= 3 && strcmp(argv[2], "huge") == 0);

	size_t map_size = (size_t)page_size * num_pages;
	int flags = MAP_PRIVATE | MAP_ANONYMOUS;	

	if (use_huge) {
		flags |= MAP_HUGETLB;
		map_size = (map_size + HUGE_PAGE_SIZE - 1) & ~(HUGE_PAGE_SIZE - 1);
	}

	char *addr;

	start = clock();

	addr = (char*) mmap(
			NULL, 
			map_size,  
			PROT_READ | PROT_WRITE, //read/write?
		       	flags,	//flags used, anon or file, private or shared, huge pages. 
			-1, //What file should be mapped, -1 because we are not using a file
			0); //Where in the file we should start, 0 because we are not using a file

	char c = 'a';
	for (int i = 0; i < num_pages; i++) {
		addr[(size_t)i*page_size] = c;
		c++;
	}

	end = clock();

	printf("Elapsed: %ld clock ticks (%.6f s, CLOCKS_PER_SEC = %ld)\n",
			(long)(end - start), (double)(end - start) / CLOCKS_PER_SEC,
			(long)CLOCKS_PER_SEC);

	for (int i = 0; (i < num_pages && i<16); i++) {
		printf("%c ", addr[(size_t)i*page_size]);
	}	
	printf("\n");
	
	munmap(addr, map_size);
	
	return 0;
}
