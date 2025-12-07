#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int ORDER, MAX_KEYS;

typedef struct BPTreeNode {
    int *keys;
    struct BPTreeNode **children;
    int n;
    bool leaf;
    struct BPTreeNode* next;
} BPTreeNode;

BPTreeNode* createNode(bool leaf) {
    BPTreeNode* node = (BPTreeNode*)malloc(sizeof(BPTreeNode));
    node->keys = (int*)malloc(sizeof(int) * MAX_KEYS);
    node->children = (BPTreeNode**)malloc(sizeof(BPTreeNode*) * (ORDER));
    node->n = 0;
    node->leaf = leaf;
    node->next = NULL;
    return node;
}

void splitChild(BPTreeNode* parent, int i, BPTreeNode* y) {
    int mid = (MAX_KEYS+1)/2;
    BPTreeNode* z = createNode(y->leaf);
    z->n = y->n - mid;

    for (int j = 0; j < z->n; j++) z->keys[j] = y->keys[j+mid];
    if (!y->leaf) {
        for (int j = 0; j <= z->n; j++) z->children[j] = y->children[j+mid];
    } else {
        z->next = y->next;
        y->next = z;
    }

    y->n = mid;

    for (int j = parent->n; j >= i+1; j--) parent->children[j+1] = parent->children[j];
    parent->children[i+1] = z;

    for (int j = parent->n-1; j >= i; j--) parent->keys[j+1] = parent->keys[j];
    parent->keys[i] = y->keys[mid-1];
    parent->n++;
}

void printTree(BPTreeNode* root, int level) {
    if (!root) return;
    printf("Level %d : ", level);
    for (int i = 0; i < root->n; i++) printf("%d ", root->keys[i]);
    printf("\n");
    if (!root->leaf) {
        for (int i = 0; i <= root->n; i++) printTree(root->children[i], level+1);
    }
}


void insertNonFull(BPTreeNode* x, int k) {
    int i = x->n - 1;
    if (x->leaf) {
        while (i >= 0 && k < x->keys[i]) {
            x->keys[i+1] = x->keys[i];
            i--;
        }
        x->keys[i+1] = k;
        x->n++;
    } else {
        while (i >= 0 && k < x->keys[i]) i--;
        i++;
        if (x->children[i]->n == MAX_KEYS) {
            splitChild(x, i, x->children[i]);
            if (k > x->keys[i]) i++;
        }
        insertNonFull(x->children[i], k);
    }
}

void insert(BPTreeNode** rootRef, int k) {
    BPTreeNode* r = *rootRef;
    if (r->n == MAX_KEYS) {
        BPTreeNode* s = createNode(false);
        s->children[0] = r;
        splitChild(s, 0, r);
        int i = 0;
        if (s->keys[0] < k) i++;
        insertNonFull(s->children[i], k);
        *rootRef = s;
    } else {
        insertNonFull(r, k);
    }
}

int main() {
    printf("Enter order of B+ Tree: ");
    scanf("%d", &ORDER);
    MAX_KEYS = ORDER - 1;

    int n;
    printf("Enter number of keys: ");
    scanf("%d", &n);

    BPTreeNode* root = createNode(true);

    for (int i = 0; i < n; i++) {
        int key;
        printf("Enter key: ");
        scanf("%d", &key);
        insert(&root, key);
        printf("Tree after inserting %d:\n", key);
        printTree(root, 0);
    }

    return 0;
}

