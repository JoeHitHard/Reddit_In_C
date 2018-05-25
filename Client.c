#include <stdlib.h>
#include "ManageArticles.h"
#include "ManageUsers.h"

int main()
{
	char * username,*comment;
	char * password;
	username = (char*)malloc(100 * sizeof(char));
	password = (char*)malloc(100 * sizeof(char));
	comment = (char*)malloc(100 * sizeof(char));
	char * title;
	char * category;
	char * content;
	char v;
	int articleid;
	struct UserNode* user = NULL;
	struct ArticleNode * article = NULL;
	struct ArticleNode ** allArticles = NULL;
	struct CommentNode *com;
	char** articles;
	int num_arts;
	int aid;
	char ch;
	while (1)
	{
		printf("MENU\n");
		printf("1:Register\n");
		printf("2:Login\n");
		printf("0:exit\n");
		fflush(stdin);
		scanf_s(" %c", &ch);
		fflush(stdin);
		switch (ch)
		{
		case '1':
			printf("enter username\n");
			scanf("%s", username);
			printf("enter password\n");
			scanf("%s", password);
			user = createUserNode(0, username, password);
			register_user(user);
			break;

		case '2':
			printf("enter username\n");
			scanf("%s", username);
			printf("enter password\n");
			scanf("%s", password);
			user = createUserNode(0, username, password);
			login_user(user);
			break;
		case '0':
			return 0;
		}
		if (is_user_logged_in()){
			break;
		}
	}
	while (1){
		printf("\n\n\t***Welcome  %s your ID is %d***\n",user->userName,user->userID);
		printf("\t\t***MENU***\n");
		printf("1:Add Article\n");
		printf("2:View Articles\n");
		printf("3:Delete Article\n");
		printf("4:Update Article\n");
		printf("5:Add Comment\n");
		printf("6:Logout\n");
		fflush(stdin);
		scanf_s(" %c", &ch);
		fflush(stdin);
		switch (ch)
		{
		case '1':
			if (!is_user_logged_in())
			{
				printf("Login before adding article!\n");
				break;
			}
			title = (char *)malloc(100 * sizeof(char));
			category = (char *)malloc(100 * sizeof(char));
			content = (char *)malloc(100 * sizeof(char));
			printf("enter title: ");
			scanf(" %[^\n]s", title);
			printf("enter category: ");
			scanf(" %[^\n]s", category);
			printf("enter content ('$' to stop): ");
			scanf(" %[^$]s", content);
			article = createArticleNode(0, 0, 0, 0, title, content, category, NULL);
			add_article(article);
			printf("Added Article!\n");
			break;
		case '2':
			printf("View Articles by:\n");
			printf("1:Recent\n");
			printf("2:User\n");
			printf("3:Category\n");
			fflush(stdin);
			printf("Enter Your Choice:\n");
			v = getch();
			fflush(stdin);
			switch (v) {
			case '1':
				viewRecent();
				break;
			case '2':
				printf("enter username\n");
				scanf("%s", username);
				ByUser(username);
				break;
			case '3':
				printf("enter category\n");
				scanf("%s", username);
				ByCategory(username);
				break;
			}
			break;
		case '3':
			if (!is_user_logged_in())
			{
				printf("Login before deleting article!\n");
				break;
			}
			articles = getmyarticles(&num_arts);
			printf("Enter Article id to Delete:");
			aid = getaid(articles, num_arts);
			if (aid != -1)
				deleteArticleInDB(aid);
			else
				printf("Enter valid article number\n");
			break;
		case '4':
			if (!is_user_logged_in())
			{
				printf("Login before Updating article!\n");
				break;
			}
			articles = getmyarticles(&num_arts);
			printf("Enter Article id to Update:");
			aid = getaid(articles, num_arts);
			if (aid != -1)
				updateArticle(aid);
			else
				printf("Enter valid article number\n");
			break;
		case '5':
			if (!is_user_logged_in())
			{
				printf("Login before Commenting article!\n");
				break;
			}
			printf("enter Article ID:");
			scanf("%d", &articleid);
			printf("enter Comment:(Enter '$' at the end)\n");
			scanf("%[^$]s", comment);
			com = createCommentNode(-1, 0, 0, 0, comment);
			addcomment(articleid, com);
			break;
		case '6':
			logout(&user);
			printf("User Logged out\n");
			main();
			break;
		default:
			break;
		}

	}
	return 0;
}
