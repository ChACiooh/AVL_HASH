#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

#define HASH_TABLE_SIZE	26

typedef struct HashNode
{
	char data[256];
	char mean[256];
	struct HashNode *next_word;
} HashNode;

void hashInit(HashNode*** hash_table);
HashNode* hashInsert(HashNode** hash_table[HASH_TABLE_SIZE], char *data, char *mean);
void hashBrowse(HashNode* hash_table[HASH_TABLE_SIZE]);
HashNode* hashSearch(HashNode *hash_table[HASH_TABLE_SIZE], char *key, int *count);
void hashRemove(HashNode*** hash_table);

#endif // HASH_H_INCLUDED
