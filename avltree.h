#ifndef AVLTREE_H_INCLUDED
#define AVLTREE_H_INCLUDED

typedef struct AvlNode
{
	int length;
	char data[256];
	char mean[256];
	struct AvlNode *left, *right;
} AvlNode;

int getHeight(AvlNode* node);
int getHeightDiff(AvlNode* node);
AvlNode* rotateLL(AvlNode* parent);
AvlNode* rotateRR(AvlNode* parent);
AvlNode* rotateRL(AvlNode* parent);
AvlNode* rotateLR(AvlNode* parent);
AvlNode* rebalance(AvlNode **node);
AvlNode* avlAdd(AvlNode **root, char* data, char* mean);
AvlNode* avlSearch(AvlNode *node, char* key, int *count);
void avlRemove(AvlNode *root);
void inorderTraveling(AvlNode *root);


#endif // AVLTREE_H_INCLUDED
