# Atomic Ring
Some code in C that allows one thread to pass items to another thread without locking

## Important notes
You should be aware of the following things:
* ~~This code has not been tested.~~
* This code has not been proved.
* This code has not even been particularly well reviewed.

You probably should not use it in production.

... but I'm going to, so if you want to too, I guess that's fine.  It's just also your problem if it doesn't work, eats your children or causes anything to explode in either a literal or figurative sense.

## Design
Atomic Ring is designed for *one* thread to send pointers to *one* other thread.  This allows you to implement a simple SPSC workqueue, where one thread passes items to another for further processing, provided you adhere to the following constraints:
* You must specify the number of items atomic ring will hold when you initialize it.  You cannot change this later.
* You may only ever add items to the atomic ring from one specific thread.
* You may only remove items from the atomic ring from one specific thread.
 * This means if you are passing items from one thread to another, you'll need another separate atomic ring to pass them back.
* If the atomic ring is full, the operation to add an item to it will fail and it is the calling thread's responsibility to go do something else.  (Probably wait.)
* If the atomic ring is empty, the operation to remove an item from it will fail and it is the calling thread's responsibility to go do something else.  (Probably wait.)
* Everything is implemented as void pointers.  Your threads should agree on what those pointers mean, because atomic ring sure won't tell you.  When you take them out, you'll need to cast them into their type correctly.

You may feel that this is quite limited.  This is by design.  Lock free programming is usually quite fiddly.  I decided the best approach for what I needed was to simply change the problem to remove most of the difficult parts.

On the upside, the following things are true:
* You may have many atomic rings and those rings can each be used to pass items between whatever pairs of threads make sense.
* Any thread may:
 * Read the number of items in the atomic ring
 * Read the total capacity of the atomic ring
* Atomic ring does not rely on anything fancier than the standardized C11 atomics.  This code is portable to every platform where C11 is implemented.

And the following things are also *hopefully* true:
* This code is correct.
* Atomic rings are lock-free. (Assuming lock-free support in the underlying atomics.)

This may also be true:
* Atomic rings are wait-free.

## Technical details
This code uses the atomic operations introduced in the C11 standard.  Which functionally means that without gcc-4.9 or later or clang 3.3 or later, you're going to have a bad time.  However, if you are having a bad time, do look in the compat/ directory, where I've placed a copy of a stdatomic.h header file from FreeBSD that invokes the atomics routines in earlier versions of the compilers.  It's a bad idea, but if you're having a bad time, maybe two bads make a good.

## Use
You can just drop this code into your project tree and use it.  The makefile shows one good way to compile it.  This code is public domain or unlicense licensed in jurisdictions that public domain does not exist, with the exception of the files in the compat/ directory, which are from different projects, are copyright of their respective owners and are licensed under the terms included at the top of each file.

## Documentation
This code provides the following functions:

### int aring_init(struct atomic_ring * aring, unsigned int size)
This function initializes the atomic ring pointed to by `aring`, with the capacity specified by `size`.  Size cannot exceed the maximum value able to be represented by an `unsigned int` on the platform.

This function is not thread safe and can only be used once, in one thread, to initialize one atomic ring.

This function returns:
* 0, on success.
* -1, if an error happened during the creation of the ring.  There are very few circumstances which could cause this.  The code in `aring_init` returns immediately on an error.  This means that on platforms that support errno it will likely be preserved and perror() can be called to determine what went wrong.

### int aring_give(struct atomic_ring * aring, void * item)
This function adds a pointer to `item` to the atomic ring specified by `aring`.  This function should only ever be called by a single thread (commonly called the producer thread) for each atomic ring.  Calling this function from any other thread is unsafe.

This function returns:
* 0, on success.
* -1, if the atomic ring is full and it cannot add an item.

### int aring_take(struct atomic_ring * aring, void ** item)
This function takes an item from the atomic ring pointed to by `aring` and puts the value into the address passed as `item`.  This function should only ever be called by a single thread (commonly called the consumer thread) for each atomic ring.  Calling this function from any other thread is unsafe.

This function returns:
* 0, on success.
* -1, on failure if the atomic ring is empty and it cannot remove an item.

### unsigned int aring_query(struct atomic_ring * aring)
This function can be used to determine how many items are currently in the atomic ring pointed to by `aring`.  It may be safely used from any thread, so long as the atomic ring has already been successfully initialized with `aring_init()`.

This function returns:
* The number of items that the atomic ring pointed to by aring contains.

### unsigned int aring_capacity(struct atomic_ring * aring)
This function can be used to determine the total capacity of the atomic ring pointed to by `aring`.  This number does not change until the atomic ring is destroyed.  It may be safely used from any thread, so long as the atomic ring has already been successfully initialized with `aring_init()`.

This function returns:
* The number of items that the atomic ring pointed to by aring can store in total.

### int aring_free(struct atomic_ring * aring)
This function frees the ring buffer for the atomic ring pointed to by `aring` and sets the capacity to zero.  Subsequent calls to atomic ring code using this atomic ring will produce bad results and may even crash.  This function should only be called before a program destroys the atomic ring.

This function returns:
* 0, on success.
* -1, if an error happened during the creation of the ring.  There are currently no errors that can cause this, but future versions of the code may return it.

## Examples
See the tests/ directory for short minimal code that uses atomic rings.

## Diagram
This is a whiteboard diagram for the atomic ring.  One thread in red, other thread in blue, shared state in brown.  Memory barriers in dashed lines with mb, implemented in the code using C11 `_release` `_acquire` semantics on accessing the items field.
![Atomic Ring Diagram](https://raw.github.com/djcapelis/atomic-ring/master/doc/diagram.jpg)

## Report Bugs
If you find bugs, I'd love to hear about them.  My website at http://capelis.dj has information on how to reach me.
