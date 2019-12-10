#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
void die(char *s)
{
    perror(s);
    exit(1);
}
void login(int sock,struct sockaddr_in serv_addr){
    char *account;
    char status[30];
    char my_account[30];
    char password[30];
    int recv_len;
    int slen = sizeof(serv_addr);
    printf("Username:");
    __fpurge(stdin);
    gets(my_account);
    account = &my_account;
    sendto(sock , "123456781" , 9 , 0,(struct sockaddr*) &serv_addr, slen );
    sendto(sock , account , strlen(account) , 0,(struct sockaddr*) &serv_addr, slen );
    if((recv_len=recvfrom(sock, status, 20 , 0,(struct sockaddr*) &serv_addr, &slen ))==-1){
        die("recvfrom()");
    }
    status[recv_len]='\0';
    if(strcmp(status,"0")==0){
        printf("Account is blocked\n");
        login(sock,serv_addr);
    }
    else if(strcmp(status,"-1")==0){
        printf("Cannot find account\n");
        login(sock,serv_addr);
    }
    else if(strcmp(status,"1")==0){
        while(1){
            printf("Password:");
            __fpurge(stdin);
            gets(password);
            sendto(sock , "123456782" , 9 , 0,(struct sockaddr*) &serv_addr, slen );
            sendto(sock , account , strlen(account) , 0,(struct sockaddr*) &serv_addr, slen );
            if((recv_len=recvfrom(sock, status, 20 , 0,(struct sockaddr*) &serv_addr, &slen ))==-1){
                die("recvfrom()");
            }
            sendto(sock , password , strlen(password) , 0,(struct sockaddr*) &serv_addr, slen );
            if((recv_len=recvfrom(sock, status, 20 , 0,(struct sockaddr*) &serv_addr, &slen ))==-1){
                die("recvfrom()");
            }
            status[recv_len]='\0';
            if(strcmp(status,"1")==0){
                logout(sock,serv_addr);
            }
            else if(strcmp(status,"-1")==0){
                printf("Account is blocked\n");
                login(sock,serv_addr);
            }
            else{
                printf("Password is incorrect\n");
            }
        }
    }
    else{
        printf("Cannot find account\n");
    }
}
void logout(int sock,struct sockaddr_in serv_addr){
    char Selection[5];
    int slen = sizeof(serv_addr);
    char *p;
    printf("Login successfully\n");
    while(1){
	printf("Continue(1):\n");
        printf("Logout(2):\n");
        __fpurge(stdin);
        gets(Selection);
        if(strcmp(Selection,"2")==0){
            printf("Logout successfully\n");
            p = &Selection;
            sendto(sock , "123456783" , 9 , 0,(struct sockaddr*) &serv_addr, slen );
            sendto(sock , p , strlen(p) , 0,(struct sockaddr*) &serv_addr, slen );
            login(sock,serv_addr);
        }
    }
}
int main(int argc, char const *argv[]){
    if(argc!=3){
        printf("Nhap sai\n");
    }
    else{
        struct sockaddr_in serv_addr;
        int sock = 0;
        int PORT = atoi(argv[2]);
        char addr[225];
        strcpy(addr,argv[1]);
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
        { 
            printf("\n Loi socket \n"); 
            return -1; 
        }
        memset((char *) &serv_addr, '0', sizeof(serv_addr)); 
        serv_addr.sin_family = AF_INET; 
        serv_addr.sin_port = htons(PORT); 
        if(inet_pton(AF_INET, addr, &serv_addr.sin_addr) <= 0)  
        { 
            printf("\nDia chi khong hop le \n"); 
            return -1; 
        } 
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
        { 
            printf("\nConnection Failed \n"); 
            return -1; 
        }
        login(sock,serv_addr);
    }
}
