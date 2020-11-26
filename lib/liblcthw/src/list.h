#ifndef lcthw_List_h
#define lcthw_List_h

/* START For Snoopy - redefine function names, to avoid symbol name collisions */
#define List_clear           snoopy_List_clear
#define List_clear_destroy   snoopy_List_clear_destroy
#define List_create          snoopy_List_create
#define List_destroy         snoopy_List_destroy
#define List_pop             snoopy_List_pop
#define List_push            snoopy_List_push
#define List_remove          snoopy_List_remove
#define List_shift           snoopy_List_shift
#define List_unshift         snoopy_List_unshift
/* END For Snoopy */

#include <stdlib.h>

struct ListNode;

typedef struct ListNode {
    struct ListNode *next;
    struct ListNode *prev;
    void *value;
} ListNode;

typedef struct List {
    int count;
    ListNode *first;
    ListNode *last;
} List;

List *List_create();
void List_destroy(List *list);
void List_clear(List *list);
void List_clear_destroy(List *list);

#define List_count(A) ((A)->count)
#define List_first(A) ((A)->first != NULL ? (A)->first->value : NULL)
#define List_last(A) ((A)->last != NULL ? (A)->last->value : NULL)

void List_push(List *list, void *value);
void *List_pop(List *list);

void List_shift(List *list, void *value);
void *List_unshift(List *list);

void *List_remove(List *list, ListNode *node);

#define LIST_FOREACH(L, S, M, V) ListNode *_node = NULL;\
    ListNode *V = NULL;\
    for(V = _node = L->S; _node != NULL; V = _node = _node->M)

#endif
