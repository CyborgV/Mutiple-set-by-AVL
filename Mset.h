#ifndef MSET_H
#define MSET_H

#include <limits.h>
#include <stdbool.h>

#define UNDEFINED INT_MIN

typedef struct mset *Mset;

// Used by MsetMostCommon and MsetCursorGet
struct item {
	int elem;
	int count;
};

////////////////////////////////////////////////////////////////////////
// Basic Operations

/**
 * Creates a new empty multiset.
 */
Mset MsetNew(void);

/**
 * Frees all memory allocated to the multiset.
 */
void MsetFree(Mset s);

/**
 * Inserts one of an item into the multiset. Does nothing if the item is
 * equal to UNDEFINED.
 */
void MsetInsert(Mset s, int item);

/**
 * Inserts the given amount of an item into the multiset. Does nothing
 * if the item is equal to UNDEFINED or the given amount is 0 or less.
 */
void MsetInsertMany(Mset s, int item, int amount);

/**
 * Deletes one of an item from the multiset.
 */
void MsetDelete(Mset s, int item);

/**
 * Deletes the given amount of an item from the multiset.
 */
void MsetDeleteMany(Mset s, int item, int amount);

/**
 * Returns the number of distinct elements in the multiset.
 */
int MsetSize(Mset s);

/**
 * Returns the sum of counts of all elements in the multiset.
 */
int MsetTotalCount(Mset s);

/**
 * Returns the count of an item in the multiset, or 0 if it doesn't
 * occur in the multiset.
 */
int MsetGetCount(Mset s, int item);

/**
 * Prints the multiset to a file.
 * The elements of the multiset should be printed in ascending order
 * inside a pair of curly braces, with elements separated by a comma
 * and space. Each element should be printed inside a pair of
 * parentheses with its count, separated by a comma and space.
 */
void MsetPrint(Mset s, FILE *file);

////////////////////////////////////////////////////////////////////////
// Advanced Operations

/**
 * Returns a new multiset representing the union of the two given
 * multisets.
 */
Mset MsetUnion(Mset s1, Mset s2);

/**
 * Returns a new multiset representing the intersection of the two
 * given multisets.
 */
Mset MsetIntersection(Mset s1, Mset s2);

/**
 * Returns true if the multiset s1 is included in the multiset s2, and
 * false otherwise.
 */
bool MsetIncluded(Mset s1, Mset s2);

/**
 * Returns true if the two given multisets are equal, and false
 * otherwise.
 */
bool MsetEquals(Mset s1, Mset s2);

/**
 * Stores the k most common elements in the multiset into the given
 * items array in decreasing order of count and returns the number of
 * elements stored. Elements with the same count should be stored in
 * increasing order. Assumes that the items array has size k.
 */
int MsetMostCommon(Mset s, int k, struct item items[]);

////////////////////////////////////////////////////////////////////////
// Cursor Operations

typedef struct cursor *MsetCursor;

/**
 * Creates a new cursor positioned at the start of the multiset.
 * (see spec for explanation of start and end)
 */
MsetCursor MsetCursorNew(Mset s);

/**
 * Frees all memory allocated to the given cursor.
 */
void MsetCursorFree(MsetCursor cur);

/**
 * Returns the element at the cursor's position and its count, or
 * {UNDEFINED, 0} if the cursor is positioned at the start or end of
 * the multiset.
 */
struct item MsetCursorGet(MsetCursor cur);

/**
 * Moves the cursor to the next greatest element, or to the end of the
 * multiset if there is no next greatest element. Does not move the
 * cursor if it is already at the end. Returns false if the cursor is at
 * the end after this operation, and true otherwise.
 */
bool MsetCursorNext(MsetCursor cur);

/**
 * Moves the cursor to the next smallest element, or to the start of the
 * multiset if there is no next smallest element. Does not move the
 * cursor if it is already at the start. Returns false if the cursor is
 * at the start after this operation, and true otherwise.
 */
bool MsetCursorPrev(MsetCursor cur);

////////////////////////////////////////////////////////////////////////

#endif

