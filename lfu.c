#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Page {
    int id;         // The ID of the page.
    int last_used;  // The time page last used.
    int cnt;        // The number of page called.
} page_t;

int  page_init     (page_t*, int);
void page_terminate(page_t*);

page_t** page_table_clone    (      page_t**);
int      page_table_find     (const page_t**, page_t*, size_t);
int      page_table_insert   (      page_t**, page_t*, size_t);
page_t*  page_table_pop      (      page_t**, int    , size_t);
void     page_table_print    (const page_t**, size_t);
int      page_table_size     (      page_t**, size_t);
void     page_table_terminate(      page_t**);

int main() {

    int      _time      = 0;     // current time
    int      table_size = 0;     // table size
    int      page_num   = 0;     // used to get page number in while loop
    int      page_idx   = -1;    // used to find pages in table
    page_t **page_table = NULL;  // page table
    page_t **tmp_pt     = NULL;  // temp page table for sorting
    
    // Get table number and allocate memory to page_table
    scanf("%d", &table_size);
    page_table = calloc(sizeof(page_t*), table_size);

    while (~scanf("%d", &page_num)) {
        page_t *tmp_page  = NULL;
        page_t *popped_pg = NULL;

        tmp_page = malloc(sizeof(page_t));

        page_init(tmp_page, page_num);
        tmp_page->last_used = _time;

        page_idx = page_table_find(
                (const page_t**) page_table, 
                tmp_page, 
                table_size
        );

        if (page_idx == -1) {
            // if page not found
            int i = 0, min = 0;
            if (page_table_size(page_table, table_size) >= table_size) {
                for (; i < table_size; i++) {
                    min = page_table[i]->cnt < page_table[min]->cnt ?
                        i : 
                        page_table[i]->cnt == page_table[min]->cnt ?
                            page_table[i]->last_used < page_table[min]->last_used ?
                            i : min :
                        min;
                }
                // if table is full
                page_table_pop(page_table, page_table[min]->id, table_size);
                page_terminate(popped_pg);
            }

            page_table_insert(page_table, tmp_page, table_size);
        } else {
            page_table[page_idx]->last_used = _time;
        }

        printf("%s\n", page_idx == -1 ? "Not found" : "Found");
        page_table_print((const page_t**) page_table, table_size);

        _time++;
    }

    return 0;
}

int min(int a, int b) {
    return a < b ? a : b;
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

int page_table_size(page_t** pt, size_t size) {
    int i = 0, cnt = 0;

    for(; i < size; i++) {
        if (pt[i] != NULL) {
            cnt++;
        }
    }

    return cnt;
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
