#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Page {
    int id;         // The ID of the page.
//  int last_used;  // The time page last used.
//  int cnt;        // The number of page called.
} page_t;

typedef struct PageNode {
    page_t          *item; // content of PageNode
    struct PageNode *link; // link to next page
} page_node_t;

typedef struct LinkedPageQueue {
    int          size;
    page_node_t *front;
    page_node_t *rear;
} linked_page_queue_t;

int  page_init     (page_t*, int);
void page_terminate(page_t*);

page_t* linked_page_queue_dequeue  (      linked_page_queue_t*);
int     linked_page_queue_enqueue  (      linked_page_queue_t*, page_t*);
page_t* linked_page_queue_find     (const linked_page_queue_t*, int(*)(const page_t*));
int     linked_page_queue_init     (      linked_page_queue_t*);
bool    linked_page_queue_is_empty (const linked_page_queue_t*);
page_t* linked_page_queue_peek     (const linked_page_queue_t*);
void    linked_page_queue_terminate(      linked_page_queue_t*);

page_t** page_table_clone    (      page_t**);
int      page_table_find     (const page_t**, page_t*, size_t);
int      page_table_insert   (      page_t**, page_t*, size_t);
page_t*  page_table_pop      (      page_t**, int    , size_t);
void     page_table_print    (const page_t**, size_t);
void     page_table_terminate(      page_t**);

int main() {

    int                   table_size = 0;     // table size
    int                   page_num   = 0;     // used to get page number in while loop
    int                   page_idx   = -1;    // used to find pages in table
    page_t              **page_table = NULL;  // page table
    page_t              **tmp_pt     = NULL;  // temp page table for sorting
    linked_page_queue_t  *queue      = NULL;  // linked page queue
    
    // Get table number and allocate memory to page_table
    scanf("%d", &table_size);
    page_table = calloc(sizeof(page_t*), table_size);

    queue = malloc(sizeof(linked_page_queue_t));
    linked_page_queue_init(queue);

    while (~scanf("%d", &page_num)) {
        page_t *tmp_page  = NULL;
        page_t *popped_pg = NULL;

        tmp_page = malloc(sizeof(page_t));

        page_init(tmp_page, page_num);
        page_idx = page_table_find(
                (const page_t**) page_table, 
                tmp_page, 
                table_size
        );

        if (page_idx == -1) {
            // if page not found
            if (queue->size >= table_size) {
                // if table is full
                popped_pg = linked_page_queue_dequeue(queue);
                page_table_pop(page_table, popped_pg->id, table_size);
                page_terminate(popped_pg);
            }

            linked_page_queue_enqueue(queue, tmp_page);
            page_table_insert(page_table, tmp_page, table_size);
        }

        printf("%s\n", page_idx == -1 ? "Not found" : "Found");
        page_table_print((const page_t**) page_table, table_size);
    }

    return 0;
}

/**
 * Initializer of Page
 *
 * @param p  page_t* page to initialize
 * @param id int     page id
 *
 * @returns 0  - if success
 */
int page_init(page_t* p, int id) {
    p->id = id;

    return 0;
}

void page_terminate(page_t* p) {
    free(p);
}

/**
 * Initializer of LinkedPageQueue
 *
 * @param q linked_page_queue_t* queue to initialize
 *
 * @returns 0 - if success
 */
int linked_page_queue_init(linked_page_queue_t* q) {
    q->front = q->rear = NULL;
    q->size = 0;

    return 0;
}

/**
 * Enqueues the element to the rear of the queue.
 *
 * @param q linked_page_queue_t* queue to enqueue a page
 * @param p page_t*              page to enqueue
 *
 * @returns 0 - if success
 *          1 - if memory allocate failed
 */
int linked_page_queue_enqueue(linked_page_queue_t* q, page_t* p) {
    page_node_t *pn;

    if ((pn = malloc(sizeof(linked_page_queue_t))) == NULL) {
        return 1;
    }

    pn->item = p;
    pn->link = NULL;

    if (q->size == 0) {
        q->front = q->rear = pn;
    } else {
        q->rear->link = pn;
        q->rear = pn;
    }

    q->size++;

    return 0;
}

/**
 * @Nullable
 * Dequeues the front element from the queue
 *
 * @param q linked_page_queue_t* queue to enqueue a page
 * 
 * @returns page_t* - the page dequeued
 *          NULL    - when no elements in the queue
 */
page_t* linked_page_queue_dequeue(linked_page_queue_t* q) {

    page_t *p = NULL;

    p = q->front->item;
    q->front = q->front->link;

    return p;
}

/**
 * @Nullable
 * Gets the front element of the queue
 *
 * @param q linked_page_queue_t* queue to enqueue a page
 *
 * @returns page_t* - the page at the front
 *          NULL    - when no elements in the queue
 */
page_t* linked_page_queue_peek(const linked_page_queue_t* q) {
    return q->front->item;
}

/**
 * @Nullable
 * Finds the element in given condition in the queue
 *
 * @param q         linked_page_queue_t*  queue to find an element
 * @param predicate int(*)(const page_t*) predicate to elements
 *
 * @returns page_t* - the page with given condition 
 *          NULL    - when no page fit with given condition
 */
page_t* linked_page_queue_find(
        const linked_page_queue_t* q,
        int(*predicate)(const page_t*)
    ) {
    page_node_t *current_node = q->front;

    while (current_node != NULL) {
        if ((*predicate)(current_node->item)) {
            return current_node->item;
        }

        current_node = current_node->link;
    }

    return NULL;
}

/**
 * Returns whether queue is empty
 *
 * @param q         linked_page_queue_t*  queue to find an element
 *
 * @returns true  - if queue is empty
 *          false - if queue is not empty
 */
bool linked_page_queue_is_empty(const linked_page_queue_t* q) {
    return q->front == q->rear;
}

void linked_page_queue_terminate(linked_page_queue_t* q) {
    free(q);
}

/**
 * Returns if page table has page.
 *
 * @param pt   page_t** page table
 * @param p    page_t*  page to find
 * @param size size_t   size of table
 *
 * @returns -1            - if page not in table
 *          index of page - if page is in table
 */
int page_table_find(const page_t** pt, page_t* p, size_t size) {
    int i = 0;

    for (; i < size; i++) {
        if (pt[i] != NULL && pt[i]->id == p->id) {
            return i;
        } 
    }

    return -1;
}

/**
 * Inserts page into table and return its index
 *
 * @param pt   page_t** page table
 * @param p    page_t*  page to insert 
 * @param size size_t   size of table
 *
 * @returns -1            - if no available space
 *          index of page - if page inserted
 */
int page_table_insert(page_t** pt, page_t* p, size_t size) {
    int i = 0;

    for (; i < size; i++) {
        if (pt[i] == NULL) {
            pt[i] = p;
            return i;
        }
    }
    return -1;
}

/**
 * Pops page from table and return itself 
 *
 * @param pt   page_t** page table
 * @param id   page_t*  page id to pop
 * @param size size_t   size of table
 *
 * @returns NULL - if no match
 *          page - if page popped
 */
page_t* page_table_pop(page_t** pt, int id, size_t size) {
    int i = 0;
    page_t *tmp = NULL;

    for (; i < size; i++) {
        if (pt[i]->id == id) {
            tmp = pt[i];
            pt[i] = NULL;
            return tmp;
        }
    }
    return NULL;
}

page_t** page_table_clone(page_t** pt) {
    page_t **new = malloc(sizeof(pt));
    int i = 0;

    for (; i < sizeof(pt) / sizeof(page_t*); i++) {
        new[i] = pt[i];
    }
    
    return new;
}

void page_table_terminate(page_t** pt) {
    free(pt);
}

void page_table_print(const page_t** pt, size_t size) {
    int i = 0;

    for (; i < size; i++) {
        if (pt[i] == NULL) {
            printf("%d |\n", i);
        } else {
            printf("%d | %d\n", i, pt[i]->id);
        }
    }
    printf("\n");
}
