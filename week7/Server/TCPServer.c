#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h> 
#include <netinet/in.h> 
#include <string.h>
#include <ctype.h>

#include <sys/socket.h> 
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include "dslkd.h"

#define BUFLEN 512

void die(char *s);
void function_bind(int server_fd,struct sockaddr_in address);
void scanlist(FILE *fin);
void updatefile(node *p);
elementtype findInforAccount(node *p,char username[]);
void logout(int connfd,struct sockaddr_in address);
void checkPassword(int connfd,struct sockaddr_in address,node *p);
int PlusTimeFalse(node *p,elementtype tmp);
void ResetTimeFalse(node *p,elementtype tmp);
void changeStatus(node *p,elementtype tmp);

void die(char *s)
{
    perror(s);
    exit(1);
}
void function_bind(int server_fd,struct sockaddr_in address){
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) { 
        die("bind");
    }
}
void scanlist(FILE *fin){
	elementtype tmp;
	while(!feof(fin))
    {
        fscanf(fin,"%s %s %d",tmp.username,tmp.password,&tmp.status);     
        insert_first(tmp);												
    }
}
void scanTimeFalse(node *p,FILE *fin){
    elementtype tmp;
    while (!feof(fin))
    {
        fscanf(fin,"%s %d",tmp.username,&tmp.TimeFalse);
        p->element.TimeFalse = tmp.TimeFalse;
        p = p->next;
    }
}
void updatelist1(node *p){
    FILE *f;
    f = fopen("account.txt","r");
    elementtype tmp;
    while(!feof(f)){
        fscanf(f,"%s %s %d",tmp.username,tmp.password,&tmp.status);
        if(p->element.status!=tmp.status){
            p->element.status=tmp.status;
        }
        p=p->next;
    }
    fclose(f);
}
void updatelist2(node *p){
    FILE *f;
    f = fopen("TimeFalse.txt","r");
    elementtype tmp;
    while(!feof(f)){
        fscanf(f,"%s %d",tmp.username,&tmp.TimeFalse);
        if(p->element.TimeFalse!=tmp.TimeFalse){
            p->element.TimeFalse=tmp.TimeFalse;
        }
        p=p->next;
    }
    fclose(f);
}
void updatefile1(node *p){
	FILE * fp;
   	fp = fopen ("account.txt", "w");
	while(p->next!=NULL){
		elementtype tmp=p->element;
		fprintf(fp, "%s %s %d\n",tmp.username,tmp.password,tmp.status );    
		p=p->next;
	}
	elementtype tmp=p->element;
	fprintf(fp, "%s %s %d",tmp.username,tmp.password,tmp.status );			
	fclose(fp);														
}
void updatefile2(node *p){
	FILE * fp;
   	fp = fopen ("TimeFalse.txt", "w");
	while(p->next!=NULL){
		elementtype tmp=p->element;
		fprintf(fp, "%s %d\n",tmp.username,tmp.TimeFalse );    
		p=p->next;
	}
	elementtype tmp=p->element;
	fprintf(fp, "%s %d",tmp.username,tmp.TimeFalse );			
	fclose(fp);														
}
elementtype findInforAccount(node *p,char username[]){
    elementtype tmp;
	while(p!=NULL){
    	tmp=p->element;
        if(strcmp(tmp.username,username)==0){
            return tmp;
        }
  		p=p->next;
  	}
    tmp.status = -1;
    return tmp;
}
void sendMessage(int connfd,char status[],struct sockaddr_in address){
    int slen = sizeof(address);
    sendto(connfd , status , 20 , 0 ,(struct sockaddr *)&address,slen);
}
void checkUsername(int connfd,struct sockaddr_in address,node *p)
{ 
    char username[BUFLEN];
    elementtype tmp;
    int recv_len;
    int slen = sizeof(address);
    recv_len = recvfrom(connfd, username, BUFLEN, 0,(struct sockaddr *)&address,&slen);
    username[recv_len] = '\0';
    updatelist1(p);
    tmp = findInforAccount(root,username);
    if(tmp.status == -1){
        sendMessage(connfd,"-1",address);
    }
    else if(tmp.status == 0){
        sendMessage(connfd,"0",address);
    }
    else{
        sendMessage(connfd,"1",address);
    }
}
void checkPassword(int connfd,struct sockaddr_in address,node *p){
    int recv_len;
    char password[BUFLEN];
    int slen = sizeof(address);
    int a;
    char username[BUFLEN];
    elementtype tmp;
    recv_len = recvfrom(connfd, username, BUFLEN, 0,(struct sockaddr *)&address,&slen);
    sendMessage(connfd,"0",address);
    username[recv_len] = '\0';
    tmp = findInforAccount(root,username);
    recv_len = recvfrom(connfd, password, BUFLEN, 0,(struct sockaddr *)&address,&slen);
    password[recv_len]='\0';
    updatelist2(p);
    if(strcmp(password,tmp.password)==0){
        sendMessage(connfd,"1",address);
        ResetTimeFalse(root,tmp);
        updatefile2(p);
    }
    else{
        a = PlusTimeFalse(p,tmp);
        updatefile2(p);
        if(a>=3){
            changeStatus(p,tmp);
            updatefile1(p);
            sendMessage(connfd,"-1",address);
        }
        else
        {
            sendMessage(connfd,"0",address);
        }
          
    }
}
void changeStatus(node *p,elementtype tmp){
    while(p!=NULL){
        if(strcmp(p->element.username,tmp.username)==0){
            p->element.status = 0;
            break;
        }
        p = p->next;
    }
}
void ResetTimeFalse(node *p,elementtype tmp){
    while(p!=NULL){
        if(strcmp(p->element.username,tmp.username)==0){
            p->element.TimeFalse = 0;
            break;
        }
        p = p->next;
    }
}
int PlusTimeFalse(node *p,elementtype tmp){
    while(p!=NULL){
        if(strcmp(p->element.username,tmp.username)==0){
            p->element.TimeFalse++;
            break;
        }
        p = p->next;
    }
    return (p->element.TimeFalse);
}
void logout(int connfd,struct sockaddr_in address){
    char selection[30];
    int slen = sizeof(address);
    recvfrom(connfd, selection, 30, 0,(struct sockaddr *)&address,&slen);
    printf("Client was logout!\n");
}
int checkvalue(int connfd,struct sockaddr_in address){
    char selection[9];
    int slen = sizeof(address);
    recvfrom(connfd, selection, 9, 0,(struct sockaddr *)&address,&slen);
    if(strcmp(selection,"123456781")==0){
        return 1;
    }
    else if(strcmp(selection,"123456782")==0){
        return 2;
    }
    else if(strcmp(selection,"123456783")==0){
        return 3;
    }
}
int main(int argc, char const *argv[]) 
{ 
    if(argc!=2){
		printf("Nhap sai\n");
	}
	else{
        FILE *f1;
        int serverSocket =  socket(PF_INET,SOCK_STREAM,0);
        f1 = fopen("account.txt","r");
        scanlist(f1);
        fclose(f1);
        root=reverse(root);
        updatefile2(root);
        int PORT = atoi(argv[1]);
        if (serverSocket == -1)
        {
            perror("CREATE SOCKET");
            exit(0);
        }
        int i = 1;
        int check = setsockopt(serverSocket,SOL_SOCKET,SO_REUSEADDR,&i,sizeof(int));
        if (check == -1)
        {
            perror("Set reuse");
        }
        struct sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(PORT);
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        socklen_t len = sizeof(struct sockaddr_in);
        check = bind(serverSocket,(struct sockaddr*)&serverAddress,len);
        if (check == -1)
        {
            perror("BIND");
            exit(0);
        }
        check = listen(serverSocket,10);
        if (check == -1)
        {
            perror("Listen");
            exit(1);
        }
         struct sockaddr_in clientAddress;
        len = sizeof(struct sockaddr_in);
        fd_set readfds;
        fd_set masterfds; 
        FD_ZERO(&masterfds);
        FD_ZERO(&readfds);
        FD_SET(serverSocket,&masterfds);
        int max_fd = serverSocket; 
        struct timeval timeout;
        timeout.tv_sec = 90; 
        timeout.tv_usec = 0;
        int n_select;
        do{
            memcpy(&readfds,&masterfds,sizeof(masterfds)); 
            n_select = select(max_fd + 1, &readfds,NULL,NULL,&timeout); 
            if (n_select < 0)
            {
                    perror("SELECT");
                    exit(0);
            }
            else
            if (n_select == 0)
            {
            }
            else
            {
                for (i = 0; i <= max_fd;i++){
                    if (FD_ISSET(i,&readfds)) {
                        int close_fd = 0;
                        if(i == serverSocket)
                        {                          
                            int newCon = accept(serverSocket,(struct sockaddr*)&clientAddress,&len); 
                            printf("New connection \n");

                            FD_SET(newCon,&masterfds);
                            if (newCon > max_fd ) max_fd = newCon;
                        }
                        else
                        {
                            int value;
                            value = checkvalue(i,clientAddress);
                            if(value==1){
                                checkUsername(i,clientAddress,root);
                            }
                            else if(value==2){
                                checkPassword(i,clientAddress,root);
                            }
                            else if(value==3){
                                logout(i,clientAddress);
                            }
                        }
                        if (close_fd == 1) 
                        {
                            FD_CLR(i,&masterfds);
                            close(i);
                        }
                    }
                }
            }
        }while(1);
    }
    return 0; 
} 
