#pragma once
#include "Structures.h"
#include "inc.h"
#include "Server.h"
#include <time.h>
#include <Windows.h>

int getAIDFromDB(){
	FILE* users = fopen("numbers.txt", "r");
	if (users == NULL){
		users = fopen("numbers.txt", "w");
		fprintf(users, "0,0");
		fclose(users);
	}
	int u, a;
	fscanf(users, "%d,%d", &u, &a);
	fclose(users);
	return a;
}
int setAIDInDB(int aid){
	FILE* users = fopen("numbers.txt", "r");
	if (users == NULL){
		users = fopen("numbers.txt", "w");
		fprintf(users, "0,0");
		fclose(users);
	}
	int u, a;
	fscanf(users, "%d,%d", &u, &a);
	fclose(users);
	users = fopen("numbers.txt", "w");
	fprintf(users, "%d,%d", u, aid);
	fclose(users);
	return a;
}
int line_num = 0;
struct ArticleNode* read_article_in_db(int articleid);


void update_article_in_db(struct ArticleNode *ptr, char *content);
void show_Article(struct ArticleNode *articles_array);
char * createArticleFileName(int id) {
	char *filename = (char*)malloc(sizeof(20));
	_itoa(id, filename, 10);
	int len = strlen(filename);
	filename = strcat(filename, "_a.txt");
	return filename;
}

char * createUserFileName(int id) {
	char *filename = (char*)malloc(sizeof(20));
	_itoa(id, filename, 10);
	int len = strlen(filename);
	filename = strcat(filename, "_u.txt");
	return filename;
}

void appendingUser(char *article_title, int article_id, int user_id)
{
	char b[20];
	_itoa(user_id, b, 10);
	int len = strlen(b);
	strcat(b, "_u.txt");
	FILE *fp_append = fopen(b, "a");
	fprintf(fp_append, "%d,%s\n", article_id, article_title);
	fclose(fp_append);
}

void appendingCategoryContent(char* category, char *title, int articleid)
{
	char cat[50];
	strcpy(cat, category);
	char* sp_category;
	sp_category = strtok(cat, ",");
	while (sp_category != NULL)
	{
		FILE *fp;
		char fileName[100];
		strcpy(fileName, sp_category);
		strcat(fileName, ".txt");
		fp = fopen(fileName, "a");
		fprintf(fp, "%d,%s$", articleid, title);
		fclose(fp);
		sp_category = strtok(NULL, ",");
	}
}

int addcommentInDB(int aid, struct CommentNode* cmt)
{
	char b[20];
	_itoa(aid, b, 10);
	strcat(b, "_a.txt");
	FILE *fp;
	fp = fopen(b, "a");
	while (cmt != NULL) {
		fprintf(fp, "c%d\n%ld\n%s$", cmt->userID, cmt->time, cmt->comment);
		cmt = cmt->nextComment;
	}
	fclose(fp);
	return 0;
}

int check_file(char *buffer) {
	if (buffer == NULL)
		return 0;
	FILE *fptr;
	char check[100];
	fptr = fopen("categories.txt", "r");
	if (fptr == NULL) {
		fptr = fopen("categories.txt", "w");
		fclose(fptr);
		return 1;
	}
	while (fscanf(fptr, "%s", check) != EOF) {
		if (strcmp(buffer, check) == 0) {
			return 1;
		}
	}
	fclose(fptr);
	return 0;
}

void add_category(char *buffer) {
	FILE *fptr;
	char token[100];
	fptr = fopen("categories.txt", "a");
	fprintf(fptr, "%s\n", buffer);
	fclose(fptr);
	strcpy(token, buffer);
	strcat(token, ".txt");
	fptr = fopen(token, "w");
	fclose(fptr);
}

void appendingCategoryFile(char *categories) {
	char *buffer, dl[3] = ",";
	char *d = (char *)malloc(sizeof(char) * 30);
	strcpy(d, categories);
	buffer = strtok(d, dl);
	if (!check_file(buffer))
		add_category(buffer);
	while (buffer != NULL) {
		buffer = strtok(NULL, dl);
		if (buffer != NULL)
			if (!check_file(buffer)) {
				add_category(buffer);
			}
	}
}

char**  extractArticleID(char *cat_file, int *n) {
	char **pid, *token, buffer[100];
	int index = 1;
	FILE *fptr;
	pid = (char**)malloc(sizeof(char*));
	pid[0] = (char*)malloc(15 * sizeof(char));
	fptr = fopen(cat_file, "r");
	if (fptr == NULL){
		return NULL;
	}
	while (fscanf(fptr, "%s", buffer) != EOF) {
		token=strtok(buffer, ",");
		strcpy(pid[index - 1], token);
		strtok(NULL, ",");;
		pid = (char**)realloc(pid, (index + 1) * sizeof(char*));
		pid[index] = (char*)malloc(15 * sizeof(char));
		index++;
	}
	int i;
	for (i = 0; i < index-1; i++) {
		printf("%s\n", pid[i]);
	}
	*n = index;
	return pid;

}

struct ArticleNode** retriveData(char *fileName, int *nu) {
	struct ArticleNode **retval = (struct ArticleNode**)malloc(sizeof(struct ArticleNode*) * 32);
	int n = 0, i;
	char **articleIDS = extractArticleID(fileName, &n);
	if (articleIDS == NULL){
		printf("No Articles to be Displayed.\n");
		return NULL;
	}
	for (i = 0; i < n; i++) {
		int j = atoi(articleIDS[i]);
		retval[i] = read_article_in_db(j);
		*nu = *nu + 1;
	}
	return retval;
}

void getlineArticle(FILE * fp, char *res)
{
	int linesize = 50;
	char c;
	fscanf(fp, "%s", res);
	return res;
}

int createArticleInDB(struct ArticleNode *article) {
	FILE *articleFile;
	int b = article->articleID;
	char* filename = createArticleFileName(b);
	appendingUser(article->title, article->articleID, article->userID);
	appendingCategoryFile(article->category);
	appendingCategoryContent(article->category, article->title, article->articleID);
	articleFile = fopen(filename, "w");
	article->time = time(NULL);
	fprintf(articleFile, "%d\n%d\n%ld\n%d\n%d\n%s\n%s\n%s$", article->articleID, article->userID,
		article->time, article->likes, article->disLikes, article->title, article->category, article->content);
	struct CommentNode *t = article->comments;
	int k = 0;
	while (t != NULL) {
		if (k == 0){
			fprintf(articleFile, "%c", 'c');
			k = 1;
		}
		fprintf(articleFile, "c%d\n%ld\n%s\n$", t->userID, t->time, t->comment);
		t = t->nextComment;
	}
	fclose(articleFile);
	return 0;
}
void viewRecent() {
	int i = getAIDFromDB()-1;
	for (; i >= 0; i--) {
		struct ArticleNode *id = read_article_in_db(i);
		show_Article(id);
	}
}
void show_Article(struct ArticleNode *articles_array) {
	if (articles_array == NULL){
		return;
	}
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO ConsoleInfo;
	GetConsoleScreenBufferInfo(hConsole, &ConsoleInfo);
	int originalAttrs = ConsoleInfo.wAttributes;

	//SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	printf("%-11s:", "Article ID");

	//SetConsoleTextAttribute(hConsole, originalAttrs);
	printf(" %d ", articles_array->articleID);

	//SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	printf("%-8s:", "User ID");

	//SetConsoleTextAttribute(hConsole, originalAttrs);
	printf(" %d ", articles_array->userID);

	//SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	printf("%s:", "Last Updated:");

	//SetConsoleTextAttribute(hConsole, originalAttrs);
	printf(" %s\n", ctime(&(articles_array->time)));

	//SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	printf("Title : ");

	//SetConsoleTextAttribute(hConsole, originalAttrs);
	printf("%s\n", articles_array->title);

	//SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	printf("Category:\n");

	//SetConsoleTextAttribute(hConsole, originalAttrs);
	printf("%s\n", articles_array->category);

	//SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	printf("Content :\n");

	//SetConsoleTextAttribute(hConsole, originalAttrs);
	printf("%s\n", articles_array->content);

	//SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	//printf("%-10s:", "Likes");

	//SetConsoleTextAttribute(hConsole, originalAttrs);
	//printf("%-5d ", articles_array->likes);

	//SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
	//printf("%-10s:", "Dislikes");
	//SetConsoleTextAttribute(hConsole, originalAttrs);
	//printf("%-5d\n", articles_array->disLikes);



	//SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	printf("Comments:\n");
	struct CommentNode *temp = articles_array->comments;
	while (temp != NULL)
	{
		//SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		printf("%-8s:", "User ID");

		//SetConsoleTextAttribute(hConsole, originalAttrs);
		printf("%d", temp->userID);

		//SetConsoleTextAttribute(hConsole, originalAttrs);
		printf("%s\n", temp->comment);

		temp = temp->nextComment;
	}
	//SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	printf("______________________________________________________________________________________________________________________\n");

}
void ViewArticle(struct ArticleNode **articles_array, int articles_count)
{
	int i = 0;
	struct CommentNode *temp;

	//system("color f0");


	for (i = 0; i < articles_count; i++)
	{
		show_Article(articles_array[i]);
	}
}
void deleteArticleInUser(int postid, int userid)
{
	char user_file[100];
	char temp[100];
	char line[1024];
	int id;
	sprintf(user_file, "%d%s.txt", userid, "_u");
	sprintf(temp, "%d%s.txt", userid, "_t");

	FILE* f1 = fopen(user_file, "r");
	FILE* f2 = fopen(temp, "w");

	int i = 0;
	if (f1 != NULL)
	{
		while (!feof(f1))
		{
			i = 0;
			line[0] = '\0';
			fgets(line, 1024, f1);
			id = atoi(line);
			if (id != postid)
			{
				fprintf(f2, line);
			}
		}

		fclose(f1);
		fclose(f2);
		remove(user_file);
		rename(temp, user_file);
	}
}

void deleteArticleInCategory(char* buff, int postid)
{
	char category_file[100];
	char temp[100];
	char line[1024];
	int id;
	sprintf(category_file, "%s.txt", buff);
	sprintf(temp, "%s%s.txt", buff, "_t");

	FILE* f1 = fopen(category_file, "r");
	FILE* f2 = fopen(temp, "w");

	int i = 0;
	if (f1 != NULL)
	{
		while (!feof(f1))
		{
			i = 0;
			line[0] = '\0';
			fgets(line, 1024, f1);
			id = atoi(line);
			if (id != postid)
				fprintf(f2, line);
		}

		fclose(f1);
		fclose(f2);
		remove(category_file);
		rename(temp, category_file);
	}
}

void deleteArticleInDB(int postid)
{
	char article_file[1024];
	char cat_file_name[1024];
	sprintf(article_file, "%d%s.txt", postid, "_a");

	struct ArticleNode *a = read_article_in_db(postid);
	deleteArticleInUser(postid, getuid());
	//cat_file_name[strlen(cat_file_name) - 1] = '\0';
	char *cat = a->category;
	char *tok;
	tok = strtok(cat, ",");
	while (tok != NULL) {
		deleteArticleInCategory(tok, postid);
		tok = strtok(NULL, ",");
	}
	remove(article_file);
	printf("Article %d deleted\n", postid);
}

char** getmyarticles(int *num_arts) {
	int uid = getuid();
	char** articles = (char**)malloc(sizeof(16 * sizeof(char*)));
	char* filename = createUserFileName(uid);
	int line_num = 0;
	FILE* users_file = fopen(filename, "r");
	if (users_file != NULL)
	{
		int capacity = 16;
		char * line = (char *)malloc(sizeof(char) * 50);
		char title[100];
		int i = 0;
		while (!feof(users_file))
		{
			line[0] = '\0';
			i = 0;
			getlineArticle(users_file, line);
			if (line[0] != '\0')
			{
				while (line[i] != ',' && line[i] != '\0') {
					title[i] = line[i];
					i++;
				}
				title[i] = '\0';
				i++;
				int a_id = atoi(title);
				strcpy(title, &line[i]);

				if (line_num == capacity - 1)
				{
					capacity *= 2;
					char** temp = (char**)malloc(sizeof(capacity * sizeof(char*)));
					for (int i = 0; i < line_num; i++)
						temp[i] = articles[i];
					articles = temp;
				}
				articles[line_num] = (char*)malloc(sizeof(char)*strlen(line) + 20);
				strcpy(articles[line_num], line);
				line_num++;
				printf("%d. %s\n", a_id, title);
			}
		}
		fclose(users_file);
	}
	*num_arts = line_num;
	return articles;
}

int getaid(char** articles, int num_arts) {
	int aid;
	scanf("%d", &aid);

	for (int i = 0; i < num_arts; i++)
	{
		if (atoi(articles[i]) == aid)
			return aid;
	}

	return -1;
}

void comment_check(struct CommentNode* temp)
{
	//temp = temp->nextComment;
	printf("Checking comments in linked lists\n");
	while (temp != NULL)
	{
		printf("%s", temp->comment);
		temp = temp->nextComment;
	}
}

struct ArticleNode* read_article_in_db(int articleid)
{
	struct ArticleNode *ptr = (struct ArticleNode *)malloc(sizeof(struct ArticleNode));
	char *buff = (char *)malloc(sizeof(char) * 20);
	char *temp_comment = (char *)malloc(sizeof(char) * 100);
	char *filename = (char *)malloc(sizeof(char) * 20);
	char ch;
	filename[0] = '\0';
	char ai[20];
	ai[0] = '\0';
	_itoa(articleid, ai, 10);
	strcat(filename, ai);
	strcat(filename, "_a.txt");
	int b;
	long int d;
	FILE *fp = fopen(filename, "r+");
	if (fp == NULL){
		return NULL;
	}
	fscanf(fp, "%d", &b);   ptr->articleID = b;
	ch = fgetc(fp);
	fscanf(fp, "%d", &b);   ptr->userID = b;
	ch = fgetc(fp);
	fscanf(fp, "%ld", &d);   ptr->time = d;
	ch = fgetc(fp);
	fscanf(fp, "%d", &b);   ptr->likes = b;
	ch = fgetc(fp);
	fscanf(fp, "%d", &b);   ptr->disLikes = b;
	ch = fgetc(fp);
	fscanf(fp, "%s", buff);
	ptr->title = (char *)malloc(sizeof(char) * strlen(buff));
	strcpy(ptr->title, buff);
	ch = fgetc(fp);
	fscanf(fp, "%s", buff);
	ptr->category = (char *)malloc(sizeof(char) * strlen(buff));
	strcpy(ptr->category, buff);
	ch = fgetc(fp);

	fscanf(fp, "%[^$]s", buff);            //printf("%s\n",buff);
	ch = fgetc(fp);
	ptr->content = (char *)malloc(sizeof(char) * strlen(buff));
	strcpy(ptr->content, buff);
	//int counter = 0;
	struct CommentNode *head=createCommentNode(-1,-1,-1,-1,"\0");
	struct CommentNode *prev=NULL, *curr;
	ptr->comments = head;
	int temp_userid, temp_time;
	ch = fgetc(fp);
	if (ch == 'c'){
		while (ch != EOF&&ch=='c')
		{
			fscanf(fp, "%d", &temp_userid);
			ch = fgetc(fp);
			fscanf(fp, "%d", &temp_time);
			ch = fgetc(fp);
			fscanf(fp, "%[^$]s", temp_comment);
			ch = fgetc(fp);
			//temp_comment = get_comment(fp);
			curr = createCommentNode(temp_userid, temp_time, 0, 0, temp_comment);
			curr->nextComment = NULL;
			//break;
			if (prev == NULL){
				prev = curr;
				head->nextComment = prev;
			}
			else{
				prev->nextComment = curr;
				prev = curr;
			}
			ch = fgetc(fp);
		}
	}
	ptr->comments = head->nextComment;
	//comment_check(head);
	fclose(fp);
	return ptr;
}

