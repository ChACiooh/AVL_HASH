#include "hash.h"
#include "common.h"

void hashInit(HashNode*** hash_table)
{
	*hash_table = (HashNode**)malloc(sizeof(HashNode*)*HASH_TABLE_SIZE);
	int i;
	for(i=0;i<HASH_TABLE_SIZE;i++)
	{
		*(*hash_table+i) = (HashNode*)calloc(sizeof(HashNode), 1);
		// 동시에 NULL로 초기화 해 줌 개이득
		(*(*hash_table + i))->next_word = *(*hash_table + i);
	}
}

HashNode* hashInsert(HashNode*** hash_table, char *data, char *mean)
{
	int id = data[0] - 'a';
	HashNode *temp = *(*hash_table+id); // 맨 처음 HashNode는 데이터로써 쓰이지 않는다.
	while(temp->next_word != *(*hash_table+id) && strcmp(data, temp->next_word->data) > 0) {
		temp = temp->next_word;
	}
	if(temp->next_word == *(*hash_table+id))
	{
		temp->next_word = (HashNode*)calloc(sizeof(HashNode), 1);
		temp = temp->next_word;
		strcpy(temp->data, data);
		strcpy(temp->mean, mean);
		temp->next_word = *(*hash_table + id);
		return temp;
	}
	else
	{
		if(strcmp(data, temp->next_word->data) == 0)
		{
			puts("중복 입력 오류.");
			return NULL;
		}
		else // strcmp < 0, 중간 삽입
		{
			HashNode *new_node = (HashNode*)calloc(sizeof(HashNode), 1);
			strcpy(new_node->data, data);
			strcpy(new_node->mean, mean);
			new_node->next_word = temp->next_word;
			temp->next_word = new_node;
			return new_node;
		}
	}
}

void hashBrowse(HashNode* hash_table[HASH_TABLE_SIZE])
{ 
	HashNode *temp = NULL;
	int i, flag;
	for(i=0;i<HASH_TABLE_SIZE;i++)
	{
		temp = hash_table[i];
		flag = 1;
		while(temp->next_word != hash_table[i])
		{
			if(flag)
			{
				flag = 0;
				printf("[%c]: ", i+'a');
			}
			temp = temp->next_word;
			printf("[%s]->", temp->data);
		}
		if(!flag)	puts("");
	}
}

HashNode* hashSearch(HashNode* hash_table[HASH_TABLE_SIZE], char *key, int *count)
{
	int id = key[0] - 'a';
	int cnt = 0;
	int found = 0;
	HashNode *temp = hash_table[id];
	while(temp->next_word != hash_table[id])
	{
		++cnt;
		temp = temp->next_word;
		if (strcmp(temp->data, key) == 0)
		{
			found = 1;
			break;
		}
		printf("%s %s\n", temp->data, temp->mean);
	}
	*count = cnt;
	if (!found)	temp = NULL;
	return temp;
}

void hashRemove(HashNode*** hash_table)
{
	HashNode *temp = NULL;
	int i;
	for(i=0;i<HASH_TABLE_SIZE;i++)
	{
		temp = *(*hash_table+i);
		while(temp->next_word != *(*hash_table+i))
		{
			HashNode *dummy = temp;
			temp = temp->next_word;
			free(dummy);
		}
		free(temp);
	}
	free(*hash_table);
}
