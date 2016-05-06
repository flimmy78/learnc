#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/unistd.h>

#define PRINT_LINE  printf("FILE: %s,  LINE: %d\n", __FILE__, __LINE__);
#define CYCLE_TIME  (5)
typedef struct msg {
    struct msg *prev;
    struct msg *next;
    int num;
} *pMsg;

pMsg head;
pMsg tail;
pthread_cond_t has_product = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
void printList();
void *consumer(void *p)
{
    struct msg *mp = NULL;
    for (;;) {
	pthread_mutex_lock(&lock);
	while (head == NULL && tail == NULL) {
	    printf("head is: %p, tail is: %p\n", head, tail);
	    pthread_cond_wait(&has_product, &lock);
	}
	//printList();
	mp = tail;
	//if tail point the first Node, tail->next is NULL, and tail->prev is also NULL!
	//So, in this condition, tail point to NULL after statement "tail = tail->prev;"
	//this happened before second node is produced, because consumer get the lock
	tail = tail->prev;
	if (tail) {//tail not point to first
	    tail->next = NULL;
	}
    else //tail point to first, so after consume node, List is EMPTY!
        head = NULL;//init List to NULL, so producer will add the next node to 
                    //List and arise tail to it, add consumer will wait until List is Not EMPTY.
	pthread_mutex_unlock(&lock);
	printf("Consume\t%d\n", mp->num);
	free(mp);		//PRINT_LINE
	sleep(rand() % CYCLE_TIME);
    }
}

void *producer(void *p)
{
    struct msg *mp;
    for (;;) {
    	mp = malloc(sizeof(struct msg));
    	mp->num = rand() % 1000 + 1;
    	printf("Produce\t%d\n", mp->num);
    	pthread_mutex_lock(&lock);
    	mp->prev = NULL;
    	mp->next = head;
    	if (tail == NULL) {	//if the first Node is consumed
    	    //before the second node is produced,
    	    //this statement arise tail to second node.
    	    tail = mp;
    	}
    	if (head)
    	    head->prev = mp;
    	head = mp;
    	//printList();
    	pthread_mutex_unlock(&lock);
    	pthread_cond_signal(&has_product);
    
    	sleep(rand() % CYCLE_TIME);
    }
}

int main(int argc, char *argv[])
{
    head = tail = NULL;
    pthread_t pid, cid;
    srand(time(NULL));
    pthread_create(&pid, NULL, producer, NULL);
    pthread_create(&cid, NULL, consumer, NULL);
    pthread_join(pid, NULL);
    pthread_join(cid, NULL);
    return 0;
}

void printList()
{
    pMsg mp=NULL;
    printf("------------List Start ----------\n");
    mp = head;
    while (mp) {
	printf
	    ("element: mp - %p, mp->prev - %p, mp->num - %d, mp->next - %p;\n",
	     mp, mp->prev, mp->num, mp->next);
	printf("head: %p, tail: %p\n", head, tail);
	mp = mp->next;
    }
    printf("------------List End ----------\n");
}
