#include "avltree.h"
#include "common.h"

int getHeight(AvlNode* node)
{
	int h = 0;
	if(node!=NULL)
	{
		h = 1 + max(getHeight(node->left), getHeight(node->right));
	}
	return h;
}

int getHeightDiff(AvlNode* node)
{
	if(node == NULL)	return 0;
	return getHeight(node->left) - getHeight(node->right);
}

AvlNode* rotateLL(AvlNode* parent)
{
	AvlNode* child = parent->left;
	parent->left = child->right;
	child->right = parent;
	return child;
}

AvlNode* rotateRR(AvlNode* parent)
{
	AvlNode* child = parent->right;
	parent->right = child->left;
	child->left = parent;
	return child;
}

AvlNode* rotateRL(AvlNode* parent)
{
	AvlNode *child = parent->right;
	parent ->right =rotateLL(child);
	return rotateRR(parent);
}

AvlNode* rotateLR(AvlNode* parent)
{
	AvlNode *child = parent->left;
	parent->left = rotateRR(child);
	return rotateLL(parent);
}

AvlNode* rebalance(AvlNode **node)
{
	int heightDiff = getHeightDiff(*node);
	if(heightDiff > 1) // 오른쪽이 더 깊다
	{
		if(getHeightDiff((*node)->left) > 0)
			*node = rotateLL(*node);
		else
			*node = rotateLR(*node);
	}
	else if(heightDiff < -1) // 왼쪽이 더 깊다
	{
		if(getHeightDiff((*node)->right)<0)
			*node = rotateRR(*node);
		else
			*node = rotateRL(*node);
	}
	return *node;
}

AvlNode* avlAdd(AvlNode **root, char* data, char* mean)
{
	if(*root == NULL)
	{
		*root = (AvlNode*)calloc(sizeof(AvlNode), 1);
		if(*root == NULL)	exit(1);
		strcpy((*root)->data, data);
		(*root)->length = strlen(data); // 비교 기준
		strcpy((*root)->mean, mean);
		(*root)->left = (*root)->right = NULL;
		return *root;
	}

	int res = strlen(data) - (*root)->length;
	if(res <= 0)
	{
		if(res == 0 && strcmp(data, (*root)->data) == 0)
		{
			printf("중복 키 삽입 오류\n");
			return *root;
		}
		else if(res == 0)
		{
			// 길이는 같은데 글자가 다른 경우
			int dif = strcmp(data, (*root)->data);
			if(dif > 0) // data가 (*root)->data 보다 후위
			{
				(*root)->right = avlAdd(&((*root)->right), data, mean);
				*root = rebalance(root);
				return *root;
			}
		}
		 // data가 (*root)->data 보다 전위
		(*root)->left = avlAdd(&((*root)->left), data, mean);
		*root = rebalance(root);
	}
	else // res > 0
	{
		(*root)->right = avlAdd(&((*root)->right),  data, mean);
		*root = rebalance(root);
	}

	return *root;
}

AvlNode* avlSearch(AvlNode *node, char* key, int *count)
{
	(*count)++;
	if(node == NULL) return NULL;
	int res = strlen(key) - node->length;

	if(res <= 0)
	{
		if(res == 0 && strcmp(key, node->data) == 0) {
			// 길이가 같고 동일한 글자
			return node;
		}
		else if(res == 0 && strcmp(key, node->data)>0) {
			// 길이는 같은데 key가 더 큼
			printf("%s %s\n", node->data, node->mean);
			return avlSearch(node->right, key, count);
		}
		printf("%s %s\n", node->data, node->mean);
		return avlSearch(node->left, key, count); // 그밖의 조건은 모두 res <= 0이면 만족.
	}
	else	return avlSearch(node->right, key, count);
}

void avlRemove(AvlNode *node)
{
	if(node)
	{
		avlRemove(node->left);
		avlRemove(node->right);
		free(node);
	}
}

void inorderTraveling(AvlNode *root)
{
	if(root != NULL)
	{
		inorderTraveling(root->left);
		printf("[%s] ", root->data);
		inorderTraveling(root->right);
	}
}
