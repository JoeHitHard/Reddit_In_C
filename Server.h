#pragma once
#include "ManageUsers.h"
#include "ManageArticles.h"
#include "inc.h"
struct ArticleNode** retriveData(char *fileName, int *nu);
void ViewArticle(struct ArticleNode **articles_array, int articles_count);
int currentID = -1;
int id;
int aid = -1;
void ByUser(char *username){
	int n = 0;
	strcat(username, "_u.txt");
	struct ArticleNode **r= retriveData(username, &n);
	ViewArticle(r, n);
}
void ByCategory(char *username){
	int n = 0;
	strcat(username, ".txt");
	struct ArticleNode **r = retriveData(username, &n);
	ViewArticle(r, n);
}
int compareFunction(const void *a, const void *b)
{
	int l = (*(struct ArticleNode **)a)->time;
	int r = (*(struct ArticleNode **)b)->time;
	return l - r;
}

struct ArticleNode **sortBasedOnTime(struct ArticleNode ** articles, int size)
{
	qsort(articles, size, sizeof(struct ArticleNode *), &compareFunction);
	return articles;
}

void register_user(struct UserNode* u) {
	id=valid_user(u);
	if (id != -1)
	{
		id = getUIDFromDB();
		u->userID = id + 1;
		createUserInDB(u);
		setUIDInDB(id + 1);
		printf("Registration success.\n");
	}
	else
	{
		printf("User already exists.\n");
		system("pause");
	}
}

void login_user(struct UserNode* u) {
	if (user_exists(u) == 1)
	{
		currentID = u->userID;
		printf("User Logged in.\n");
	}
	else
	{
		printf("Invalid Credentials.\n");
		system("pause");
	}
}

void logout(struct UserNode** u) {
	free((*u)->userName);
	free((*u)->password);
	free(*u);
	*u = NULL;
	id = -1;
	printf("User Logged out.");
}

int valid_article(struct ArticleNode* article) {
	if (article->category == NULL || article->content == NULL || article->title == NULL) {
		return -1;
	}
	return 1;
}

void add_article(struct ArticleNode* article) {
	if (valid_article(article) == 1) {
		article->userID = currentID;
		aid = getAIDFromDB();
		article->articleID = aid;
		aid = aid + 1;
		setAIDInDB(aid);
		createArticleInDB(article);
	}
}
int is_user_logged_in() {
	return (currentID != -1);
}
int addcomment(int articleid, struct CommentNode *comment){
	comment->userID = currentID;
	addcommentInDB(articleid, comment);
	return 1;
}
int getuid() {
	return currentID;
}

void updateArticle(int aid)
{
	//ArticleNode *ptr = (ArticleNode *)malloc(sizeof(ArticleNode)*1);

	char *filename = (char *)malloc(sizeof(char) * 10);
	char ch;
	filename[0] = '\0';
	char ai[20];
	_itoa(aid, ai, 10);
	strcat(filename, ai);
	strcat(filename, "_a.txt");
	remove(filename);
	char *title = (char *)malloc(100 * sizeof(char));
	char *category = (char *)malloc(100 * sizeof(char));
	char *content = (char *)malloc(100 * sizeof(char));
	printf("enter title: ");
	scanf(" %[^\n]s", title);
	printf("enter category: ");
	scanf(" %[^\n]s", category);
	printf("enter content ('$' to stop): ");
	scanf(" %[^$]s", content);
	struct ArticleNode *article = createArticleNode(0, 0, 0, 0, title, content, category, NULL);
	add_article(article);
}
