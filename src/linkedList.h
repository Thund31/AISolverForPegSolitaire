/**
 * This file contains headers to functions that create, use and destroy a linked list
 * @author Siyang Qiu
 * @version 1.0
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

// linked list structs
typedef struct node listNode_t;

struct node {
    void *data;
    listNode_t *next;
};

// list functions
void *listInsert(listNode_t *node, void *d);
void freeList(listNode_t *node);
void newList(listNode_t* node);