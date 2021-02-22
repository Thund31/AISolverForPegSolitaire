#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#include "ai.h"
#include "utils.h"
#include "hashtable.h"
#include "stack.h"
#include "linkedList.h"

#define NUM_DIRECTIONS 4

listNode_t *listHead;

void copy_state(state_t* dst, state_t* src){
	
	//Copy field
	memcpy( dst->field, src->field, SIZE*SIZE*sizeof(int8_t) );

	dst->cursor = src->cursor;
	dst->selected = src->selected;
}

/**
 * Saves the path up to the node as the best solution found so far
*/
void save_solution( node_t* solution_node ){
	node_t* n = solution_node;
	while( n->parent != NULL ){
		copy_state( &(solution[n->depth]), &(n->state) );
		solution_moves[n->depth-1] = n->move;

		n = n->parent;
	}
	solution_size = solution_node->depth;
}


node_t* create_init_node( state_t* init_state ){
	node_t * new_n = (node_t *) malloc(sizeof(node_t));
	new_n->parent = NULL;	
	new_n->depth = 0;
	copy_state(&(new_n->state), init_state);
	return new_n;
}

/**
 * Apply an action to node n and return a new node resulting from executing the action
*/
node_t* applyAction(node_t* n, position_s* selected_peg, move_t action ){

    node_t* newNode = (node_t*)malloc(sizeof(node_t));
	assert(newNode);
	
	// point to parent
	newNode->parent = n;

	// copy state and change cursor
	copy_state(&(newNode->state), &(n->state));
	newNode->state.cursor.x = selected_peg->x;
	newNode->state.cursor.y = selected_peg->y;

	// update depth and action
	newNode->depth = n->depth + 1;
	newNode->move = action;

	// perform move
    execute_move_t( &(newNode->state), selected_peg, action );
	
	return newNode;
}

/**
 * Find a solution path as per algorithm description in the handout
 */

void find_solution( state_t* init_state  ){

	HashTable table;

	// Choose initial capacity of PRIME NUMBER 
	// Specify the size of the keys and values you want to store once 
	ht_setup(&table, sizeof(int8_t) * SIZE * SIZE, sizeof(int8_t) * SIZE * SIZE, 16769023);

	// Initialize Stack
	initialize_stack();

	// initialise linked list
	newList(listHead);

	// Algorithm 1, lines 2-4
	node_t* n = create_init_node( init_state );
	stack_push(n);
	int remainingPegs = num_pegs(&(n->state));

	// create position variable
	position_s position;

	// Algorithm 1, lines 5-29, while loop 
	while (!is_stack_empty(n)) {
		n = stack_top();
		stack_pop(); // line 6
		listHead = listInsert(listHead, n); // add n to linked list to free after using

		expanded_nodes++; // line 7

		// if this is a better solution than current saved
		if (num_pegs(&(n->state)) < remainingPegs) { // line 8
			save_solution(n); // line 9
			remainingPegs = num_pegs(&(n->state)); // line 10
		}

		// Algorithm 1, line 12-25, for loop
		for (int y = 0; y < SIZE; y++) {
			for (int x = 0; x < SIZE; x++) {
				for (int direction = left; direction <= down; direction++) {
					// create position variable
					position.x = x;
					position.y = y;

					// if a is a legal action for node n
					if (can_apply(&(n->state), &position, direction)) { // line 13
						node_t *newNode = applyAction(n, &position, direction); // line 14
						generated_nodes++; // line 15

						// if the game is won
						if (won(&(newNode->state))) { // line 16
							save_solution(newNode); // line 17
							remainingPegs = num_pegs(&(newNode->state)); // line 18

							free(newNode);
							ht_destroy(&table);
							free_stack();
							return; // line 19
						}

						// check if already in hash table
						int seenFirstTime = true; // (default true) 
						for (int i = 0; i < NUM_DIRECTIONS; i++) {
							// if found a match
							if (ht_contains(&table, newNode->state.field) == HT_FOUND) {
								free(newNode);
								seenFirstTime = false;
								break;
							}
							rotateBoard(&(newNode->state));
						}

						// if the board is seen for the first time, insert into hashtable and stack
						if (seenFirstTime) { // line 21
							stack_push(newNode); // line 22
							ht_insert(&table, newNode->state.field, newNode->state.field);
						}
					}
				}
			}
		}

		// check if exceeded the budget
		if (expanded_nodes >= budget) { // line 26 - 27

			// free all other dynamic memory
			ht_destroy(&table);
			free_stack();
			return;
		}
	}
}

void free_memory() {
	freeList(listHead);
}