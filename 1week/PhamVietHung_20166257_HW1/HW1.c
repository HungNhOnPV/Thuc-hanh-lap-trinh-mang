#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
	char userName[50];
	char password[50];
	int  status;
	int  signin;

	struct node *next;
}user;

user *root,*cur,*new;

void menu() {
	printf("USER MANAGEMENT PROGRAM\n");
    printf("--------------------------\n");  
    printf("1. Register\n");  
    printf("2. Sign in\n");
    printf("3. Search\n");  
    printf("4. Sign out\n");
    printf("Your choice (1-4, other to quit) : ");
}

user* checkUserExist(char userName[50]) {
    cur = root;
    while (cur!= NULL){
        if (strcmp(cur->userName,userName) == 0) {
            return cur;
        }

        cur = cur->next;
    }
    return NULL;
}

void Register(){
    char userName[50];
    char password[50];

    printf("UserName : ");
    scanf("%s",userName);

    if (checkUserExist(userName)!= NULL) {
        printf("Account existed \n");
    } else {
        printf("Password : ");
        scanf("%s",password);

        cur = root;
        while (cur->next != NULL) cur = cur->next;
        new = (user*) malloc(sizeof(user));
        strcpy(new->userName,userName);
        strcpy(new->password,password);
        new->status = 3;
        new->next = NULL;
        cur->next = new;

        printf("Successful registration\n");
    };
}

void signin(){
    char userName[50];
    char password[50];

    printf("UserName : ");
    scanf("%s",userName);

    cur = checkUserExist(userName); 

    if (cur != NULL) {
        if (cur->signin != 0) { 
            printf("Account is signed in\n");
            return;
        }

        if (cur->status == 0) {
            printf("Account is blocked\n");
            return;
        }

        printf("Password : ");
        scanf("%s",password);

        if (strcmp(cur->password,password) != 0) {
            printf("Password is incorrect\n");
            cur->status--;

            if(cur->status == 0) {
                printf("Account is blocked\n");
            }
        } else { 
            printf("Hello %s\n", cur->userName);
            cur->signin = 1;
            cur->status = 3;
        }
    } else {
        printf("Cannot find account\n");   
    }
}

void search() { 
	char userName[50];

    printf("UserName : ");
    scanf("%s",userName);

    cur = checkUserExist(userName);

    if (cur == NULL) { 
        printf("Cannot find account\n");
        return ;
    }
    if (cur->status != 0) {
        printf("Account is active\n");
    } else {
        printf("Account is blocked\n");
    }
}

void signout() {
	char userName[50];

    printf("UserName : ");
    scanf("%s",userName);

    cur = checkUserExist(userName);

    if (cur == NULL) {
        printf("Cannot find account\n");
        return;
    } else {
        if (cur->signin == 0) {
            printf("Account is not sign in\n");
        } else {
            printf("Goodbye %s\n", cur->userName);
            cur->signin = 0;
        }
    }
}

void getUser(FILE *f) {
    char c,s[50]; 
    int mark = 1;
    int count = 0;
    
    root = (user*) malloc(sizeof(user));
    cur  = (user*) malloc(sizeof(user));
    new  = (user*) malloc(sizeof(user));

    root->next = NULL;
    cur = root;

    while (1){
        c = getc(f);

        if (feof(f)){
            break;
        }

        if (c == '\n'){
            mark = 1;
            s[count] = '\0';
            count = 0; 
            new->status = atoi(s);

            if (new->status == 1) {
                new->status = 3;
            }

            new->signin = 0;
            new->next = NULL ;
            cur->next = new;
            cur = new ;

            new = (user*) malloc(sizeof(user)); 
        } else if (c == ' '){
            s[count] = '\0';
            count = 0;

            if (mark == 1) {
                strcpy(new->userName,s);
            } else if (mark ==2) {
                strcpy(new->password,s);
            }

            mark ++;
        } else {
            s[count++] = c;
        }
    }
}

int main() {
    FILE *fptr = fopen("account.txt","r+");

	if (fptr == NULL) {
		printf("Cannot open file!\n");
		return 1;
    }

    getUser(fptr);

    int choice = 1;

    while (choice >=1 && choice <= 4){
        menu();

        scanf ("%d",&choice);

        switch (choice){
            case 1 : 
                Register();
                break;
            case 2 :
                signin();
                break;
            case 3 :
                search();
                break;
            case 4 : 
                signout();
                break;              
        }    
    }
    fclose(fptr);
    return 0;
}
