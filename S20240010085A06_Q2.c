#include <stdio.h>
#include <stdlib.h>

typedef struct BTreeNode {
    int *keys;
    struct BTreeNode **child;
    int n;
    int leaf;
} BTreeNode;

BTreeNode* createNode(int t, int leaf) {
    BTreeNode* node = malloc(sizeof(BTreeNode));
    node->keys = malloc((2*t-1)*sizeof(int));
    node->child = malloc((2*t)*sizeof(BTreeNode*));
    node->n = 0;
    node->leaf = leaf;
    return node;
}

void splitChild(BTreeNode* x, int i, int t) {
    BTreeNode* y = x->child[i];
    BTreeNode* z = createNode(t,y->leaf);
    z->n = t-1;
    for(int j=0;j<t-1;j++) z->keys[j] = y->keys[j+t];
    if(!y->leaf) {
        for(int j=0;j<t;j++) z->child[j] = y->child[j+t];
    }
    y->n = t-1;
    for(int j=x->n;j>=i+1;j--) x->child[j+1]=x->child[j];
    x->child[i+1]=z;
    for(int j=x->n-1;j>=i;j--) x->keys[j+1]=x->keys[j];
    x->keys[i]=y->keys[t-1];
    x->n++;
}

void insertNonFull(BTreeNode* x,int k,int t) {
    int i=x->n-1;
    if(x->leaf) {
        while(i>=0 && k<x->keys[i]) {x->keys[i+1]=x->keys[i]; i--;}
        x->keys[i+1]=k;
        x->n++;
    } else {
        while(i>=0 && k<x->keys[i]) i--;
        i++;
        if(x->child[i]->n==2*t-1) {
            splitChild(x,i,t);
            if(k>x->keys[i]) i++;
        }
        insertNonFull(x->child[i],k,t);
    }
}

BTreeNode* insert(BTreeNode* root,int k,int t) {
    if(root->n==2*t-1) {
        BTreeNode* s=createNode(t,0);
        s->child[0]=root;
        splitChild(s,0,t);
        int i=0;
        if(k>s->keys[0]) i++;
        insertNonFull(s->child[i],k,t);
        return s;
    } else {
        insertNonFull(root,k,t);
        return root;
    }
}

int getPred(BTreeNode* x,int idx) {
    BTreeNode* cur=x->child[idx];
    while(!cur->leaf) cur=cur->child[cur->n];
    return cur->keys[cur->n-1];
}
int getSucc(BTreeNode* x,int idx) {
    BTreeNode* cur=x->child[idx+1];
    while(!cur->leaf) cur=cur->child[0];
    return cur->keys[0];
}

void merge(BTreeNode* x,int idx,int t) {
    BTreeNode* c1=x->child[idx];
    BTreeNode* c2=x->child[idx+1];
    c1->keys[t-1]=x->keys[idx];
    for(int i=0;i<c2->n;i++) c1->keys[i+t]=c2->keys[i];
    if(!c1->leaf) for(int i=0;i<=c2->n;i++) c1->child[i+t]=c2->child[i];
    for(int i=idx+1;i<x->n;i++) x->keys[i-1]=x->keys[i];
    for(int i=idx+2;i<=x->n;i++) x->child[i-1]=x->child[i];
    c1->n+=c2->n+1;
    x->n--;
    free(c2->keys); free(c2->child); free(c2);
}

void fill(BTreeNode* x,int idx,int t);

void borrowFromPrev(BTreeNode* x,int idx) {
    BTreeNode* child=x->child[idx];
    BTreeNode* sibling=x->child[idx-1];
    for(int i=child->n-1;i>=0;i--) child->keys[i+1]=child->keys[i];
    if(!child->leaf) for(int i=child->n;i>=0;i--) child->child[i+1]=child->child[i];
    child->keys[0]=x->keys[idx-1];
    if(!child->leaf) child->child[0]=sibling->child[sibling->n];
    x->keys[idx-1]=sibling->keys[sibling->n-1];
    child->n++; sibling->n--;
}

void borrowFromNext(BTreeNode* x,int idx) {
    BTreeNode* child=x->child[idx];
    BTreeNode* sibling=x->child[idx+1];
    child->keys[child->n]=x->keys[idx];
    if(!child->leaf) child->child[child->n+1]=sibling->child[0];
    x->keys[idx]=sibling->keys[0];
    for(int i=1;i<sibling->n;i++) sibling->keys[i-1]=sibling->keys[i];
    if(!sibling->leaf) for(int i=1;i<=sibling->n;i++) sibling->child[i-1]=sibling->child[i];
    child->n++; sibling->n--;
}

void removeFromNode(BTreeNode* x,int idx,int t) {
    if(x->leaf) {
        for(int i=idx+1;i<x->n;i++) x->keys[i-1]=x->keys[i];
        x->n--;
    } else {
        int k=x->keys[idx];
        if(x->child[idx]->n>=t) {
            int pred=getPred(x,idx);
            x->keys[idx]=pred;
            removeFromNode(x->child[idx],x->child[idx]->n-1,t);
        } else if(x->child[idx+1]->n>=t) {
            int succ=getSucc(x,idx);
            x->keys[idx]=succ;
            removeFromNode(x->child[idx+1],0,t);
        } else {
            merge(x,idx,t);
            removeFromNode(x->child[idx],t-1,t);
        }
    }
}

void fill(BTreeNode* x,int idx,int t) {
    if(idx!=0 && x->child[idx-1]->n>=t) borrowFromPrev(x,idx);
    else if(idx!=x->n && x->child[idx+1]->n>=t) borrowFromNext(x,idx);
    else {
        if(idx!=x->n) merge(x,idx,t);
        else merge(x,idx-1,t);
    }
}

void removeKey(BTreeNode* x,int k,int t) {
    int idx=0;
    while(idx<x->n && x->keys[idx]<k) idx++;
    if(idx<x->n && x->keys[idx]==k) {
        removeFromNode(x,idx,t);
    } else {
        if(x->leaf) return;
        int flag=(idx==x->n);
        if(x->child[idx]->n<t) fill(x,idx,t);
        if(flag && idx>x->n) removeKey(x->child[idx-1],k,t);
        else removeKey(x->child[idx],k,t);
    }
}

BTreeNode* delete(BTreeNode* root,int k,int t) {
    removeKey(root,k,t);
    if(root->n==0) {
        BTreeNode* tmp=root;
        if(root->leaf) root=NULL;
        else root=root->child[0];
        free(tmp->keys); free(tmp->child); free(tmp);
    }
    return root;
}

void printTree(BTreeNode* root,int level) {
    if(root) {
        printf("Level %d : ",level);
        for(int i=0;i<root->n;i++) printf("%d ",root->keys[i]);
        printf("\n");
        if(!root->leaf) for(int i=0;i<=root->n;i++) printTree(root->child[i],level+1);
    }
}

int main() {
    int t,n,k,ch;
    printf("Enter order: ");
    scanf("%d",&t);
    BTreeNode* root=createNode(t,1);
    printf("Enter number of keys to insert: ");
    scanf("%d",&n);
    printf("Enter keys: ");
    for(int i=0;i<n;i++) {scanf("%d",&k); root=insert(root,k,t);}
    while(1) {
        printf("\n1.Delete\n2.Print\n3.Exit\nChoice: ");
        scanf("%d",&ch);
        if(ch==1){printf("Enter key: "); scanf("%d",&k); root=delete(root,k,t);}
        else if(ch==2){printTree(root,0);}
        else break;
    }
}
