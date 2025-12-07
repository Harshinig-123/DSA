#include <stdio.h>
#include <stdlib.h>

typedef struct BTreeNode {
    int *keys;
    struct BTreeNode **child;
    int n;
    int leaf;
} BTreeNode;

BTreeNode* createNode(int maxKeys, int leaf) {
    BTreeNode* node = malloc(sizeof(BTreeNode));
    node->keys = malloc(maxKeys * sizeof(int));
    node->child = malloc((maxKeys + 1) * sizeof(BTreeNode*));
    node->n = 0;
    node->leaf = leaf;
    return node;
}

void splitChild(BTreeNode* x, int i, int maxKeys) {
    int t = (maxKeys + 1) / 2;
    BTreeNode* y = x->child[i];
    BTreeNode* z = createNode(maxKeys, y->leaf);
    z->n = y->n - t;

    for (int j = 0; j < z->n; j++)
        z->keys[j] = y->keys[j + t];

    if (!y->leaf) {
        for (int j = 0; j <= z->n; j++)
            z->child[j] = y->child[j + t];
    }

    y->n = t - 1;

    for (int j = x->n; j >= i + 1; j--)
        x->child[j + 1] = x->child[j];
    x->child[i + 1] = z;


    for (int j = x->n - 1; j >= i; j--)
        x->keys[j + 1] = x->keys[j];
    x->keys[i] = y->keys[t - 1];
    x->n++;
}

void insertNonFull(BTreeNode* x, int k, int maxKeys) {
    int i = x->n - 1;
    if (x->leaf) {
        while (i >= 0 && k < x->keys[i]) {
            x->keys[i + 1] = x->keys[i];
            i--;
        }
        x->keys[i + 1] = k;
        x->n++;
    } else {
        while (i >= 0 && k < x->keys[i]) i--;
        i++;
        if (x->child[i]->n == maxKeys)
        {
            splitChild(x, i, maxKeys);
            if (k > x->keys[i]) i++;
        }
        insertNonFull(x->child[i], k, maxKeys);
    }
}

BTreeNode* insert(BTreeNode* root, int k, int maxKeys) {
    if (root->n == maxKeys) {
        BTreeNode* s = createNode(maxKeys, 0);
        s->child[0] = root;
        splitChild(s, 0, maxKeys);
        int i = 0;
        if (k > s->keys[0]) i++;
        insertNonFull(s->child[i], k, maxKeys);
        return s;
    } else {
        insertNonFull(root, k, maxKeys);
        return root;
    }
}

void printTree(BTreeNode* root, int level) {
    if (!root) return;
    printf("Level %d : ", level);
    for (int i = 0; i < root->n; i++)
        printf("%d ", root->keys[i]);
    printf("\n");
    if (!root->leaf) {
        for (int i = 0; i <= root->n; i++)
            printTree(root->child[i], level + 1);
    }
}

int main() {
    int order, n, k;
    printf("Enter order (max keys per node + 1): ");
    scanf("%d", &order);
    int maxKeys = order - 1;
    BTreeNode* root = createNode(maxKeys, 1);

    printf("Enter number of keys: ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        printf("Enter key: ");
        scanf("%d", &k);
        root = insert(root, k, maxKeys);
        printf("Tree after inserting %d:\n", k);
        printTree(root, 0);
        printf("---------------------\n");
    }
    return 0;
}
