/**
 * This file contains basic functions to create, use and destroy a linked list
 * @author Siyang Qiu
 * @version 1.0
 */

#include "linkedList.h"

// add node to the back of the list
void *listInsert(listNode_t *node, void *d) {

    listNode_t *new = (listNode_t *)malloc(sizeof(listNode_t));
    assert(new);

    // initialise the node
    new->data = d;
    new->next = node;

    return new;
}

// free all memory associated with a list
void freeList(listNode_t *node) {

    listNode_t **firstNode = &node;
    listNode_t *nodeToFree;

    while (node) {
        nodeToFree = node;
        node = node->next;

        free(nodeToFree->data);
        free(nodeToFree);
    }

    *firstNode = NULL;
}

void newList(listNode_t* node) {
    node = NULL;
}