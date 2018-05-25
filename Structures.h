#pragma once
#include "inc.h"

struct UserNode {
	int userID;
	char *userName;
	char *password;
};

struct CommentNode {
	int userID;
	long int time;
	int likes;
	int disLikes;
	char *comment;
	struct CommentNode *nextComment;
};

struct ArticleNode {
	int articleID;
	int userID;
	long int time;
	int likes;
	int disLikes;
	char *title;
	char *content;
	char *category;
	struct CommentNode *comments;
};

int stringLenght(char *str) {
	int i = 0;
	while (str[i] != '\0') {
		i++;
	}
	return i;
}

char* stringCopy(char *sou) {
	char *des = (char*)malloc(stringLenght(sou) + 10);
	int i = 0;
	while (sou[i] != '\0') {
		des[i] = sou[i];
		i++;
	}
	des[i] = '\0';
	return des;
}

struct UserNode* createUserNode(int userID, char *userName, char *password) {
	struct UserNode *retval = (struct UserNode *)malloc(sizeof(struct UserNode));
	retval->userID = userID;
	retval->userName = stringCopy(userName);
	retval->password = stringCopy(password);
	return retval;
}

struct CommentNode* createCommentNode(int userID, long int time, int likes, int disLikes, char *comment) {
	struct CommentNode *retval = (struct CommentNode *)malloc(sizeof(struct CommentNode));
	retval->userID = userID;
	retval->nextComment = NULL;
	retval->comment = stringCopy(comment);
	retval->disLikes = 0;
	retval->likes = 0;
	retval->time = time;
	return retval;
}

struct ArticleNode* createArticleNode(int articleID, int userID, int likes, int disLikes, char *title, char *content, char *category, struct CommentNode *comment) {
	struct ArticleNode *retval = (struct ArticleNode *)malloc(sizeof(struct ArticleNode));
	retval->category = stringCopy(category);
	retval->comments = comment;
	retval->articleID = articleID;
	retval->content = stringCopy(content);
	retval->userID = userID;
	retval->disLikes = disLikes;
	retval->likes = likes;
	retval->title = stringCopy(title);
	return retval;
}

char *stringReverse(char *str) {
	int i = 0;
	int len = stringLenght(str);
	char *ret = (char*)malloc(len + 5);
	len = len - 1;
	while (str[i] != '\0') {
		ret[len - i] = str[i];
		i++;
	}
	return ret;
}

char * numberToString(int num) {
	int i = num;
	int j = 0;
	char *s = (char*)malloc(10);
	while (i > 0) {
		s[j] = i % 10;
		i = i / 10;
		j++;
	}
	s[j] = '\0';
	return stringReverse(s);
}

int stringToNumber(char* num) {
	int i = 0;
	int j = 0;
	while (num[i] != '\0') {
		j = j * 10 + (num[i] - '0');
		i++;
	}
	return j;
}
