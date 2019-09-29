#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// int i;
struct hostent *host;
struct in_addr **addr_list;

// check parameter from user is Ip or domain
int checkIpOrDomain(char *argv){ // out : 0 ~ IP , 1~domain
    char A[3];
    int i = 0;
    int count = 0;
    int temp = 0;

    while(i < strlen(argv)) {
        if (argv[i] == '.') {
            count++;

            if (count > 0 || count < 4) {
                A[temp] = '\0';
            } else return 1;

            if (atoi(A)<0 || atoi(A)>255) return 1;
            temp = 0;
            A[temp]='\0';
        } else if(argv[i] != '.') {
            if (argv[i]<'0' || argv[i]>'9') return 1;
            A[temp] = argv[i];
            temp++;
            }

            i++;
    }

    if (count != 3) {
        return 2;
    }
    A[3]= '\0';

    // if (atoi(a)<0 || atoi(a)>255) return 1;
    return 0;
}

// print domainname from ip
void ipToDomain(char *argv){
    int i = 0;
    struct in_addr ipv4addr;
    inet_pton(AF_INET,argv, &ipv4addr);
    host = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);
    printf("Official name: %s\n", host->h_name);

    while(host->h_aliases[i] != NULL) {
        printf("Alias name:\n%s\n", host->h_aliases[i]);
        i++;
    }
}
// print ip from domainname
void domainToIp(char *argv){ // input : domain name -> output : IPs of this domain
    int i = 1;
    int j = 0;
    int count = 0;

    while(j < strlen(argv)) {
        if (argv[j] == '.') {
            count++;
        }

            j++;
    }

    if (count != 1) {
        printf("Not found information\n");
        return;
    } else {
        host = gethostbyname(argv);
        // check exist of the host name
        if (gethostbyname(argv) != NULL){
        // print IPs of this host
        addr_list = (struct in_addr **)host->h_addr_list;
        if (addr_list[0] != NULL ) {
            printf("Official IP: %s\n", inet_ntoa(*addr_list[0]));
        }
        if (addr_list[1] != NULL) {
            printf("Alias IP: \n");
            while(addr_list[i] != NULL) {
                printf("%s\n", inet_ntoa(*addr_list[i]));
                i++;
            }
        }
        } else {
            printf("Not found information\n");
            return;
        }
    }
}

int main(int argc, char *argv[])
{
    // check command from user
    if (argc != 2) {
        printf("Usage: %s <hostname> or %s <IP>\n", argv[0],argv[0]);
        return 1;
    }

    if(checkIpOrDomain(argv[1]) == 0) {
        ipToDomain(argv[1]);
    } else if(checkIpOrDomain(argv[1]) == 1) {
        domainToIp(argv[1]);
    } else if(checkIpOrDomain(argv[1]) == 2) {
        printf("Not found information\n");
    }

    return 0;
}
