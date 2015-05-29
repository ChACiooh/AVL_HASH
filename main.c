#include "avltree.h"
#include "common.h"
#include "hash.h"
#include <Windows.h>
#include <time.h>

enum Menu { ADD_AVL = 1, ADD_HASH, BR_AVL, BR_HASH, SR_AVL, SR_HASH, EVAL, QUIT };

void print_init_menu(int *cmd)
{
	/// 출력하고, cmd에 입력하자.
	system("cls"); // clean the console window
	puts("========== Menu ==========");
	puts("1. Addition of AVL Tree");
	puts("2. Addition of Hash-table");
	puts("3. Browse of AVL Tree");
	puts("4. Browse of Hash-table");
	puts("5. Search of AVL Tree");
	puts("6. Search of Hash-table");
	puts("7. Evaluate the statistics");
	puts("8. quit");
	puts("==========================");
	printf("command: ");
	scanf("%d", cmd);
}

void evaluate_mean_in_search(AvlNode *root, HashNode **hash_table);

void act(int *cmd, AvlNode** root, HashNode** hash_table[HASH_TABLE_SIZE])
{
	system("cls"); // clean the console window
	printf("command: %d\n", *cmd);
	char word[256], mean[256];
	AvlNode *res = NULL;
	HashNode *res_h = NULL;
	int count = 0;
	ULONG64 start, end; // 최댓값이 18446744073709551615
	HANDLE hwnd;

	switch(*cmd)
	{
	case ADD_AVL:
		printf("word: ");
		scanf("%s", word);
		printf("mean: ");
		scanf("%s", mean);
		res = avlAdd(root, word, mean);
		if(res)	puts("addition complete!");
		break;
	case ADD_HASH:
		printf("word: ");
		scanf("%s", word);
		printf("mean: ");
		scanf("%s", mean);
		res_h = hashInsert(hash_table, word, mean);
		if(res_h)	puts("addition complete!");
		break;
	case BR_AVL:
		inorderTraveling(*root);
		break;
	case BR_HASH:
		hashBrowse(*hash_table);
		break;
	case SR_AVL:
		printf("word: ");
		scanf("%s", word);
		/// calculating time
		hwnd = GetCurrentProcess();
		QueryProcessCycleTime(hwnd, &start);
		res = avlSearch(*root, word, &count);
		QueryProcessCycleTime(hwnd, &end);
		if(res)
			printf("검색 결과 : %s\n\n", res->mean);
		else
			puts("\nNot found.\n");
		printf("count : %d\n", count);
		printf("time : %llu cycles\n", end - start);
		break;
	case SR_HASH:
		printf("word: ");
		scanf("%s", word);
		/// calculating time
		hwnd = GetCurrentProcess();
		QueryProcessCycleTime(hwnd, &start);
		res_h = hashSearch(*hash_table, word, &count);
		QueryProcessCycleTime(hwnd, &end);
		if(res_h)
			printf("검색 결과 : %s\n\n", res_h->mean);
		else
			puts("\nNot found.\n");
		printf("count : %d\n", count);
		printf("time : %llu cycles\n", end - start);
		break;
	case EVAL:
		evaluate_mean_in_search(*root, *hash_table);
		break;
	case QUIT:
	default:
		puts("\n프로그램을 종료합니다.");
		break;
	}
}

void avlSave_with_inorder(AvlNode *root, FILE *fp)
{
	if(root != NULL)
	{
		avlSave_with_inorder(root->left, fp);
		fprintf(fp, "%s %s\n", root->data, root->mean);
		avlSave_with_inorder(root->right, fp);
	}
}

void avlSave(AvlNode *root)
{
	FILE *fp = fopen("input.txt", "w");
	avlSave_with_inorder(root, fp);
	fclose(fp);
}

void avlLoad(AvlNode **root)
{
	FILE *fp = fopen("input.txt", "r");
    char word[256], mean[256];

    while(!feof(fp) && fscanf(fp, "%s %s", word, mean) != EOF) {
		avlAdd(root, word, mean);
	}
	fclose(fp);
}

void hashSave(HashNode ***hash_table)
{
	FILE *fp = fopen("inputH.txt", "w");
	int i;
	HashNode *temp = NULL;
	for(i=0;i<HASH_TABLE_SIZE;i++)
	{
		temp = *(*hash_table+i);
		while(temp->next_word != *(*hash_table+i))
		{
			temp = temp->next_word;
			fprintf(fp, "%s %s\n", temp->data, temp->mean);
		}
	}
	fclose(fp);
}

void hashLoad(HashNode ***hash_table)
{
	FILE *fp = fopen("inputH.txt", "r");
	char word[256], mean[256];

	while(!feof(fp) && fscanf(fp, "%s %s", word, mean) != EOF){
		hashInsert(hash_table, word, mean);
	}
	fclose(fp);
}

void evaluate_mean_in_search(AvlNode *root, HashNode **hash_table)
{
	system("cls");
	fflush(stdin);
	avlSave(root);
	hashSave(&hash_table);

	HANDLE hwnd;
	FILE *fp = fopen("inputH.txt", "r");
	ULONG64 start, end;
	int i, count = 0;
	char data[256], mean[256];
	/// hash 구하기
	hwnd = GetCurrentProcess();
	QueryProcessCycleTime(hwnd, &start);
	while (!feof(fp) && fscanf(fp, "%s %s", data, mean) != EOF)
	{
		int cnt = 0;
		hashSearch(hash_table, data, &cnt);
		count += cnt;
	}
	QueryProcessCycleTime(hwnd, &end);
	ULONG64 res = end - start;
	double delta_time = (double)(res) / (double)count;
	double mean_cnt = (double)count / (double)HASH_TABLE_SIZE;
	printf("Hash의 평균 :\n%g회 %g processes\n", mean_cnt, delta_time);
	fclose(fp);

	fp = fopen("input.txt", "r");
	count = 0;
	int num_of_data_in_avltree = 0;
	/// avl 구하기
	hwnd = GetCurrentProcess();
	QueryProcessCycleTime(hwnd, &start);
	while (!feof(fp) && fscanf(fp, "%s %s", data, mean) != EOF)
	{
		int cnt = 0;
		avlSearch(root, data, &cnt);
		count += cnt;
		num_of_data_in_avltree++;
	}
	QueryProcessCycleTime(hwnd, &end);
	res = end - start;
	delta_time = (double)res / (double)count;
	mean_cnt = (double)count / (double)num_of_data_in_avltree;
	printf("AVL Tree의 평균 :\n%g회 %g processes\n", mean_cnt, delta_time);
	fclose(fp);
}

int main()
{
    AvlNode* root = NULL; // AVL tree's root node pointer
    HashNode** hash_table = NULL;
    int cmd; // a variable for scanning user's command

    /// input progress
    printf("loading...");
    hashInit(&hash_table);
    avlLoad(&root);
    hashLoad(&hash_table);
	printf("complete!\n");
	printf("press enter key...");
	getchar();

	/// main action progress
    do
	{
		print_init_menu(&cmd);
		act(&cmd, &root, &hash_table);
		fflush(stdin);
		printf("\npress enter key...");
		getchar();
	} while(cmd >= ADD_AVL && cmd < QUIT);
	
	/// save
	avlSave(root);
	avlRemove(root);
	hashSave(&hash_table);
	hashRemove(&hash_table);
    return 0;
}
