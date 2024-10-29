#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Mset.h"
#include "MsetStructs.h"

#define MAX(a,b) ((a) > (b) ? (a) : (b))

typedef struct node *Node;

void freeNode(Node *n);

bool bstInsert(Node *tree, int item, int amount);

int bstDelete(Node *tree, int item, int amount);

void Zero(Node *tree, int item);

void delNode(Node *tree);

Node findMax(Node t);

Node findMin(Node t);

int height(Node node);

int getCount(Node n, int item);

void ascOrder(Node node, FILE *file, bool *isFirst);

void insertAll(Node tree, Mset result);

bool isIncluded(Node node, Mset s2);

bool areNodesEqual(Node n1, Node n2);

bool checkNodesEqual(Node node, Mset otherSet);

void findIntersection(Node tree, Mset otherSet, Mset result);

int max(int a, int b);

int min(int a, int b);

int getBalance(Node N);

Node leftRotate(Node t);

Node rightRotate(Node t);

void balanceNode(Node *tree);

void collectItems(Node node, struct item items[], int *index);

int compareItems(const void *a, const void *b);

////////////////////////////////////////////////////////////////////////
// Basic Operations

/**
 * Creates a new empty multiset.
 */
Mset MsetNew(void) {
	Mset newMset = malloc(sizeof(struct mset));
	newMset->tree = NULL;
	newMset->size = 0;
    newMset->totalCount = 0;
	return newMset;
}

/**
 * Frees all memory allocated to the multiset.
 */
void MsetFree(Mset s) {
	if (s == NULL) {
        return;
    }

    freeNode(&(s->tree));

    free(s);
}

void freeNode(Node *n) {
	if (*n == NULL) {
		return;
	}
	freeNode(&((*n)->left));
	freeNode(&((*n)->right));
	free(*n);
    *n = NULL;
}

/**
 * Inserts one of an item into the multiset. Does nothing if the item is
 * equal to UNDEFINED.
 */
void MsetInsert(Mset s, int item) {
    if (item != UNDEFINED) {
        int amount = 1;
        bool isNew = bstInsert(&(s->tree), item, amount);
        if (isNew) {
            s->size += 1;
        }
        s->totalCount += amount;
    }
}

/**
 * Inserts the given amount of an item into the multiset. Does nothing
 * if the item is equal to UNDEFINED or the given amount is 0 or less.
 */
void MsetInsertMany(Mset s, int item, int amount) {
    if (item != UNDEFINED && amount > 0) {
        bool isNew = bstInsert(&(s->tree), item, amount);
        if (isNew) {
            s->size += 1;
        }
        s->totalCount += amount;
    }
}

/**
 * Used by MsetInsert & MsetInsertMany
 * Inserts one or a given amount of an item into the node tree. Does nothing
 * if the item is equal to UNDEFINED or the given amount is 0 or less.
 */
bool bstInsert(Node *tree, int item, int amount) {
	bool isNew = false;
    if (*tree == NULL) {
        Node newNode = malloc(sizeof(struct node));
        newNode->elem = item;
		newNode->count = amount;
        newNode->left = NULL;
		newNode->right = NULL;
        newNode->height = 1;
		*tree = newNode;
		return true;
    }

    if (item < (*tree)->elem) {
        isNew = bstInsert(&((*tree)->left), item, amount);
    } else if (item > (*tree)->elem) {
        isNew = bstInsert(&((*tree)->right), item, amount);
    } else {
		(*tree)->count += amount;
		return false;
	}

    (*tree)->height = 1 + max(height((*tree)->left), height((*tree)->right));
    
    balanceNode(tree);

	return isNew;
}

/**
 * Deletes one of an item from the multiset.
 */
void MsetDelete(Mset s, int item) {
    int amount = 1;
    int currentCount = MsetGetCount(s, item);  
    int deletedCount = bstDelete(&(s->tree), item, amount);

    s->totalCount -= deletedCount;
    
    Zero(&(s->tree), item);

    if (currentCount == deletedCount && deletedCount > 0) {
        s->size -= 1;
    }
}


/**
 * Deletes the given amount of an item from the multiset.
 */
void MsetDeleteMany(Mset s, int item, int amount) {
    int currentCount = MsetGetCount(s, item); 
    int deletedCount = bstDelete(&(s->tree), item, amount);

    s->totalCount -= deletedCount;

    Zero(&(s->tree), item);

    if (currentCount == deletedCount && deletedCount > 0) {
        s->size -= 1;
    }
}

/**
 * Used by MsetDelete & MsetDeleteMany
 * Returns the number of distinct elements in the multiset.
 */
int bstDelete(Node *tree, int item, int amount) {
    if (*tree == NULL) {
        return 0;
    }
    int deletedCount = 0;

    if (item < (*tree)->elem) {
        deletedCount = bstDelete(&((*tree)->left), item, amount);
    } else if (item > (*tree)->elem) {
        deletedCount = bstDelete(&((*tree)->right), item, amount);
    } else {
        if (amount >= (*tree)->count) {
            deletedCount = (*tree)->count;
            delNode(tree);
        } else {
            (*tree)->count -= amount;
            deletedCount = amount;
        }
    }

    if (*tree == NULL) {
        return deletedCount;
    }

    void balanceNode(Node *tree);

    return deletedCount;
}

void Zero(Node *tree, int item) {
	if (*tree == NULL) {
        return;
    }

    if (item < (*tree)->elem) {
        Zero(&((*tree)->left), item);
    } else if (item > (*tree)->elem) {
        Zero(&((*tree)->right), item);
    } else {
		if ((*tree)->count == 0) {
			delNode(tree);
		}
	}

    if (*tree == NULL)
        return;

    balanceNode(tree);
}

void delNode(Node *tree) {
	if ((*tree)->left == NULL && (*tree)->right == NULL) {
        free(*tree);
        *tree = NULL;
	} else if ((*tree)->left == NULL && (*tree)->right != NULL) {
        Node temp = (*tree)->right;
        free(*tree);
        *tree = temp;
    } else if ((*tree)->right == NULL && (*tree)->left != NULL) {
        Node temp = (*tree)->left;
        free(*tree);
        *tree = temp;
	} else {
		if (height((*tree)->left) > height((*tree)->right)) {
            Node maxNode = findMax((*tree)->left);
            (*tree)->elem = maxNode->elem;
            (*tree)->count = maxNode->count;
            bstDelete(&((*tree)->left), maxNode->elem, maxNode->count);
        } else {
            Node minNode = findMin((*tree)->right);
            (*tree)->elem = minNode->elem;
            (*tree)->count = minNode->count;
            bstDelete(&((*tree)->right), minNode->elem, minNode->count);
        }
	}

    if (*tree == NULL)
        return;

    balanceNode(tree);
}

Node findMax(Node t) {
    if (t == NULL) {
        return NULL;
    }
    if (t->right == NULL) {
        return t;
    }
    return findMax(t->right);
}

Node findMin(Node t) {
    if (t == NULL) {
        return NULL;
    }
    if (t->left == NULL) {
        return t;
    }
    return findMin(t->left);
}


int height(Node node) {
    if (node == NULL) {
        return 0;
    }
    return 1 + MAX((height(node->left)), height((node->right)));
}

int MsetSize(Mset s) {
	return s->size;
}

/**
 * Returns the sum of counts of all elements in the multiset.
 */
int MsetTotalCount(Mset s) {
	return s->totalCount;
}

/**
 * Returns the count of an item in the multiset, or 0 if it doesn't
 * occur in the multiset.
 */
int MsetGetCount(Mset s, int item) {
    if (s == NULL) {
        fprintf(stderr, "Error: Mset is NULL in MsetGetCount\n");
        return 0;
    }
	return getCount(s->tree, item);
}

int getCount(Node n, int item) {
    if (n == NULL) {
        return 0;
    }
    if (item < n->elem) {
        return getCount(n->left, item);
    } else if (item > n->elem) {
        return getCount(n->right, item);
    } else {
        return n->count;
    }
}

/**
 * Prints the multiset to a file.
 * The elements of the multiset should be printed in ascending order
 * inside a pair of curly braces, with elements separated by a comma
 * and space. Each element should be printed inside a pair of
 * parentheses with its count, separated by a comma and space.
 */
void MsetPrint(Mset s, FILE *file) {
    fprintf(file, "{");

    bool isFirst = true;
    ascOrder(s->tree, file, &isFirst);

    fprintf(file, "}");
}

void ascOrder(Node node, FILE *file, bool *isFirst) {
    if (node == NULL) {
        return;
    }

    ascOrder(node->left, file, isFirst);

    if (!(*isFirst)) {
        fprintf(file, ", ");
    } else {
        *isFirst = false;
    }

    fprintf(file, "(%d, %d)", node->elem, node->count);

    ascOrder(node->right, file, isFirst);
}


////////////////////////////////////////////////////////////////////////
// Advanced Operations

/**
 * Returns a new multiset representing the union of the two given
 * multisets.
 */
Mset MsetUnion(Mset s1, Mset s2) {
    if (s1 == NULL && s2 == NULL) {
        return NULL;
    }

    Mset result = MsetNew(); 

    insertAll(s1->tree, result);
    insertAll(s2->tree, result);

    return result;
}

void insertAll(Node tree, Mset result) {
    if (tree == NULL) {
        return;
    }

    insertAll(tree->left, result);
    
    int currentCount = MsetGetCount(result, tree->elem);
    if (currentCount < tree->count) {
        MsetInsertMany(result, tree->elem, tree->count - currentCount);
    }

    insertAll(tree->right, result);
}


/**
 * Returns a new multiset representing the intersection of the two
 * given multisets.
 */
Mset MsetIntersection(Mset s1, Mset s2) {
    if (s1 == NULL || s2 == NULL) {
        return NULL;
    }

    Mset result = MsetNew();

    findIntersection(s1->tree, s2, result);

    return result;
}

void findIntersection(Node tree, Mset otherSet, Mset result) {
    if (tree == NULL) {
        return;
    }

    findIntersection(tree->left, otherSet, result);

    int otherCount = MsetGetCount(otherSet, tree->elem);
    if (otherCount > 0) {
        int minCount = (tree->count < otherCount) ? tree->count : otherCount;
        MsetInsertMany(result, tree->elem, minCount);
    }

    findIntersection(tree->right, otherSet, result);
}


/**
 * Returns true if the multiset s1 is included in the multiset s2, and
 * false otherwise.
 */
/* bool MsetIncluded(Mset s1, Mset s2) {
    if (s1 == NULL || s1->totalCount == 0) {
        return true;
    }
    if (s2 == NULL) {
        return false;
    }

    Mset unionSet = MsetUnion(s1, s2);
    bool result = MsetEquals(unionSet, s2);
    MsetFree(unionSet);

    return result;
} */

bool MsetIncluded(Mset s1, Mset s2) {
    if (s1 == NULL || s1->totalCount == 0) {
        return true;
    }
    if (s2 == NULL || s2->totalCount == 0) {
        return false;
    }
    return isIncluded(s1->tree, s2);
}

bool isIncluded(Node node, Mset s2) {
    if (node == NULL) {
        return true;
    }

    int countInS2 = MsetGetCount(s2, node->elem);
    if (countInS2 < node->count) {
        return false;
    }

    if (!isIncluded(node->left, s2)) {
        return false;
    }
    if (!isIncluded(node->right, s2)) {
        return false;
    }

    return true;
}


/**
 * Returns true if the two given multisets are equal, and false
 * otherwise.
 */
/* the two functions below only suitable for treap.

bool MsetEquals(Mset s1, Mset s2) {
    if (s1 == NULL && s2 == NULL) {
        return true;
    }

    if (s1 == NULL || s2 == NULL) {
        return false;
    }

    if (s1->size != s2->size || s1->totalCount != s2->totalCount) {
        return false;
    }

    return areNodesEqual(s1->tree, s2->tree);
}

bool areNodesEqual(Node n1, Node n2) {

    if (n1 == NULL && n2 == NULL) {
        return true;
    }

    if (n1 == NULL || n2 == NULL) {
        return false;
    }

    if (n1->elem != n2->elem || n1->count != n2->count) {
        return false;
    }

    return areNodesEqual(n1->left, n2->left) && areNodesEqual(n1->right, n2->right);
} */

/* the two functions below can be simplified

bool MsetEquals(Mset s1, Mset s2) {
    if (s1 == NULL && s2 == NULL) {
        return true;
    }

    if (s1 == NULL || s2 == NULL) {
        return false;
    }

    if (s1->size != s2->size || s1->totalCount != s2->totalCount) {
        return false;
    }

    if (!checkNodesEqual(s1->tree, s2)) {
        return false;
    }

    if (!checkNodesEqual(s2->tree, s1)) {
        return false;
    }

    return true;
}

bool checkNodesEqual(Node node, Mset otherSet) {
    if (node == NULL) {
        return true;
    }

    int countInOther = MsetGetCount(otherSet, node->elem);
    if (countInOther != node->count) {
        return false;
    }

    if (!checkNodesEqual(node->left, otherSet)) {
        return false;
    }
    if (!checkNodesEqual(node->right, otherSet)) {
        return false;
    }

    return true;
}
 */

 bool MsetEquals(Mset s1, Mset s2) {
    if (s1 == NULL && s2 == NULL) {
        return true;
    }
    if (s1 == NULL || s2 == NULL) {
        return false;
    }
    if (s1->size != s2->size || s1->totalCount != s2->totalCount) {
        return false;
    }
    // Check mutual inclusion
    return MsetIncluded(s1, s2) && MsetIncluded(s2, s1);
}


/**
 * Stores the k most common elements in the multiset into the given
 * items array in decreasing order of count and returns the number of
 * elements stored. Elements with the same count should be stored in
 * increasing order. Assumes that the items array has size k.
 */
int MsetMostCommon(Mset s, int k, struct item items[]);
void collectItems(Node node, struct item items[], int *index);
int compareItems(const void *a, const void *b);

// Implementation of MsetMostCommon
int MsetMostCommon(Mset s, int k, struct item items[]) {
    if (k <= 0 || s == NULL || s->size == 0) {
        return 0;
    }

    int n = s->size;  
    struct item *allItems = malloc(n * sizeof(struct item));
    if (allItems == NULL) {
        return 0;
    }

    int index = 0;
    collectItems(s->tree, allItems, &index);

    // Sort the items based on count (decreasing) and elem (increasing)
    qsort(allItems, n, sizeof(struct item), compareItems);

    int numStored = (k < n) ? k : n;

    for (int i = 0; i < numStored; i++) {
        items[i] = allItems[i];
    }

    free(allItems);
    return numStored;
}

// Helper function to collect items from the tree
void collectItems(Node node, struct item items[], int *index) {
    if (node == NULL) {
        return;
    }
    collectItems(node->left, items, index);
    items[*index].elem = node->elem;
    items[*index].count = node->count;
    (*index)++;
    collectItems(node->right, items, index);
}

// Comparison function for qsort
int compareItems(const void *a, const void *b) {
    const struct item *item1 = (const struct item *)a;
    const struct item *item2 = (const struct item *)b;

    if (item1->count != item2->count) {
        return item2->count - item1->count;
    } else {
        return item1->elem - item2->elem;
    }
}

//balance bst

int max(int a, int b) {
    return (a > b) ? a : b;
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

int getBalance(Node N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

Node leftRotate(Node x) {
    Node y = x->right;
    Node z = y->left;

    y->left = x;
    x->right = z;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

Node rightRotate(Node y) {
    Node x = y->left;
    Node z = x->right;

    x->right = y;
    y->left = z;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

void balanceNode(Node *tree) {
    if (*tree == NULL)
        return;

    (*tree)->height = 1 + max(height((*tree)->left), height((*tree)->right));

    int balance = getBalance(*tree);

    // Left Left Case
    if (balance > 1 && getBalance((*tree)->left) >= 0) {
        *tree = rightRotate(*tree);
    }
    // Left Right Case
    else if (balance > 1 && getBalance((*tree)->left) < 0) {
        (*tree)->left = leftRotate((*tree)->left);
        *tree = rightRotate(*tree);
    }
    // Right Right Case
    else if (balance < -1 && getBalance((*tree)->right) <= 0) {
        *tree = leftRotate(*tree);
    }
    // Right Left Case
    else if (balance < -1 && getBalance((*tree)->right) > 0) {
        (*tree)->right = rightRotate((*tree)->right);
        *tree = leftRotate(*tree);
    }

    (*tree)->height = 1 + max(height((*tree)->left), height((*tree)->right));
}


////////////////////////////////////////////////////////////////////////
// Cursor Operations

/**
 * Creates a new cursor positioned at the start of the multiset.
 * (see spec for explanation of start and end)
 */
MsetCursor MsetCursorNew(Mset s) {
	// TODO
	return NULL;
}

/**
 * Frees all memory allocated to the given cursor.
 */
void MsetCursorFree(MsetCursor cur) {
	// TODO
}

/**
 * Returns the element at the cursor's position and its count, or
 * {UNDEFINED, 0} if the cursor is positioned at the start or end of
 * the multiset.
 */
struct item MsetCursorGet(MsetCursor cur) {
	// TODO
	return (struct item){UNDEFINED, 0};
}

/**
 * Moves the cursor to the next greatest element, or to the end of the
 * multiset if there is no next greatest element. Does not move the
 * cursor if it is already at the end. Returns false if the cursor is at
 * the end after this operation, and true otherwise.
 */
bool MsetCursorNext(MsetCursor cur) {
	// TODO
	return false;
}

/**
 * Moves the cursor to the next smallest element, or to the start of the
 * multiset if there is no next smallest element. Does not move the
 * cursor if it is already at the start. Returns false if the cursor is
 * at the start after this operation, and true otherwise.
 */
bool MsetCursorPrev(MsetCursor cur) {
	// TODO
	return false;
}

////////////////////////////////////////////////////////////////////////

