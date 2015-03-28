/******************************************************************
 * aring.h
 *
 * This is the header file for all atomic ring functions.
 *
 * Licensed under the included terms (unlicense)
 *
 * D J Capelis, 2015
 *****************************************************************/

#include<stdlib.h>
#include"aring.h"

/* Initializes atomic rings */
int aring_init(struct atomic_ring * aring, unsigned int size)
{
	atomic_init(&aring->items, 0);
	aring->rb = malloc(size * sizeof(void *));
	if(aring->rb == NULL)
		return -1;
	aring->size = size;
	aring->head = 0;
	aring->tail = 0;
	atomic_thread_fence(memory_order_release);
	return 0;
}

/* Places an item in the atomic ring */
int aring_give(struct atomic_ring * aring, void * item)
{
	unsigned int i;
	i = atomic_load_explicit(&aring->items, memory_order_acquire);
	if(i == aring->size)
		return -1;
	aring->head++;
	if(aring->head == aring->size)
		aring->head = 0;
	aring->rb[aring->head] = item;
	/* memory barrier below ensures writes above become visible and complete */
	atomic_fetch_add_explicit(&aring->items, 1, memory_order_release);
	return 0;
}

/* Removes an item from the atomic ring */
int aring_take(struct atomic_ring * aring, void ** item)
{
	unsigned int i;
	i = atomic_load_explicit(&aring->items, memory_order_acquire);
	if(i == 0)
		return -1;
	aring->tail++;
	if(aring->tail == aring->size)
		aring->tail = 0;
	*item = aring->rb[aring->tail];
	/* memory barrier below ensures writes above become visible and complete */
	atomic_fetch_sub_explicit(&aring->items, 1, memory_order_release);
	return 0;
}

/* Determines how many items are in the atomic ring */
unsigned int aring_query(struct atomic_ring * aring)
{
	return atomic_load_explicit(&aring->items, memory_order_relaxed);
}

/* Determines the total number of items that can be stored in a given atomic ring */
unsigned int aring_capacity(struct atomic_ring * aring)
{
	return aring->size;
}

/* Frees an atomic ring */
int aring_free(struct atomic_ring * aring)
{
	if(aring->rb)
		free(aring->rb);
	aring->rb = NULL;
	atomic_store_explicit(&aring->items, 0, memory_order_seq_cst);
	return 0;
}
