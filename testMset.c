#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Mset.h"
#include "MsetStructs.h"

// These functions are deliberately not static to make testing more
// convenient.

void testMsetInsert(void);
void testMsetInsertMany(void);
void testMsetDelete(void);
void testMsetDeleteMany(void);
void testMsetSize(void);
void testMsetTotalCount(void);
void testMsetGetCount(void);
void testMsetPrint(void);
void checkMsetPrint(Mset s, char *expectedPrint);
void checkFileContents(FILE *file, char *expectedContents);

void testMsetUnion(void);
void testMsetIntersection(void);
void testMsetIncluded(void);
void testMsetEquals(void);
void testMsetMostCommon(void);

void testBalance1(void);
void testBalance2(void);
bool isHeightBalanced(struct node *t);
bool doIsHeightBalanced(struct node *t, int *height);

void testMsetCursor1(void);
void testMsetCursor2(void);

int main(int argc, char *argv[]) {
	testMsetInsert();
	testMsetInsertMany();
	testMsetDelete();
	testMsetDeleteMany();
	testMsetSize();
	testMsetTotalCount();
	testMsetGetCount();
	testMsetPrint();

	testMsetUnion();
	testMsetIntersection();
	// testMsetIncluded();
	testMsetEquals();
	testMsetMostCommon();

	testBalance1();
	testBalance2();

	testMsetCursor1();
	testMsetCursor2();
}

void testMsetInsert(void) {
	// NOTE: MsetInsert can't be tested on its own unless we directly
	//       access the internal representation of the ADT

	Mset s = MsetNew();

	MsetInsert(s, 4);
	MsetInsert(s, 7);
	MsetInsert(s, 1);
	MsetInsert(s, 3);
	MsetInsert(s, 7);
	MsetInsert(s, 3);
	MsetInsert(s, 7);

	assert(MsetSize(s) == 4);
	assert(MsetTotalCount(s) == 7);

	MsetFree(s);
}

void testMsetInsertMany(void) {
	// NOTE: The note in testMsetInsert also applies to MsetInsertMany

	Mset s = MsetNew();

	MsetInsertMany(s, 4, 2); // insert two 4's
	MsetInsertMany(s, 7, 3); // insert three 7's
	MsetInsertMany(s, 1, 5); // insert five 1's
	MsetInsertMany(s, 3, 1); // insert one 3

	assert(MsetSize(s) == 4);
	assert(MsetTotalCount(s) == 11);

	MsetFree(s);
}

void testMsetDelete(void) {
	// NOTE: MsetDelete can't be tested without either MsetInsert or
	//       MsetInsertMany

	Mset s = MsetNew();

	MsetInsert(s, 4);
	MsetInsert(s, 7);
	MsetInsert(s, 1);
	MsetInsert(s, 3);
	MsetInsert(s, 7);
	MsetInsert(s, 3);
	MsetInsert(s, 7);

	MsetDelete(s, 1);
	assert(MsetSize(s) == 3);
	assert(MsetTotalCount(s) == 6);

	MsetDelete(s, 7);
	assert(MsetSize(s) == 3);
	assert(MsetTotalCount(s) == 5);

	MsetDelete(s, 1);
	MsetDelete(s, 6);
	assert(MsetSize(s) == 3);
	assert(MsetTotalCount(s) == 5);

	MsetFree(s);
}

void testMsetDeleteMany(void) {
	Mset s = MsetNew();

	MsetInsertMany(s, 4, 2);
	MsetInsertMany(s, 7, 3);
	MsetInsertMany(s, 1, 5);
	MsetInsertMany(s, 3, 1);

	MsetDeleteMany(s, 1, 2);
	assert(MsetSize(s) == 4);
	assert(MsetTotalCount(s) == 9);

	MsetDeleteMany(s, 3, 1);
	assert(MsetSize(s) == 3);
	assert(MsetTotalCount(s) == 8);

	MsetDeleteMany(s, 1, 4);
	assert(MsetSize(s) == 2);
	assert(MsetTotalCount(s) == 5);

	MsetDeleteMany(s, 6, 3);
	assert(MsetSize(s) == 2);
	assert(MsetTotalCount(s) == 5);

	MsetFree(s);
}

void testMsetSize(void) {
	Mset s = MsetNew();
	assert(MsetSize(s) == 0);

	MsetInsert(s, 4);
	MsetInsert(s, 7);
	MsetInsert(s, 1);
	MsetInsert(s, 3);
	assert(MsetSize(s) == 4);

	MsetInsert(s, 8);
	MsetInsert(s, 1);
	assert(MsetSize(s) == 5);

	MsetInsertMany(s, 6, 5);
	MsetInsertMany(s, 4, 2);
	assert(MsetSize(s) == 6);

	MsetFree(s);
}

void testMsetTotalCount(void) {
	Mset s = MsetNew();

	MsetInsert(s, 4);
	MsetInsert(s, 7);
	MsetInsert(s, 1);
	MsetInsert(s, 3);
	assert(MsetTotalCount(s) == 4);

	MsetInsert(s, 8);
	MsetInsert(s, 1);
	assert(MsetTotalCount(s) == 6);

	MsetInsertMany(s, 6, 5);
	MsetInsertMany(s, 4, 2);
	assert(MsetTotalCount(s) == 13);

	MsetFree(s);
}

void testMsetGetCount(void) {
	Mset s = MsetNew();

	MsetInsert(s, 4);
	MsetInsert(s, 7);
	MsetInsert(s, 1);
	MsetInsert(s, 3);
	assert(MsetGetCount(s, 1) == 1);
	assert(MsetGetCount(s, 3) == 1);
	assert(MsetGetCount(s, 4) == 1);
	assert(MsetGetCount(s, 7) == 1);
	assert(MsetGetCount(s, 8) == 0);

	MsetInsert(s, 8);
	MsetInsert(s, 1);
	assert(MsetGetCount(s, 1) == 2);
	assert(MsetGetCount(s, 8) == 1);

	MsetInsertMany(s, 6, 5);
	MsetInsertMany(s, 4, 2);
	assert(MsetGetCount(s, 6) == 5);
	assert(MsetGetCount(s, 4) == 3);

	MsetFree(s);
}

void testMsetPrint(void) {
	Mset s = MsetNew();

	MsetInsert(s, 4);
	MsetInsert(s, 7);
	MsetInsert(s, 1);
	MsetInsert(s, 3);
	MsetInsert(s, 7);
	MsetInsert(s, 3);
	MsetInsert(s, 7);

	checkMsetPrint(s, "{(1, 1), (3, 2), (4, 1), (7, 3)}");

	MsetFree(s);
}

void checkMsetPrint(Mset s, char *expectedPrint) {
	FILE *out = tmpfile();
	MsetPrint(s, out);
	checkFileContents(out, expectedPrint);
	fclose(out);
}

/**
 * This function checks if the contents of a file matches the expected
 * contents. If the contents do not match, the program will exit with an
 * assertion error.
 */
void checkFileContents(FILE *file, char *expectedContents) {
	fflush(file);
	fseek(file, 0, SEEK_SET);
	char *line = NULL;
	size_t n = 0;
	getline(&line, &n, file);
	if (strcmp(line, expectedContents) != 0) {
		printf("Test failed for MsetPrint: expected \"%s\", "
		       "saw \"%s\"\n", expectedContents, line);

		assert(strcmp(line, expectedContents) == 0);
	}
	free(line);
}

////////////////////////////////////////////////////////////////////////

void testMsetUnion(void) {
	Mset a = MsetNew();
	MsetInsertMany(a, 4, 2);
	MsetInsertMany(a, 7, 3);
	MsetInsertMany(a, 1, 5);
	MsetInsertMany(a, 3, 1);

	Mset b = MsetNew();
	MsetInsertMany(b, 4, 4);
	MsetInsertMany(b, 7, 1);
	MsetInsertMany(b, 3, 2);
	MsetInsertMany(b, 8, 3);

	Mset c = MsetUnion(a, b);
	assert(MsetGetCount(c, 1) == 5);
	assert(MsetGetCount(c, 3) == 2);
	assert(MsetGetCount(c, 4) == 4);
	assert(MsetGetCount(c, 7) == 3);
	assert(MsetGetCount(c, 8) == 3);
	assert(MsetSize(c) == 5);
	assert(MsetTotalCount(c) == 17);

	MsetFree(a);
	MsetFree(b);
	MsetFree(c);
}

void testMsetIntersection(void) {
	Mset a = MsetNew();
	MsetInsertMany(a, 4, 2);
	MsetInsertMany(a, 7, 3);
	MsetInsertMany(a, 1, 5);
	MsetInsertMany(a, 3, 1);

	Mset b = MsetNew();
	MsetInsertMany(b, 4, 4);
	MsetInsertMany(b, 7, 1);
	MsetInsertMany(b, 3, 2);
	MsetInsertMany(b, 8, 3);

	Mset c = MsetIntersection(a, b);
	assert(MsetGetCount(c, 1) == 0);
	assert(MsetGetCount(c, 3) == 1);
	assert(MsetGetCount(c, 4) == 2);
	assert(MsetGetCount(c, 7) == 1);
	assert(MsetGetCount(c, 8) == 0);
	assert(MsetSize(c) == 3);
	assert(MsetTotalCount(c) == 4);

	MsetFree(a);
	MsetFree(b);
	MsetFree(c);
}

 void testMsetIncluded(void) {
 	Mset a = MsetNew();
 	MsetInsertMany(a, 4, 2);
 	MsetInsertMany(a, 7, 3);
 	MsetInsertMany(a, 1, 5);

 	Mset b = MsetNew();
 	MsetInsertMany(b, 4, 5);
 	MsetInsertMany(b, 7, 3);
 	MsetInsertMany(b, 1, 6);
 	MsetInsertMany(b, 3, 1);

 	Mset c = MsetNew();
 	MsetInsertMany(c, 4, 5);
 	MsetInsertMany(c, 7, 3);
 	MsetInsertMany(c, 1, 4);
 	MsetInsertMany(c, 3, 1);

 	assert(MsetIncluded(a, b));
 	assert(!MsetIncluded(a, c));

 	MsetFree(a);
 	MsetFree(b);
 	MsetFree(c);
 }

void testMsetEquals(void) {
	Mset a = MsetNew();
	MsetInsertMany(a, 4, 2);
	MsetInsertMany(a, 7, 3);
	MsetInsertMany(a, 1, 5);
	MsetInsertMany(a, 3, 1);

	Mset b = MsetNew();
	MsetInsertMany(b, 3, 1);
	MsetInsertMany(b, 1, 5);
	MsetInsertMany(b, 7, 3);
	MsetInsertMany(b, 4, 2);

	Mset c = MsetNew();
	MsetInsertMany(c, 4, 2);
	MsetInsertMany(c, 7, 3);
	MsetInsertMany(c, 1, 5);
	MsetInsertMany(c, 3, 4);

	assert(MsetEquals(a, b));
	assert(!MsetEquals(a, c));

	MsetFree(a);
	MsetFree(b);
	MsetFree(c);
}

void testMsetMostCommon(void) {
	Mset s = MsetNew();

	MsetInsertMany(s, 4, 2);
	MsetInsertMany(s, 7, 3);
	MsetInsertMany(s, 1, 5);
	MsetInsertMany(s, 3, 1);
	MsetInsertMany(s, 8, 3);

	int count = 0;
	struct item items[10];

	count = MsetMostCommon(s, 1, items);

	assert(count == 1);
	assert(items[0].elem == 1);
	assert(items[0].count == 5);

	count = MsetMostCommon(s, 2, items);
	assert(count == 2);
	assert(items[0].elem  == 1);
	assert(items[0].count == 5);
	assert(items[1].elem  == 7);
	assert(items[1].count == 3);

	count = MsetMostCommon(s, 3, items);
	assert(count == 3);
	assert(items[0].elem  == 1);
	assert(items[0].count == 5);
	assert(items[1].elem  == 7);
	assert(items[1].count == 3);
	assert(items[2].elem  == 8);
	assert(items[2].count == 3);

	count = MsetMostCommon(s, 8, items);
	assert(count == 5);
	assert(items[0].elem  == 1);
	assert(items[0].count == 5);
	assert(items[1].elem  == 7);
	assert(items[1].count == 3);
	assert(items[2].elem  == 8);
	assert(items[2].count == 3);
	assert(items[3].elem  == 4);
	assert(items[3].count == 2);
	assert(items[4].elem  == 3);
	assert(items[4].count == 1);

	MsetFree(s);
}

////////////////////////////////////////////////////////////////////////

void testBalance1(void) {
	Mset s = MsetNew();

	MsetInsert(s, 8);
	MsetInsert(s, 5);
	MsetInsert(s, 2);

	// The tree should have been rebalanced after inserting 2
	// NOTE: Normally, a user should not have access to the concrete
	//       representation of an ADT, but since we have #included
	//       MsetStructs.h, we have access for testing purposes.
	
	//printf("%d", s->tree->height);
	assert(isHeightBalanced(s->tree));

	MsetFree(s);
}

void testBalance2(void) {
	Mset s = MsetNew();

	MsetInsert(s, 4);
	MsetInsert(s, 2);
	MsetInsert(s, 7);
	MsetInsert(s, 1);
	MsetDelete(s, 7);

	// The tree should have been rebalanced after deleting 7
	assert(isHeightBalanced(s->tree));

	MsetFree(s);
}

bool isHeightBalanced(struct node *t) {
	int height = -1;
	return doIsHeightBalanced(t, &height);
}

bool doIsHeightBalanced(struct node *t, int *height) {
	if (t == NULL) {
		*height = -1;
		return true;
	}

	int lHeight = -1;
	int rHeight = -1;
	if (doIsHeightBalanced(t->left, &lHeight)
			&& doIsHeightBalanced(t->right, &rHeight)
			&& abs(lHeight - rHeight) <= 1) {
		*height = (lHeight > rHeight ? lHeight : rHeight) + 1;
		return true;
	} else {
		return false;
	}
}

////////////////////////////////////////////////////////////////////////

void testMsetCursor1(void) {
	Mset s = MsetNew();

	MsetInsertMany(s, 4, 2);
	MsetInsertMany(s, 7, 3);
	MsetInsertMany(s, 1, 5);
	MsetInsertMany(s, 3, 1);

	MsetCursor cur = MsetCursorNew(s);

	struct item item;

	// start  1  3  4  7  end
	//   ^
	item = MsetCursorGet(cur);
	assert(item.elem == UNDEFINED);
	assert(item.count == 0);

	assert(MsetCursorNext(cur));
	// start  1  3  4  7  end
	//        ^
	item = MsetCursorGet(cur);
	assert(item.elem == 1);
	assert(item.count == 5);

	assert(MsetCursorNext(cur));
	// start  1  3  4  7  end
	//           ^
	item = MsetCursorGet(cur);
	assert(item.elem == 3);
	assert(item.count == 1);

	assert(MsetCursorNext(cur));
	// start  1  3  4  7  end
	//              ^
	item = MsetCursorGet(cur);
	assert(item.elem == 4);
	assert(item.count == 2);

	assert(MsetCursorNext(cur));
	// start  1  3  4  7  end
	//                 ^
	item = MsetCursorGet(cur);
	assert(item.elem == 7);
	assert(item.count == 3);

	assert(!MsetCursorNext(cur));
	// start  1  3  4  7  end
	//                     ^
	item = MsetCursorGet(cur);
	assert(item.elem == UNDEFINED);
	assert(item.count == 0);

	assert(MsetCursorPrev(cur));
	// start  1  3  4  7  end
	//                 ^
	item = MsetCursorGet(cur);
	assert(item.elem == 7);
	assert(item.count == 3);

	assert(MsetCursorPrev(cur));
	// start  1  3  4  7  end
	//              ^
	item = MsetCursorGet(cur);
	assert(item.elem == 4);
	assert(item.count == 2);

	assert(MsetCursorPrev(cur));
	// start  1  3  4  7  end
	//           ^
	item = MsetCursorGet(cur);
	assert(item.elem == 3);
	assert(item.count == 1);

	assert(MsetCursorPrev(cur));
	// start  1  3  4  7  end
	//        ^
	item = MsetCursorGet(cur);
	assert(item.elem == 1);
	assert(item.count == 5);

	assert(!MsetCursorPrev(cur));
	// start  1  3  4  7  end
	//   ^
	item = MsetCursorGet(cur);
	assert(item.elem == UNDEFINED);
	assert(item.count == 0);

	MsetCursorFree(cur);
	MsetFree(s);
}

void testMsetCursor2(void) {
	Mset s = MsetNew();

	MsetInsertMany(s, 4, 2);
	MsetInsertMany(s, 7, 3);
	MsetInsertMany(s, 1, 5);
	MsetInsertMany(s, 6, 1);

	MsetCursor cur = MsetCursorNew(s);

	struct item item;

	// start  1  4  6  7  end
	//   ^
	item = MsetCursorGet(cur);
	assert(item.elem == UNDEFINED);
	assert(item.count == 0);

	assert(MsetCursorNext(cur));
	// start  1  4  6  7  end
	//        ^
	item = MsetCursorGet(cur);
	assert(item.elem == 1);
	assert(item.count == 5);

	MsetInsertMany(s, 2, 4);
	// start  1  2  4  6  7  end
	//        ^

	assert(MsetCursorNext(cur));
	// start  1  2  4  6  7  end
	//           ^
	item = MsetCursorGet(cur);
	assert(item.elem == 2);
	assert(item.count == 4);

	assert(MsetCursorNext(cur));
	// start  1  2  4  6  7  end
	//              ^
	item = MsetCursorGet(cur);
	assert(item.elem == 4);
	assert(item.count == 2);

	MsetDelete(s, 6);
	// start  1  2  4  7  end
	//              ^

	assert(MsetCursorNext(cur));
	// start  1  2  4  7  end
	//                 ^
	item = MsetCursorGet(cur);
	assert(item.elem == 7);
	assert(item.count == 3);

	MsetCursorFree(cur);
	MsetFree(s);
}
