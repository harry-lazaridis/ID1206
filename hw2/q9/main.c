
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define REF_LEN 10000
#define LIMIT 0.7

typedef struct page {

	int page_id;

	int reference_bit;

	struct page *next;

	// @other auxiliary
	int total_ref;
} Node;

typedef struct {
	Node *head;
	Node *tail;
	int size;
} List;

// @create an active list
List active_list = {NULL, NULL, 0};

// @create an inactive list
List inactive_list = {NULL, NULL, 0};

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int n; 
int m;

Node *pages;
int ref_string[REF_LEN];
int player_done = 0;

static int list_remove(List *l, Node *p) {
	Node *prev = NULL;
	Node *cur = l->head;
	while (cur != NULL && cur != p) {
		prev = cur;
		cur = cur->next;
	}

	if (cur == NULL) { return 0; }

	if (prev == NULL) { 
		l->head = p->next; 
	} else { 
		prev->next = p->next; 
	}

	if (l->tail == p) { l->tail = prev; }

	p->next = NULL;
	l->size--;
	return 1;
}

static void list_push_back(List *l, Node *p) {
	p->next = NULL;
	if (l->tail == NULL) {
		l->head = p;
	} else {
		l->tail->next = p;
	}

	l->tail = p;
	l->size++;
}

static void *list_pop_front(List *l) {
	Node *p = l->head;
	if (p == NULL) {
		return NULL;
	}
	l->head = p->next;
	if (l->head == NULL) {
		l->tail = NULL;
	}
	p->next = NULL;
	l->size--;
	return p;
}

static void print_list(List *l) {
	for (Node *p = l->head; p != NULL; p = p->next) {
		printf("%d ", p->page_id);
	}
	printf("\n");
}

void *player_thread_func() {

// @Add code for player

	int limit = (int)(LIMIT * n);
	
	for (int i = 0; i < REF_LEN; i++) {
		Node *p = &pages[ref_string[i]];

		pthread_mutex_lock(&lock);

		p->reference_bit = 1;

		if(!list_remove(&active_list, p)) {
			list_remove(&inactive_list, p);
		}
		list_push_back(&active_list, p);

		while (active_list.size > limit) {
			Node *old = list_pop_front(&active_list);
			list_push_back(&inactive_list, old);
		}

		pthread_mutex_unlock(&lock);

		usleep(10);
	}

	pthread_mutex_lock(&lock);
	player_done = 1;
	pthread_mutex_unlock(&lock);

	pthread_exit(0);

}



void *checker_thread_func() {

// @Add code for checker
	int done = 0;

	while (!done) {
		usleep(m);

		pthread_mutex_lock(&lock);

		for (Node *p = active_list.head; p != NULL; p = p->next) {
			if (p->reference_bit == 1) {
				p->total_ref++;
				p->reference_bit = 0;
			}
		}

		done = player_done;

		pthread_mutex_unlock(&lock);
	}
	pthread_exit(0);

}

int main(int argc, char *argv[]) {

	n = atoi(argv[1]);
	m = atoi(argv[2]);

	pages = calloc(n, sizeof(Node));
	for (int i = 0; i < n; i++) {
		pages[i].page_id = i;
	}

	// @create a random reference string of N
	srand(time(NULL));
	for (int i = 0; i < REF_LEN; i++) {
		ref_string[i] = rand() % n;
	}


	/* Create two workers */

	pthread_t player;

	pthread_t checker;

	pthread_create(&player, NULL, player_thread_func, NULL);

	pthread_create(&checker, NULL, checker_thread_func, NULL);

	pthread_join(player, NULL);

	pthread_join(checker, NULL);


	printf("Page_Id, Total_Referenced\n");

	//@Print out the statistics of page references
	for (int i = 0; i < n; i++) {
		printf("%d, %d\n", pages[i].page_id, pages[i].total_ref);
	}
	printf("Pages in active list: ");
	//@Print out the list of pages in active list
	print_list(&active_list);

	printf("Pages in active list: ");
	//@Print out the list of pages in inactive list
	print_list(&inactive_list);
	/*free up resources properly */

	free(pages);
	pthread_mutex_destroy(&lock);
	return 0;
}
