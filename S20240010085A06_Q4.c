// QUESTION 4: B+-Tree Removal and Balancing (Refactored)
// NOTE: This is a foundational implementation and simplifies some
// recursive parent-finding and internal node rebalancing for clarity.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BPTREE_ORDER 4

typedef struct BPlusNode {
    bool isLeafNode;
    int keyValues[BPTREE_ORDER];
    struct BPlusNode *childLinks[BPTREE_ORDER + 1];
    int currentKeys;
    struct BPlusNode *parentNode;
    struct BPlusNode *nextLeaf;
} BPlusNode;

BPlusNode *bpTreeRoot = NULL;

BPlusNode *allocateBPlusNode(bool is_leaf) {
    BPlusNode *newNode = (BPlusNode *)malloc(sizeof(BPlusNode));
    newNode->isLeafNode = is_leaf;
    newNode->currentKeys = 0;
    newNode->parentNode = NULL;
    newNode->nextLeaf = NULL;
    for (int i = 0; i < BPTREE_ORDER + 1; i++) newNode->childLinks[i] = NULL;
    return newNode;
}

void handleInternalRemoval(int x, BPlusNode *node, BPlusNode *child); // Forward declaration

void removeKeyValue(int x) {
    if (bpTreeRoot == NULL) {
        printf("Cannot remove from an empty tree.\n");
        return;
    }

    BPlusNode *currentNode = bpTreeRoot;
    BPlusNode *parent = NULL;
    int leftSiblingIdx, rightSiblingIdx;

    while (!currentNode->isLeafNode) {
        for (int i = 0; i < currentNode->currentKeys; i++) {
            parent = currentNode;
            leftSiblingIdx = i - 1;
            rightSiblingIdx = i + 1;
            if (x < currentNode->keyValues[i]) {
                currentNode = currentNode->childLinks[i];
                break;
            }
            if (i == currentNode->currentKeys - 1) {
                leftSiblingIdx = i;
                rightSiblingIdx = i + 2;
                currentNode = currentNode->childLinks[i + 1];
                break;
            }
        }
    }

    bool keyFound = false;
    int pos;
    for (pos = 0; pos < currentNode->currentKeys; pos++) {
        if (currentNode->keyValues[pos] == x) {
            keyFound = true;
            break;
        }
    }

    if (!keyFound) {
        printf("Key %d was not found.\n", x);
        return;
    }

    for (int i = pos; i < currentNode->currentKeys - 1; i++) {
        currentNode->keyValues[i] = currentNode->keyValues[i + 1];
    }
    currentNode->currentKeys--;

    // Check for underflow
    int minKeys = (BPTREE_ORDER + 1) / 2;
    if (currentNode->currentKeys >= minKeys || currentNode == bpTreeRoot) {
        return;
    }

    // Attempt to borrow from a sibling
    if (leftSiblingIdx >= 0) {
        BPlusNode *leftNode = parent->childLinks[leftSiblingIdx];
        if (leftNode->currentKeys > minKeys) {
            // Borrow from left
            for (int i = currentNode->currentKeys; i > 0; i--) currentNode->keyValues[i] = currentNode->keyValues[i - 1];
            currentNode->keyValues[0] = leftNode->keyValues[leftNode->currentKeys - 1];
            leftNode->currentKeys--;
            currentNode->currentKeys++;
            parent->keyValues[leftSiblingIdx] = currentNode->keyValues[0];
            return;
        }
    }

    if (rightSiblingIdx <= parent->currentKeys) {
        BPlusNode *rightNode = parent->childLinks[rightSiblingIdx];
        if (rightNode->currentKeys > minKeys) {
             // Borrow from right
             currentNode->keyValues[currentNode->currentKeys] = rightNode->keyValues[0];
             currentNode->currentKeys++;
             for (int i = 0; i < rightNode->currentKeys - 1; i++) rightNode->keyValues[i] = rightNode->keyValues[i + 1];
             rightNode->currentKeys--;
             parent->keyValues[rightSiblingIdx - 1] = rightNode->keyValues[0];
             return;
        }
    }

    // Merge with a sibling if borrowing is not possible
    if (leftSiblingIdx >= 0) {
        BPlusNode *leftNode = parent->childLinks[leftSiblingIdx];
        for (int i = 0; i < currentNode->currentKeys; i++) {
            leftNode->keyValues[leftNode->currentKeys + i] = currentNode->keyValues[i];
        }
        leftNode->currentKeys += currentNode->currentKeys;
        leftNode->nextLeaf = currentNode->nextLeaf;
        handleInternalRemoval(parent->keyValues[leftSiblingIdx], parent, currentNode);
        free(currentNode);
    } else if (rightSiblingIdx <= parent->currentKeys) {
        BPlusNode *rightNode = parent->childLinks[rightSiblingIdx];
        for (int i = 0; i < rightNode->currentKeys; i++) {
            currentNode->keyValues[currentNode->currentKeys + i] = rightNode->keyValues[i];
        }
        currentNode->currentKeys += rightNode->currentKeys;
        currentNode->nextLeaf = rightNode->nextLeaf;
        handleInternalRemoval(parent->keyValues[rightSiblingIdx - 1], parent, rightNode);
        free(rightNode);
    }
}

void handleInternalRemoval(int x, BPlusNode *node, BPlusNode *child) {
    // A full implementation would recursively rebalance the parent nodes.
    // This is a placeholder to represent that step.
    printf("\n(Info) Internal node updated. Rebalancing might be needed higher up.\n");
}

// --- Simplified functions to build a tree for the demo ---
void basicInsertForDemo(int x) {
    if(bpTreeRoot == NULL) bpTreeRoot = allocateBPlusNode(true);
    BPlusNode* cursor = bpTreeRoot;
    while(!cursor->isLeafNode) cursor = cursor->childLinks[0];
    if(cursor->currentKeys < BPTREE_ORDER) {
        cursor->keyValues[cursor->currentKeys++] = x;
    }
}
void basicPrintForDemo() {
    if(!bpTreeRoot) return;
    BPlusNode* c = bpTreeRoot;
    while(!c->isLeafNode) c = c->childLinks[0];
    while(c != NULL) {
        printf("[");
        for(int i=0; i<c->currentKeys; i++) printf(" %d", c->keyValues[i]);
        printf(" ]-->");
        c = c->nextLeaf;
    }
    printf("NIL\n");
}

int main() {
    printf("B+ Tree Removal (Order %d)\n", BPTREE_ORDER);

    // Manually create a simple tree to demonstrate deletion and merging
    bpTreeRoot = allocateBPlusNode(false);
    BPlusNode* leaf1 = allocateBPlusNode(true);
    BPlusNode* leaf2 = allocateBPlusNode(true);

    leaf1->keyValues[0] = 10; leaf1->keyValues[1] = 20;
    leaf1->currentKeys = 2; leaf1->parentNode = bpTreeRoot;

    leaf2->keyValues[0] = 30;
    leaf2->currentKeys = 1; leaf2->parentNode = bpTreeRoot;

    leaf1->nextLeaf = leaf2;
    bpTreeRoot->keyValues[0] = 30;
    bpTreeRoot->currentKeys = 1;
    bpTreeRoot->childLinks[0] = leaf1;
    bpTreeRoot->childLinks[1] = leaf2;

    printf("Initial Tree State: ");
    basicPrintForDemo();
    printf("==========================================\n");

    int key_to_remove;
    printf("Enter key to remove (e.g., 30 to cause a merge): ");
    scanf("%d", &key_to_remove);

    removeKeyValue(key_to_remove);

    printf("\nTree after removing %d: ", key_to_remove);
    basicPrintForDemo();
    printf("==========================================\n");

    return 0;
}
