/******************************************************************
 * aring.h
 *
 * This is the header file for all atomic ring functions.
 *
 * Licensed under the included terms (unlicense)
 *
 * D J Capelis, 2015
 *****************************************************************/

#ifndef ARING_H
#define ARING_H

#include<stdatomic.h>
/* 
 * If your system does not include <stdatomic.h>, you can try commenting 
 * that line out and uncommenting the line below which attempts to 
 * invoke atomics routines in compilers on systems without completed 
 * support for C11.
 */
/* #include "compat/stdatomic.h" */

struct atomic_ring;

int aring_init(struct atomic_ring * aring, unsigned int size);
int aring_give(struct atomic_ring * aring, void * item);
int aring_take(struct atomic_ring * aring, void ** item);
unsigned int aring_query(struct atomic_ring * aring);
unsigned int aring_capacity(struct atomic_ring * aring);
int aring_free(struct atomic_ring * aring);

struct atomic_ring
{
	atomic_uint items;
	unsigned int size;
	unsigned int head;
	unsigned int tail;
	void ** rb; /* ring buffer */
};

#endif /* ARING_H */
