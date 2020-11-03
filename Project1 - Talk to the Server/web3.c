#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(){
    char* hostname = "text.npr.org";
    char ip[20];
    struct hostent *he;
    struct in_addr **addr_list;

    if ( (he = gethostbyname( hostname ) ) == NULL){
        herror("gethostbyname");
        return 1;
    }
    addr_list = (struct in_addr **) he->h_addr_list;
    if(addr_list[0] != NULL){
        strcpy(ip , inet_ntoa(*addr_list[0]) );
        printf("%s\n",ip);
        return 0;
    }

    return 1;
}
