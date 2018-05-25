#pragma once
#include "Structures.h"

char* getline(FILE * fp)
{
	if (fp == NULL) {
		return "\0";
	}
	int linesize = 50;
	char * res = (char*)malloc(linesize * sizeof(char));
	char c;
	int i = 0;
	for (; 1; i++)
	{
		c = fgetc(fp);
		if (c == EOF || c == '\n')
			break;
		if (i == linesize - 1)
		{
			linesize *= 2;

			char* temp = (char*)malloc(linesize * sizeof(char));
			strncpy(temp, res, linesize / 2);
			free(res);
			res = temp;
		}
		res[i] = c;
	}
	res[i] = '\0';
	return res;
}

int valid_user(struct UserNode* u) {
	FILE* users = fopen("users.txt", "r");
	if (users == NULL){
		return 0;
	}
	char* user_i = getline(users);
	int count = 0;
	while (user_i[0] != '\0')
	{
		int uid;
		char* name = NULL;
		char* pword = NULL;
		char* temp = strtok(user_i, ",");
		if (temp != NULL)
		{
			uid = atoi(temp);
			temp = strtok(NULL, ",");
		}
		if (temp != NULL)
		{
			name = temp;
			temp = strtok(NULL, ",");
		}
		if (temp != NULL)
		{
			pword = temp;
			temp = strtok(NULL, ",");
		}
		if (!strcmp(name, u->userName))
		{
			fclose(users);
			return -1;
		}
		count = uid;
		free(user_i);
		user_i = getline(users);
	}
	fclose(users);
	return count;
}

int user_exists(struct UserNode* u) {
	FILE* users = fopen("users.txt", "r");
	char* user_i = getline(users);
	while (user_i[0] != '\0')
	{
		int uid;
		char* name = NULL;
		char* pword = NULL;
		char* temp = strtok(user_i, ",");
		if (temp != NULL)
		{
			uid = atoi(temp);
			temp = strtok(NULL, ",");
		}
		if (temp != NULL)
		{
			name = temp;
			temp = strtok(NULL, ",");
		}
		if (temp != NULL)
		{
			pword = temp;
			temp = strtok(NULL, ",");
		}
		if (!strcmp(name, u->userName) && !strcmp(pword, u->password))
		{
			u->userID = uid;
			fclose(users);
			return 1;
		}
		free(user_i);
		user_i = getline(users);
	}
	fclose(users);
	return -1;
}
int getUIDFromDB(){
	int u, a;
	FILE* users = fopen("numbers.txt", "r");
	if (users == NULL){
		users = fopen("numbers.txt", "w");
		fprintf(users, "0,0");
		fclose(users);
		users = fopen("numbers.txt", "r");
	}
	fscanf(users, "%d,%d", &u, &a);
	fclose(users);
	return u;
}
int setUIDInDB(int uid){
	FILE* users = fopen("numbers.txt", "r");
	if (users == NULL){
		users = fopen("numbers.txt", "w");
		fprintf(users, "0,0");
		fclose(users);
		users = fopen("numbers.txt", "r");
	}
	int u, a;
	fscanf(users, "%d,%d", &u, &a);
	fclose(users);
	users = fopen("numbers.txt", "w");
	fprintf(users, "%d,%d", uid, a);
	fclose(users);
	return a;
}
void createUserInDB(struct UserNode* u) {
	FILE* users = fopen("users.txt", "r");
	if (users == NULL){
		users = fopen("users.txt", "w");
		fclose(users);
	}
	if (valid_user(u) != -1)
	{
		users = fopen("users.txt", "a");
		fprintf(users, "%d,%s,%s\n", u->userID, u->userName, u->password);
		fclose(users);
		char* filename = (char*)malloc(sizeof(u->userName) + 10);
		sprintf(filename, "%d", u->userID);
		strcat(filename, "_u.txt");
		FILE* fp = fopen(filename, "w");
		fclose(fp);
		free(filename);
		filename = NULL;
	}
}
